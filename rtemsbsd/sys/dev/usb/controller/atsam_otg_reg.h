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

#ifndef __ATSAM_OTG_REG_H__
#define	__ATSAM_OTG_REG_H__

/* Device General Control Register */
#define	USBHS_DEVCTRL	0x0000
#define	USBHS_DEVCTRL_OPMODE2		(1U << 16)
#define	USBHS_DEVCTRL_TSTPCKT		(1U << 15)
#define	USBHS_DEVCTRL_TSTK		(1U << 14)
#define	USBHS_DEVCTRL_TSTJ		(1U << 13)
#define	USBHS_DEVCTRL_LS		(1U << 12)
#define	USBHS_DEVCTRL_SPDCONF(n)	(((n) & 3U) << 10)
#define	USBHS_DEVCTRL_RMWKUP		(1U << 9)
#define	USBHS_DEVCTRL_DETACH		(1U << 8)
#define	USBHS_DEVCTRL_ADDEN		(1U << 7)
#define	USBHS_DEVCTRL_UADD(n)		((n) & 127U)

/* Device Global Interrupt Status Register */
#define	USBHS_DEVISR	0x0004
#define	USBHS_DEVISR_DMA_MASK(n)	(1U << (24 + (n)))
#define	USBHS_DEVISR_PEP_MASK(n)	(1U << (12 + (n)))
#define	USBHS_DEVISR_UPRSM		(1U << 6)
#define	USBHS_DEVISR_EORSM		(1U << 5)
#define	USBHS_DEVISR_WAKEUP		(1U << 4)
#define	USBHS_DEVISR_EORST		(1U << 3)
#define	USBHS_DEVISR_SOF		(1U << 2)
#define	USBHS_DEVISR_MSOF		(1U << 1)
#define	USBHS_DEVISR_SUSP		(1U << 0)

/* Device Global Interrupt Clear Register */
#define	USBHS_DEVICR	0x0008
#define	USBHS_DEVICR_UPRSM		(1U << 6)
#define	USBHS_DEVICR_EORSM		(1U << 5)
#define	USBHS_DEVICR_WAKEUP		(1U << 4)
#define	USBHS_DEVICR_EORST		(1U << 3)
#define	USBHS_DEVICR_SOF		(1U << 2)
#define	USBHS_DEVICR_MSOF		(1U << 1)
#define	USBHS_DEVICR_SUSP		(1U << 0)

/* Device Global Interrupt Set Register */
#define	USBHS_DEVIFR	0x000C
#define	USBHS_DEVIFR_DMA_MASK(n)	(1U << (24 + (n)))
#define	USBHS_DEVIFR_UPRSM		(1U << 6)
#define	USBHS_DEVIFR_EORSM		(1U << 5)
#define	USBHS_DEVIFR_WAKEUP		(1U << 4)
#define	USBHS_DEVIFR_EORST		(1U << 3)
#define	USBHS_DEVIFR_SOF		(1U << 2)
#define	USBHS_DEVIFR_MSOF		(1U << 1)
#define	USBHS_DEVIFR_SUSP		(1U << 0)

/* Device Global Interrupt Mask Register */
#define	USBHS_DEVIMR	0x0010
#define	USBHS_DEVIMR_DMA_MASK(n)	(1U << (24 + (n)))
#define	USBHS_DEVIMR_PEP_MASK(n)	(1U << (12 + (n)))
#define	USBHS_DEVIMR_UPRSM		(1U << 6)
#define	USBHS_DEVIMR_EORSM		(1U << 5)
#define	USBHS_DEVIMR_WAKEUP		(1U << 4)
#define	USBHS_DEVIMR_EORST		(1U << 3)
#define	USBHS_DEVIMR_SOF		(1U << 2)
#define	USBHS_DEVIMR_MSOF		(1U << 1)
#define	USBHS_DEVIMR_SUSP		(1U << 0)

/* Device Global Interrupt Disable Register */
#define	USBHS_DEVIDR	0x0014
#define	USBHS_DEVIDR_DMA_MASK(n)	(1U << (24 + (n)))
#define	USBHS_DEVIDR_PEP_MASK(n)	(1U << (12 + (n)))
#define	USBHS_DEVIDR_UPRSM		(1U << 6)
#define	USBHS_DEVIDR_EORSM		(1U << 5)
#define	USBHS_DEVIDR_WAKEUP		(1U << 4)
#define	USBHS_DEVIDR_EORST		(1U << 3)
#define	USBHS_DEVIDR_SOF		(1U << 2)
#define	USBHS_DEVIDR_MSOF		(1U << 1)
#define	USBHS_DEVIDR_SUSP		(1U << 0)

