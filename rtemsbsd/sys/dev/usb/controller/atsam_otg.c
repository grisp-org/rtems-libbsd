#include <machine/rtems-bsd-kernel-space.h>

/* $FreeBSD$ */
/*-
 * Copyright (c) 2016 Hans Petter Selasky. All rights reserved.
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

/*
 * This file contains the driver for the Atmel's SAM ARM series of USB
 * OTG controllers. It should be noted that the hardware does not
 * support the USB HIGH speed transaction translator which means FULL
 * and LOW speed devices will not work through USB HUBs unless the HUB
 * is FULL speed or the hardware is configured FULL speed only.
 */

#ifdef USB_GLOBAL_INCLUDE_FILE
#include USB_GLOBAL_INCLUDE_FILE
#else
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
#include <sys/sysctl.h>
#include <sys/sx.h>
#include <sys/unistd.h>
#include <sys/callout.h>
#include <sys/malloc.h>
#include <sys/priv.h>

#include <dev/usb/usb.h>
#include <dev/usb/usbdi.h>

#define	USB_DEBUG_VAR ats_otg_debug

#include <dev/usb/usb_core.h>
#include <dev/usb/usb_debug.h>
#include <dev/usb/usb_busdma.h>
#include <dev/usb/usb_process.h>
#include <dev/usb/usb_transfer.h>
#include <dev/usb/usb_device.h>
#include <dev/usb/usb_hub.h>
#include <dev/usb/usb_util.h>

#include <dev/usb/usb_controller.h>
#include <dev/usb/usb_bus.h>
#endif					/* USB_GLOBAL_INCLUDE_FILE */

#include <dev/usb/controller/atsam_otg_reg.h>
#include <dev/usb/controller/atsam_otg.h>

#define	ATS_OTG_BUS2SC(bus) \
   ((struct ats_otg_softc *)(((uint8_t *)(bus)) - \
    ((uint8_t *)&(((struct ats_otg_softc *)0)->sc_bus))))

#define	ATS_OTG_PC2SC(pc) \
   ATS_OTG_BUS2SC(USB_DMATAG_TO_XROOT((pc)->tag_parent)->bus)

#define	ATS_OTG_PC2UDEV(pc) \
   (USB_DMATAG_TO_XROOT((pc)->tag_parent)->udev)

#ifdef USB_DEBUG
static int ats_otg_debug = 0;

static
SYSCTL_NODE(_hw_usb, OID_AUTO, atsam_otg, CTLFLAG_RW, 0,
    "USB ATS OTG DCI");

SYSCTL_INT(_hw_usb_atsam_otg, OID_AUTO, debug, CTLFLAG_RWTUN,
    &ats_otg_debug, 0, "ATS OTG DCI debug level");
#endif

#define	ATS_OTG_INTR_ENDPT 1

#define	ATS_OTG_DEVIMR_THREAD_IRQ ( \
    USBHS_DEVIMR_WAKEUP | \
    USBHS_DEVIMR_EORST	| \
    USBHS_DEVIMR_SUSP)

#define	ATS_OTG_HSTIMR_THREAD_IRQ ( \
   USBHS_HSTIMR_RXRSM	| \
   USBHS_HSTIMR_RSMED	| \
   USBHS_HSTIMR_RST	| \
   USBHS_HSTIMR_DDISC	| \
   USBHS_HSTIMR_DCONN)

/* prototypes */

static const struct usb_bus_methods ats_otg_bus_methods;
static const struct usb_pipe_methods ats_otg_non_isoc_methods;
static const struct usb_pipe_methods ats_otg_isoc_methods;

static ats_otg_cmd_t ats_otg_device_setup_rx;
static ats_otg_cmd_t ats_otg_device_data_rx;
static ats_otg_cmd_t ats_otg_device_data_tx;
static ats_otg_cmd_t ats_otg_device_data_tx_sync;

static ats_otg_cmd_t ats_otg_host_setup_tx;
static ats_otg_cmd_t ats_otg_host_data_rx;
static ats_otg_cmd_t ats_otg_host_data_tx;

static void ats_otg_root_intr(struct ats_otg_softc *);
static void ats_otg_init_fifo(struct ats_otg_softc *, uint8_t);
static void ats_otg_device_done(struct usb_xfer *, usb_error_t);
static void ats_otg_standard_done(struct usb_xfer *);
static void ats_otg_do_poll(struct usb_bus *);

/*
 * Here is a list of what the chip supports:
 */
static const struct usb_hw_ep_profile ats_otg_ep_profile[3] = {

	[0] = {
		.max_in_frame_size = 64,
		.max_out_frame_size = 64,
		.is_simplex = 1,
		.support_control = 1,
	},
	[1] = {
		.max_in_frame_size = 64,
		.max_out_frame_size = 64,
		.is_simplex = 1,
		.support_bulk = 1,
		.support_interrupt = 1,
		.support_isochronous = 1,
		.support_in = 1,
		.support_out = 1,
	},
	[2] = {
		.max_in_frame_size = 512,
		.max_out_frame_size = 512,
		.is_simplex = 1,
		.support_bulk = 1,
		.support_interrupt = 1,
		.support_isochronous = 1,
		.support_in = 1,
		.support_out = 1,
	},
};

static void
ats_otg_get_hw_ep_profile(struct usb_device *udev,
    const struct usb_hw_ep_profile **ppf, uint8_t ep_addr)
{
	if (ep_addr == 0)
		*ppf = ats_otg_ep_profile + 0;
	else if (ep_addr < ATS_OTG_MAX_DEVICE_HS_EP)
		*ppf = ats_otg_ep_profile + 2;
	else if (ep_addr < ATS_OTG_MAX_DEVICE_ENDPOINTS)
		*ppf = ats_otg_ep_profile + 1;
	else
		*ppf = NULL;
}

static void
ats_otg_pull_up(struct ats_otg_softc *sc)
{
	/* pullup D+, if possible */

	if (!sc->sc_flags.d_pulled_up &&
	    sc->sc_flags.port_powered) {
		uint32_t temp;

		temp = ATS_OTG_READ_4(sc, USBHS_DEVCTRL) &
		    ~USBHS_DEVCTRL_DETACH;
		ATS_OTG_WRITE_4(sc, USBHS_DEVCTRL, temp);

		sc->sc_flags.d_pulled_up = 1;
	}
}

static void
ats_otg_pull_down(struct ats_otg_softc *sc)
{
	/* pulldown D+, if possible */

	if (sc->sc_flags.d_pulled_up) {
		uint32_t temp;

		temp = ATS_OTG_READ_4(sc, USBHS_DEVCTRL) |
		    USBHS_DEVCTRL_DETACH;
		ATS_OTG_WRITE_4(sc, USBHS_DEVCTRL, temp);

		sc->sc_flags.d_pulled_up = 0;
	}
}

static void
ats_otg_resume_irq(struct ats_otg_softc *sc)
{
	if (sc->sc_flags.status_suspend) {
		/* update status bits */
		sc->sc_flags.status_suspend = 0;
		sc->sc_flags.change_suspend = 1;

		if (sc->sc_flags.status_device_mode) {
			/* Disable resume and enable suspend interrupt */
			ATS_OTG_WRITE_4(sc, USBHS_DEVIDR, USBHS_DEVIDR_WAKEUP);
			ATS_OTG_WRITE_4(sc, USBHS_DEVIER, USBHS_DEVIER_SUSP);
		}
		/* complete root HUB interrupt endpoint */
		ats_otg_root_intr(sc);
	}
}

static void
ats_otg_suspend_irq(struct ats_otg_softc *sc)
{
	if (!sc->sc_flags.status_suspend) {
		/* update status bits */
		sc->sc_flags.status_suspend = 1;
		sc->sc_flags.change_suspend = 1;

		if (sc->sc_flags.status_device_mode) {
			/* Disable suspend and enable resume interrupt */
			ATS_OTG_WRITE_4(sc, USBHS_DEVIDR, USBHS_DEVIDR_SUSP);
			ATS_OTG_WRITE_4(sc, USBHS_DEVIER, USBHS_DEVIER_WAKEUP);
		}
		/* complete root HUB interrupt endpoint */
		ats_otg_root_intr(sc);
	}
}

static void
ats_otg_wakeup_peer(struct ats_otg_softc *sc)
{
	uint32_t temp;

	if (!sc->sc_flags.status_suspend)
		return;

	DPRINTFN(5, "Remote wakeup\n");

	if (sc->sc_flags.status_device_mode) {

		temp = ATS_OTG_READ_4(sc, USBHS_DEVCTRL) |
		    USBHS_DEVCTRL_RMWKUP;
		ATS_OTG_WRITE_4(sc, USBHS_DEVCTRL, temp);

		/* wait 8 milliseconds */
		usb_pause_mtx(&sc->sc_bus.bus_mtx, hz / 125);

		/* hardware should have cleared RMWKUP bit */
	} else {
		temp = ATS_OTG_READ_4(sc, USBHS_HSTCTRL);

		temp &= ~(USBHS_HSTCTRL_RESET | USBHS_HSTCTRL_RESUME);

		/* enable USB port */
		temp |= USBHS_HSTCTRL_SOFE;
		ATS_OTG_WRITE_4(sc, USBHS_HSTCTRL, temp);

		/* wait 10ms */
		usb_pause_mtx(&sc->sc_bus.bus_mtx, hz / 100);

		/* resume port */
		temp |= USBHS_HSTCTRL_RESUME;
		ATS_OTG_WRITE_4(sc, USBHS_HSTCTRL, temp);

		/* Wait 100ms for resume signalling to complete. */
		usb_pause_mtx(&sc->sc_bus.bus_mtx, hz / 10);

		/* clear resume */
		temp &= ~USBHS_HSTCTRL_RESUME;
		ATS_OTG_WRITE_4(sc, USBHS_HSTCTRL, temp);

		/* Wait 4ms */
		usb_pause_mtx(&sc->sc_bus.bus_mtx, hz / 250);
	}

	/* need to fake resume IRQ */
	ats_otg_resume_irq(sc);
}

static void
ats_otg_set_address(struct ats_otg_softc *sc, uint8_t addr)
{
	uint32_t temp;

	DPRINTFN(5, "addr=%d\n", addr);

	temp = ATS_OTG_READ_4(sc, USBHS_DEVCTRL) &
	    ~USBHS_DEVCTRL_UADD(-1U);
	temp |= USBHS_DEVCTRL_UADD(addr) |
	    USBHS_DEVCTRL_ADDEN;

	/* write and enable new address */
	ATS_OTG_WRITE_4(sc, USBHS_DEVCTRL, temp);
}

static uint8_t
ats_otg_device_setup_rx(struct ats_otg_softc *sc, struct ats_otg_td *td)
{
	struct usb_device_request req __aligned(4);
	uint32_t fifo_off;
	uint32_t temp;
	uint16_t count;

	/* get endpoint status */
	temp = ATS_OTG_READ_4(sc, USBHS_DEVEPTISR(0));

	DPRINTFN(5, "DEVEPTISR(0)=0x%08x\n", temp);

	if (!(temp & USBHS_DEVEPTISR_RXSTP))
		goto not_complete;

	/* clear did stall */
	td->did_stall = 0;

	/* get the packet byte count */
	count = (temp & USBHS_DEVEPTISR_BYCT(-1U)) /
	    USBHS_DEVEPTISR_BYCT(1);

	/* verify data length */
	if (count != td->remainder) {
		DPRINTFN(0, "Invalid SETUP packet "
		    "length, %d bytes\n", count);
		goto not_complete;
	}
	if (count != sizeof(req)) {
		DPRINTFN(0, "Unsupported SETUP packet "
		    "length, %d bytes\n", count);
		goto not_complete;
	}
	/* get FIFO offset */
	fifo_off = ATS_OTG_FIFO_OFFSET(td->ep_no);

	/* read data from FIFO */
	ATS_OTG_READ_FIFO_1(sc, fifo_off, (void *)&req, sizeof(req));

	/* copy data into real buffer */
	usbd_copy_in(td->pc, 0, &req, sizeof(req));

	td->offset = sizeof(req);
	td->remainder = 0;

	/* sneak peek the set address */
	if ((req.bmRequestType == UT_WRITE_DEVICE) &&
	    (req.bRequest == UR_SET_ADDRESS)) {
		sc->sc_dv_addr = req.wValue[0] & 0x7F;

		/* must write address before ZLP */
		temp = ATS_OTG_READ_4(sc, USBHS_DEVCTRL) &
		    ~USBHS_DEVCTRL_UADD(-1U);
		temp |= USBHS_DEVCTRL_UADD(sc->sc_dv_addr);
		ATS_OTG_WRITE_4(sc, USBHS_DEVCTRL, temp);
	} else {
		sc->sc_dv_addr = 0xFF;
	}

	/* Clear SETUP packet interrupt and all other previous interrupts */
	ATS_OTG_WRITE_4(sc, USBHS_DEVEPTICR(0), -1U);
	return (0);			/* complete */

not_complete:
	/* abort any ongoing transfer */
	if (!td->did_stall) {
		DPRINTFN(5, "stalling\n");
		ATS_OTG_WRITE_4(sc, USBHS_DEVEPTIER(0), USBHS_DEVEPTIER_STALLRQ);
		td->did_stall = 1;
	}
	/* Clear SETUP packet interrupt and all other previous interrupts */
	if (temp & USBHS_DEVEPTISR_RXSTP)
		ATS_OTG_WRITE_4(sc, USBHS_DEVEPTICR(0), -1U);

	/* we only want to know if there is a SETUP packet */
	ATS_OTG_WRITE_4(sc, USBHS_DEVEPTIER(0),
	    USBHS_DEVEPTIER_RXSTP);
	ATS_OTG_WRITE_4(sc, USBHS_DEVEPTIDR(0),
	    USBHS_DEVEPTIDR_TXIN | USBHS_DEVEPTIDR_RXOUT);
	ATS_OTG_WRITE_4(sc, USBHS_DEVIER, USBHS_DEVIER_PEP_MASK(0));

	return (1);			/* not complete */
}

