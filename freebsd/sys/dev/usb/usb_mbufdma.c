#include <machine/rtems-bsd-kernel-space.h>

/* $FreeBSD$ */
/*-
 * Copyright (c) 2008 Hans Petter Selasky. All rights reserved.
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

#include <sys/systm.h>
#include <sys/condvar.h>

#include <dev/usb/usb.h>
#include <dev/usb/usbdi.h>
#include <dev/usb/usb_core.h>
#include <dev/usb/usb_busdma.h>

/*------------------------------------------------------------------------*
 *  usbd_m_copy_in - copy a mbuf chain directly into DMA-able memory
 *------------------------------------------------------------------------*/
#if USB_HAVE_MBUF
struct usb_m_copy_in_arg {
	struct usb_page_cache *cache;
	usb_frlength_t dst_offset;
};

static int
usbd_m_copy_in_cb(void *arg, void *src, uint32_t count)
{
	register struct usb_m_copy_in_arg *ua = arg;

	usbd_copy_in(ua->cache, ua->dst_offset, src, count);
	ua->dst_offset += count;
	return (0);
}

void
usbd_m_copy_in(struct usb_page_cache *cache, usb_frlength_t dst_offset,
    struct mbuf *m, usb_size_t src_offset, usb_frlength_t src_len)
{
	struct usb_m_copy_in_arg arg = {cache, dst_offset};
	(void) m_apply(m, src_offset, src_len, &usbd_m_copy_in_cb, &arg);
}
#endif