/* Device Global Interrupt Enable Register */
#define	USBHS_DEVIER	0x0018
#define	USBHS_DEVIER_DMA_MASK(n)	(1U << (24 + (n)))
#define	USBHS_DEVIER_PEP_MASK(n)	(1U << (12 + (n)))
#define	USBHS_DEVIER_UPRSM		(1U << 6)
#define	USBHS_DEVIER_EORSM		(1U << 5)
#define	USBHS_DEVIER_WAKEUP		(1U << 4)
#define	USBHS_DEVIER_EORST		(1U << 3)
#define	USBHS_DEVIER_SOF		(1U << 2)
#define	USBHS_DEVIER_MSOF		(1U << 1)
#define	USBHS_DEVIER_SUSP		(1U << 0)

/* Device Endpoint Register */
#define	USBHS_DEVEPT	0x001C
#define	USBHS_DEVEPT_EPRST(n)		(1U << (16 + (n)))
#define	USBHS_DEVEPT_EPEN(n)		(1U << (0 + (n)))

/* Device Frame Number Register */
#define	USBHS_DEVFNUM	0x0020

/* Device Endpoint Configuration Register */
#define	USBHS_DEVEPTCFG(n)	(0x0100 + ((n) * 0x04) + 0x00)
#define	USBHS_DEVEPTCFG_NBTRANS_0	(0U << 13)
#define	USBHS_DEVEPTCFG_NBTRANS_1	(1U << 13)
#define	USBHS_DEVEPTCFG_NBTRANS_2	(2U << 13)
#define	USBHS_DEVEPTCFG_NBTRANS_3	(3U << 13)
#define	USBHS_DEVEPTCFG_EPTYPE(n)	(((n) & 3U) << 11)
#define	USBHS_DEVEPTCFG_AUTOSW		(1U << 9)
#define	USBHS_DEVEPTCFG_EPDIR		(1U << 8)
#define	USBHS_DEVEPTCFG_EPSIZE_8	(0U << 4)
#define	USBHS_DEVEPTCFG_EPSIZE_16	(1U << 4)
#define	USBHS_DEVEPTCFG_EPSIZE_32	(2U << 4)
#define	USBHS_DEVEPTCFG_EPSIZE_64	(3U << 4)
#define	USBHS_DEVEPTCFG_EPSIZE_128	(4U << 4)
#define	USBHS_DEVEPTCFG_EPSIZE_256	(5U << 4)
#define	USBHS_DEVEPTCFG_EPSIZE_512	(6U << 4)
#define	USBHS_DEVEPTCFG_EPSIZE_1024	(7U << 4)
#define	USBHS_DEVEPTCFG_EPBK_1		(0U << 2)
#define	USBHS_DEVEPTCFG_EPBK_2		(1U << 2)
#define	USBHS_DEVEPTCFG_EPBK_3		(2U << 2)
#define	USBHS_DEVEPTCFG_ALLOC		(1U << 1)

/* Device Endpoint Status Register */
#define	USBHS_DEVEPTISR(n)	(0x0100 + ((n) * 0x04) + 0x30)
#define	USBHS_DEVEPTISR_BYCT(n)		(((n) & 0x7FFU) << 20)
#define	USBHS_DEVEPTISR_CFGOK		(1U << 18)
#define	USBHS_DEVEPTISR_CTRLDIR		(1U << 17)
#define	USBHS_DEVEPTISR_RWALL		(1U << 16)
#define	USBHS_DEVEPTISR_CURRBK(n)	(((n) & 0x3U) << 14)
#define	USBHS_DEVEPTISR_NBUSYBK(n)	(((n) & 0x3U) << 12)
#define	USBHS_DEVEPTISR_ERRORTRANS	(1U << 10)
#define	USBHS_DEVEPTISR_DTSEQ(n)	(((n) & 0x3U) << 8)
#define	USBHS_DEVEPTISR_SHORTPACKET	(1U << 7)
#define	USBHS_DEVEPTISR_STALLED		(1U << 6)
#define	USBHS_DEVEPTISR_CRCERR		(1U << 6)
#define	USBHS_DEVEPTISR_OVERF		(1U << 5)
#define	USBHS_DEVEPTISR_NAKIN		(1U << 4)
#define	USBHS_DEVEPTISR_HBISOFLUSH	(1U << 4)
#define	USBHS_DEVEPTISR_NAKOUT		(1U << 3)
#define	USBHS_DEVEPTISR_HBISOINERR	(1U << 3)
#define	USBHS_DEVEPTISR_RXSTP		(1U << 2)
#define	USBHS_DEVEPTISR_UNDERF		(1U << 2)
#define	USBHS_DEVEPTISR_RXOUT		(1U << 1)
#define	USBHS_DEVEPTISR_TXIN		(1U << 0)