static uint8_t
ats_otg_device_data_rx(struct ats_otg_softc *sc, struct ats_otg_td *td)
{
	struct usb_page_search buf_res;
	uint32_t fifo_off;
	uint32_t temp;
	uint16_t count;
	uint8_t to;
	uint8_t got_short;

	to = 3;				/* don't loop forever! */
	got_short = 0;
repeat:
	/* get endpoint status */
	temp = ATS_OTG_READ_4(sc, USBHS_DEVEPTISR(td->ep_no));

	DPRINTFN(5, "DEVEPTISR(%u)=0x%08x rem=%u\n",
	    td->ep_no, temp, td->remainder);

	/* check if any of the FIFO banks have data */
	if (temp & USBHS_DEVEPTISR_RXSTP) {
		if (td->remainder == 0) {
			/*
			 * We are actually complete and have
			 * received the next SETUP
			 */
			DPRINTFN(5, "faking complete\n");
			return (0);	/* complete */
		}
		/*
	         * USB Host Aborted the transfer.
	         */
		td->error_any = 1;
		return (0);		/* complete */
	}
	if ((temp & USBHS_DEVEPTISR_NBUSYBK(-1U)) == 0) {
		/* cannot read any data */
		goto not_complete;
	}
	/* get the packet byte count */
	count = (temp & USBHS_DEVEPTISR_BYCT(-1U)) /
	    USBHS_DEVEPTISR_BYCT(1);

	/* verify the packet byte count */
	if (count != td->max_packet_size) {
		if (count < td->max_packet_size) {
			/* we have a short packet */
			td->short_pkt = 1;
			got_short = 1;
		} else {
			/* invalid USB packet */
			td->error_any = 1;
			return (0);	/* we are complete */
		}
	}
	/* verify the packet byte count */
	if (count > td->remainder) {
		/* invalid USB packet */
		td->error_any = 1;
		return (0);		/* we are complete */
	}
	/* get FIFO offset */
	fifo_off = ATS_OTG_FIFO_OFFSET(td->ep_no);

	while (count > 0) {
		usbd_get_page(td->pc, td->offset, &buf_res);

		/* get correct length */
		if (buf_res.length > count) {
			buf_res.length = count;
		}
		/* read data from FIFO */
		ATS_OTG_READ_FIFO_1(sc, fifo_off,
		    buf_res.buffer, buf_res.length);

		/* update counters */
		count -= buf_res.length;
		td->offset += buf_res.length;
		td->remainder -= buf_res.length;
	}

	/* clear OUT packet interrupt */
	ATS_OTG_WRITE_4(sc, USBHS_DEVEPTICR(td->ep_no), USBHS_DEVEPTICR_RXOUT);

	/* release FIFO bank */
	ATS_OTG_WRITE_4(sc, USBHS_DEVEPTIDR(td->ep_no), USBHS_DEVEPTIDR_FIFOCON);

	/* check if we are complete */
	if ((td->remainder == 0) || got_short) {
		if (td->short_pkt) {
			/* we are complete */
			return (0);
		}
		/* else need to receive a zero length packet */
	}
	if (--to)
		goto repeat;

not_complete:
	/* we only want to know if there is a SETUP packet or OUT packet */
	ATS_OTG_WRITE_4(sc, USBHS_DEVEPTIER(td->ep_no),
	    USBHS_DEVEPTIER_RXSTP | USBHS_DEVEPTIMR_RXOUT);
	ATS_OTG_WRITE_4(sc, USBHS_DEVEPTIDR(td->ep_no),
	    USBHS_DEVEPTIDR_TXIN);
	ATS_OTG_WRITE_4(sc, USBHS_DEVIER, USBHS_DEVIER_PEP_MASK(td->ep_no));
	return (1);			/* not complete */
}

static uint8_t
ats_otg_device_data_tx(struct ats_otg_softc *sc, struct ats_otg_td *td)
{
	struct usb_page_search buf_res;
	uint32_t temp;
	uint32_t fifo_off;
	uint16_t count;
	uint8_t to;

	to = 3;				/* don't loop forever! */
repeat:
	/* get endpoint status */
	temp = ATS_OTG_READ_4(sc, USBHS_DEVEPTISR(td->ep_no));

	DPRINTFN(5, "DEVEPTISR(%u)=0x%08x rem=%u\n",
	    td->ep_no, temp, td->remainder);

	if (temp & USBHS_DEVEPTISR_RXSTP) {
		/*
	         * The current transfer was aborted
	         * by the USB Host
	         */
		td->error_any = 1;
		return (0);		/* complete */
	}
	if (temp & USBHS_DEVEPTISR_NBUSYBK(-1U)) {
		/* cannot write any data - a bank is busy */
		goto not_complete;
	}
	count = td->max_packet_size;
	if (td->remainder < count) {
		/* we have a short packet */
		td->short_pkt = 1;
		count = td->remainder;
	}

	/* get FIFO offset */
	fifo_off = ATS_OTG_FIFO_OFFSET(td->ep_no);

	while (count > 0) {

		usbd_get_page(td->pc, td->offset, &buf_res);

		/* get correct length */
		if (buf_res.length > count) {
			buf_res.length = count;
		}
		/* write data to FIFO */
		ATS_OTG_WRITE_FIFO_1(sc, fifo_off,
		    buf_res.buffer, buf_res.length);

		/* update counters */
		count -= buf_res.length;
		td->offset += buf_res.length;
		td->remainder -= buf_res.length;
	}

	/* clear IN packet interrupt */
	ATS_OTG_WRITE_4(sc, USBHS_DEVEPTICR(td->ep_no), USBHS_DEVEPTICR_TXIN);

	/* allocate FIFO bank */
	ATS_OTG_WRITE_4(sc, USBHS_DEVEPTIDR(td->ep_no), USBHS_DEVEPTIDR_FIFOCON);

	/* check remainder */
	if (td->remainder == 0) {
		if (td->short_pkt) {
			return (0);	/* complete */
		}
		/* else we need to transmit a short packet */
	}
	if (--to)
		goto repeat;

not_complete:
	/* we only want to know if there is a SETUP packet or free IN packet */
	ATS_OTG_WRITE_4(sc, USBHS_DEVEPTIER(td->ep_no),
	    USBHS_DEVEPTIER_RXSTP | USBHS_DEVEPTIER_TXIN);
	ATS_OTG_WRITE_4(sc, USBHS_DEVEPTIDR(td->ep_no),
	    USBHS_DEVEPTIDR_RXOUT);
	ATS_OTG_WRITE_4(sc, USBHS_DEVIER, USBHS_DEVIER_PEP_MASK(td->ep_no));
	return (1);			/* not complete */
}

static uint8_t
ats_otg_device_data_tx_sync(struct ats_otg_softc *sc, struct ats_otg_td *td)
{
	uint32_t temp;

	/* get endpoint status */
	temp = ATS_OTG_READ_4(sc, USBHS_DEVEPTISR(td->ep_no));

	DPRINTFN(5, "DEVEPTISR(%u)=0x%08x rem=%u\n",
	    td->ep_no, temp, td->remainder);

	if (temp & USBHS_DEVEPTISR_RXSTP) {
		DPRINTFN(5, "faking complete\n");
		/* Race condition */
		return (0);		/* complete */
	}
	/*
	 * The control endpoint has only got one bank, so if that bank
	 * is free the packet has been transferred!
	 */
	if (temp & USBHS_DEVEPTISR_NBUSYBK(-1U)) {
		/* cannot write any data - a bank is busy */
		goto not_complete;
	}
	if (sc->sc_dv_addr != 0xFF) {
		/* set new address */
		ats_otg_set_address(sc, sc->sc_dv_addr);
	}
	return (0);			/* complete */

not_complete:
	/* we only want to know if there is a SETUP packet or free IN packet */
	ATS_OTG_WRITE_4(sc, USBHS_DEVEPTIER(td->ep_no),
	    USBHS_DEVEPTIER_RXSTP | USBHS_DEVEPTIER_TXIN);
	ATS_OTG_WRITE_4(sc, USBHS_DEVEPTIDR(td->ep_no),
	    USBHS_DEVEPTIDR_RXOUT);
	ATS_OTG_WRITE_4(sc, USBHS_DEVIER, USBHS_DEVIER_PEP_MASK(td->ep_no));
	return (1);			/* not complete */
}

static uint8_t
ats_otg_host_channel_alloc(struct ats_otg_softc *sc, struct ats_otg_td *td,
    uint32_t ep_token)
{
	uint32_t temp;
	uint8_t x;

	if (td->channel < ATS_OTG_MAX_HOST_CHANNELS)
		return (0);		/* already allocated */

	/* check if device is suspended */
	if (ATS_OTG_PC2UDEV(td->pc)->flags.self_suspended != 0)
		return (1);		/* busy - cannot transfer data */

	/* compute key */
	temp = td->dev_index | (td->ep_no << 8) | (td->ep_type << 16);
	if (td->ep_type == UE_CONTROL ||
	    ep_token == USBHS_HSTPIPCFG_PTOKEN_IN) {
		temp |= 0x8000;
	}

	for (x = 0; x != ATS_OTG_MAX_HOST_CHANNELS; x++) {
		/* check if key matches */
		if (sc->sc_chan_state[x].key == temp)
			break;
		/* check if we should allocate a new host pipe */
		if (sc->sc_chan_state[x].key == 0) {
			uint8_t y;

			if ((sc->sc_host_memory_used + td->max_packet_size) >
			    ATS_OTG_MAX_HOST_MEMORY || td->max_packet_size > 1024)
				return (1);	/* busy - out of memory */
			if (td->ep_type == UE_CONTROL) {
				y = 3;	/* 64-bytes */
			} else {
				for (y = 0; y != 8; y++) {
					if (td->max_packet_size <= (1U << (y + 3)))
						break;
				}
			}
			/* allocate key */
			sc->sc_chan_state[x].key = temp;

			/* enable host pipe */
			temp = ATS_OTG_READ_4(sc, USBHS_HSTPIP);
			temp |= USBHS_HSTPIP_PEN(x);
			ATS_OTG_WRITE_4(sc, USBHS_HSTPIP, temp);

			/* allocate FIFO */
			ATS_OTG_WRITE_4(sc, USBHS_HSTPIPCFG(x),
			    USBHS_HSTPIPCFG_PSIZE(y) |
			    USBHS_HSTPIPCFG_PBK_1 |
			    USBHS_HSTPIPCFG_ALLOC |
			    ep_token |
			    USBHS_HSTPIPCFG_PEPNUM(td->ep_no) |
			    USBHS_HSTPIPCFG_PTYPE(td->ep_type));

			/* update host memory used */
			sc->sc_host_memory_used += (1U << (y + 3));
			break;
		}
	}
	if (x == ATS_OTG_MAX_HOST_CHANNELS)
		return (1);		/* busy - out of channels */

	/* update token, if any */
	temp = ATS_OTG_READ_4(sc, USBHS_HSTPIPCFG(x));
	temp &= ~USBHS_HSTPIPCFG_PTOKEN(-1U);
	temp |= ep_token;
	ATS_OTG_WRITE_4(sc, USBHS_HSTPIPCFG(x), temp);

	/* reset host pipe, if any */
	if (ep_token == USBHS_HSTPIPCFG_PTOKEN_SETUP) {
		/* reset host pipe */
		temp = ATS_OTG_READ_4(sc, USBHS_HSTPIP);
		ATS_OTG_WRITE_4(sc, USBHS_HSTPIP, temp |
		    USBHS_HSTPIP_PRST(x));
		ATS_OTG_WRITE_4(sc, USBHS_HSTPIP, temp);
	} else if (td->ep_type != UE_CONTROL && td->toggle == 0) {
		/* reset host pipe */
		temp = ATS_OTG_READ_4(sc, USBHS_HSTPIP);
		ATS_OTG_WRITE_4(sc, USBHS_HSTPIP, temp |
		    USBHS_HSTPIP_PRST(x));
		ATS_OTG_WRITE_4(sc, USBHS_HSTPIP, temp);

		/* reset data toggle */
		ATS_OTG_WRITE_4(sc, USBHS_HSTPIPIER(x),
		    USBHS_HSTPIPIER_RSTDT);
	}

	/* update host address, if any */
	temp = ATS_OTG_READ_4(sc, USBHS_HSTADDR(x / 4));
	temp &= ~USBHS_HSTADDR_ADDR(-1U, x % 4);
	temp |= USBHS_HSTADDR_ADDR(td->dev_addr, x % 4);
	ATS_OTG_WRITE_4(sc, USBHS_HSTADDR(x / 4), temp);

	/* enable host pipe interrupts */
	ATS_OTG_WRITE_4(sc, USBHS_HSTIER, USBHS_HSTIER_PEP(x));

	td->channel = x;

	return (0);			/* allocated */
}

