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

/* Disable some BSP provided header files visible via <libchip/chip.h> */
#define	USBHS_H
#define	_SAME70_USBHS_COMPONENT_
#define	_SAMS70_USBHS_COMPONENT_
#define	_SAMV71_USBHS_COMPONENT_

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
#include <dev/usb/controller/atsam_otg.h>
#include <dev/usb/controller/atsam_otg_reg.h>

#include <sys/rman.h>

#include <bsp/irq.h>

#include <libchip/chip.h>

static device_probe_t ats_otg_7x_probe;
static device_attach_t ats_otg_7x_attach;
static device_detach_t ats_otg_7x_detach;

#define PINS_VBUS_EN { PIO_PC16, PIOC, ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT }

static const Pin pin_vbus_en[] = { PINS_VBUS_EN };

static void
ats_otg_7x_power_on(struct ats_otg_softc *sc, uint8_t enable)
{
	sysclk_enable_usb();
	PMC_EnablePeripheral(ID_USBHS);

	PIO_Configure(pin_vbus_en, PIO_LISTSIZE(pin_vbus_en));

	/* Power ON/OFF USB devices */
	if (enable)
		PIO_Clear(pin_vbus_en);
	else
		PIO_Set(pin_vbus_en);

	/* Enable UTMI USB port reset */
	UTMI->UTMI_OHCIICR = 0;

	/* Disable UTMI USB port reset */
	UTMI->UTMI_OHCIICR = UTMI_OHCIICR_RES0;

	/* set 12MHz reference clock */
	UTMI->UTMI_CKTRIM = UTMI_CKTRIM_FREQ_XTAL12 | (1U << 16);
}

static int
ats_otg_7x_probe(device_t dev)
{

	device_set_desc(dev, "Atmel USB High-Speed Interface (USBHS)");
	return (0);
}

static int
ats_otg_7x_attach(device_t dev)
{
	struct ats_otg_softc *sc = device_get_softc(dev);
	int err;
	int rid;

	/* select USB HOST mode */
	sc->sc_mode = ATS_MODE_HOST;

	/* set power mode callback */
	sc->sc_pwr_cmd = &ats_otg_7x_power_on;

	/* initialise some bus fields */
	sc->sc_bus.parent = dev;
	sc->sc_bus.devices = sc->sc_devices;
	sc->sc_bus.devices_max = ATS_OTG_MAX_DEVICES;
	sc->sc_bus.dma_bits = 32;

	/* get all DMA memory */
	if (usb_bus_mem_alloc_all(&sc->sc_bus,
	    USB_GET_DMA_TAG(dev), NULL)) {
		return (ENOMEM);
	}

	rid = 0;
	sc->sc_io_res = bus_alloc_resource_any(dev, SYS_RES_MEMORY, &rid,
	    RF_ACTIVE);
	if (sc->sc_io_res == NULL) {
		err = ENOMEM;
		goto error;
	}
	sc->sc_io_tag = rman_get_bustag(sc->sc_io_res);
	sc->sc_io_hdl = rman_get_bushandle(sc->sc_io_res);

	rid = 1;
	sc->sc_fifo_res = bus_alloc_resource_any(dev, SYS_RES_MEMORY, &rid,
	    RF_ACTIVE);
	if (sc->sc_fifo_res == NULL) {
		err = ENOMEM;
		goto error;
	}
	sc->sc_fifo_tag = rman_get_bustag(sc->sc_fifo_res);
	sc->sc_fifo_hdl = rman_get_bushandle(sc->sc_fifo_res);

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

	/* set power off by default */
	ats_otg_7x_power_on(sc, 0);

	err = bus_setup_intr(dev, sc->sc_irq_res, INTR_TYPE_TTY | INTR_MPSAFE,
	    ats_otg_filter_interrupt, ats_otg_interrupt, sc, &sc->sc_intr_hdl);
	if (err != 0) {
		sc->sc_intr_hdl = NULL;
		goto error;
	}

	err = ats_otg_init(sc);
	if (err != 0) {
		goto error;
	}

	err = device_probe_and_attach(sc->sc_bus.bdev);
	if (err != 0) {
		goto error;
	}

	return (0);

error:
	ats_otg_7x_detach(dev);
	return (ENXIO);
}

static int
ats_otg_7x_detach(device_t dev)
{

	panic("ats_otg_7x_detach");
}

static device_method_t ats_otg_7x_methods[] = {
	/* Device interface */
	DEVMETHOD(device_probe, ats_otg_7x_probe),
	DEVMETHOD(device_attach, ats_otg_7x_attach),
	DEVMETHOD(device_detach, ats_otg_7x_detach),
	DEVMETHOD(device_suspend, bus_generic_suspend),
	DEVMETHOD(device_resume, bus_generic_resume),
	DEVMETHOD(device_shutdown, bus_generic_shutdown),

	DEVMETHOD_END
};

static driver_t ats_otg_7x_driver = {
	.name = "ats_otg_7x",
	.methods = ats_otg_7x_methods,
	.size = sizeof(struct ats_otg_softc),
};

static devclass_t ats_otg_7x_devclass;

DRIVER_MODULE(ats_otg_7x, nexus, ats_otg_7x_driver,
    ats_otg_7x_devclass, 0, 0);
MODULE_DEPEND(ats_otg_7x, usb, 1, 1, 1);
SYSINIT_DRIVER_REFERENCE(usbus, ats_otg_7x);

#endif /* LIBBSP_ARM_ATSAM_BSP_H */