/* Device Endpoint Clear Register */
#define	USBHS_DEVEPTICR(n)	(0x0100 + ((n) * 0x04) + 0x60)
#define	USBHS_DEVEPTICR_SHORTPACKET	(1U << 7)
#define	USBHS_DEVEPTICR_STALLED		(1U << 6)
#define	USBHS_DEVEPTICR_CRCERR		(1U << 6)
#define	USBHS_DEVEPTICR_OVERF		(1U << 5)
#define	USBHS_DEVEPTICR_NAKIN		(1U << 4)
#define	USBHS_DEVEPTICR_HBISOFLUSH	(1U << 4)
#define	USBHS_DEVEPTICR_NAKOUT		(1U << 3)
#define	USBHS_DEVEPTICR_HBISOINERR	(1U << 3)
#define	USBHS_DEVEPTICR_RXSTP		(1U << 2)
#define	USBHS_DEVEPTICR_UNDERF		(1U << 2)
#define	USBHS_DEVEPTICR_RXOUT		(1U << 1)
#define	USBHS_DEVEPTICR_TXIN		(1U << 0)

/* Device Endpoint Set Register */
#define	USBHS_DEVEPTIFR(n)	(0x0100 + ((n) * 0x04) + 0x90)
/* Device Endpoint Mask Register */
#define	USBHS_DEVEPTIMR(n)	(0x0100 + ((n) * 0x04) + 0xc0)
#define	USBHS_DEVEPTIMR_STALLRQ		(1U << 19)
#define	USBHS_DEVEPTIMR_RSTDT		(1U << 18)
#define	USBHS_DEVEPTIMR_NYETDIS		(1U << 17)
#define	USBHS_DEVEPTIMR_EPDISHDMA	(1U << 16)
#define	USBHS_DEVEPTIMR_FIFOCON		(1U << 14)
#define	USBHS_DEVEPTIMR_KILLBK		(1U << 13)
#define	USBHS_DEVEPTIMR_NBUSYBK		(1U << 12)
#define	USBHS_DEVEPTIMR_ERRORTRANS	(1U << 10)
#define	USBHS_DEVEPTIMR_DATAX		(1U << 9)
#define	USBHS_DEVEPTIMR_MDATA		(1U << 8)
#define	USBHS_DEVEPTIMR_SHORTPACKET	(1U << 7)
#define	USBHS_DEVEPTIMR_STALLED		(1U << 6)
#define	USBHS_DEVEPTIMR_CRCERR		(1U << 6)
#define	USBHS_DEVEPTIMR_OVERF		(1U << 5)
#define	USBHS_DEVEPTIMR_NAKIN		(1U << 4)
#define	USBHS_DEVEPTIMR_HBISOFLUSH	(1U << 4)
#define	USBHS_DEVEPTIMR_NAKOUT		(1U << 3)
#define	USBHS_DEVEPTIMR_HBISOINERR	(1U << 3)
#define	USBHS_DEVEPTIMR_RXSTP		(1U << 2)
#define	USBHS_DEVEPTIMR_UNDERF		(1U << 2)
#define	USBHS_DEVEPTIMR_RXOUT		(1U << 1)
#define	USBHS_DEVEPTIMR_TXIN		(1U << 0)