static void
ats_otg_host_channel_free(struct ats_otg_softc *sc, struct ats_otg_td *td)
{
	uint8_t x;

	if (td->channel == ATS_OTG_MAX_HOST_CHANNELS)
		return;			/* already freed */

	/* free channel */
	x = td->channel;
	td->channel = ATS_OTG_MAX_HOST_CHANNELS;

	DPRINTF("CH=%d\n", x);

	/* disable token generation */
	ATS_OTG_WRITE_4(sc, USBHS_HSTPIPIER(x), USBHS_HSTPIPIER_PFREEZ);

	/* disable interrupts */
	ATS_OTG_WRITE_4(sc, USBHS_HSTIDR, USBHS_HSTIDR_PEP(x));
	ATS_OTG_WRITE_4(sc, USBHS_HSTPIPIDR(x),
	    USBHS_HSTPIPIDR_NBUSYBK |
	    USBHS_HSTPIPIDR_SHORTPACKET |
	    USBHS_HSTPIPIDR_RXSTALLED |
	    USBHS_HSTPIPIDR_CRCERR |
	    USBHS_HSTPIPIDR_OVERF |
	    USBHS_HSTPIPIDR_NAKED |
	    USBHS_HSTPIPIDR_PERR |
	    USBHS_HSTPIPIDR_TXSTP |
	    USBHS_HSTPIPIDR_TXOUT |
	    USBHS_HSTPIPIDR_RXIN);

	/* clear pending interrupts */
	ATS_OTG_WRITE_4(sc, USBHS_HSTPIPICR(x), -1U);

	/* clear PIPE errors */
	ATS_OTG_WRITE_4(sc, USBHS_HSTPIPERR(x), 0);
}

static uint8_t
ats_otg_host_setup_tx(struct ats_otg_softc *sc, struct ats_otg_td *td)
{
	struct usb_device_request req __aligned(4);
	uint32_t fifo_off;
	uint32_t temp;

	/* try to allocate a free channel */
	if (ats_otg_host_channel_alloc(sc, td, USBHS_HSTPIPCFG_PTOKEN_SETUP))
		return (1);	/* busy */

	/* get endpoint status */
	temp = ATS_OTG_READ_4(sc, USBHS_HSTPIPISR(td->channel));

	/* clear all interrupts */
	ATS_OTG_WRITE_4(sc, USBHS_HSTPIPICR(td->channel), temp);

	DPRINTFN(5, "HSTPIPISR(%d)=0x%08x\n", td->channel, temp);

	/* check transfer status */
	if (temp & (USBHS_HSTPIPISR_RXSTALLED | USBHS_HSTPIPISR_PERR)) {
		td->error_any = 1;
		ats_otg_host_channel_free(sc, td);
		return (0);		/* complete */
	} else if (temp & USBHS_DEVEPTISR_NBUSYBK(-1U)) {
		/* cannot write any data - a bank is busy */
		return (1);		/* busy */
	} else if (td->remainder == 0) {
		/* we are complete */
		ats_otg_host_channel_free(sc, td);
		/* update data toggle */
		td->toggle ^= 1;
		return (0);		/* complete */
	} else if (td->remainder != sizeof(req)) {
		DPRINTFN(0, "Unsupported SETUP packet "
		    "length, %d bytes\n", td->remainder);
		td->error_any = 1;
		ats_otg_host_channel_free(sc, td);
		return (0);		/* complete */
	}
	/* copy data into real buffer */
	usbd_copy_out(td->pc, 0, &req, sizeof(req));

	/* get FIFO offset */
	fifo_off = ATS_OTG_FIFO_OFFSET(td->channel);

	/* write setup packet to FIFO */
	ATS_OTG_WRITE_FIFO_1(sc, fifo_off, (void *)&req, sizeof(req));

	/* update offset and remainder */
	td->offset += sizeof(req);
	td->remainder -= sizeof(req);

	/* avoid extra interrupt */
	ATS_OTG_WRITE_4(sc, USBHS_HSTPIPICR(td->channel),
	    USBHS_HSTPIPICR_TXSTP);

	/* enable interrupts */
	ATS_OTG_WRITE_4(sc, USBHS_HSTPIPIER(td->channel),
	    USBHS_HSTPIPIER_RXSTALLED |
	    USBHS_HSTPIPIER_PERR |
	    USBHS_HSTPIPIER_TXSTP);

	/* allocate FIFO bank and start token generation */
	ATS_OTG_WRITE_4(sc, USBHS_HSTPIPIDR(td->channel),
	    USBHS_HSTPIPIDR_FIFOCON | USBHS_HSTPIPIDR_PFREEZ);

	return (1);			/* busy */
}

static uint8_t
ats_otg_host_data_rx(struct ats_otg_softc *sc, struct ats_otg_td *td)
{
	struct usb_page_search buf_res;
	uint32_t fifo_off;
	uint32_t temp;
	uint16_t count;
	uint8_t got_short;

	/* try to allocate a free channel */
	if (ats_otg_host_channel_alloc(sc, td, USBHS_HSTPIPCFG_PTOKEN_IN))
		return (1);	/* busy */

	got_short = 0;

	/* get endpoint status */
	temp = ATS_OTG_READ_4(sc, USBHS_HSTPIPISR(td->channel));

	/* clear all interrupts */
	ATS_OTG_WRITE_4(sc, USBHS_HSTPIPICR(td->channel), temp);

	DPRINTFN(5, "HSTPIPISR(%u)=0x%08x rem=%u\n",
	    td->channel, temp, td->remainder);

	/* check for transfer errors */
	if (temp & USBHS_HSTPIPISR_RXSTALLED) {
		td->error_any = 1;
		td->error_stall = 1;
		ats_otg_host_channel_free(sc, td);
		return (0);		/* complete */
	} else if (temp & USBHS_HSTPIPISR_PERR) {
		td->error_any = 1;
		ats_otg_host_channel_free(sc, td);
		return (0);		/* complete */
	} else if ((temp & USBHS_DEVEPTISR_NBUSYBK(-1U)) == 0) {
		/* cannot read any data */
		if (td->did_nak == 0)
			goto start_in;
		return (1);		/* busy */
	}
	/* get the packet byte count */
	count = (temp & USBHS_HSTPIPISR_BYCT(-1U)) /
	    USBHS_HSTPIPISR_BYCT(1);

	/* verify the packet byte count */
	if (count != td->max_packet_size) {
		if (count < td->max_packet_size) {
			/* we have a short packet */
			td->short_pkt = 1;
			got_short = 1;
		} else {
			/* invalid USB packet */
			td->error_any = 1;
			ats_otg_host_channel_free(sc, td);
			return (0);	/* we are complete */
		}
	}
	/* verify the packet byte count */
	if (count > td->remainder) {
		/* invalid USB packet */
		td->error_any = 1;
		ats_otg_host_channel_free(sc, td);
		return (0);		/* we are complete */
	}
	/* get FIFO offset */
	fifo_off = ATS_OTG_FIFO_OFFSET(td->channel);

	while (count > 0) {
		usbd_get_page(td->pc, td->offset, &buf_res);

		/* get correct length */
		if (buf_res.length > count) {
			buf_res.length = count;
		}
		/* read data from FIFO */
		ATS_OTG_READ_FIFO_1(sc, fifo_off,
		    buf_res.buffer, buf_res.length);

		/* update counters */
		count -= buf_res.length;
		td->offset += buf_res.length;
		td->remainder -= buf_res.length;
	}

	/* update data toggle */
	td->toggle ^= 1;

	/* release FIFO bank */
	ATS_OTG_WRITE_4(sc, USBHS_HSTPIPIDR(td->ep_no), USBHS_HSTPIPIDR_FIFOCON);

	/* check if we are complete */
	if ((td->remainder == 0) || got_short) {
		if (td->short_pkt) {
			/* we are complete */
			ats_otg_host_channel_free(sc, td);
			return (0);
		}
		/* else need to receive a zero length packet */
	}
start_in:
	/* only do it once */
	td->did_nak = 1;

	/* enable interrupts */
	ATS_OTG_WRITE_4(sc, USBHS_HSTPIPIER(td->channel),
	    USBHS_HSTPIPIER_RXSTALLED |
	    USBHS_HSTPIPIER_PERR |
	    USBHS_HSTPIPIER_RXIN);

	/* start generating one IN packet */
	ATS_OTG_WRITE_4(sc, USBHS_HSTPIPIDR(td->channel), USBHS_HSTPIPIDR_PFREEZ);
	return (1);			/* not complete */
}

static uint8_t
ats_otg_host_data_tx(struct ats_otg_softc *sc, struct ats_otg_td *td)
{
	struct usb_page_search buf_res;
	uint32_t fifo_off;
	uint32_t temp;
	uint16_t count;

	/* try to allocate a free channel */
	if (ats_otg_host_channel_alloc(sc, td, USBHS_HSTPIPCFG_PTOKEN_OUT))
		return (1);	/* busy */

	/* get endpoint status */
	temp = ATS_OTG_READ_4(sc, USBHS_HSTPIPISR(td->channel));

	/* clear all interrupts */
	ATS_OTG_WRITE_4(sc, USBHS_HSTPIPICR(td->channel), temp);

	DPRINTFN(5, "HSTPIPISR(%u)=0x%08x rem=%u\n",
	    td->channel, temp, td->remainder);

	/* check for transfer errors */
	if (temp & USBHS_HSTPIPISR_RXSTALLED) {
		td->error_any = 1;
		td->error_stall = 1;
		ats_otg_host_channel_free(sc, td);
		return (0);		/* complete */
	} else if (temp & USBHS_HSTPIPISR_PERR) {
		td->error_any = 1;
		ats_otg_host_channel_free(sc, td);
		return (0);		/* complete */
	} else if (temp & USBHS_DEVEPTISR_NBUSYBK(-1U)) {
		/* cannot write any data - a bank is busy */
		return (1);		/* busy */
	} else if (td->remainder == 0) {
		if (td->short_pkt) {
			ats_otg_host_channel_free(sc, td);
			return (0);	/* complete */
		}
		/* else we need to transmit a short packet */
	}
	/* update data toggle */
	td->toggle ^= 1;

	count = td->max_packet_size;
	if (td->remainder < count) {
		/* we have a short packet */
		td->short_pkt = 1;
		count = td->remainder;
	}

	/* get FIFO offset */
	fifo_off = ATS_OTG_FIFO_OFFSET(td->channel);

	while (count > 0) {

		usbd_get_page(td->pc, td->offset, &buf_res);

		/* get correct length */
		if (buf_res.length > count) {
			buf_res.length = count;
		}
		/* write data to FIFO */
		ATS_OTG_WRITE_FIFO_1(sc, fifo_off,
		    buf_res.buffer, buf_res.length);

		/* update counters */
		count -= buf_res.length;
		td->offset += buf_res.length;
		td->remainder -= buf_res.length;
	}

	/* avoid extra interrupt */
	ATS_OTG_WRITE_4(sc, USBHS_HSTPIPICR(td->channel),
	    USBHS_HSTPIPICR_TXOUT);

	/* enable interrupts */
	ATS_OTG_WRITE_4(sc, USBHS_HSTPIPIER(td->channel),
	    USBHS_HSTPIPIER_RXSTALLED |
	    USBHS_HSTPIPIER_PERR |
	    USBHS_HSTPIPIER_TXOUT);

	/* allocate FIFO bank and start token generation */
	ATS_OTG_WRITE_4(sc, USBHS_HSTPIPIDR(td->channel),
	    USBHS_HSTPIPIDR_FIFOCON | USBHS_HSTPIPIDR_PFREEZ);

	return (1);			/* not complete */
}

