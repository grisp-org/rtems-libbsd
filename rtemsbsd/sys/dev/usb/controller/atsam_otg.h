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

#ifndef __ATSAM_OTG_H__
#define	__ATSAM_OTG_H__

#define	ATS_OTG_MAX_DEVICES MIN(USB_MAX_DEVICES, 32)
#define	ATS_OTG_MAX_HOST_MEMORY 4096	/* bytes */
#define	ATS_OTG_MAX_HOST_CHANNELS 9
#define	ATS_OTG_MAX_DEVICE_ENDPOINTS 10
#define	ATS_OTG_MAX_DEVICE_HS_EP 8
#define	ATS_OTG_FRAME_MASK 0x7FFU
#define	ATS_OTG_FIFO_OFFSET(n)	((n) << 15)

#define	ATS_OTG_READ_4(sc, reg) \
    bus_space_read_4((sc)->sc_io_tag, (sc)->sc_io_hdl, reg)

#define	ATS_OTG_WRITE_4(sc, reg, data)	\
    bus_space_write_4((sc)->sc_io_tag, (sc)->sc_io_hdl, reg, data)

#define	ATS_OTG_READ_FIFO_1(sc, off, data, len)	do {			\
    if ((uintptr_t)(data) & 3) {					\
	bus_space_read_region_1((sc)->sc_fifo_tag, (sc)->sc_fifo_hdl,	\
	    (off), (data), (len));					\
	(off) += (len);							\
    } else {								\
	uint32_t __rem = (len) & 3;					\
	bus_space_read_region_4((sc)->sc_fifo_tag, (sc)->sc_fifo_hdl,	\
	    (off), (data), (len) / 4);					\
	(off) += (len);							\
	if (__rem == 0)							\
		break;							\
	bus_space_read_region_1((sc)->sc_fifo_tag, (sc)->sc_fifo_hdl,	\
	    (off) - __rem, ((uint8_t *)(data)) + (len) - __rem, __rem); \
   }									\
} while (0)

#define	ATS_OTG_WRITE_FIFO_1(sc, off, data, len) do {			\
    if ((uintptr_t)(data) & 3) {					\
	bus_space_write_region_1((sc)->sc_fifo_tag, (sc)->sc_fifo_hdl,	\
	    (off), (data), (len));					\
	(off) += (len);							\
    } else {								\
	uint32_t __rem = (len) & 3;					\
	bus_space_write_region_4((sc)->sc_fifo_tag, (sc)->sc_fifo_hdl,	\
	    (off), (data), (len) / 4);					\
	(off) += (len);							\
	if (__rem == 0)							\
		break;							\
	bus_space_write_region_1((sc)->sc_fifo_tag, (sc)->sc_fifo_hdl,	\
	    (off) - __rem, ((uint8_t *)(data)) + (len) - __rem, __rem); \
   }									\
} while (0)

struct ats_otg_td;
struct ats_otg_softc;

typedef uint8_t (ats_otg_cmd_t)(struct ats_otg_softc *sc, struct ats_otg_td *td);

struct ats_otg_td {
	struct ats_otg_td *obj_next;
	ats_otg_cmd_t *func;
	struct usb_page_cache *pc;
	uint32_t offset;
	uint32_t remainder;
	uint16_t max_packet_size;	/* packet_size */
	uint8_t	errcnt;
	uint8_t	ep_no;
	uint8_t	ep_type;
	uint8_t	dev_addr;
	uint8_t	dev_index;
	uint8_t	channel;
	uint8_t	error_any:1;
	uint8_t	error_stall:1;
	uint8_t	alt_next:1;
	uint8_t	short_pkt:1;
	uint8_t	did_stall:1;
	uint8_t	toggle:1;
	uint8_t	set_toggle:1;
	uint8_t	got_short:1;
	uint8_t	did_nak:1;
};

struct ats_otg_std_temp {
	ats_otg_cmd_t *func;
	struct usb_page_cache *pc;
	struct ats_otg_td *td;
	struct ats_otg_td *td_next;
	uint32_t len;
	uint32_t offset;
	uint16_t max_frame_size;
	uint8_t	short_pkt;

	/*
	 * short_pkt = 0: transfer should be short terminated
	 * short_pkt = 1: transfer should not be short terminated
	 */
	uint8_t	setup_alt_next;
	uint8_t	did_stall;
	uint8_t	bulk_or_control;
};

struct ats_otg_config_desc {
	struct usb_config_descriptor confd;
	struct usb_interface_descriptor ifcd;
	struct usb_endpoint_descriptor endpd;
} __packed;

union ats_otg_hub_temp {
	uWord	wValue;
	struct usb_port_status ps;
};

struct ats_otg_flags {
	uint8_t	change_connect:1;
	uint8_t	change_suspend:1;
	uint8_t	change_reset:1;
	uint8_t	change_enabled:1;
	uint8_t	change_over_current:1;
	uint8_t	status_suspend:1;	/* set if suspended */
	uint8_t	status_bus_reset:1;	/* set if reset complete */
	uint8_t	status_high_speed:1;	/* set if High Speed is selected */
	uint8_t	status_low_speed:1;	/* set if Low Speed is selected */
	uint8_t	status_device_mode:1;	/* set if device mode */
	uint8_t	self_powered:1;
	uint8_t	clocks_off:1;
	uint8_t	port_powered:1;
	uint8_t	port_enabled:1;
	uint8_t	port_over_current:1;
	uint8_t	d_pulled_up:1;
};

struct ats_otg_chan_state {
	uint32_t key;
};

struct ats_otg_softc {
	struct usb_bus sc_bus;
	union ats_otg_hub_temp sc_hub_temp;

	struct usb_device *sc_devices[ATS_OTG_MAX_DEVICES];

	struct resource *sc_io_res;
	struct resource *sc_fifo_res;
	struct resource *sc_irq_res;
	void *sc_intr_hdl;

	bus_space_tag_t sc_io_tag;
	bus_space_handle_t sc_io_hdl;
	bus_space_tag_t sc_fifo_tag;
	bus_space_handle_t sc_fifo_hdl;

	struct ats_otg_chan_state sc_chan_state[ATS_OTG_MAX_HOST_CHANNELS];
	uint32_t sc_xfer_complete;
	uint32_t sc_host_memory_used;

	uint8_t	sc_rt_addr;		/* root HUB address */
	uint8_t	sc_dv_addr;		/* temporary device address */
	uint8_t	sc_conf;		/* root HUB config */

	uint8_t	sc_mode;		/* mode of operation */
#define	ATS_MODE_DEVICE 0	/* device mode */
#define	ATS_MODE_HOST 1		/* host mode */
#define	ATS_MODE_OTG  2		/* both modes */

	uint8_t	sc_hub_idata[1];

	struct ats_otg_flags sc_flags;
};

/* prototypes */

driver_filter_t ats_otg_filter_interrupt;
driver_intr_t ats_otg_interrupt;
int	ats_otg_init(struct ats_otg_softc *);
void	ats_otg_uninit(struct ats_otg_softc *);

#endif					/* __ATSAM_OTG_H__ */