/* Device Endpoint Enable Register */
#define	USBHS_DEVEPTIER(n)	(0x0100 + ((n) * 0x04) + 0xf0)
#define	USBHS_DEVEPTIER_STALLRQ		(1U << 19)
#define	USBHS_DEVEPTIER_RSTDT		(1U << 18)
#define	USBHS_DEVEPTIER_NYETDIS		(1U << 17)
#define	USBHS_DEVEPTIER_EPDISHDMA	(1U << 16)
#define	USBHS_DEVEPTIER_FIFOCON		(1U << 14)
#define	USBHS_DEVEPTIER_KILLBK		(1U << 13)
#define	USBHS_DEVEPTIER_NBUSYBK		(1U << 12)
#define	USBHS_DEVEPTIER_ERRORTRANS	(1U << 10)
#define	USBHS_DEVEPTIER_DATAX		(1U << 9)
#define	USBHS_DEVEPTIER_MDATAX		(1U << 8)
#define	USBHS_DEVEPTIER_SHORTPACKET	(1U << 7)
#define	USBHS_DEVEPTIER_STALLED		(1U << 6)
#define	USBHS_DEVEPTIER_CRCERR		(1U << 6)
#define	USBHS_DEVEPTIER_OVERF		(1U << 5)
#define	USBHS_DEVEPTIER_NAKIN		(1U << 4)
#define	USBHS_DEVEPTIER_HBISOFLUSH	(1U << 4)
#define	USBHS_DEVEPTIER_NAKOUT		(1U << 3)
#define	USBHS_DEVEPTIER_HBISOINERR	(1U << 3)
#define	USBHS_DEVEPTIER_RXSTP		(1U << 2)
#define	USBHS_DEVEPTIER_UNDERF		(1U << 2)
#define	USBHS_DEVEPTIER_RXOUT		(1U << 1)
#define	USBHS_DEVEPTIER_TXIN		(1U << 0)

/* Device Endpoint Disable Register */
#define	USBHS_DEVEPTIDR(n)	(0x0100 + ((n) * 0x04) + 0x120)
#define	USBHS_DEVEPTIDR_STALLRQ		(1U << 19)
#define	USBHS_DEVEPTIDR_NYETDIS		(1U << 17)
#define	USBHS_DEVEPTIDR_EPDISHDMA	(1U << 16)
#define	USBHS_DEVEPTIDR_FIFOCON		(1U << 14)
#define	USBHS_DEVEPTIDR_KILLBK		(1U << 13)
#define	USBHS_DEVEPTIDR_NBUSYBK		(1U << 12)
#define	USBHS_DEVEPTIDR_ERRORTRANS	(1U << 10)
#define	USBHS_DEVEPTIDR_DATAX		(1U << 9)
#define	USBHS_DEVEPTIDR_MDATAX		(1U << 8)
#define	USBHS_DEVEPTIDR_SHORTPACKET	(1U << 7)
#define	USBHS_DEVEPTIDR_STALLED		(1U << 6)
#define	USBHS_DEVEPTIDR_CRCERR		(1U << 6)
#define	USBHS_DEVEPTIDR_OVERF		(1U << 5)
#define	USBHS_DEVEPTIDR_NAKIN		(1U << 4)
#define	USBHS_DEVEPTIDR_HBISOFLUSH	(1U << 4)
#define	USBHS_DEVEPTIDR_NAKOUT		(1U << 3)
#define	USBHS_DEVEPTIDR_HBISOINERR	(1U << 3)
#define	USBHS_DEVEPTIDR_RXSTP		(1U << 2)
#define	USBHS_DEVEPTIDR_UNDERF		(1U << 2)
#define	USBHS_DEVEPTIDR_RXOUT		(1U << 1)
#define	USBHS_DEVEPTIDR_TXIN		(1U << 0)

/* Device DMA Channel Next Descriptor */
#define	USBHS_DEVDMANXTDSC(n)	(0x0300 + ((n) * 0x10) + 0x00)

/* Device DMA Channel Address Register */
#define	USBHS_DEVDMAADDRESS(n)	(0x0300 + ((n) * 0x10) + 0x04)

/* Device DMA Channel Control Register */
#define	USBHS_DEVDMACONTROL(n)	(0x0300 + ((n) * 0x10) + 0x08)

/* Device DMA Channel Status Register */
#define	USBHS_DEVDMASTATUS(n)	(0x0300 + ((n) * 0x10) + 0x0c)

/* Host Global Control Register */
#define	USBHS_HSTCTRL	0x0400
#define	USBHS_HSTCTRL_SPDCONF		(3U << 12)
#define	USBHS_HSTCTRL_RESUME		(1U << 10)
#define	USBHS_HSTCTRL_RESET		(1U << 9)
#define	USBHS_HSTCTRL_SOFE		(1U << 8)