static void
ats_otg_xfer_do_fifo(struct ats_otg_softc *sc, struct usb_xfer *xfer)
{
	struct ats_otg_td *td;
	uint8_t toggle;

	DPRINTFN(9, "\n");

	td = xfer->td_transfer_cache;
	if (td == NULL)
		return;

	while (1) {
		if ((td->func) (sc, td)) {
			/* operation in progress */
			break;
		}
		if (((void *)td) == xfer->td_transfer_last) {
			goto done;
		}
		if (td->error_any) {
			goto done;
		} else if (td->remainder > 0) {
			/*
			 * We had a short transfer. If there is no alternate
			 * next, stop processing !
			 */
			if (!td->alt_next) {
				goto done;
			}
		}
		/*
		 * Fetch the next transfer descriptor and transfer
		 * some flags to the next transfer descriptor
		 */
		toggle = td->toggle;
		td = td->obj_next;
		xfer->td_transfer_cache = td;
		td->toggle = toggle;	/* transfer toggle */
	}
	return;

done:
	xfer->td_transfer_cache = NULL;
	sc->sc_xfer_complete = 1;
}

static void
ats_otg_update_host_transfer_schedule_locked(struct ats_otg_softc *sc)
{
	TAILQ_HEAD(, usb_xfer) head;
	struct usb_xfer *xfer;
	struct usb_xfer *xfer_next;
	struct ats_otg_td *td;

	TAILQ_INIT(&head);

	TAILQ_FOREACH_SAFE(xfer, &sc->sc_bus.intr_q.head, wait_entry, xfer_next) {
		td = xfer->td_transfer_cache;
		if (td == NULL || td->ep_type != UE_ISOCHRONOUS)
			continue;

		TAILQ_REMOVE(&sc->sc_bus.intr_q.head, xfer, wait_entry);
		TAILQ_INSERT_TAIL(&head, xfer, wait_entry);
	}
	TAILQ_FOREACH_SAFE(xfer, &sc->sc_bus.intr_q.head, wait_entry, xfer_next) {
		td = xfer->td_transfer_cache;
		if (td == NULL || td->ep_type != UE_INTERRUPT)
			continue;
		TAILQ_REMOVE(&sc->sc_bus.intr_q.head, xfer, wait_entry);
		TAILQ_INSERT_TAIL(&head, xfer, wait_entry);
	}
	TAILQ_FOREACH_SAFE(xfer, &sc->sc_bus.intr_q.head, wait_entry, xfer_next) {
		td = xfer->td_transfer_cache;
		if (td == NULL || td->ep_type != UE_CONTROL)
			continue;
		TAILQ_REMOVE(&sc->sc_bus.intr_q.head, xfer, wait_entry);
		TAILQ_INSERT_TAIL(&head, xfer, wait_entry);
	}
	TAILQ_FOREACH_SAFE(xfer, &sc->sc_bus.intr_q.head, wait_entry, xfer_next) {
		td = xfer->td_transfer_cache;
		if (td == NULL || td->ep_type != UE_BULK)
			continue;
		TAILQ_REMOVE(&sc->sc_bus.intr_q.head, xfer, wait_entry);
		TAILQ_INSERT_TAIL(&head, xfer, wait_entry);
	}

	/* Put transfers in execution order at the end */
	TAILQ_CONCAT(&sc->sc_bus.intr_q.head, &head, wait_entry);
}

static void
ats_otg_interrupt_poll_locked(struct ats_otg_softc *sc)
{
	struct usb_xfer *xfer;

	if (sc->sc_flags.status_device_mode == 0) {
		/* Update host transfer schedule */
		ats_otg_update_host_transfer_schedule_locked(sc);
	}
	/* execute FIFOs */
	TAILQ_FOREACH(xfer, &sc->sc_bus.intr_q.head, wait_entry)
	    ats_otg_xfer_do_fifo(sc, xfer);
}

static uint8_t
ats_otg_xfer_do_complete_locked(struct ats_otg_softc *sc, struct usb_xfer *xfer)
{
	struct ats_otg_td *td;

	DPRINTFN(9, "\n");

	td = xfer->td_transfer_cache;
	if (td == NULL) {
		/* compute all actual lengths */
		ats_otg_standard_done(xfer);
		return (1);
	}
	return (0);
}

static void
ats_otg_interrupt_complete_locked(struct ats_otg_softc *sc)
{
	struct usb_xfer *xfer;

repeat:
	/* scan for completion events */
	TAILQ_FOREACH(xfer, &sc->sc_bus.intr_q.head, wait_entry) {
		if (ats_otg_xfer_do_complete_locked(sc, xfer))
			goto repeat;
	}
}

int
ats_otg_filter_interrupt(void *arg)
{
	struct ats_otg_softc *sc = arg;
	int retval = FILTER_HANDLED;
	uint32_t temp;

	USB_BUS_SPIN_LOCK(&sc->sc_bus);

	/* read and clear device interrupt status */
	temp = ATS_OTG_READ_4(sc, USBHS_DEVISR);

	/* clear interrupts we are handling here */
	ATS_OTG_WRITE_4(sc, USBHS_DEVICR, temp & ~ATS_OTG_DEVIMR_THREAD_IRQ);

	/* check for USB state change interrupts */
	if ((temp & ATS_OTG_DEVIMR_THREAD_IRQ) != 0)
		retval = FILTER_SCHEDULE_THREAD;

	/* read and clear host interrupt status */
	temp = ATS_OTG_READ_4(sc, USBHS_HSTISR);

	/* clear interrupts we are handling here */
	ATS_OTG_WRITE_4(sc, USBHS_HSTICR, temp & ~ATS_OTG_HSTIMR_THREAD_IRQ);

	/* check for USB state change interrupts */
	if ((temp & ATS_OTG_HSTIMR_THREAD_IRQ) != 0)
		retval = FILTER_SCHEDULE_THREAD;

	/* poll FIFOs, if any */
	ats_otg_interrupt_poll_locked(sc);

	if (sc->sc_xfer_complete != 0)
		retval = FILTER_SCHEDULE_THREAD;

	USB_BUS_SPIN_UNLOCK(&sc->sc_bus);

	return (retval);
}

void
ats_otg_interrupt(void *arg)
{
	struct ats_otg_softc *sc = arg;
	uint32_t temp;

	USB_BUS_LOCK(&sc->sc_bus);
	USB_BUS_SPIN_LOCK(&sc->sc_bus);

	/* read and clear device interrupt status */
	temp = ATS_OTG_READ_4(sc, USBHS_DEVISR);

	/* clear interrupts we are handling here */
	ATS_OTG_WRITE_4(sc, USBHS_DEVICR, temp & ATS_OTG_DEVIMR_THREAD_IRQ);

	DPRINTFN(14, "DEVISR=0x%08x\n", temp);

	/* check for any bus state change interrupts */
	if (temp & USBHS_DEVISR_EORST) {

		uint32_t speed;

		DPRINTFN(5, "end of reset\n");

		/* set correct state */
		sc->sc_flags.status_device_mode = 1;
		sc->sc_flags.status_bus_reset = 1;
		sc->sc_flags.status_suspend = 0;
		sc->sc_flags.change_suspend = 0;
		sc->sc_flags.change_connect = 1;
		sc->sc_flags.status_high_speed = 0;
		sc->sc_flags.status_low_speed = 0;
		sc->sc_flags.port_enabled = 1;

		/* reset FIFOs */
		ats_otg_init_fifo(sc, ATS_MODE_DEVICE);

		/* reset function address */
		ats_otg_set_address(sc, 0);

		/* figure out enumeration speed */
		speed = ATS_OTG_READ_4(sc, USBHS_SR) & USBHS_SR_SPEED(-1U);
		if (speed == USBHS_SR_SPEED_HIGH)
			sc->sc_flags.status_high_speed = 1;
		else if (speed == USBHS_SR_SPEED_LOW)
			sc->sc_flags.status_low_speed = 1;

		/* Disable resume and enable suspend interrupt */
		ATS_OTG_WRITE_4(sc, USBHS_DEVIDR, USBHS_DEVIDR_WAKEUP);
		ATS_OTG_WRITE_4(sc, USBHS_DEVIER, USBHS_DEVIER_SUSP);

		/* complete root HUB interrupt endpoint */
		ats_otg_root_intr(sc);
	}
	/*
	 * If resume and suspend is set at the same time we interpret
	 * that like RESUME. Resume is set when there is at least 3
	 * milliseconds of inactivity on the USB BUS.
	 */
	if (temp & USBHS_DEVISR_WAKEUP) {

		DPRINTFN(5, "resume interrupt\n");

		ats_otg_resume_irq(sc);

	} else if (temp & USBHS_DEVISR_SUSP) {

		DPRINTFN(5, "suspend interrupt\n");

		ats_otg_suspend_irq(sc);
	}
	/* read and clear host interrupt status */
	temp = ATS_OTG_READ_4(sc, USBHS_HSTISR);

	/* clear interrupts we are handling here */
	ATS_OTG_WRITE_4(sc, USBHS_HSTICR, temp & ATS_OTG_HSTIMR_THREAD_IRQ);

	DPRINTFN(14, "HSTISR=0x%08x\n", temp);

	if (temp & USBHS_HSTISR_DCONN) {
		sc->sc_flags.status_device_mode = 0;
		sc->sc_flags.status_bus_reset = 1;
		sc->sc_flags.change_connect = 1;

		/* complete root HUB interrupt endpoint */
		ats_otg_root_intr(sc);
	} else if (temp & USBHS_HSTISR_DDISC) {
		sc->sc_flags.status_device_mode = 0;
		sc->sc_flags.status_bus_reset = 0;
		sc->sc_flags.change_connect = 1;
		sc->sc_flags.port_enabled = 0;
		sc->sc_flags.change_enabled = 1;

		/* complete root HUB interrupt endpoint */
		ats_otg_root_intr(sc);
	} else if (temp & USBHS_HSTISR_RST) {
		uint32_t speed;

		sc->sc_flags.status_bus_reset = 1;
		sc->sc_flags.status_device_mode = 0;
		sc->sc_flags.status_high_speed = 0;
		sc->sc_flags.status_low_speed = 0;
		sc->sc_flags.port_enabled = 1;

		/* figure out enumeration speed */
		speed = ATS_OTG_READ_4(sc, USBHS_SR) & USBHS_SR_SPEED(-1U);
		if (speed == USBHS_SR_SPEED_HIGH)
			sc->sc_flags.status_high_speed = 1;
		else if (speed == USBHS_SR_SPEED_LOW)
			sc->sc_flags.status_low_speed = 1;

		/* complete root HUB interrupt endpoint */
		ats_otg_root_intr(sc);
	}
	if (sc->sc_flags.status_device_mode == 0 &&
	    sc->sc_flags.status_bus_reset != 0) {
		uint32_t sof;

		sof = ATS_OTG_READ_4(sc, USBHS_HSTCTRL);
		if (sof & USBHS_HSTCTRL_SOFE)
			ats_otg_resume_irq(sc);
		else
			ats_otg_suspend_irq(sc);
	}
	if (sc->sc_xfer_complete != 0) {
		sc->sc_xfer_complete = 0;

		/* complete FIFOs, if any */
		ats_otg_interrupt_complete_locked(sc);
	}
	USB_BUS_SPIN_UNLOCK(&sc->sc_bus);
	USB_BUS_UNLOCK(&sc->sc_bus);
}

