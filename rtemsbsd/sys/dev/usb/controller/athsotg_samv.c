/*-
 * Copyright (c) 2007-2016 Hans Petter Selasky. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <machine/rtems-bsd-kernel-space.h>

#include <bsp.h>

#ifdef LIBBSP_ARM_ATSAM_BSP_H

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/stdint.h>
#include <sys/stddef.h>
#include <rtems/bsd/sys/param.h>
#include <sys/queue.h>
#include <sys/types.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/bus.h>
#include <sys/module.h>
#include <rtems/bsd/sys/lock.h>
#include <sys/mutex.h>
#include <sys/condvar.h>
#include <sys/sx.h>
#include <sys/callout.h>
#include <sys/malloc.h>

#include <dev/usb/usb.h>
#include <dev/usb/usbdi.h>

#include <dev/usb/usb_core.h>
#include <dev/usb/usb_busdma.h>
#include <dev/usb/usb_process.h>
#include <dev/usb/usb_util.h>

#include <dev/usb/usb_controller.h>
#include <dev/usb/usb_bus.h>
#include <dev/usb/controller/athsotg.h>

#include <sys/rman.h>

#include <bsp/irq.h>

#include <libchip/chip.h>

static device_probe_t athsotg_samv_probe;
static device_attach_t athsotg_samv_attach;
static device_detach_t athsotg_samv_detach;

#define PINS_VBUS_EN { PIO_PC16, PIOC, ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT }

static const Pin pin_vbus_en[] = { PINS_VBUS_EN };

static void
athsotg_samv_on(struct athsotg_softc *sc)
{
	uint32_t reg;

	sysclk_enable_usb();
	PMC_EnablePeripheral(ID_USBHS);

	PIO_Configure(pin_vbus_en, PIO_LISTSIZE(pin_vbus_en));

	/* Power off USB devices */
	PIO_Set(pin_vbus_en);

	/* Set host mode */
	reg = ATHSOTG_READ_4(sc, ATHSOTG_CTRL);
	reg &= ~ATHSOTG_CTRL_UIMOD_DEVICE;
	ATHSOTG_WRITE_4(sc, ATHSOTG_CTRL, reg);

	/* Enable USB */
	reg = ATHSOTG_READ_4(sc, ATHSOTG_CTRL);
	reg |= ATHSOTG_CTRL_USBE;
	ATHSOTG_WRITE_4(sc, ATHSOTG_CTRL, reg);

	/* Unfreeze USB clock */
	reg = ATHSOTG_READ_4(sc, ATHSOTG_CTRL);
	reg &= ~ATHSOTG_CTRL_FRZCLK;
	ATHSOTG_WRITE_4(sc, ATHSOTG_CTRL, reg);

	/* Check USB clock */
	while ((ATHSOTG_READ_4(sc, ATHSOTG_SR) & ATHSOTG_SR_CLKUSABLE) == 0) {
		/* Wait */
	}

	/* Clear all interrupts */
	ATHSOTG_WRITE_4(sc, ATHSOTG_HSTICR, 0xffffffffu);

	/* Power on USB devices */
	PIO_Clear(pin_vbus_en);
}

static int
athsotg_samv_probe(device_t dev)
{

	device_set_desc(dev, "Atmel USB High-Speed Interface (USBHS)");
	return (0);
}

static int
athsotg_samv_attach(device_t dev)
{
	struct athsotg_softc *sc = device_get_softc(dev);
	int err;
	int rid;

	rid = 0;
	sc->sc_io_res = bus_alloc_resource_any(dev, SYS_RES_MEMORY, &rid,
	    RF_ACTIVE);
	if (sc->sc_io_res == NULL) {
		err = ENOMEM;
		goto error;
	}
	sc->sc_io_tag = rman_get_bustag(sc->sc_io_res);
	sc->sc_io_hdl = rman_get_bushandle(sc->sc_io_res);

	rid = 0;
	sc->sc_irq_res = bus_alloc_resource_any(dev, SYS_RES_IRQ, &rid,
	   RF_ACTIVE);
	if (sc->sc_irq_res == NULL) {
		goto error;
	}
	sc->sc_bus.bdev = device_add_child(dev, "usbus", -1);
	if (sc->sc_bus.bdev == NULL) {
		goto error;
	}
	device_set_ivars(sc->sc_bus.bdev, &sc->sc_bus);

	athsotg_samv_on(sc);

	err = bus_setup_intr(dev, sc->sc_irq_res, INTR_TYPE_TTY | INTR_MPSAFE,
	    NULL, athsotg_interrupt, sc, &sc->sc_intr_hdl);
	if (err) {
		sc->sc_intr_hdl = NULL;
		goto error;
	}

	err = athsotg_init(sc);
	if (!err) {
		err = device_probe_and_attach(sc->sc_bus.bdev);
	}
	if (err) {
		goto error;
	}
	return (0);

error:
	athsotg_samv_detach(dev);
	return (ENXIO);
}

static int
athsotg_samv_detach(device_t dev)
{

	panic("athsotg_samv_detach");
}

static device_method_t athsotg_samv_methods[] = {
	/* Device interface */
	DEVMETHOD(device_probe, athsotg_samv_probe),
	DEVMETHOD(device_attach, athsotg_samv_attach),
	DEVMETHOD(device_detach, athsotg_samv_detach),
	DEVMETHOD(device_suspend, bus_generic_suspend),
	DEVMETHOD(device_resume, bus_generic_resume),
	DEVMETHOD(device_shutdown, bus_generic_shutdown),

	DEVMETHOD_END
};

static driver_t athsotg_samv_driver = {
	.name = "athsotg_samv",
	.methods = athsotg_samv_methods,
	.size = sizeof(struct athsotg_softc),
};

static devclass_t athsotg_samv_devclass;

DRIVER_MODULE(athsotg_samv, nexus, athsotg_samv_driver,
    athsotg_samv_devclass, 0, 0);

#endif /* LIBBSP_ARM_ATSAM_BSP_H */