/* Host Global Interrupt Status Register */
#define	USBHS_HSTISR	0x0404
#define	USBHS_HSTISR_DMA(n)		(1U << (24 + (n)))
#define	USBHS_HSTISR_PEP(n)		(1U << (8 + (n)))
#define	USBHS_HSTISR_HWUP		(1U << 6)
#define	USBHS_HSTISR_HSOF		(1U << 5)
#define	USBHS_HSTISR_RXRSM		(1U << 4)
#define	USBHS_HSTISR_RSMED		(1U << 3)
#define	USBHS_HSTISR_RST		(1U << 2)
#define	USBHS_HSTISR_DDISC		(1U << 1)
#define	USBHS_HSTISR_DCONN		(1U << 0)

/* Host Global Interrupt Clear Register */
#define	USBHS_HSTICR	0x0408
#define	USBHS_HSTICR_HWUP		(1U << 6)
#define	USBHS_HSTICR_HSOF		(1U << 5)
#define	USBHS_HSTICR_RXRSM		(1U << 4)
#define	USBHS_HSTICR_RSMED		(1U << 3)
#define	USBHS_HSTICR_RST		(1U << 2)
#define	USBHS_HSTICR_DDISC		(1U << 1)
#define	USBHS_HSTICR_DCONN		(1U << 0)

/* Host Global Interrupt Set Register */
#define	USBHS_HSTIFR	0x040C
#define	USBHS_HSTIFR_DMA(n)		(1U << (24 + (n)))
#define	USBHS_HSTIFR_HWUP		(1U << 6)
#define	USBHS_HSTIFR_HSOF		(1U << 5)
#define	USBHS_HSTIFR_RXRSM		(1U << 4)
#define	USBHS_HSTIFR_RSMED		(1U << 3)
#define	USBHS_HSTIFR_RST		(1U << 2)
#define	USBHS_HSTIFR_DDISC		(1U << 1)
#define	USBHS_HSTIFR_DCONN		(1U << 0)

/* Host Global Interrupt Mask Register */
#define	USBHS_HSTIMR	0x0410
#define	USBHS_HSTIMR_DMA(n)		(1U << (24 + (n)))
#define	USBHS_HSTIMR_PEP(n)		(1U << (8 + (n)))
#define	USBHS_HSTIMR_HWUP		(1U << 6)
#define	USBHS_HSTIMR_HSOF		(1U << 5)
#define	USBHS_HSTIMR_RXRSM		(1U << 4)
#define	USBHS_HSTIMR_RSMED		(1U << 3)
#define	USBHS_HSTIMR_RST		(1U << 2)
#define	USBHS_HSTIMR_DDISC		(1U << 1)
#define	USBHS_HSTIMR_DCONN		(1U << 0)

/* Host Global Interrupt Disable Register */
#define	USBHS_HSTIDR	0x0414
#define	USBHS_HSTIDR_DMA(n)		(1U << (24 + (n)))
#define	USBHS_HSTIDR_PEP(n)		(1U << (8 + (n)))
#define	USBHS_HSTIDR_HWUP		(1U << 6)
#define	USBHS_HSTIDR_HSOF		(1U << 5)
#define	USBHS_HSTIDR_RXRSM		(1U << 4)
#define	USBHS_HSTIDR_RSMED		(1U << 3)
#define	USBHS_HSTIDR_RST		(1U << 2)
#define	USBHS_HSTIDR_DDISC		(1U << 1)
#define	USBHS_HSTIDR_DCONN		(1U << 0)

/* Host Global Interrupt Enable Register */
#define	USBHS_HSTIER	0x0418
#define	USBHS_HSTIER_DMA(n)		(1U << (24 + (n)))
#define	USBHS_HSTIER_PEP(n)		(1U << (8 + (n)))
#define	USBHS_HSTIER_HWUP		(1U << 6)
#define	USBHS_HSTIER_HSOF		(1U << 5)
#define	USBHS_HSTIER_RXRSM		(1U << 4)
#define	USBHS_HSTIER_RSMED		(1U << 3)
#define	USBHS_HSTIER_RST		(1U << 2)
#define	USBHS_HSTIER_DDISC		(1U << 1)
#define	USBHS_HSTIER_DCONN		(1U << 0)