static void
ats_otg_setup_standard_chain_sub(struct ats_otg_std_temp *temp)
{
	struct ats_otg_td *td;

	/* get current Transfer Descriptor */
	td = temp->td_next;
	temp->td = td;

	/* prepare for next TD */
	temp->td_next = td->obj_next;

	/* fill out the Transfer Descriptor */
	td->func = temp->func;
	td->pc = temp->pc;
	td->offset = temp->offset;
	td->remainder = temp->len;
	td->error_any = 0;
	td->error_stall = 0;
	td->did_stall = temp->did_stall;
	td->short_pkt = temp->short_pkt;
	td->alt_next = temp->setup_alt_next;
	td->set_toggle = 0;
	td->got_short = 0;
	td->did_nak = 0;
	td->channel = ATS_OTG_MAX_HOST_CHANNELS;
	td->errcnt = 0;
}

static void
ats_otg_setup_standard_chain(struct usb_xfer *xfer)
{
	struct ats_otg_std_temp temp;
	struct ats_otg_td *td;
	uint32_t x;
	uint8_t need_sync;
	uint8_t is_host;

	DPRINTFN(9, "addr=%d endpt=%d sumlen=%d speed=%d\n",
	    xfer->address, UE_GET_ADDR(xfer->endpointno),
	    xfer->sumlen, usbd_get_speed(xfer->xroot->udev));

	temp.max_frame_size = xfer->max_frame_size;

	td = xfer->td_start[0];
	xfer->td_transfer_first = td;
	xfer->td_transfer_cache = td;

	/* setup temp */

	temp.pc = NULL;
	temp.td = NULL;
	temp.td_next = xfer->td_start[0];
	temp.offset = 0;
	temp.setup_alt_next = xfer->flags_int.short_frames_ok ||
	    xfer->flags_int.isochronous_xfr;
	temp.did_stall = !xfer->flags_int.control_stall;

	is_host = (xfer->xroot->udev->flags.usb_mode == USB_MODE_HOST);

	/* check if we should prepend a setup message */

	if (xfer->flags_int.control_xfr) {
		if (xfer->flags_int.control_hdr) {

			if (is_host)
				temp.func = &ats_otg_host_setup_tx;
			else
				temp.func = &ats_otg_device_setup_rx;

			temp.len = xfer->frlengths[0];
			temp.pc = xfer->frbuffers + 0;
			temp.short_pkt = temp.len ? 1 : 0;

			/* check for last frame */
			if (xfer->nframes == 1) {
				/* no STATUS stage yet, SETUP is last */
				if (xfer->flags_int.control_act)
					temp.setup_alt_next = 0;
			}
			ats_otg_setup_standard_chain_sub(&temp);
		}
		x = 1;
	} else {
		x = 0;
	}

	if (x != xfer->nframes) {
		if (xfer->endpointno & UE_DIR_IN) {
			if (is_host) {
				temp.func = &ats_otg_host_data_rx;
				need_sync = 0;
			} else {
				temp.func = &ats_otg_device_data_tx;
				need_sync = 1;
			}
		} else {
			if (is_host) {
				temp.func = &ats_otg_host_data_tx;
				need_sync = 0;
			} else {
				temp.func = &ats_otg_device_data_rx;
				need_sync = 0;
			}
		}

		/* setup "pc" pointer */
		temp.pc = xfer->frbuffers + x;
	} else {
		need_sync = 0;
	}
	while (x != xfer->nframes) {

		/* DATA0 / DATA1 message */

		temp.len = xfer->frlengths[x];

		x++;

		if (x == xfer->nframes) {
			if (xfer->flags_int.control_xfr) {
				if (xfer->flags_int.control_act) {
					temp.setup_alt_next = 0;
				}
			} else {
				temp.setup_alt_next = 0;
			}
		}
		if (temp.len == 0) {

			/* make sure that we send an USB packet */

			temp.short_pkt = 0;

		} else {

			/* regular data transfer */

			temp.short_pkt = (xfer->flags.force_short_xfer ? 0 : 1);
		}

		ats_otg_setup_standard_chain_sub(&temp);

		if (xfer->flags_int.isochronous_xfr) {
			temp.offset += temp.len;
		} else {
			/* get next Page Cache pointer */
			temp.pc = xfer->frbuffers + x;
		}
	}

	if (xfer->flags_int.control_xfr) {

		/* always setup a valid "pc" pointer for status and sync */
		temp.pc = xfer->frbuffers + 0;
		temp.len = 0;
		temp.short_pkt = 0;
		temp.setup_alt_next = 0;

		/* check if we need to sync */
		if (need_sync) {
			/* we need a SYNC point after TX */
			temp.func = &ats_otg_device_data_tx_sync;
			ats_otg_setup_standard_chain_sub(&temp);
		}
		/* check if we should append a status stage */
		if (!xfer->flags_int.control_act) {

			/*
			 * Send a DATA1 message and invert the current
			 * endpoint direction.
			 */
			if (xfer->endpointno & UE_DIR_IN) {
				if (is_host) {
					temp.func = &ats_otg_host_data_tx;
					need_sync = 0;
				} else {
					temp.func = &ats_otg_device_data_rx;
					need_sync = 0;
				}
			} else {
				if (is_host) {
					temp.func = &ats_otg_host_data_rx;
					need_sync = 0;
				} else {
					temp.func = &ats_otg_device_data_tx;
					need_sync = 1;
				}
			}

			ats_otg_setup_standard_chain_sub(&temp);

			/* data toggle should be DATA1 */
			td = temp.td;
			td->set_toggle = 1;

			if (need_sync) {
				/* we need a SYNC point after TX */
				temp.func = &ats_otg_device_data_tx_sync;
				ats_otg_setup_standard_chain_sub(&temp);
			}
		}
	} else {
		/* check if we need to sync */
		if (need_sync) {

			temp.pc = xfer->frbuffers + 0;
			temp.len = 0;
			temp.short_pkt = 0;
			temp.setup_alt_next = 0;

			/* we need a SYNC point after TX */
			temp.func = &ats_otg_device_data_tx_sync;
			ats_otg_setup_standard_chain_sub(&temp);
		}
	}

	/* must have at least one frame! */
	td = temp.td;
	xfer->td_transfer_last = td;

	if (is_host) {
		/* compute initial data toggle */
		td = xfer->td_transfer_first;
		td->toggle = (xfer->endpoint->toggle_next ? 1 : 0);
	}
}

static void
ats_otg_timeout(void *arg)
{
	struct usb_xfer *xfer = arg;

	DPRINTF("xfer=%p\n", xfer);

	USB_BUS_LOCK_ASSERT(xfer->xroot->bus, MA_OWNED);

	/* transfer is transferred */
	ats_otg_device_done(xfer, USB_ERR_TIMEOUT);
}

static void
ats_otg_start_standard_chain(struct usb_xfer *xfer)
{
	struct ats_otg_softc *sc = ATS_OTG_BUS2SC(xfer->xroot->bus);

	DPRINTFN(9, "\n");

	USB_BUS_SPIN_LOCK(&sc->sc_bus);

	/* poll one time which turns on the endpoint interrupts */
	ats_otg_xfer_do_fifo(sc, xfer);

	if (ats_otg_xfer_do_complete_locked(sc, xfer) == 0) {
		/* put transfer on interrupt queue */
		usbd_transfer_enqueue(&xfer->xroot->bus->intr_q, xfer);

		/* start timeout, if any */
		if (xfer->timeout != 0) {
			usbd_transfer_timeout_ms(xfer,
			    &ats_otg_timeout, xfer->timeout);
		}
	}
	USB_BUS_SPIN_UNLOCK(&sc->sc_bus);
}

static void
ats_otg_root_intr(struct ats_otg_softc *sc)
{
	DPRINTFN(9, "\n");

	USB_BUS_LOCK_ASSERT(&sc->sc_bus, MA_OWNED);

	/* set port bit */
	sc->sc_hub_idata[0] = 0x02;	/* we only have one port */

	uhub_root_intr(&sc->sc_bus, sc->sc_hub_idata,
	    sizeof(sc->sc_hub_idata));
}

static usb_error_t
ats_otg_standard_done_sub(struct usb_xfer *xfer)
{
	struct ats_otg_td *td;
	uint32_t len;
	usb_error_t error;

	DPRINTFN(9, "\n");

	td = xfer->td_transfer_cache;

	do {
		len = td->remainder;

		/* store last data toggle */
		xfer->endpoint->toggle_next = td->toggle;

		if (xfer->aframes != xfer->nframes) {
			/*
			 * Verify the length and subtract
			 * the remainder from "frlengths[]":
			 */
			if (len > xfer->frlengths[xfer->aframes]) {
				td->error_any = 1;
			} else {
				xfer->frlengths[xfer->aframes] -= len;
			}
		}
		/* Check for transfer error */
		if (td->error_any) {
			/* the transfer is finished */
			error = (td->error_stall ?
			    USB_ERR_STALLED : USB_ERR_IOERROR);
			td = NULL;
			break;
		}
		/* Check for short transfer */
		if (len > 0) {
			if (xfer->flags_int.short_frames_ok ||
			    xfer->flags_int.isochronous_xfr) {
				/* follow alt next */
				if (td->alt_next) {
					td = td->obj_next;
				} else {
					td = NULL;
				}
			} else {
				/* the transfer is finished */
				td = NULL;
			}
			error = 0;
			break;
		}
		td = td->obj_next;

		/* this USB frame is complete */
		error = 0;
		break;

	} while (0);

	/* update transfer cache */

	xfer->td_transfer_cache = td;

	return (error);
}

static void
ats_otg_standard_done(struct usb_xfer *xfer)
{
	usb_error_t err = 0;

	DPRINTFN(13, "xfer=%p endpoint=%p transfer done\n",
	    xfer, xfer->endpoint);

	/* reset scanner */

	xfer->td_transfer_cache = xfer->td_transfer_first;

	if (xfer->flags_int.control_xfr) {

		if (xfer->flags_int.control_hdr) {

			err = ats_otg_standard_done_sub(xfer);
		}
		xfer->aframes = 1;

		if (xfer->td_transfer_cache == NULL) {
			goto done;
		}
	}
	while (xfer->aframes != xfer->nframes) {

		err = ats_otg_standard_done_sub(xfer);
		xfer->aframes++;

		if (xfer->td_transfer_cache == NULL) {
			goto done;
		}
	}

	if (xfer->flags_int.control_xfr &&
	    !xfer->flags_int.control_act) {

		err = ats_otg_standard_done_sub(xfer);
	}
done:
	ats_otg_device_done(xfer, err);
}

/*------------------------------------------------------------------------*
 *	ats_otg_device_done
 *
 * NOTE: this function can be called more than one time on the
 * same USB transfer!
 *------------------------------------------------------------------------*/
static void
ats_otg_device_done(struct usb_xfer *xfer, usb_error_t error)
{
	struct ats_otg_softc *sc = ATS_OTG_BUS2SC(xfer->xroot->bus);

	DPRINTFN(9, "xfer=%p, endpoint=%p, error=%d\n",
	    xfer, xfer->endpoint, error);

	USB_BUS_SPIN_LOCK(&sc->sc_bus);

	if (xfer->flags_int.usb_mode == USB_MODE_DEVICE) {
		/* disable interrupts */
		ATS_OTG_WRITE_4(sc, USBHS_DEVIDR,
		    USBHS_DEVIDR_PEP_MASK(xfer->endpointno & UE_ADDR));
	} else {
		struct ats_otg_td *td;

		td = xfer->td_transfer_cache;
		if (td != NULL)
			ats_otg_host_channel_free(sc, td);
	}
	/* dequeue transfer and start next transfer */
	usbd_transfer_done(xfer, error);

	USB_BUS_SPIN_UNLOCK(&sc->sc_bus);
}

static void
ats_otg_xfer_stall(struct usb_xfer *xfer)
{
	ats_otg_device_done(xfer, USB_ERR_STALLED);
}