/* Host Pipe Register */
#define	USBHS_HSTPIP	0x041C
#define	USBHS_HSTPIP_PRST(n)	(1U << ((n) + 16))
#define	USBHS_HSTPIP_PEN(n)	(1U << (n))

/* Host Frame Number Register */
#define	USBHS_HSTFNUM	0x0420

/* Host Address n=0,1,2 Registers */
#define	USBHS_HSTADDR(n)	(0x0424 + ((n) * 0x04))
#define	USBHS_HSTADDR_ADDR(x,n)	(((x) & 127U) << (8*(n)))

/* Host Pipe Configuration Register */
#define	USBHS_HSTPIPCFG(n)	(0x0500 + ((n) * 0x04) + 0x00)
#define	USBHS_HSTPIPCFG_INTFRQ(n)	(((n) & 0xFFU) << 24)
#define	USBHS_HSTPIPCFG_PINGEN		(1U << 20)
#define	USBHS_HSTPIPCFG_PEPNUM(n)	(((n) & 0xFU) << 16)
#define	USBHS_HSTPIPCFG_PTYPE(n)	(((n) & 0x3U) << 12)
#define	USBHS_HSTPIPCFG_AUTOSW		(1U << 10)
#define	USBHS_HSTPIPCFG_PTOKEN(n)	(((n) & 3U) << 8)
#define	USBHS_HSTPIPCFG_PTOKEN_SETUP	(0U << 8)
#define	USBHS_HSTPIPCFG_PTOKEN_IN	(1U << 8)
#define	USBHS_HSTPIPCFG_PTOKEN_OUT	(2U << 8)
#define	USBHS_HSTPIPCFG_PSIZE_8		(0U << 4)
#define	USBHS_HSTPIPCFG_PSIZE_16	(1U << 4)
#define	USBHS_HSTPIPCFG_PSIZE_32	(2U << 4)
#define	USBHS_HSTPIPCFG_PSIZE_64	(3U << 4)
#define	USBHS_HSTPIPCFG_PSIZE_128	(4U << 4)
#define	USBHS_HSTPIPCFG_PSIZE_256	(5U << 4)
#define	USBHS_HSTPIPCFG_PSIZE_512	(6U << 4)
#define	USBHS_HSTPIPCFG_PSIZE_1024	(7U << 4)
#define	USBHS_HSTPIPCFG_PBK_1		(0U << 2)
#define	USBHS_HSTPIPCFG_PBK_2		(1U << 2)
#define	USBHS_HSTPIPCFG_PBK_3		(2U << 2)
#define	USBHS_HSTPIPCFG_ALLOC		(1U << 1)

/* Host Pipe Status Register */
#define	USBHS_HSTPIPISR(n)	(0x0500 + ((n) * 0x04) + 0x30)
#define	USBHS_HSTPIPISR_BYCT(n)		(((n) & 0x7FFU) << 20)
#define	USBHS_HSTPIPISR_CFGOK		(1U << 18)
#define	USBHS_HSTPIPISR_RWALL		(1U << 16)
#define	USBHS_HSTPIPISR_CURRBK(n)	(((n) & 0x3U) << 14)
#define	USBHS_HSTPIPISR_NBUSYBK(n)	(((n) & 0x3U) << 12)
#define	USBHS_HSTPIPISR_DTSEQ(n)	(((n) & 0x3U) << 8)
#define	USBHS_HSTPIPISR_SHORTPACKET	(1U << 7)
#define	USBHS_HSTPIPISR_RXSTALLED	(1U << 6)
#define	USBHS_HSTPIPISR_CRCERR		(1U << 6)
#define	USBHS_HSTPIPISR_OVERF		(1U << 5)
#define	USBHS_HSTPIPISR_NAKED		(1U << 4)
#define	USBHS_HSTPIPISR_PERR		(1U << 3)
#define	USBHS_HSTPIPISR_TXSTP		(1U << 2)
#define	USBHS_HSTPIPISR_UNDERF		(1U << 2)
#define	USBHS_HSTPIPISR_TXOUT		(1U << 1)
#define	USBHS_HSTPIPISR_RXIN		(1U << 0)

/* Host Pipe Clear Register */
#define	USBHS_HSTPIPICR(n)	(0x0500 + ((n) * 0x04) + 0x60)
#define	USBHS_HSTPIPICR_SHORTPACKET	(1U << 7)
#define	USBHS_HSTPIPICR_RXSTALLED	(1U << 6)
#define	USBHS_HSTPIPICR_CRCERR		(1U << 6)
#define	USBHS_HSTPIPICR_OVERF		(1U << 5)
#define	USBHS_HSTPIPICR_NAKED		(1U << 4)
#define	USBHS_HSTPIPICR_PERR		(1U << 3)
#define	USBHS_HSTPIPICR_TXSTP		(1U << 2)
#define	USBHS_HSTPIPICR_UNDERF		(1U << 2)
#define	USBHS_HSTPIPICR_TXOUT		(1U << 1)
#define	USBHS_HSTPIPICR_RXIN		(1U << 0)

/* Host Pipe Set Register */
#define	USBHS_HSTPIPIF(n)	(0x0500 + ((n) * 0x04) + 0x90)
#define	USBHS_HSTPIPIFR_NBUSYBK		(1U << 12)
#define	USBHS_HSTPIPIFR_SHORTPACKET	(1U << 7)
#define	USBHS_HSTPIPIFR_RXSTALLED	(1U << 6)
#define	USBHS_HSTPIPIFR_CRCERR		(1U << 6)
#define	USBHS_HSTPIPIFR_OVERF		(1U << 5)
#define	USBHS_HSTPIPIFR_OVERF		(1U << 5)
#define	USBHS_HSTPIPIFR_NAKED		(1U << 4)
#define	USBHS_HSTPIPIFR_PERRE		(1U << 3)
#define	USBHS_HSTPIPIFR_TXSTP		(1U << 2)
#define	USBHS_HSTPIPIFR_UNDERF		(1U << 2)
#define	USBHS_HSTPIPIFR_TXOUT		(1U << 1)
#define	USBHS_HSTPIPIFR_RXIN		(1U << 0)

/* Host Pipe Mask Register */
#define	USBHS_HSTPIPIMR(n)	(0x0500 + ((n) * 0x04) + 0xC0)
#define	USBHS_HSTPIPIMR_RSTDT		(1U << 18)
#define	USBHS_HSTPIPIMR_PFREEZ		(1U << 17)
#define	USBHS_HSTPIPIMR_PDISHDMA	(1U << 16)
#define	USBHS_HSTPIPIMR_FIFOCON		(1U << 14)
#define	USBHS_HSTPIPIMR_NBUSYBK		(1U << 12)
#define	USBHS_HSTPIPIMR_SHORTPACKET	(1U << 7)
#define	USBHS_HSTPIPIMR_RXSTALLED	(1U << 6)
#define	USBHS_HSTPIPIMR_CRCERR		(1U << 6)
#define	USBHS_HSTPIPIMR_OVERF		(1U << 5)
#define	USBHS_HSTPIPIMR_NAKED		(1U << 4)
#define	USBHS_HSTPIPIMR_PERR		(1U << 3)
#define	USBHS_HSTPIPIMR_TXSTP		(1U << 2)
#define	USBHS_HSTPIPIMR_UNDERF		(1U << 2)
#define	USBHS_HSTPIPIMR_TXOUT		(1U << 1)
#define	USBHS_HSTPIPIMR_RXIN		(1U << 0)

/* Host Pipe Enable Register */
#define	USBHS_HSTPIPIER(n)	(0x0500 + ((n) * 0x04) + 0xf0)
#define	USBHS_HSTPIPIER_RSTDT		(1U << 18)
#define	USBHS_HSTPIPIER_PFREEZ		(1U << 17)
#define	USBHS_HSTPIPIER_PDISHDMA	(1U << 16)
#define	USBHS_HSTPIPIER_FIFOCON		(1U << 14)
#define	USBHS_HSTPIPIER_NBUSYBK		(1U << 12)
#define	USBHS_HSTPIPIER_SHORTPACKET	(1U << 7)
#define	USBHS_HSTPIPIER_RXSTALLED	(1U << 6)
#define	USBHS_HSTPIPIER_CRCERR		(1U << 6)
#define	USBHS_HSTPIPIER_OVERF		(1U << 5)
#define	USBHS_HSTPIPIER_NAKED		(1U << 4)
#define	USBHS_HSTPIPIER_PERR		(1U << 3)
#define	USBHS_HSTPIPIER_TXSTP		(1U << 2)
#define	USBHS_HSTPIPIER_UNDERF		(1U << 2)
#define	USBHS_HSTPIPIER_TXOUT		(1U << 1)
#define	USBHS_HSTPIPIER_RXIN		(1U << 0)