static void
ats_otg_set_stall(struct usb_device *udev,
    struct usb_endpoint *ep, uint8_t *did_stall)
{
	struct ats_otg_softc *sc;
	uint8_t ep_no;

	USB_BUS_LOCK_ASSERT(udev->bus, MA_OWNED);

	/* check mode */
	if (udev->flags.usb_mode != USB_MODE_DEVICE) {
		/* not supported */
		return;
	}
	sc = ATS_OTG_BUS2SC(udev->bus);

	USB_BUS_SPIN_LOCK(&sc->sc_bus);

	/* get endpoint address */
	ep_no = ep->edesc->bEndpointAddress;

	DPRINTFN(5, "endpoint=0x%x\n", ep_no);

	ATS_OTG_WRITE_4(sc, USBHS_DEVEPTIER(ep_no), USBHS_DEVEPTIER_STALLRQ);

	USB_BUS_SPIN_UNLOCK(&sc->sc_bus);
}

static void
ats_otg_clear_stall_sub_locked(struct ats_otg_softc *sc, uint32_t mps,
    uint8_t ep_no, uint8_t ep_type, uint8_t ep_dir)
{
	uint32_t temp;

	if (ep_type == UE_CONTROL) {
		/* clearing stall is not needed */
		return;
	}
	temp = ATS_OTG_READ_4(sc, USBHS_DEVEPT);
	/* enable endpoint */
	temp |= USBHS_DEVEPT_EPEN(ep_no);
	ATS_OTG_WRITE_4(sc, USBHS_DEVEPT, temp);

	/* reset endpoint */
	ATS_OTG_WRITE_4(sc, USBHS_DEVEPT, temp | USBHS_DEVEPT_EPRST(ep_no));
	ATS_OTG_WRITE_4(sc, USBHS_DEVEPT, temp);

	/* configure endpoint type */
	temp = ATS_OTG_READ_4(sc, USBHS_DEVEPTCFG(ep_no));
	temp &= ~(USBHS_DEVEPTCFG_EPTYPE(-1U) | USBHS_DEVEPTCFG_EPDIR);
	temp |= USBHS_DEVEPTCFG_EPTYPE(ep_type);
	if (ep_dir)
		temp |= USBHS_DEVEPTCFG_EPDIR;
	ATS_OTG_WRITE_4(sc, USBHS_DEVEPTCFG(ep_no), temp);

	/* reset data tooggle and clear stall */
	ATS_OTG_WRITE_4(sc, USBHS_DEVEPTIER(ep_no), USBHS_DEVEPTIER_RSTDT);
	ATS_OTG_WRITE_4(sc, USBHS_DEVEPTIDR(ep_no), USBHS_DEVEPTIDR_STALLRQ);

	/* poll interrupt */
	ats_otg_interrupt_poll_locked(sc);
	ats_otg_interrupt_complete_locked(sc);
}

static void
ats_otg_clear_stall(struct usb_device *udev, struct usb_endpoint *ep)
{
	struct ats_otg_softc *sc;
	struct usb_endpoint_descriptor *ed;

	DPRINTFN(5, "endpoint=%p\n", ep);

	USB_BUS_LOCK_ASSERT(udev->bus, MA_OWNED);

	/* check mode */
	if (udev->flags.usb_mode != USB_MODE_DEVICE) {
		/* not supported */
		return;
	}
	/* get softc */
	sc = ATS_OTG_BUS2SC(udev->bus);

	USB_BUS_SPIN_LOCK(&sc->sc_bus);

	/* get endpoint descriptor */
	ed = ep->edesc;

	/* reset endpoint */
	ats_otg_clear_stall_sub_locked(sc,
	    UGETW(ed->wMaxPacketSize),
	    (ed->bEndpointAddress & UE_ADDR),
	    (ed->bmAttributes & UE_XFERTYPE),
	    (ed->bEndpointAddress & (UE_DIR_IN | UE_DIR_OUT)));

	USB_BUS_SPIN_UNLOCK(&sc->sc_bus);
}

static void
ats_otg_device_state_change(struct usb_device *udev)
{
	/* check mode */
	if (udev->flags.usb_mode != USB_MODE_DEVICE) {
		/* not supported */
		return;
	}
	/* deactivate all other endpoint but the control endpoint */
	if (udev->state == USB_STATE_CONFIGURED ||
	    udev->state == USB_STATE_ADDRESSED) {
		struct ats_otg_softc *sc;
		uint8_t x;

		/* get softc */
		sc = ATS_OTG_BUS2SC(udev->bus);

		USB_BUS_LOCK(&sc->sc_bus);
		ATS_OTG_WRITE_4(sc, USBHS_DEVEPT, USBHS_DEVEPT_EPEN(0));

		for (x = 1; x != ATS_OTG_MAX_DEVICE_ENDPOINTS; x++)
			ATS_OTG_WRITE_4(sc, USBHS_DEVEPTICR(x), -1U);
		USB_BUS_UNLOCK(&sc->sc_bus);
	}
}

/* The FIFO layout must fit within 4K */
static void
ats_otg_init_fifo(struct ats_otg_softc *sc, uint8_t mode)
{
	uint8_t x;

	if (mode == ATS_MODE_HOST) {
		/* reset channel state */
		USB_BUS_SPIN_LOCK(&sc->sc_bus);
		memset(sc->sc_chan_state, 0, sizeof(sc->sc_chan_state));
		sc->sc_host_memory_used = 0;
		USB_BUS_SPIN_UNLOCK(&sc->sc_bus);
	} else {
		for (x = 0; x != ATS_OTG_MAX_DEVICE_ENDPOINTS; x++) {
			if (x == 0 || x >= ATS_OTG_MAX_DEVICE_HS_EP) {
				ATS_OTG_WRITE_4(sc, USBHS_DEVEPTCFG(x),
				    USBHS_DEVEPTCFG_EPSIZE_64 | USBHS_DEVEPTCFG_EPBK_1);
				ATS_OTG_WRITE_4(sc, USBHS_DEVEPTCFG(x),
				    USBHS_DEVEPTCFG_EPSIZE_64 | USBHS_DEVEPTCFG_EPBK_1 |
				    USBHS_DEVEPTCFG_ALLOC);
			} else {
				ATS_OTG_WRITE_4(sc, USBHS_DEVEPTCFG(x),
				    USBHS_DEVEPTCFG_EPSIZE_512 | USBHS_DEVEPTCFG_EPBK_1);
				ATS_OTG_WRITE_4(sc, USBHS_DEVEPTCFG(x),
				    USBHS_DEVEPTCFG_EPSIZE_512 | USBHS_DEVEPTCFG_EPBK_1 |
				    USBHS_DEVEPTCFG_ALLOC);
			}
		}
	}
}

int
ats_otg_init(struct ats_otg_softc *sc)
{
	DPRINTF("start\n");

	/* set up the bus structure */
	sc->sc_bus.usbrev = USB_REV_2_0;
	sc->sc_bus.methods = &ats_otg_bus_methods;

	USB_BUS_LOCK(&sc->sc_bus);

	/* disable USB block */
	ATS_OTG_WRITE_4(sc, USBHS_CTRL, USBHS_CTRL_VBUSHWC);

	/* wait for host to detect disconnect */
	usb_pause_mtx(&sc->sc_bus.bus_mtx, hz / 32);

	switch (sc->sc_mode) {
	case ATS_MODE_HOST:
		ATS_OTG_WRITE_4(sc, USBHS_CTRL,
		    USBHS_CTRL_VBUSHWC | USBHS_CTRL_USBE);

		/* try to set mode early on */
		sc->sc_flags.status_device_mode = 0;
		break;
	default:
		ATS_OTG_WRITE_4(sc, USBHS_CTRL,
		    USBHS_CTRL_VBUSHWC | USBHS_CTRL_USBE |
		    USBHS_CTRL_UIMOD);

		/* try to set mode early on */
		sc->sc_flags.status_device_mode = 1;
		break;
	}

	ATS_OTG_WRITE_4(sc, USBHS_SFR, USBHS_SFR_VBUSRQS);

	/* wait a little bit for block to enable */
	usb_pause_mtx(&sc->sc_bus.bus_mtx, hz / 128);

	/* enable interrupts */
	ATS_OTG_WRITE_4(sc, USBHS_DEVIER, ATS_OTG_DEVIMR_THREAD_IRQ);
	ATS_OTG_WRITE_4(sc, USBHS_HSTIER, ATS_OTG_HSTIMR_THREAD_IRQ);

	USB_BUS_UNLOCK(&sc->sc_bus);

	/* catch any lost interrupts */

	ats_otg_do_poll(&sc->sc_bus);

	return (0);			/* success */
}

void
ats_otg_uninit(struct ats_otg_softc *sc)
{
	USB_BUS_LOCK(&sc->sc_bus);

	/* disable interrupts */
	ATS_OTG_WRITE_4(sc, USBHS_DEVIDR, -1U);
	ATS_OTG_WRITE_4(sc, USBHS_HSTIDR, -1U);

	sc->sc_flags.port_enabled = 0;
	sc->sc_flags.port_powered = 0;
	sc->sc_flags.status_bus_reset = 0;
	sc->sc_flags.status_suspend = 0;
	sc->sc_flags.change_suspend = 0;
	sc->sc_flags.change_connect = 1;

	ats_otg_pull_down(sc);

	/* disable USB block */
	ATS_OTG_WRITE_4(sc, USBHS_CTRL,
	    USBHS_CTRL_VBUSHWC | USBHS_CTRL_FRZCLK);

	USB_BUS_UNLOCK(&sc->sc_bus);
}

static void
ats_otg_suspend(struct ats_otg_softc *sc)
{
	return;
}

static void
ats_otg_resume(struct ats_otg_softc *sc)
{
	return;
}

static void
ats_otg_do_poll(struct usb_bus *bus)
{
	struct ats_otg_softc *sc = ATS_OTG_BUS2SC(bus);

	USB_BUS_LOCK(&sc->sc_bus);
	USB_BUS_SPIN_LOCK(&sc->sc_bus);
	ats_otg_interrupt_poll_locked(sc);
	ats_otg_interrupt_complete_locked(sc);
	USB_BUS_SPIN_UNLOCK(&sc->sc_bus);
	USB_BUS_UNLOCK(&sc->sc_bus);
}

/*------------------------------------------------------------------------*
 * ATS OTG BULK support
 * ATS OTG CONTROL support
 * ATS OTG INTERRUPT support
 *------------------------------------------------------------------------*/
static void
ats_otg_non_isoc_open(struct usb_xfer *xfer)
{
}

static void
ats_otg_non_isoc_close(struct usb_xfer *xfer)
{
	ats_otg_device_done(xfer, USB_ERR_CANCELLED);
}

static void
ats_otg_non_isoc_enter(struct usb_xfer *xfer)
{
}

static void
ats_otg_non_isoc_start(struct usb_xfer *xfer)
{
	/* setup TDs */
	ats_otg_setup_standard_chain(xfer);
	ats_otg_start_standard_chain(xfer);
}

static const struct usb_pipe_methods ats_otg_non_isoc_methods =
{
	.open = ats_otg_non_isoc_open,
	.close = ats_otg_non_isoc_close,
	.enter = ats_otg_non_isoc_enter,
	.start = ats_otg_non_isoc_start,
};

/*------------------------------------------------------------------------*
 * ATS OTG ISOCHRONOUS support
 *------------------------------------------------------------------------*/
static void
ats_otg_isoc_open(struct usb_xfer *xfer)
{
}

static void
ats_otg_isoc_close(struct usb_xfer *xfer)
{
	ats_otg_device_done(xfer, USB_ERR_CANCELLED);
}

static void
ats_otg_isoc_enter(struct usb_xfer *xfer)
{
}

static void
ats_otg_isoc_start(struct usb_xfer *xfer)
{
	struct ats_otg_softc *sc = ATS_OTG_BUS2SC(xfer->xroot->bus);
	uint32_t temp;
	uint32_t msframes;
	uint32_t framenum;
	uint8_t shift = usbd_xfer_get_fps_shift(xfer);

	DPRINTFN(6, "xfer=%p next=%d nframes=%d\n",
	    xfer, xfer->endpoint->isoc_next, xfer->nframes);

	if (xfer->xroot->udev->flags.usb_mode == USB_MODE_HOST) {
		temp = ATS_OTG_READ_4(sc, USBHS_HSTFNUM);
	} else {
		temp = ATS_OTG_READ_4(sc, USBHS_DEVFNUM);
	}

	/* get the current frame index */
	framenum = (temp / 8) & ATS_OTG_FRAME_MASK;

	/*
	 * Compute number of milliseconds worth of data traffic for
	 * this USB transfer:
	 */
	if (xfer->xroot->udev->speed == USB_SPEED_HIGH)
		msframes = ((xfer->nframes << shift) + 7) / 8;
	else
		msframes = xfer->nframes;

	/*
	 * check if the frame index is within the window where the frames
	 * will be inserted
	 */
	temp = (framenum - xfer->endpoint->isoc_next) & ATS_OTG_FRAME_MASK;

	if ((xfer->endpoint->is_synced == 0) || (temp < msframes)) {
		/*
		 * If there is data underflow or the pipe queue is
		 * empty we schedule the transfer a few frames ahead
		 * of the current frame position. Else two isochronous
		 * transfers might overlap.
		 */
		xfer->endpoint->isoc_next = (framenum + 3) & ATS_OTG_FRAME_MASK;
		xfer->endpoint->is_synced = 1;
		DPRINTFN(3, "start next=%d\n", xfer->endpoint->isoc_next);
	}
	/*
	 * compute how many milliseconds the insertion is ahead of the
	 * current frame position:
	 */
	temp = (xfer->endpoint->isoc_next - framenum) & ATS_OTG_FRAME_MASK;

	/*
	 * pre-compute when the isochronous transfer will be finished:
	 */
	xfer->isoc_time_complete =
	    usb_isoc_time_expand(&sc->sc_bus, framenum) + temp + msframes;

	/* setup TDs */
	ats_otg_setup_standard_chain(xfer);

	/* compute frame number for next insertion */
	xfer->endpoint->isoc_next += msframes;

	/* start TD chain */
	ats_otg_start_standard_chain(xfer);
}

static const struct usb_pipe_methods ats_otg_isoc_methods =
{
	.open = ats_otg_isoc_open,
	.close = ats_otg_isoc_close,
	.enter = ats_otg_isoc_enter,
	.start = ats_otg_isoc_start,
};

/*------------------------------------------------------------------------*
 * ATS OTG root control support
 *------------------------------------------------------------------------*
 * Simulate a hardware HUB by handling all the necessary requests.
 *------------------------------------------------------------------------*/

static const struct usb_device_descriptor ats_otg_devd = {
	.bLength = sizeof(struct usb_device_descriptor),
	.bDescriptorType = UDESC_DEVICE,
	.bcdUSB = {0x00, 0x02},
	.bDeviceClass = UDCLASS_HUB,
	.bDeviceSubClass = UDSUBCLASS_HUB,
	.bDeviceProtocol = UDPROTO_HSHUBSTT,
	.bMaxPacketSize = 64,
	.bcdDevice = {0x00, 0x01},
	.iManufacturer = 1,
	.iProduct = 2,
	.bNumConfigurations = 1,
};

static const struct ats_otg_config_desc ats_otg_confd = {
	.confd = {
		.bLength = sizeof(struct usb_config_descriptor),
		.bDescriptorType = UDESC_CONFIG,
		.wTotalLength[0] = sizeof(ats_otg_confd),
		.bNumInterface = 1,
		.bConfigurationValue = 1,
		.iConfiguration = 0,
		.bmAttributes = UC_SELF_POWERED,
		.bMaxPower = 0,
	},
	.ifcd = {
		.bLength = sizeof(struct usb_interface_descriptor),
		.bDescriptorType = UDESC_INTERFACE,
		.bNumEndpoints = 1,
		.bInterfaceClass = UICLASS_HUB,
		.bInterfaceSubClass = UISUBCLASS_HUB,
		.bInterfaceProtocol = 0,
	},
	.endpd = {
		.bLength = sizeof(struct usb_endpoint_descriptor),
		.bDescriptorType = UDESC_ENDPOINT,
		.bEndpointAddress = (UE_DIR_IN | ATS_OTG_INTR_ENDPT),
		.bmAttributes = UE_INTERRUPT,
		.wMaxPacketSize[0] = 8,
		.bInterval = 255,
	},
};

#define	HSETW(ptr, val) ptr = { (uint8_t)(val), (uint8_t)((val) >> 8) }

static const struct usb_hub_descriptor_min ats_otg_hubd = {
	.bDescLength = sizeof(ats_otg_hubd),
	.bDescriptorType = UDESC_HUB,
	.bNbrPorts = 1,
	HSETW(.wHubCharacteristics, (UHD_PWR_NO_SWITCH | UHD_OC_INDIVIDUAL)),
	.bPwrOn2PwrGood = 50,
	.bHubContrCurrent = 0,
	.DeviceRemovable = {0},		/* port is removable */
};

#define	STRING_VENDOR \
  "A\0T\0S\0O\0T\0G"

#define	STRING_PRODUCT \
  "O\0T\0G\0 \0R\0o\0o\0t\0 \0H\0U\0B"

USB_MAKE_STRING_DESC(STRING_VENDOR, ats_otg_vendor);
USB_MAKE_STRING_DESC(STRING_PRODUCT, ats_otg_product);

static usb_error_t
ats_otg_roothub_exec(struct usb_device *udev,
    struct usb_device_request *req, const void **pptr, uint16_t *plength)
{
	struct ats_otg_softc *sc = ATS_OTG_BUS2SC(udev->bus);
	const void *ptr;
	uint16_t len;
	uint16_t value;
	uint16_t index;
	usb_error_t err;

	USB_BUS_LOCK_ASSERT(&sc->sc_bus, MA_OWNED);

	/* buffer reset */
	ptr = (const void *)&sc->sc_hub_temp;
	len = 0;
	err = 0;

	value = UGETW(req->wValue);
	index = UGETW(req->wIndex);

	/* demultiplex the control request */

	switch (req->bmRequestType) {
	case UT_READ_DEVICE:
		switch (req->bRequest) {
		case UR_GET_DESCRIPTOR:
			goto tr_handle_get_descriptor;
		case UR_GET_CONFIG:
			goto tr_handle_get_config;
		case UR_GET_STATUS:
			goto tr_handle_get_status;
		default:
			goto tr_stalled;
		}
		break;

	case UT_WRITE_DEVICE:
		switch (req->bRequest) {
		case UR_SET_ADDRESS:
			goto tr_handle_set_address;
		case UR_SET_CONFIG:
			goto tr_handle_set_config;
		case UR_CLEAR_FEATURE:
			goto tr_valid;	/* nop */
		case UR_SET_DESCRIPTOR:
			goto tr_valid;	/* nop */
		case UR_SET_FEATURE:
		default:
			goto tr_stalled;
		}
		break;

	case UT_WRITE_ENDPOINT:
		switch (req->bRequest) {
		case UR_CLEAR_FEATURE:
			switch (UGETW(req->wValue)) {
			case UF_ENDPOINT_HALT:
				goto tr_handle_clear_halt;
			case UF_DEVICE_REMOTE_WAKEUP:
				goto tr_handle_clear_wakeup;
			default:
				goto tr_stalled;
			}
			break;
		case UR_SET_FEATURE:
			switch (UGETW(req->wValue)) {
			case UF_ENDPOINT_HALT:
				goto tr_handle_set_halt;
			case UF_DEVICE_REMOTE_WAKEUP:
				goto tr_handle_set_wakeup;
			default:
				goto tr_stalled;
			}
			break;
		case UR_SYNCH_FRAME:
			goto tr_valid;	/* nop */
		default:
			goto tr_stalled;
		}
		break;

	case UT_READ_ENDPOINT:
		switch (req->bRequest) {
		case UR_GET_STATUS:
			goto tr_handle_get_ep_status;
		default:
			goto tr_stalled;
		}
		break;

	case UT_WRITE_INTERFACE:
		switch (req->bRequest) {
		case UR_SET_INTERFACE:
			goto tr_handle_set_interface;
		case UR_CLEAR_FEATURE:
			goto tr_valid;	/* nop */
		case UR_SET_FEATURE:
		default:
			goto tr_stalled;
		}
		break;

	case UT_READ_INTERFACE:
		switch (req->bRequest) {
		case UR_GET_INTERFACE:
			goto tr_handle_get_interface;
		case UR_GET_STATUS:
			goto tr_handle_get_iface_status;
		default:
			goto tr_stalled;
		}
		break;

	case UT_WRITE_CLASS_INTERFACE:
	case UT_WRITE_VENDOR_INTERFACE:
		/* XXX forward */
		break;

	case UT_READ_CLASS_INTERFACE:
	case UT_READ_VENDOR_INTERFACE:
		/* XXX forward */
		break;

	case UT_WRITE_CLASS_DEVICE:
		switch (req->bRequest) {
		case UR_CLEAR_FEATURE:
			goto tr_valid;
		case UR_SET_DESCRIPTOR:
		case UR_SET_FEATURE:
			break;
		default:
			goto tr_stalled;
		}
		break;

	case UT_WRITE_CLASS_OTHER:
		switch (req->bRequest) {
		case UR_CLEAR_FEATURE:
			goto tr_handle_clear_port_feature;
		case UR_SET_FEATURE:
			goto tr_handle_set_port_feature;
		case UR_CLEAR_TT_BUFFER:
		case UR_RESET_TT:
		case UR_STOP_TT:
			goto tr_valid;

		default:
			goto tr_stalled;
		}
		break;

	case UT_READ_CLASS_OTHER:
		switch (req->bRequest) {
		case UR_GET_TT_STATE:
			goto tr_handle_get_tt_state;
		case UR_GET_STATUS:
			goto tr_handle_get_port_status;
		default:
			goto tr_stalled;
		}
		break;

	case UT_READ_CLASS_DEVICE:
		switch (req->bRequest) {
		case UR_GET_DESCRIPTOR:
			goto tr_handle_get_class_descriptor;
		case UR_GET_STATUS:
			goto tr_handle_get_class_status;

		default:
			goto tr_stalled;
		}
		break;
	default:
		goto tr_stalled;
	}
	goto tr_valid;

tr_handle_get_descriptor:
	switch (value >> 8) {
	case UDESC_DEVICE:
		if (value & 0xff) {
			goto tr_stalled;
		}
		len = sizeof(ats_otg_devd);
		ptr = (const void *)&ats_otg_devd;
		goto tr_valid;
	case UDESC_CONFIG:
		if (value & 0xff) {
			goto tr_stalled;
		}
		len = sizeof(ats_otg_confd);
		ptr = (const void *)&ats_otg_confd;
		goto tr_valid;
	case UDESC_STRING:
		switch (value & 0xff) {
		case 0:		/* Language table */
			len = sizeof(usb_string_lang_en);
			ptr = (const void *)&usb_string_lang_en;
			goto tr_valid;

		case 1:		/* Vendor */
			len = sizeof(ats_otg_vendor);
			ptr = (const void *)&ats_otg_vendor;
			goto tr_valid;

		case 2:		/* Product */
			len = sizeof(ats_otg_product);
			ptr = (const void *)&ats_otg_product;
			goto tr_valid;
		default:
			break;
		}
		break;
	default:
		goto tr_stalled;
	}
	goto tr_stalled;

tr_handle_get_config:
	len = 1;
	sc->sc_hub_temp.wValue[0] = sc->sc_conf;
	goto tr_valid;

tr_handle_get_status:
	len = 2;
	USETW(sc->sc_hub_temp.wValue, UDS_SELF_POWERED);
	goto tr_valid;

tr_handle_set_address:
	if (value & 0xFF00) {
		goto tr_stalled;
	}
	sc->sc_rt_addr = value;
	goto tr_valid;

tr_handle_set_config:
	if (value >= 2) {
		goto tr_stalled;
	}
	sc->sc_conf = value;
	goto tr_valid;

tr_handle_get_interface:
	len = 1;
	sc->sc_hub_temp.wValue[0] = 0;
	goto tr_valid;

tr_handle_get_tt_state:
tr_handle_get_class_status:
tr_handle_get_iface_status:
tr_handle_get_ep_status:
	len = 2;
	USETW(sc->sc_hub_temp.wValue, 0);
	goto tr_valid;

tr_handle_set_halt:
tr_handle_set_interface:
tr_handle_set_wakeup:
tr_handle_clear_wakeup:
tr_handle_clear_halt:
	goto tr_valid;

tr_handle_clear_port_feature:
	if (index != 1)
		goto tr_stalled;