/* Host Pipe Disable Register */
#define	USBHS_HSTPIPIDR(n)	(0x0500 + ((n) * 0x04) + 0x120)
#define	USBHS_HSTPIPIDR_PFREEZ		(1U << 17)
#define	USBHS_HSTPIPIDR_PDISHDMA	(1U << 16)
#define	USBHS_HSTPIPIDR_FIFOCON		(1U << 14)
#define	USBHS_HSTPIPIDR_NBUSYBK		(1U << 12)
#define	USBHS_HSTPIPIDR_SHORTPACKET	(1U << 7)
#define	USBHS_HSTPIPIDR_RXSTALLED	(1U << 6)
#define	USBHS_HSTPIPIDR_CRCERR		(1U << 6)
#define	USBHS_HSTPIPIDR_OVERF		(1U << 5)
#define	USBHS_HSTPIPIDR_NAKED		(1U << 4)
#define	USBHS_HSTPIPIDR_PERR		(1U << 3)
#define	USBHS_HSTPIPIDR_TXSTP		(1U << 2)
#define	USBHS_HSTPIPIDR_UNDERF		(1U << 2)
#define	USBHS_HSTPIPIDR_TXOUT		(1U << 1)
#define	USBHS_HSTPIPIDR_RXIN		(1U << 0)

/* Host Pipe IN Request Register */
#define	USBHS_HSTPIPINRQ(n)	(0x0500 + ((n) * 0x04) + 0x150)
#define	USBHS_HSTPIPINRQ_INRQ(n)	((n) & 0xFFU)
#define	USBHS_HSTPIPINRQ_INMODE		(1U << 8)

/* Host Pipe Error Register */
#define	USBHS_HSTPIPERR(n)	(0x0500 + ((n) * 0x04) + 0x180)
#define	USBHS_HSTPIPERR_COUNTER(n)	(((n) & 0x3U) << 5)
#define	USBHS_HSTPIPERR_CRC16		(1U << 4)
#define	USBHS_HSTPIPERR_TIMEOUT		(1U << 3)
#define	USBHS_HSTPIPERR_PID		(1U << 2)
#define	USBHS_HSTPIPERR_DATAPID		(1U << 1)
#define	USBHS_HSTPIPERR_DATATGL		(1U << 0)

/* Host DMA Channel Next Descriptor Address  Register */
#define	USBHS_HSTDMANXTDSC(n)	(0x0700 + ((n) * 0x10) + 0x00)
/* Host DMA Channel Address Register */
#define	USBHS_HSTDMAADDRESS(n)	(0x0700 + ((n) * 0x10) + 0x04)
/* Host DMA Channel Control Register */
#define	USBHS_HSTDMACONTROL(n)	(0x0700 + ((n) * 0x10) + 0x08)
/* Host DMA Channel Status Register */
#define	USBHS_HSTDMASTATUS(n)	(0x0700 + ((n) * 0x10) + 0x0C)

/* General Status Control Register */
#define	USBHS_CTRL	0x0800
#define	USBHS_CTRL_UIMOD		(1U << 25)
#define	USBHS_CTRL_UID			(1U << 24)
#define	USBHS_CTRL_USBE			(1U << 15)
#define	USBHS_CTRL_FRZCLK		(1U << 14)
#define	USBHS_CTRL_VBUSHWC		(1U << 8)
#define	USBHS_CTRL_RDERRE		(1U << 4)

/* General Status Register */
#define	USBHS_SR	0x0804
#define	USBHS_SR_CLKUSABLE		(1U << 14)
#define	USBHS_SR_SPEED(n)		(((n) & 3U) << 12)
#define	USBHS_SR_SPEED_FULL		(0U << 12)
#define	USBHS_SR_SPEED_HIGH		(1U << 12)
#define	USBHS_SR_SPEED_LOW		(2U << 12)
#define	USBHS_SR_RDERRI			(1U << 4)

/* General Status Clear Register */
#define	USBHS_SCR	0x0808
#define	USBHS_SCR_RDERRIC		(1U << 4)

/* General Status Set Register */
#define	USBHS_SFR	0x080C
#define	USBHS_SFR_VBUSRQS		(1U << 9)
#define	USBHS_SFR_RDERRIS		(1U << 4)

#endif					/* __ATSAM_OTG_REG_H__ */