	DPRINTFN(9, "UR_CLEAR_PORT_FEATURE on port %d\n", index);

	switch (value) {
	case UHF_PORT_SUSPEND:
		ats_otg_wakeup_peer(sc);
		break;

	case UHF_PORT_ENABLE:
		sc->sc_flags.port_enabled = 0;
		break;

	case UHF_C_PORT_RESET:
		sc->sc_flags.change_reset = 0;
		break;

	case UHF_C_PORT_ENABLE:
		sc->sc_flags.change_enabled = 0;
		break;

	case UHF_C_PORT_OVER_CURRENT:
		sc->sc_flags.change_over_current = 0;
		break;

	case UHF_PORT_TEST:
	case UHF_PORT_INDICATOR:
		/* nops */
		break;

	case UHF_PORT_POWER:
		sc->sc_flags.port_powered = 0;
		ats_otg_pull_down(sc);
		break;

	case UHF_C_PORT_CONNECTION:
		/* clear connect change flag */
		sc->sc_flags.change_connect = 0;
		break;

	case UHF_C_PORT_SUSPEND:
		sc->sc_flags.change_suspend = 0;
		break;

	default:
		err = USB_ERR_IOERROR;
		goto done;
	}
	goto tr_valid;

tr_handle_set_port_feature:
	if (index != 1) {
		goto tr_stalled;
	}
	DPRINTFN(9, "UR_SET_PORT_FEATURE\n");

	switch (value) {
	case UHF_PORT_ENABLE:
		break;

	case UHF_PORT_SUSPEND:
		if (sc->sc_flags.status_device_mode == 0) {
			uint32_t temp;

			/* set suspend BIT */
			temp = ATS_OTG_READ_4(sc, USBHS_HSTCTRL);
			temp &= ~(USBHS_HSTCTRL_SOFE | USBHS_HSTCTRL_RESET |
			    USBHS_HSTCTRL_RESUME);
			ATS_OTG_WRITE_4(sc, USBHS_HSTCTRL, temp);

			/* generate HUB suspend event */
			ats_otg_suspend_irq(sc);
		}
		break;

	case UHF_PORT_RESET:
		if (sc->sc_flags.status_device_mode == 0) {
			uint32_t temp;

			DPRINTF("PORT RESET\n");

			/* enable PORT reset */
			temp = ATS_OTG_READ_4(sc, USBHS_HSTCTRL);
			temp &= ~(USBHS_HSTCTRL_SOFE | USBHS_HSTCTRL_RESET |
			    USBHS_HSTCTRL_RESUME);
			ATS_OTG_WRITE_4(sc, USBHS_HSTCTRL,
			    temp | USBHS_HSTCTRL_RESET);

			/* Wait 62.5ms for reset to complete */
			usb_pause_mtx(&sc->sc_bus.bus_mtx, hz / 16);

			ATS_OTG_WRITE_4(sc, USBHS_HSTCTRL,
			    temp | USBHS_HSTCTRL_SOFE);

			/* Wait 62.5ms for reset to complete */
			usb_pause_mtx(&sc->sc_bus.bus_mtx, hz / 16);

			/* reset FIFOs */
			ats_otg_init_fifo(sc, ATS_MODE_HOST);

			sc->sc_flags.change_reset = 1;
		} else {
			err = USB_ERR_IOERROR;
		}
		break;

	case UHF_PORT_TEST:
	case UHF_PORT_INDICATOR:
		/* nops */
		break;
	case UHF_PORT_POWER:
		sc->sc_flags.port_powered = 1;
		if (sc->sc_mode == ATS_MODE_DEVICE || sc->sc_mode == ATS_MODE_OTG) {
			/* pull up D+, if any */
			ats_otg_pull_up(sc);
		}
		break;
	default:
		err = USB_ERR_IOERROR;
		goto done;
	}
	goto tr_valid;

tr_handle_get_port_status:

	DPRINTFN(9, "UR_GET_PORT_STATUS\n");

	if (index != 1)
		goto tr_stalled;

	/* Select Device Side Mode */

	if (sc->sc_flags.status_device_mode)
		value = UPS_PORT_MODE_DEVICE;
	else
		value = 0;

	if (sc->sc_flags.status_high_speed)
		value |= UPS_HIGH_SPEED;
	else if (sc->sc_flags.status_low_speed)
		value |= UPS_LOW_SPEED;

	if (sc->sc_flags.port_powered)
		value |= UPS_PORT_POWER;

	if (sc->sc_flags.port_enabled)
		value |= UPS_PORT_ENABLED;

	if (sc->sc_flags.port_over_current)
		value |= UPS_OVERCURRENT_INDICATOR;

	if (sc->sc_flags.status_bus_reset)
		value |= UPS_CURRENT_CONNECT_STATUS;

	if (sc->sc_flags.status_suspend)
		value |= UPS_SUSPEND;

	USETW(sc->sc_hub_temp.ps.wPortStatus, value);

	value = 0;

	if (sc->sc_flags.change_enabled)
		value |= UPS_C_PORT_ENABLED;
	if (sc->sc_flags.change_connect)
		value |= UPS_C_CONNECT_STATUS;
	if (sc->sc_flags.change_suspend)
		value |= UPS_C_SUSPEND;
	if (sc->sc_flags.change_reset)
		value |= UPS_C_PORT_RESET;
	if (sc->sc_flags.change_over_current)
		value |= UPS_C_OVERCURRENT_INDICATOR;

	USETW(sc->sc_hub_temp.ps.wPortChange, value);
	len = sizeof(sc->sc_hub_temp.ps);
	goto tr_valid;

tr_handle_get_class_descriptor:
	if (value & 0xFF) {
		goto tr_stalled;
	}
	ptr = (const void *)&ats_otg_hubd;
	len = sizeof(ats_otg_hubd);
	goto tr_valid;

tr_stalled:
	err = USB_ERR_STALLED;
tr_valid:
done:
	*plength = len;
	*pptr = ptr;
	return (err);
}

static void
ats_otg_xfer_setup(struct usb_setup_params *parm)
{
	struct usb_xfer *xfer;
	void *last_obj;
	uint32_t ntd;
	uint32_t n;
	uint8_t ep_no;
	uint8_t ep_type;

	xfer = parm->curr_xfer;

	/*
	 * NOTE: This driver does not use any of the parameters that
	 * are computed from the following values. Just set some
	 * reasonable dummies:
	 */
	parm->hc_max_packet_size = 0x500;
	parm->hc_max_packet_count = 3;
	parm->hc_max_frame_size = 3 * 0x500;

	usbd_transfer_setup_sub(parm);

	/*
	 * compute maximum number of TDs
	 */
	ep_type = (xfer->endpoint->edesc->bmAttributes & UE_XFERTYPE);

	if (ep_type == UE_CONTROL) {

		ntd = xfer->nframes + 1 /* STATUS */ + 1 /* SYNC 1 */
		    + 1 /* SYNC 2 */ + 1 /* SYNC 3 */ ;
	} else {

		ntd = xfer->nframes + 1 /* SYNC */ ;
	}

	/*
	 * check if "usbd_transfer_setup_sub" set an error
	 */
	if (parm->err)
		return;

	/*
	 * allocate transfer descriptors
	 */
	last_obj = NULL;

	ep_no = xfer->endpointno & UE_ADDR;

	/*
	 * Check for a valid endpoint profile in USB device mode:
	 */
	if (xfer->flags_int.usb_mode == USB_MODE_DEVICE) {
		const struct usb_hw_ep_profile *pf;

		ats_otg_get_hw_ep_profile(parm->udev, &pf, ep_no);

		if (pf == NULL) {
			/* should not happen */
			parm->err = USB_ERR_INVAL;
			return;
		}
	}
	/* align data */
	parm->size[0] += ((-parm->size[0]) & (USB_HOST_ALIGN - 1));

	for (n = 0; n != ntd; n++) {

		struct ats_otg_td *td;

		if (parm->buf) {

			td = USB_ADD_BYTES(parm->buf, parm->size[0]);

			/* init TD */
			td->max_packet_size = xfer->max_packet_size;
			td->ep_no = ep_no;
			td->ep_type = ep_type;
			td->dev_addr = xfer->address;
			td->dev_index = parm->udev->device_index;
			td->obj_next = last_obj;

			last_obj = td;
		}
		parm->size[0] += sizeof(*td);
	}

	xfer->td_start[0] = last_obj;
}

static void
ats_otg_xfer_unsetup(struct usb_xfer *xfer)
{
	return;
}

static void
ats_otg_ep_init(struct usb_device *udev, struct usb_endpoint_descriptor *edesc,
    struct usb_endpoint *ep)
{
	struct ats_otg_softc *sc = ATS_OTG_BUS2SC(udev->bus);

	DPRINTFN(2, "endpoint=%p, addr=%d, endpt=%d, mode=%d (%d,%d)\n",
	    ep, udev->address,
	    edesc->bEndpointAddress, udev->flags.usb_mode,
	    sc->sc_rt_addr, udev->device_index);

	if (udev->device_index != sc->sc_rt_addr) {

		if (udev->flags.usb_mode == USB_MODE_DEVICE) {
			if (udev->speed != USB_SPEED_FULL &&
			    udev->speed != USB_SPEED_HIGH) {
				/* not supported */
				return;
			}
		} else {
			if (udev->speed == USB_SPEED_HIGH &&
			    (edesc->wMaxPacketSize[1] & 0x18) != 0) {
				/* not supported */
				DPRINTFN(-1, "High bandwidth endpoints are "
				    "not supported\n");
				return;
			}
			if (UGETW(edesc->wMaxPacketSize) > 512) {
				/* not supported */
				DPRINTFN(-1, "wMaxPacketSize(%u) must be less than "
				    "or equal to 512 bytes\n",
				    UGETW(edesc->wMaxPacketSize));
				return;
			}
			if (udev->parent_hub != NULL &&
			    udev->parent_hub->parent_hub != NULL &&
			    udev->speed != udev->parent_hub->speed) {
				/* not supported */
				DPRINTFN(-1, "USB split transactions are "
				    "not supported\n");
				return;
			}
		}
		if ((edesc->bmAttributes & UE_XFERTYPE) == UE_ISOCHRONOUS)
			ep->methods = &ats_otg_isoc_methods;
		else
			ep->methods = &ats_otg_non_isoc_methods;
	}
}

static void
ats_otg_set_hw_power_sleep(struct usb_bus *bus, uint32_t state)
{
	struct ats_otg_softc *sc = ATS_OTG_BUS2SC(bus);

	switch (state) {
	case USB_HW_POWER_SUSPEND:
		ats_otg_suspend(sc);
		break;
	case USB_HW_POWER_SHUTDOWN:
		ats_otg_uninit(sc);
		break;
	case USB_HW_POWER_RESUME:
		ats_otg_resume(sc);
		break;
	default:
		break;
	}
}

static void
ats_otg_get_dma_delay(struct usb_device *udev, uint32_t *pus)
{
	/* DMA delay - wait until any use of memory is finished */
	*pus = 0;			/* microseconds */
}

static void
ats_otg_device_resume(struct usb_device *udev)
{
	DPRINTF("\n");

	/* poll all transfers again to restart resumed ones */
	ats_otg_do_poll(udev->bus);
}

static void
ats_otg_device_suspend(struct usb_device *udev)
{
	DPRINTF("\n");
}

static const struct usb_bus_methods ats_otg_bus_methods =
{
	.endpoint_init = &ats_otg_ep_init,
	.xfer_setup = &ats_otg_xfer_setup,
	.xfer_unsetup = &ats_otg_xfer_unsetup,
	.get_hw_ep_profile = &ats_otg_get_hw_ep_profile,
	.xfer_stall = &ats_otg_xfer_stall,
	.set_stall = &ats_otg_set_stall,
	.clear_stall = &ats_otg_clear_stall,
	.roothub_exec = &ats_otg_roothub_exec,
	.xfer_poll = &ats_otg_do_poll,
	.device_state_change = &ats_otg_device_state_change,
	.set_hw_power_sleep = &ats_otg_set_hw_power_sleep,
	.get_dma_delay = &ats_otg_get_dma_delay,
	.device_resume = &ats_otg_device_resume,
	.device_suspend = &ats_otg_device_suspend,
};
