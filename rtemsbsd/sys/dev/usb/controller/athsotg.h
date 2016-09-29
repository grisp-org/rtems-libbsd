/* $FreeBSD$ */
/*-
 * Copyright (c) 2015, Atmel Corporation
 * Copyright (c) 2016 Hans Petter Selasky <hselasky@FreeBSD.org>
 * All rights reserved.
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

#ifndef ATHSOTG_H_
#define	ATHSOTG_H_

/* Device General Control Register */
#define ATHSOTG_DEVCTRL 0x0000
#define ATHSOTG_DEVCTRL_UADD_Pos 0
#define ATHSOTG_DEVCTRL_UADD_Msk (0x7fu << ATHSOTG_DEVCTRL_UADD_Pos) /* USB Address */
#define ATHSOTG_DEVCTRL_UADD(value) ((ATHSOTG_DEVCTRL_UADD_Msk & ((value) << ATHSOTG_DEVCTRL_UADD_Pos)))
#define ATHSOTG_DEVCTRL_ADDEN (0x1u << 7) /* Address Enable */
#define ATHSOTG_DEVCTRL_DETACH (0x1u << 8) /* Detach */
#define ATHSOTG_DEVCTRL_RMWKUP (0x1u << 9) /* Remote Wake-Up */
#define ATHSOTG_DEVCTRL_SPDCONF_Pos 10
#define ATHSOTG_DEVCTRL_SPDCONF_Msk (0x3u << ATHSOTG_DEVCTRL_SPDCONF_Pos) /* Mode Configuration */
#define ATHSOTG_DEVCTRL_SPDCONF(value) ((ATHSOTG_DEVCTRL_SPDCONF_Msk & ((value) << ATHSOTG_DEVCTRL_SPDCONF_Pos)))
#define   ATHSOTG_DEVCTRL_SPDCONF_NORMAL (0x0u << 10) /* The peripheral starts in Full-speed mode and performs a high-speed reset to switch to High-speed mode if the host is high-speed-capable. */
#define   ATHSOTG_DEVCTRL_SPDCONF_LOW_POWER (0x1u << 10) /* For a better consumption, if high speed is not needed. */
#define   ATHSOTG_DEVCTRL_SPDCONF_HIGH_SPEED (0x2u << 10) /* Forced high speed. */
#define   ATHSOTG_DEVCTRL_SPDCONF_FORCED_FS (0x3u << 10) /* The peripheral remains in Full-speed mode whatever the host speed capability. */
#define ATHSOTG_DEVCTRL_LS (0x1u << 12) /* Low-Speed Mode Force */
#define ATHSOTG_DEVCTRL_TSTJ (0x1u << 13) /* Test mode J */
#define ATHSOTG_DEVCTRL_TSTK (0x1u << 14) /* Test mode K */
#define ATHSOTG_DEVCTRL_TSTPCKT (0x1u << 15) /* Test packet mode */
#define ATHSOTG_DEVCTRL_OPMODE2 (0x1u << 16) /* Specific Operational mode */

/* Device Global Interrupt Status Register */
#define ATHSOTG_DEVISR 0x0004
#define ATHSOTG_DEVISR_SUSP (0x1u << 0) /* Suspend Interrupt */
#define ATHSOTG_DEVISR_MSOF (0x1u << 1) /* Micro Start of Frame Interrupt */
#define ATHSOTG_DEVISR_SOF (0x1u << 2) /* Start of Frame Interrupt */
#define ATHSOTG_DEVISR_EORST (0x1u << 3) /* End of Reset Interrupt */
#define ATHSOTG_DEVISR_WAKEUP (0x1u << 4) /* Wake-Up Interrupt */
#define ATHSOTG_DEVISR_EORSM (0x1u << 5) /* End of Resume Interrupt */
#define ATHSOTG_DEVISR_UPRSM (0x1u << 6) /* Upstream Resume Interrupt */
#define ATHSOTG_DEVISR_PEP_0 (0x1u << 12) /* Endpoint 0 Interrupt */
#define ATHSOTG_DEVISR_PEP_1 (0x1u << 13) /* Endpoint 1 Interrupt */
#define ATHSOTG_DEVISR_PEP_2 (0x1u << 14) /* Endpoint 2 Interrupt */
#define ATHSOTG_DEVISR_PEP_3 (0x1u << 15) /* Endpoint 3 Interrupt */
#define ATHSOTG_DEVISR_PEP_4 (0x1u << 16) /* Endpoint 4 Interrupt */
#define ATHSOTG_DEVISR_PEP_5 (0x1u << 17) /* Endpoint 5 Interrupt */
#define ATHSOTG_DEVISR_PEP_6 (0x1u << 18) /* Endpoint 6 Interrupt */
#define ATHSOTG_DEVISR_PEP_7 (0x1u << 19) /* Endpoint 7 Interrupt */
#define ATHSOTG_DEVISR_PEP_8 (0x1u << 20) /* Endpoint 8 Interrupt */
#define ATHSOTG_DEVISR_PEP_9 (0x1u << 21) /* Endpoint 9 Interrupt */
#define ATHSOTG_DEVISR_PEP_10 (0x1u << 22) /* Endpoint 10 Interrupt */
#define ATHSOTG_DEVISR_PEP_11 (0x1u << 23) /* Endpoint 11 Interrupt */
#define ATHSOTG_DEVISR_DMA_1 (0x1u << 25) /* DMA Channel 1 Interrupt */
#define ATHSOTG_DEVISR_DMA_2 (0x1u << 26) /* DMA Channel 2 Interrupt */
#define ATHSOTG_DEVISR_DMA_3 (0x1u << 27) /* DMA Channel 3 Interrupt */
#define ATHSOTG_DEVISR_DMA_4 (0x1u << 28) /* DMA Channel 4 Interrupt */
#define ATHSOTG_DEVISR_DMA_5 (0x1u << 29) /* DMA Channel 5 Interrupt */
#define ATHSOTG_DEVISR_DMA_6 (0x1u << 30) /* DMA Channel 6 Interrupt */
#define ATHSOTG_DEVISR_DMA_7 (0x1u << 31) /* DMA Channel 7 Interrupt */

/* Device Global Interrupt Clear Register */
#define ATHSOTG_DEVICR 0x0008
#define ATHSOTG_DEVICR_SUSPC (0x1u << 0) /* Suspend Interrupt Clear */
#define ATHSOTG_DEVICR_MSOFC (0x1u << 1) /* Micro Start of Frame Interrupt Clear */
#define ATHSOTG_DEVICR_SOFC (0x1u << 2) /* Start of Frame Interrupt Clear */
#define ATHSOTG_DEVICR_EORSTC (0x1u << 3) /* End of Reset Interrupt Clear */
#define ATHSOTG_DEVICR_WAKEUPC (0x1u << 4) /* Wake-Up Interrupt Clear */
#define ATHSOTG_DEVICR_EORSMC (0x1u << 5) /* End of Resume Interrupt Clear */
#define ATHSOTG_DEVICR_UPRSMC (0x1u << 6) /* Upstream Resume Interrupt Clear */

/* Device Global Interrupt Set Register */
#define ATHSOTG_DEVIFR 0x000C
#define ATHSOTG_DEVIFR_SUSPS (0x1u << 0) /* Suspend Interrupt Set */
#define ATHSOTG_DEVIFR_MSOFS (0x1u << 1) /* Micro Start of Frame Interrupt Set */
#define ATHSOTG_DEVIFR_SOFS (0x1u << 2) /* Start of Frame Interrupt Set */
#define ATHSOTG_DEVIFR_EORSTS (0x1u << 3) /* End of Reset Interrupt Set */
#define ATHSOTG_DEVIFR_WAKEUPS (0x1u << 4) /* Wake-Up Interrupt Set */
#define ATHSOTG_DEVIFR_EORSMS (0x1u << 5) /* End of Resume Interrupt Set */
#define ATHSOTG_DEVIFR_UPRSMS (0x1u << 6) /* Upstream Resume Interrupt Set */
#define ATHSOTG_DEVIFR_DMA_1 (0x1u << 25) /* DMA Channel 1 Interrupt Set */
#define ATHSOTG_DEVIFR_DMA_2 (0x1u << 26) /* DMA Channel 2 Interrupt Set */
#define ATHSOTG_DEVIFR_DMA_3 (0x1u << 27) /* DMA Channel 3 Interrupt Set */
#define ATHSOTG_DEVIFR_DMA_4 (0x1u << 28) /* DMA Channel 4 Interrupt Set */
#define ATHSOTG_DEVIFR_DMA_5 (0x1u << 29) /* DMA Channel 5 Interrupt Set */
#define ATHSOTG_DEVIFR_DMA_6 (0x1u << 30) /* DMA Channel 6 Interrupt Set */
#define ATHSOTG_DEVIFR_DMA_7 (0x1u << 31) /* DMA Channel 7 Interrupt Set */

/* Device Global Interrupt Mask Register */
#define ATHSOTG_DEVIMR 0x0010
#define ATHSOTG_DEVIMR_SUSPE (0x1u << 0) /* Suspend Interrupt Mask */
#define ATHSOTG_DEVIMR_MSOFE (0x1u << 1) /* Micro Start of Frame Interrupt Mask */
#define ATHSOTG_DEVIMR_SOFE (0x1u << 2) /* Start of Frame Interrupt Mask */
#define ATHSOTG_DEVIMR_EORSTE (0x1u << 3) /* End of Reset Interrupt Mask */
#define ATHSOTG_DEVIMR_WAKEUPE (0x1u << 4) /* Wake-Up Interrupt Mask */
#define ATHSOTG_DEVIMR_EORSME (0x1u << 5) /* End of Resume Interrupt Mask */
#define ATHSOTG_DEVIMR_UPRSME (0x1u << 6) /* Upstream Resume Interrupt Mask */
#define ATHSOTG_DEVIMR_PEP_0 (0x1u << 12) /* Endpoint 0 Interrupt Mask */
#define ATHSOTG_DEVIMR_PEP_1 (0x1u << 13) /* Endpoint 1 Interrupt Mask */
#define ATHSOTG_DEVIMR_PEP_2 (0x1u << 14) /* Endpoint 2 Interrupt Mask */
#define ATHSOTG_DEVIMR_PEP_3 (0x1u << 15) /* Endpoint 3 Interrupt Mask */
#define ATHSOTG_DEVIMR_PEP_4 (0x1u << 16) /* Endpoint 4 Interrupt Mask */
#define ATHSOTG_DEVIMR_PEP_5 (0x1u << 17) /* Endpoint 5 Interrupt Mask */
#define ATHSOTG_DEVIMR_PEP_6 (0x1u << 18) /* Endpoint 6 Interrupt Mask */
#define ATHSOTG_DEVIMR_PEP_7 (0x1u << 19) /* Endpoint 7 Interrupt Mask */
#define ATHSOTG_DEVIMR_PEP_8 (0x1u << 20) /* Endpoint 8 Interrupt Mask */
#define ATHSOTG_DEVIMR_PEP_9 (0x1u << 21) /* Endpoint 9 Interrupt Mask */
#define ATHSOTG_DEVIMR_PEP_10 (0x1u << 22) /* Endpoint 10 Interrupt Mask */
#define ATHSOTG_DEVIMR_PEP_11 (0x1u << 23) /* Endpoint 11 Interrupt Mask */
#define ATHSOTG_DEVIMR_DMA_1 (0x1u << 25) /* DMA Channel 1 Interrupt Mask */
#define ATHSOTG_DEVIMR_DMA_2 (0x1u << 26) /* DMA Channel 2 Interrupt Mask */
#define ATHSOTG_DEVIMR_DMA_3 (0x1u << 27) /* DMA Channel 3 Interrupt Mask */
#define ATHSOTG_DEVIMR_DMA_4 (0x1u << 28) /* DMA Channel 4 Interrupt Mask */
#define ATHSOTG_DEVIMR_DMA_5 (0x1u << 29) /* DMA Channel 5 Interrupt Mask */
#define ATHSOTG_DEVIMR_DMA_6 (0x1u << 30) /* DMA Channel 6 Interrupt Mask */
#define ATHSOTG_DEVIMR_DMA_7 (0x1u << 31) /* DMA Channel 7 Interrupt Mask */

/* Device Global Interrupt Disable Register */
#define ATHSOTG_DEVIDR 0x0014
#define ATHSOTG_DEVIDR_SUSPEC (0x1u << 0) /* Suspend Interrupt Disable */
#define ATHSOTG_DEVIDR_MSOFEC (0x1u << 1) /* Micro Start of Frame Interrupt Disable */
#define ATHSOTG_DEVIDR_SOFEC (0x1u << 2) /* Start of Frame Interrupt Disable */
#define ATHSOTG_DEVIDR_EORSTEC (0x1u << 3) /* End of Reset Interrupt Disable */
#define ATHSOTG_DEVIDR_WAKEUPEC (0x1u << 4) /* Wake-Up Interrupt Disable */
#define ATHSOTG_DEVIDR_EORSMEC (0x1u << 5) /* End of Resume Interrupt Disable */
#define ATHSOTG_DEVIDR_UPRSMEC (0x1u << 6) /* Upstream Resume Interrupt Disable */
#define ATHSOTG_DEVIDR_PEP_0 (0x1u << 12) /* Endpoint 0 Interrupt Disable */
#define ATHSOTG_DEVIDR_PEP_1 (0x1u << 13) /* Endpoint 1 Interrupt Disable */
#define ATHSOTG_DEVIDR_PEP_2 (0x1u << 14) /* Endpoint 2 Interrupt Disable */
#define ATHSOTG_DEVIDR_PEP_3 (0x1u << 15) /* Endpoint 3 Interrupt Disable */
#define ATHSOTG_DEVIDR_PEP_4 (0x1u << 16) /* Endpoint 4 Interrupt Disable */
#define ATHSOTG_DEVIDR_PEP_5 (0x1u << 17) /* Endpoint 5 Interrupt Disable */
#define ATHSOTG_DEVIDR_PEP_6 (0x1u << 18) /* Endpoint 6 Interrupt Disable */
#define ATHSOTG_DEVIDR_PEP_7 (0x1u << 19) /* Endpoint 7 Interrupt Disable */
#define ATHSOTG_DEVIDR_PEP_8 (0x1u << 20) /* Endpoint 8 Interrupt Disable */
#define ATHSOTG_DEVIDR_PEP_9 (0x1u << 21) /* Endpoint 9 Interrupt Disable */
#define ATHSOTG_DEVIDR_PEP_10 (0x1u << 22) /* Endpoint 10 Interrupt Disable */
#define ATHSOTG_DEVIDR_PEP_11 (0x1u << 23) /* Endpoint 11 Interrupt Disable */
#define ATHSOTG_DEVIDR_DMA_1 (0x1u << 25) /* DMA Channel 1 Interrupt Disable */
#define ATHSOTG_DEVIDR_DMA_2 (0x1u << 26) /* DMA Channel 2 Interrupt Disable */
#define ATHSOTG_DEVIDR_DMA_3 (0x1u << 27) /* DMA Channel 3 Interrupt Disable */
#define ATHSOTG_DEVIDR_DMA_4 (0x1u << 28) /* DMA Channel 4 Interrupt Disable */
#define ATHSOTG_DEVIDR_DMA_5 (0x1u << 29) /* DMA Channel 5 Interrupt Disable */
#define ATHSOTG_DEVIDR_DMA_6 (0x1u << 30) /* DMA Channel 6 Interrupt Disable */
#define ATHSOTG_DEVIDR_DMA_7 (0x1u << 31) /* DMA Channel 7 Interrupt Disable */

/* Device Global Interrupt Enable Register */
#define ATHSOTG_DEVIER 0x0018
#define ATHSOTG_DEVIER_SUSPES (0x1u << 0) /* Suspend Interrupt Enable */
#define ATHSOTG_DEVIER_MSOFES (0x1u << 1) /* Micro Start of Frame Interrupt Enable */
#define ATHSOTG_DEVIER_SOFES (0x1u << 2) /* Start of Frame Interrupt Enable */
#define ATHSOTG_DEVIER_EORSTES (0x1u << 3) /* End of Reset Interrupt Enable */
#define ATHSOTG_DEVIER_WAKEUPES (0x1u << 4) /* Wake-Up Interrupt Enable */
#define ATHSOTG_DEVIER_EORSMES (0x1u << 5) /* End of Resume Interrupt Enable */
#define ATHSOTG_DEVIER_UPRSMES (0x1u << 6) /* Upstream Resume Interrupt Enable */
#define ATHSOTG_DEVIER_PEP_0 (0x1u << 12) /* Endpoint 0 Interrupt Enable */
#define ATHSOTG_DEVIER_PEP_1 (0x1u << 13) /* Endpoint 1 Interrupt Enable */
#define ATHSOTG_DEVIER_PEP_2 (0x1u << 14) /* Endpoint 2 Interrupt Enable */
#define ATHSOTG_DEVIER_PEP_3 (0x1u << 15) /* Endpoint 3 Interrupt Enable */
#define ATHSOTG_DEVIER_PEP_4 (0x1u << 16) /* Endpoint 4 Interrupt Enable */
#define ATHSOTG_DEVIER_PEP_5 (0x1u << 17) /* Endpoint 5 Interrupt Enable */
#define ATHSOTG_DEVIER_PEP_6 (0x1u << 18) /* Endpoint 6 Interrupt Enable */
#define ATHSOTG_DEVIER_PEP_7 (0x1u << 19) /* Endpoint 7 Interrupt Enable */
#define ATHSOTG_DEVIER_PEP_8 (0x1u << 20) /* Endpoint 8 Interrupt Enable */
#define ATHSOTG_DEVIER_PEP_9 (0x1u << 21) /* Endpoint 9 Interrupt Enable */
#define ATHSOTG_DEVIER_PEP_10 (0x1u << 22) /* Endpoint 10 Interrupt Enable */
#define ATHSOTG_DEVIER_PEP_11 (0x1u << 23) /* Endpoint 11 Interrupt Enable */
#define ATHSOTG_DEVIER_DMA_1 (0x1u << 25) /* DMA Channel 1 Interrupt Enable */
#define ATHSOTG_DEVIER_DMA_2 (0x1u << 26) /* DMA Channel 2 Interrupt Enable */
#define ATHSOTG_DEVIER_DMA_3 (0x1u << 27) /* DMA Channel 3 Interrupt Enable */
#define ATHSOTG_DEVIER_DMA_4 (0x1u << 28) /* DMA Channel 4 Interrupt Enable */
#define ATHSOTG_DEVIER_DMA_5 (0x1u << 29) /* DMA Channel 5 Interrupt Enable */
#define ATHSOTG_DEVIER_DMA_6 (0x1u << 30) /* DMA Channel 6 Interrupt Enable */
#define ATHSOTG_DEVIER_DMA_7 (0x1u << 31) /* DMA Channel 7 Interrupt Enable */

/* Device Endpoint Register */
#define ATHSOTG_DEVEPT 0x001C
#define ATHSOTG_DEVEPT_EPEN0 (0x1u << 0) /* Endpoint 0 Enable */
#define ATHSOTG_DEVEPT_EPEN1 (0x1u << 1) /* Endpoint 1 Enable */
#define ATHSOTG_DEVEPT_EPEN2 (0x1u << 2) /* Endpoint 2 Enable */
#define ATHSOTG_DEVEPT_EPEN3 (0x1u << 3) /* Endpoint 3 Enable */
#define ATHSOTG_DEVEPT_EPEN4 (0x1u << 4) /* Endpoint 4 Enable */
#define ATHSOTG_DEVEPT_EPEN5 (0x1u << 5) /* Endpoint 5 Enable */
#define ATHSOTG_DEVEPT_EPEN6 (0x1u << 6) /* Endpoint 6 Enable */
#define ATHSOTG_DEVEPT_EPEN7 (0x1u << 7) /* Endpoint 7 Enable */
#define ATHSOTG_DEVEPT_EPEN8 (0x1u << 8) /* Endpoint 8 Enable */
#define ATHSOTG_DEVEPT_EPRST0 (0x1u << 16) /* Endpoint 0 Reset */
#define ATHSOTG_DEVEPT_EPRST1 (0x1u << 17) /* Endpoint 1 Reset */
#define ATHSOTG_DEVEPT_EPRST2 (0x1u << 18) /* Endpoint 2 Reset */
#define ATHSOTG_DEVEPT_EPRST3 (0x1u << 19) /* Endpoint 3 Reset */
#define ATHSOTG_DEVEPT_EPRST4 (0x1u << 20) /* Endpoint 4 Reset */
#define ATHSOTG_DEVEPT_EPRST5 (0x1u << 21) /* Endpoint 5 Reset */
#define ATHSOTG_DEVEPT_EPRST6 (0x1u << 22) /* Endpoint 6 Reset */
#define ATHSOTG_DEVEPT_EPRST7 (0x1u << 23) /* Endpoint 7 Reset */
#define ATHSOTG_DEVEPT_EPRST8 (0x1u << 24) /* Endpoint 8 Reset */

/* Device Frame Number Register */
#define ATHSOTG_DEVFNUM 0x0020
#define ATHSOTG_DEVFNUM_MFNUM_Pos 0
#define ATHSOTG_DEVFNUM_MFNUM_Msk (0x7u << ATHSOTG_DEVFNUM_MFNUM_Pos) /* Micro Frame Number */
#define ATHSOTG_DEVFNUM_FNUM_Pos 3
#define ATHSOTG_DEVFNUM_FNUM_Msk (0x7ffu << ATHSOTG_DEVFNUM_FNUM_Pos) /* Frame Number */
#define ATHSOTG_DEVFNUM_FNCERR (0x1u << 15) /* Frame Number CRC Error */

/* Device Endpoint Configuration Register (n = 0) */
#define ATHSOTG_DEVEPTCFG 0x100
#define ATHSOTG_DEVEPTCFG_ALLOC (0x1u << 1) /* Endpoint Memory Allocate */
#define ATHSOTG_DEVEPTCFG_EPBK_Pos 2
#define ATHSOTG_DEVEPTCFG_EPBK_Msk (0x3u << ATHSOTG_DEVEPTCFG_EPBK_Pos) /* Endpoint Banks */
#define ATHSOTG_DEVEPTCFG_EPBK(value) ((ATHSOTG_DEVEPTCFG_EPBK_Msk & ((value) << ATHSOTG_DEVEPTCFG_EPBK_Pos)))
#define   ATHSOTG_DEVEPTCFG_EPBK_1_BANK (0x0u << 2) /* Single-bank endpoint */
#define   ATHSOTG_DEVEPTCFG_EPBK_2_BANK (0x1u << 2) /* Double-bank endpoint */
#define   ATHSOTG_DEVEPTCFG_EPBK_3_BANK (0x2u << 2) /* Triple-bank endpoint */
#define ATHSOTG_DEVEPTCFG_EPSIZE_Pos 4
#define ATHSOTG_DEVEPTCFG_EPSIZE_Msk (0x7u << ATHSOTG_DEVEPTCFG_EPSIZE_Pos) /* Endpoint Size */
#define ATHSOTG_DEVEPTCFG_EPSIZE(value) ((ATHSOTG_DEVEPTCFG_EPSIZE_Msk & ((value) << ATHSOTG_DEVEPTCFG_EPSIZE_Pos)))
#define   ATHSOTG_DEVEPTCFG_EPSIZE_8_BYTE (0x0u << 4) /* 8 bytes */
#define   ATHSOTG_DEVEPTCFG_EPSIZE_16_BYTE (0x1u << 4) /* 16 bytes */
#define   ATHSOTG_DEVEPTCFG_EPSIZE_32_BYTE (0x2u << 4) /* 32 bytes */
#define   ATHSOTG_DEVEPTCFG_EPSIZE_64_BYTE (0x3u << 4) /* 64 bytes */
#define   ATHSOTG_DEVEPTCFG_EPSIZE_128_BYTE (0x4u << 4) /* 128 bytes */
#define   ATHSOTG_DEVEPTCFG_EPSIZE_256_BYTE (0x5u << 4) /* 256 bytes */
#define   ATHSOTG_DEVEPTCFG_EPSIZE_512_BYTE (0x6u << 4) /* 512 bytes */
#define   ATHSOTG_DEVEPTCFG_EPSIZE_1024_BYTE (0x7u << 4) /* 1024 bytes */
#define ATHSOTG_DEVEPTCFG_EPDIR (0x1u << 8) /* Endpoint Direction */
#define   ATHSOTG_DEVEPTCFG_EPDIR_OUT (0x0u << 8) /* The endpoint direction is OUT. */
#define   ATHSOTG_DEVEPTCFG_EPDIR_IN (0x1u << 8) /*. */
#define ATHSOTG_DEVEPTCFG_AUTOSW (0x1u << 9) /* Automatic Switch */
#define ATHSOTG_DEVEPTCFG_EPTYPE_Pos 11
#define ATHSOTG_DEVEPTCFG_EPTYPE_Msk (0x3u << ATHSOTG_DEVEPTCFG_EPTYPE_Pos) /* Endpoint Type */
#define ATHSOTG_DEVEPTCFG_EPTYPE(value) ((ATHSOTG_DEVEPTCFG_EPTYPE_Msk & ((value) << ATHSOTG_DEVEPTCFG_EPTYPE_Pos)))
#define   ATHSOTG_DEVEPTCFG_EPTYPE_CTRL (0x0u << 11) /* Control */
#define   ATHSOTG_DEVEPTCFG_EPTYPE_ISO (0x1u << 11) /* Isochronous */
#define   ATHSOTG_DEVEPTCFG_EPTYPE_BLK (0x2u << 11) /* Bulk */
#define   ATHSOTG_DEVEPTCFG_EPTYPE_INTRPT (0x3u << 11) /* Interrupt */
#define ATHSOTG_DEVEPTCFG_NBTRANS_Pos 13
#define ATHSOTG_DEVEPTCFG_NBTRANS_Msk (0x3u << ATHSOTG_DEVEPTCFG_NBTRANS_Pos) /* Number of transactions per microframe for isochronous endpoint */
#define ATHSOTG_DEVEPTCFG_NBTRANS(value) ((ATHSOTG_DEVEPTCFG_NBTRANS_Msk & ((value) << ATHSOTG_DEVEPTCFG_NBTRANS_Pos)))
#define   ATHSOTG_DEVEPTCFG_NBTRANS_0_TRANS (0x0u << 13) /* Reserved to endpoint that does not have the high-bandwidth isochronous capability. */
#define   ATHSOTG_DEVEPTCFG_NBTRANS_1_TRANS (0x1u << 13) /* Default value: one transaction per microframe. */
#define   ATHSOTG_DEVEPTCFG_NBTRANS_2_TRANS (0x2u << 13) /* Two transactions per microframe. This endpoint should be configured as double-bank. */
#define   ATHSOTG_DEVEPTCFG_NBTRANS_3_TRANS (0x3u << 13) /* Three transactions per microframe. This endpoint should be configured as triple-bank. */

/* Device Endpoint Status Register (n = 0) */
#define ATHSOTG_DEVEPTISR 0x130
#define ATHSOTG_DEVEPTISR_TXINI (0x1u << 0) /* Transmitted IN Data Interrupt */
#define ATHSOTG_DEVEPTISR_RXOUTI (0x1u << 1) /* Received OUT Data Interrupt */
#define ATHSOTG_DEVEPTISR_RXSTPI (0x1u << 2) /* Received SETUP Interrupt */
#define ATHSOTG_DEVEPTISR_NAKOUTI (0x1u << 3) /* NAKed OUT Interrupt */
#define ATHSOTG_DEVEPTISR_NAKINI (0x1u << 4) /* NAKed IN Interrupt */
#define ATHSOTG_DEVEPTISR_OVERFI (0x1u << 5) /* Overflow Interrupt */
#define ATHSOTG_DEVEPTISR_STALLEDI (0x1u << 6) /* STALLed Interrupt */
#define ATHSOTG_DEVEPTISR_SHORTPACKET (0x1u << 7) /* Short Packet Interrupt */
#define ATHSOTG_DEVEPTISR_DTSEQ_Pos 8
#define ATHSOTG_DEVEPTISR_DTSEQ_Msk (0x3u << ATHSOTG_DEVEPTISR_DTSEQ_Pos) /* Data Toggle Sequence */
#define   ATHSOTG_DEVEPTISR_DTSEQ_DATA0 (0x0u << 8) /* Data0 toggle sequence */
#define   ATHSOTG_DEVEPTISR_DTSEQ_DATA1 (0x1u << 8) /* Data1 toggle sequence */
#define   ATHSOTG_DEVEPTISR_DTSEQ_DATA2 (0x2u << 8) /* Reserved for high-bandwidth isochronous endpoint */
#define   ATHSOTG_DEVEPTISR_DTSEQ_MDATA (0x3u << 8) /* Reserved for high-bandwidth isochronous endpoint */
#define ATHSOTG_DEVEPTISR_NBUSYBK_Pos 12
#define ATHSOTG_DEVEPTISR_NBUSYBK_Msk (0x3u << ATHSOTG_DEVEPTISR_NBUSYBK_Pos) /* Number of Busy Banks */
#define   ATHSOTG_DEVEPTISR_NBUSYBK_0_BUSY (0x0u << 12) /* */
#define   ATHSOTG_DEVEPTISR_NBUSYBK_1_BUSY (0x1u << 12) /* 1 busy bank */
#define   ATHSOTG_DEVEPTISR_NBUSYBK_2_BUSY (0x2u << 12) /* 2 busy banks */
#define   ATHSOTG_DEVEPTISR_NBUSYBK_3_BUSY (0x3u << 12) /* 3 busy banks */
#define ATHSOTG_DEVEPTISR_CURRBK_Pos 14
#define ATHSOTG_DEVEPTISR_CURRBK_Msk (0x3u << ATHSOTG_DEVEPTISR_CURRBK_Pos) /* Current Bank */
#define   ATHSOTG_DEVEPTISR_CURRBK_BANK0 (0x0u << 14) /* Current bank is bank0 */
#define   ATHSOTG_DEVEPTISR_CURRBK_BANK1 (0x1u << 14) /* Current bank is bank1 */
#define   ATHSOTG_DEVEPTISR_CURRBK_BANK2 (0x2u << 14) /* Current bank is bank2 */
#define ATHSOTG_DEVEPTISR_RWALL (0x1u << 16) /* Read/Write Allowed */
#define ATHSOTG_DEVEPTISR_CTRLDIR (0x1u << 17) /* Control Direction */
#define ATHSOTG_DEVEPTISR_CFGOK (0x1u << 18) /* Configuration OK Status */
#define ATHSOTG_DEVEPTISR_BYCT_Pos 20
#define ATHSOTG_DEVEPTISR_BYCT_Msk (0x7ffu << ATHSOTG_DEVEPTISR_BYCT_Pos) /* Byte Count */
#define ATHSOTG_DEVEPTISR_UNDERFI (0x1u << 2) /* Underflow Interrupt */
#define ATHSOTG_DEVEPTISR_HBISOINERRI (0x1u << 3) /* High Bandwidth Isochronous IN Underflow Error Interrupt */
#define ATHSOTG_DEVEPTISR_HBISOFLUSHI (0x1u << 4) /* High Bandwidth Isochronous IN Flush Interrupt */
#define ATHSOTG_DEVEPTISR_CRCERRI (0x1u << 6) /* CRC Error Interrupt */
#define ATHSOTG_DEVEPTISR_ERRORTRANS (0x1u << 10) /* High-bandwidth Isochronous OUT Endpoint Transaction Error Interrupt */

/* Device Endpoint Clear Register (n = 0) */
#define ATHSOTG_DEVEPTICR 0x160
#define ATHSOTG_DEVEPTICR_TXINIC (0x1u << 0) /* Transmitted IN Data Interrupt Clear */
#define ATHSOTG_DEVEPTICR_RXOUTIC (0x1u << 1) /* Received OUT Data Interrupt Clear */
#define ATHSOTG_DEVEPTICR_RXSTPIC (0x1u << 2) /* Received SETUP Interrupt Clear */
#define ATHSOTG_DEVEPTICR_NAKOUTIC (0x1u << 3) /* NAKed OUT Interrupt Clear */
#define ATHSOTG_DEVEPTICR_NAKINIC (0x1u << 4) /* NAKed IN Interrupt Clear */
#define ATHSOTG_DEVEPTICR_OVERFIC (0x1u << 5) /* Overflow Interrupt Clear */
#define ATHSOTG_DEVEPTICR_STALLEDIC (0x1u << 6) /* STALLed Interrupt Clear */
#define ATHSOTG_DEVEPTICR_SHORTPACKETC (0x1u << 7) /* Short Packet Interrupt Clear */
#define ATHSOTG_DEVEPTICR_UNDERFIC (0x1u << 2) /* Underflow Interrupt Clear */
#define ATHSOTG_DEVEPTICR_HBISOINERRIC (0x1u << 3) /* High Bandwidth Isochronous IN Underflow Error Interrupt Clear */
#define ATHSOTG_DEVEPTICR_HBISOFLUSHIC (0x1u << 4) /* High Bandwidth Isochronous IN Flush Interrupt Clear */
#define ATHSOTG_DEVEPTICR_CRCERRIC (0x1u << 6) /* CRC Error Interrupt Clear */

/* Device Endpoint Set Register (n = 0) */
#define ATHSOTG_DEVEPTIFR 0x190
#define ATHSOTG_DEVEPTIFR_TXINIS (0x1u << 0) /* Transmitted IN Data Interrupt Set */
#define ATHSOTG_DEVEPTIFR_RXOUTIS (0x1u << 1) /* Received OUT Data Interrupt Set */
#define ATHSOTG_DEVEPTIFR_RXSTPIS (0x1u << 2) /* Received SETUP Interrupt Set */
#define ATHSOTG_DEVEPTIFR_NAKOUTIS (0x1u << 3) /* NAKed OUT Interrupt Set */
#define ATHSOTG_DEVEPTIFR_NAKINIS (0x1u << 4) /* NAKed IN Interrupt Set */
#define ATHSOTG_DEVEPTIFR_OVERFIS (0x1u << 5) /* Overflow Interrupt Set */
#define ATHSOTG_DEVEPTIFR_STALLEDIS (0x1u << 6) /* STALLed Interrupt Set */
#define ATHSOTG_DEVEPTIFR_SHORTPACKETS (0x1u << 7) /* Short Packet Interrupt Set */
#define ATHSOTG_DEVEPTIFR_NBUSYBKS (0x1u << 12) /* Number of Busy Banks Interrupt Set */
#define ATHSOTG_DEVEPTIFR_UNDERFIS (0x1u << 2) /* Underflow Interrupt Set */
#define ATHSOTG_DEVEPTIFR_HBISOINERRIS (0x1u << 3) /* High Bandwidth Isochronous IN Underflow Error Interrupt Set */
#define ATHSOTG_DEVEPTIFR_HBISOFLUSHIS (0x1u << 4) /* High Bandwidth Isochronous IN Flush Interrupt Set */
#define ATHSOTG_DEVEPTIFR_CRCERRIS (0x1u << 6) /* CRC Error Interrupt Set */

/* Device Endpoint Mask Register (n = 0) */
#define ATHSOTG_DEVEPTIMR 0x1C0
#define ATHSOTG_DEVEPTIMR_TXINE (0x1u << 0) /* Transmitted IN Data Interrupt */
#define ATHSOTG_DEVEPTIMR_RXOUTE (0x1u << 1) /* Received OUT Data Interrupt */
#define ATHSOTG_DEVEPTIMR_RXSTPE (0x1u << 2) /* Received SETUP Interrupt */
#define ATHSOTG_DEVEPTIMR_NAKOUTE (0x1u << 3) /* NAKed OUT Interrupt */
#define ATHSOTG_DEVEPTIMR_NAKINE (0x1u << 4) /* NAKed IN Interrupt */
#define ATHSOTG_DEVEPTIMR_OVERFE (0x1u << 5) /* Overflow Interrupt */
#define ATHSOTG_DEVEPTIMR_STALLEDE (0x1u << 6) /* STALLed Interrupt */
#define ATHSOTG_DEVEPTIMR_SHORTPACKETE (0x1u << 7) /* Short Packet Interrupt */
#define ATHSOTG_DEVEPTIMR_NBUSYBKE (0x1u << 12) /* Number of Busy Banks Interrupt */
#define ATHSOTG_DEVEPTIMR_KILLBK (0x1u << 13) /* Kill IN Bank */
#define ATHSOTG_DEVEPTIMR_FIFOCON (0x1u << 14) /* FIFO Control */
#define ATHSOTG_DEVEPTIMR_EPDISHDMA (0x1u << 16) /* Endpoint Interrupts Disable HDMA Request */
#define ATHSOTG_DEVEPTIMR_NYETDIS (0x1u << 17) /* NYET Token Disable */
#define ATHSOTG_DEVEPTIMR_RSTDT (0x1u << 18) /* Reset Data Toggle */
#define ATHSOTG_DEVEPTIMR_STALLRQ (0x1u << 19) /* STALL Request */
#define ATHSOTG_DEVEPTIMR_UNDERFE (0x1u << 2) /* Underflow Interrupt */
#define ATHSOTG_DEVEPTIMR_HBISOINERRE (0x1u << 3) /* High Bandwidth Isochronous IN Error Interrupt */
#define ATHSOTG_DEVEPTIMR_HBISOFLUSHE (0x1u << 4) /* High Bandwidth Isochronous IN Flush Interrupt */
#define ATHSOTG_DEVEPTIMR_CRCERRE (0x1u << 6) /* CRC Error Interrupt */
#define ATHSOTG_DEVEPTIMR_MDATAE (0x1u << 8) /* MData Interrupt */
#define ATHSOTG_DEVEPTIMR_DATAXE (0x1u << 9) /* DataX Interrupt */
#define ATHSOTG_DEVEPTIMR_ERRORTRANSE (0x1u << 10) /* Transaction Error Interrupt */

/* Device Endpoint Enable Register (n = 0) */
#define ATHSOTG_DEVEPTIER 0x1F0
#define ATHSOTG_DEVEPTIER_TXINES (0x1u << 0) /* Transmitted IN Data Interrupt Enable */
#define ATHSOTG_DEVEPTIER_RXOUTES (0x1u << 1) /* Received OUT Data Interrupt Enable */
#define ATHSOTG_DEVEPTIER_RXSTPES (0x1u << 2) /* Received SETUP Interrupt Enable */
#define ATHSOTG_DEVEPTIER_NAKOUTES (0x1u << 3) /* NAKed OUT Interrupt Enable */
#define ATHSOTG_DEVEPTIER_NAKINES (0x1u << 4) /* NAKed IN Interrupt Enable */
#define ATHSOTG_DEVEPTIER_OVERFES (0x1u << 5) /* Overflow Interrupt Enable */
#define ATHSOTG_DEVEPTIER_STALLEDES (0x1u << 6) /* STALLed Interrupt Enable */
#define ATHSOTG_DEVEPTIER_SHORTPACKETES (0x1u << 7) /* Short Packet Interrupt Enable */
#define ATHSOTG_DEVEPTIER_NBUSYBKES (0x1u << 12) /* Number of Busy Banks Interrupt Enable */
#define ATHSOTG_DEVEPTIER_KILLBKS (0x1u << 13) /* Kill IN Bank */
#define ATHSOTG_DEVEPTIER_FIFOCONS (0x1u << 14) /* FIFO Control */
#define ATHSOTG_DEVEPTIER_EPDISHDMAS (0x1u << 16) /* Endpoint Interrupts Disable HDMA Request Enable */
#define ATHSOTG_DEVEPTIER_NYETDISS (0x1u << 17) /* NYET Token Disable Enable */
#define ATHSOTG_DEVEPTIER_RSTDTS (0x1u << 18) /* Reset Data Toggle Enable */
#define ATHSOTG_DEVEPTIER_STALLRQS (0x1u << 19) /* STALL Request Enable */
#define ATHSOTG_DEVEPTIER_UNDERFES (0x1u << 2) /* Underflow Interrupt Enable */
#define ATHSOTG_DEVEPTIER_HBISOINERRES (0x1u << 3) /* High Bandwidth Isochronous IN Error Interrupt Enable */
#define ATHSOTG_DEVEPTIER_HBISOFLUSHES (0x1u << 4) /* High Bandwidth Isochronous IN Flush Interrupt Enable */
#define ATHSOTG_DEVEPTIER_CRCERRES (0x1u << 6) /* CRC Error Interrupt Enable */
#define ATHSOTG_DEVEPTIER_MDATAES (0x1u << 8) /* MData Interrupt Enable */
#define ATHSOTG_DEVEPTIER_DATAXES (0x1u << 9) /* DataX Interrupt Enable */
#define ATHSOTG_DEVEPTIER_ERRORTRANSES (0x1u << 10) /* Transaction Error Interrupt Enable */

/* Device Endpoint Disable Register (n = 0) */
#define ATHSOTG_DEVEPTIDR 0x220
#define ATHSOTG_DEVEPTIDR_TXINEC (0x1u << 0) /* Transmitted IN Interrupt Clear */
#define ATHSOTG_DEVEPTIDR_RXOUTEC (0x1u << 1) /* Received OUT Data Interrupt Clear */
#define ATHSOTG_DEVEPTIDR_RXSTPEC (0x1u << 2) /* Received SETUP Interrupt Clear */
#define ATHSOTG_DEVEPTIDR_NAKOUTEC (0x1u << 3) /* NAKed OUT Interrupt Clear */
#define ATHSOTG_DEVEPTIDR_NAKINEC (0x1u << 4) /* NAKed IN Interrupt Clear */
#define ATHSOTG_DEVEPTIDR_OVERFEC (0x1u << 5) /* Overflow Interrupt Clear */
#define ATHSOTG_DEVEPTIDR_STALLEDEC (0x1u << 6) /* STALLed Interrupt Clear */
#define ATHSOTG_DEVEPTIDR_SHORTPACKETEC (0x1u << 7) /* Shortpacket Interrupt Clear */
#define ATHSOTG_DEVEPTIDR_NBUSYBKEC (0x1u << 12) /* Number of Busy Banks Interrupt Clear */
#define ATHSOTG_DEVEPTIDR_FIFOCONC (0x1u << 14) /* FIFO Control Clear */
#define ATHSOTG_DEVEPTIDR_EPDISHDMAC (0x1u << 16) /* Endpoint Interrupts Disable HDMA Request Clear */
#define ATHSOTG_DEVEPTIDR_NYETDISC (0x1u << 17) /* NYET Token Disable Clear */
#define ATHSOTG_DEVEPTIDR_STALLRQC (0x1u << 19) /* STALL Request Clear */
#define ATHSOTG_DEVEPTIDR_UNDERFEC (0x1u << 2) /* Underflow Interrupt Clear */
#define ATHSOTG_DEVEPTIDR_HBISOINERREC (0x1u << 3) /* High Bandwidth Isochronous IN Error Interrupt Clear */
#define ATHSOTG_DEVEPTIDR_HBISOFLUSHEC (0x1u << 4) /* High Bandwidth Isochronous IN Flush Interrupt Clear */
#define ATHSOTG_DEVEPTIDR_CRCERREC (0x1u << 6) /* CRC Error Interrupt Clear */
#define ATHSOTG_DEVEPTIDR_MDATEC (0x1u << 8) /* MData Interrupt Clear */
#define ATHSOTG_DEVEPTIDR_DATAXEC (0x1u << 9) /* DataX Interrupt Clear */
#define ATHSOTG_DEVEPTIDR_ERRORTRANSEC (0x1u << 10) /* Transaction Error Interrupt Clear */

/* Device DMA Channel Next Descriptor Address Register */
#define ATHSOTG_DEVDMANXTDSC_NXT_DSC_ADD_Pos 0
#define ATHSOTG_DEVDMANXTDSC_NXT_DSC_ADD_Msk (0xffffffffu << ATHSOTG_DEVDMANXTDSC_NXT_DSC_ADD_Pos) /* Next Descriptor Address */
#define ATHSOTG_DEVDMANXTDSC_NXT_DSC_ADD(value) ((ATHSOTG_DEVDMANXTDSC_NXT_DSC_ADD_Msk & ((value) << ATHSOTG_DEVDMANXTDSC_NXT_DSC_ADD_Pos)))

/* Device DMA Channel Address Register */
#define ATHSOTG_DEVDMAADDRESS_BUFF_ADD_Pos 0
#define ATHSOTG_DEVDMAADDRESS_BUFF_ADD_Msk (0xffffffffu << ATHSOTG_DEVDMAADDRESS_BUFF_ADD_Pos) /* Buffer Address */
#define ATHSOTG_DEVDMAADDRESS_BUFF_ADD(value) ((ATHSOTG_DEVDMAADDRESS_BUFF_ADD_Msk & ((value) << ATHSOTG_DEVDMAADDRESS_BUFF_ADD_Pos)))

/* Device DMA Channel Control Register */
#define ATHSOTG_DEVDMACONTROL_CHANN_ENB (0x1u << 0) /* Channel Enable Command */
#define ATHSOTG_DEVDMACONTROL_LDNXT_DSC (0x1u << 1) /* Load Next Channel Transfer Descriptor Enable Command */
#define ATHSOTG_DEVDMACONTROL_END_TR_EN (0x1u << 2) /* */
#define ATHSOTG_DEVDMACONTROL_END_B_EN (0x1u << 3) /* End of Buffer Enable Control */
#define ATHSOTG_DEVDMACONTROL_END_TR_IT (0x1u << 4) /* End of Transfer Interrupt Enable */
#define ATHSOTG_DEVDMACONTROL_END_BUFFIT (0x1u << 5) /* End of Buffer Interrupt Enable */
#define ATHSOTG_DEVDMACONTROL_DESC_LD_IT (0x1u << 6) /* Descriptor Loaded Interrupt Enable */
#define ATHSOTG_DEVDMACONTROL_BURST_LCK (0x1u << 7) /* Burst Lock Enable */
#define ATHSOTG_DEVDMACONTROL_BUFF_LENGTH_Pos 16
#define ATHSOTG_DEVDMACONTROL_BUFF_LENGTH_Msk (0xffffu << ATHSOTG_DEVDMACONTROL_BUFF_LENGTH_Pos) /* */
#define ATHSOTG_DEVDMACONTROL_BUFF_LENGTH(value) ((ATHSOTG_DEVDMACONTROL_BUFF_LENGTH_Msk & ((value) << ATHSOTG_DEVDMACONTROL_BUFF_LENGTH_Pos)))

/* Device DMA Channel Status Register */
#define ATHSOTG_DEVDMASTATUS_CHANN_ENB (0x1u << 0) /* Channel Enable Status */
#define ATHSOTG_DEVDMASTATUS_CHANN_ACT (0x1u << 1) /* Channel Active Status */
#define ATHSOTG_DEVDMASTATUS_END_TR_ST (0x1u << 4) /* End of Channel Transfer Status */
#define ATHSOTG_DEVDMASTATUS_END_BF_ST (0x1u << 5) /* End of Channel Buffer Status */
#define ATHSOTG_DEVDMASTATUS_DESC_LDST (0x1u << 6) /* Descriptor Loaded Status */
#define ATHSOTG_DEVDMASTATUS_BUFF_COUNT_Pos 16
#define ATHSOTG_DEVDMASTATUS_BUFF_COUNT_Msk (0xffffu << ATHSOTG_DEVDMASTATUS_BUFF_COUNT_Pos) /* Buffer Byte Count */
#define ATHSOTG_DEVDMASTATUS_BUFF_COUNT(value) ((ATHSOTG_DEVDMASTATUS_BUFF_COUNT_Msk & ((value) << ATHSOTG_DEVDMASTATUS_BUFF_COUNT_Pos)))

/* Host General Control Register */
#define ATHSOTG_HSTCTRL 0x0400
#define ATHSOTG_HSTCTRL_SOFE (0x1u << 8) /* Start of Frame Generation Enable */
#define ATHSOTG_HSTCTRL_RESET (0x1u << 9) /* Send USB Reset */
#define ATHSOTG_HSTCTRL_RESUME (0x1u << 10) /* Send USB Resume */
#define ATHSOTG_HSTCTRL_SPDCONF_Pos 12
#define ATHSOTG_HSTCTRL_SPDCONF_Msk (0x3u << ATHSOTG_HSTCTRL_SPDCONF_Pos) /* Mode Configuration */
#define ATHSOTG_HSTCTRL_SPDCONF(value) ((ATHSOTG_HSTCTRL_SPDCONF_Msk & ((value) << ATHSOTG_HSTCTRL_SPDCONF_Pos)))
#define   ATHSOTG_HSTCTRL_SPDCONF_NORMAL (0x0u << 12) /* The host starts in Full-speed mode and performs a high-speed reset to switch to High-speed mode if the downstream peripheral is high-speed capable. */
#define   ATHSOTG_HSTCTRL_SPDCONF_LOW_POWER (0x1u << 12) /* For a better consumption, if high speed is not needed. */
#define   ATHSOTG_HSTCTRL_SPDCONF_HIGH_SPEED (0x2u << 12) /* Forced high speed. */
#define   ATHSOTG_HSTCTRL_SPDCONF_FORCED_FS (0x3u << 12) /* The host remains in Full-speed mode whatever the peripheral speed capability. */

/* Host Global Interrupt Status Register */
#define ATHSOTG_HSTISR 0x0404
#define ATHSOTG_HSTISR_DCONNI (0x1u << 0) /* Device Connection Interrupt */
#define ATHSOTG_HSTISR_DDISCI (0x1u << 1) /* Device Disconnection Interrupt */
#define ATHSOTG_HSTISR_RSTI (0x1u << 2) /* USB Reset Sent Interrupt */
#define ATHSOTG_HSTISR_RSMEDI (0x1u << 3) /* Downstream Resume Sent Interrupt */
#define ATHSOTG_HSTISR_RXRSMI (0x1u << 4) /* Upstream Resume Received Interrupt */
#define ATHSOTG_HSTISR_HSOFI (0x1u << 5) /* Host Start of Frame Interrupt */
#define ATHSOTG_HSTISR_HWUPI (0x1u << 6) /* Host Wake-Up Interrupt */
#define ATHSOTG_HSTISR_PEP_0 (0x1u << 8) /* Pipe 0 Interrupt */
#define ATHSOTG_HSTISR_PEP_1 (0x1u << 9) /* Pipe 1 Interrupt */
#define ATHSOTG_HSTISR_PEP_2 (0x1u << 10) /* Pipe 2 Interrupt */
#define ATHSOTG_HSTISR_PEP_3 (0x1u << 11) /* Pipe 3 Interrupt */
#define ATHSOTG_HSTISR_PEP_4 (0x1u << 12) /* Pipe 4 Interrupt */
#define ATHSOTG_HSTISR_PEP_5 (0x1u << 13) /* Pipe 5 Interrupt */
#define ATHSOTG_HSTISR_PEP_6 (0x1u << 14) /* Pipe 6 Interrupt */
#define ATHSOTG_HSTISR_PEP_7 (0x1u << 15) /* Pipe 7 Interrupt */
#define ATHSOTG_HSTISR_PEP_8 (0x1u << 16) /* Pipe 8 Interrupt */
#define ATHSOTG_HSTISR_PEP_9 (0x1u << 17) /* Pipe 9 Interrupt */
#define ATHSOTG_HSTISR_PEP_10 (0x1u << 18) /* Pipe 10 Interrupt */
#define ATHSOTG_HSTISR_PEP_11 (0x1u << 19) /* Pipe 11 Interrupt */
#define ATHSOTG_HSTISR_DMA_1 (0x1u << 25) /* DMA Channel 1 Interrupt */
#define ATHSOTG_HSTISR_DMA_2 (0x1u << 26) /* DMA Channel 2 Interrupt */
#define ATHSOTG_HSTISR_DMA_3 (0x1u << 27) /* DMA Channel 3 Interrupt */
#define ATHSOTG_HSTISR_DMA_4 (0x1u << 28) /* DMA Channel 4 Interrupt */
#define ATHSOTG_HSTISR_DMA_5 (0x1u << 29) /* DMA Channel 5 Interrupt */
#define ATHSOTG_HSTISR_DMA_6 (0x1u << 30) /* DMA Channel 6 Interrupt */
#define ATHSOTG_HSTISR_DMA_7 (0x1u << 31) /* DMA Channel 7 Interrupt */

/* Host Global Interrupt Clear Register */
#define ATHSOTG_HSTICR 0x0408
#define ATHSOTG_HSTICR_DCONNIC (0x1u << 0) /* Device Connection Interrupt Clear */
#define ATHSOTG_HSTICR_DDISCIC (0x1u << 1) /* Device Disconnection Interrupt Clear */
#define ATHSOTG_HSTICR_RSTIC (0x1u << 2) /* USB Reset Sent Interrupt Clear */
#define ATHSOTG_HSTICR_RSMEDIC (0x1u << 3) /* Downstream Resume Sent Interrupt Clear */
#define ATHSOTG_HSTICR_RXRSMIC (0x1u << 4) /* Upstream Resume Received Interrupt Clear */
#define ATHSOTG_HSTICR_HSOFIC (0x1u << 5) /* Host Start of Frame Interrupt Clear */
#define ATHSOTG_HSTICR_HWUPIC (0x1u << 6) /* Host Wake-Up Interrupt Clear */

/* Host Global Interrupt Set Register */
#define ATHSOTG_HSTIFR 0x040C
#define ATHSOTG_HSTIFR_DCONNIS (0x1u << 0) /* Device Connection Interrupt Set */
#define ATHSOTG_HSTIFR_DDISCIS (0x1u << 1) /* Device Disconnection Interrupt Set */
#define ATHSOTG_HSTIFR_RSTIS (0x1u << 2) /* USB Reset Sent Interrupt Set */
#define ATHSOTG_HSTIFR_RSMEDIS (0x1u << 3) /* Downstream Resume Sent Interrupt Set */
#define ATHSOTG_HSTIFR_RXRSMIS (0x1u << 4) /* Upstream Resume Received Interrupt Set */
#define ATHSOTG_HSTIFR_HSOFIS (0x1u << 5) /* Host Start of Frame Interrupt Set */
#define ATHSOTG_HSTIFR_HWUPIS (0x1u << 6) /* Host Wake-Up Interrupt Set */
#define ATHSOTG_HSTIFR_DMA_1 (0x1u << 25) /* DMA Channel 1 Interrupt Set */
#define ATHSOTG_HSTIFR_DMA_2 (0x1u << 26) /* DMA Channel 2 Interrupt Set */
#define ATHSOTG_HSTIFR_DMA_3 (0x1u << 27) /* DMA Channel 3 Interrupt Set */
#define ATHSOTG_HSTIFR_DMA_4 (0x1u << 28) /* DMA Channel 4 Interrupt Set */
#define ATHSOTG_HSTIFR_DMA_5 (0x1u << 29) /* DMA Channel 5 Interrupt Set */
#define ATHSOTG_HSTIFR_DMA_6 (0x1u << 30) /* DMA Channel 6 Interrupt Set */
#define ATHSOTG_HSTIFR_DMA_7 (0x1u << 31) /* DMA Channel 7 Interrupt Set */

/* Host Global Interrupt Mask Register */
#define ATHSOTG_HSTIMR 0x0410
#define ATHSOTG_HSTIMR_DCONNIE (0x1u << 0) /* Device Connection Interrupt Enable */
#define ATHSOTG_HSTIMR_DDISCIE (0x1u << 1) /* Device Disconnection Interrupt Enable */
#define ATHSOTG_HSTIMR_RSTIE (0x1u << 2) /* USB Reset Sent Interrupt Enable */
#define ATHSOTG_HSTIMR_RSMEDIE (0x1u << 3) /* Downstream Resume Sent Interrupt Enable */
#define ATHSOTG_HSTIMR_RXRSMIE (0x1u << 4) /* Upstream Resume Received Interrupt Enable */
#define ATHSOTG_HSTIMR_HSOFIE (0x1u << 5) /* Host Start of Frame Interrupt Enable */
#define ATHSOTG_HSTIMR_HWUPIE (0x1u << 6) /* Host Wake-Up Interrupt Enable */
#define ATHSOTG_HSTIMR_PEP_0 (0x1u << 8) /* Pipe 0 Interrupt Enable */
#define ATHSOTG_HSTIMR_PEP_1 (0x1u << 9) /* Pipe 1 Interrupt Enable */
#define ATHSOTG_HSTIMR_PEP_2 (0x1u << 10) /* Pipe 2 Interrupt Enable */
#define ATHSOTG_HSTIMR_PEP_3 (0x1u << 11) /* Pipe 3 Interrupt Enable */
#define ATHSOTG_HSTIMR_PEP_4 (0x1u << 12) /* Pipe 4 Interrupt Enable */
#define ATHSOTG_HSTIMR_PEP_5 (0x1u << 13) /* Pipe 5 Interrupt Enable */
#define ATHSOTG_HSTIMR_PEP_6 (0x1u << 14) /* Pipe 6 Interrupt Enable */
#define ATHSOTG_HSTIMR_PEP_7 (0x1u << 15) /* Pipe 7 Interrupt Enable */
#define ATHSOTG_HSTIMR_PEP_8 (0x1u << 16) /* Pipe 8 Interrupt Enable */
#define ATHSOTG_HSTIMR_PEP_9 (0x1u << 17) /* Pipe 9 Interrupt Enable */
#define ATHSOTG_HSTIMR_PEP_10 (0x1u << 18) /* Pipe 10 Interrupt Enable */
#define ATHSOTG_HSTIMR_PEP_11 (0x1u << 19) /* Pipe 11 Interrupt Enable */
#define ATHSOTG_HSTIMR_DMA_1 (0x1u << 25) /* DMA Channel 1 Interrupt Enable */
#define ATHSOTG_HSTIMR_DMA_2 (0x1u << 26) /* DMA Channel 2 Interrupt Enable */
#define ATHSOTG_HSTIMR_DMA_3 (0x1u << 27) /* DMA Channel 3 Interrupt Enable */
#define ATHSOTG_HSTIMR_DMA_4 (0x1u << 28) /* DMA Channel 4 Interrupt Enable */
#define ATHSOTG_HSTIMR_DMA_5 (0x1u << 29) /* DMA Channel 5 Interrupt Enable */
#define ATHSOTG_HSTIMR_DMA_6 (0x1u << 30) /* DMA Channel 6 Interrupt Enable */
#define ATHSOTG_HSTIMR_DMA_7 (0x1u << 31) /* DMA Channel 7 Interrupt Enable */

/* Host Global Interrupt Disable Register */
#define ATHSOTG_HSTIDR 0x0414
#define ATHSOTG_HSTIDR_DCONNIEC (0x1u << 0) /* Device Connection Interrupt Disable */
#define ATHSOTG_HSTIDR_DDISCIEC (0x1u << 1) /* Device Disconnection Interrupt Disable */
#define ATHSOTG_HSTIDR_RSTIEC (0x1u << 2) /* USB Reset Sent Interrupt Disable */
#define ATHSOTG_HSTIDR_RSMEDIEC (0x1u << 3) /* Downstream Resume Sent Interrupt Disable */
#define ATHSOTG_HSTIDR_RXRSMIEC (0x1u << 4) /* Upstream Resume Received Interrupt Disable */
#define ATHSOTG_HSTIDR_HSOFIEC (0x1u << 5) /* Host Start of Frame Interrupt Disable */
#define ATHSOTG_HSTIDR_HWUPIEC (0x1u << 6) /* Host Wake-Up Interrupt Disable */
#define ATHSOTG_HSTIDR_PEP_0 (0x1u << 8) /* Pipe 0 Interrupt Disable */
#define ATHSOTG_HSTIDR_PEP_1 (0x1u << 9) /* Pipe 1 Interrupt Disable */
#define ATHSOTG_HSTIDR_PEP_2 (0x1u << 10) /* Pipe 2 Interrupt Disable */
#define ATHSOTG_HSTIDR_PEP_3 (0x1u << 11) /* Pipe 3 Interrupt Disable */
#define ATHSOTG_HSTIDR_PEP_4 (0x1u << 12) /* Pipe 4 Interrupt Disable */
#define ATHSOTG_HSTIDR_PEP_5 (0x1u << 13) /* Pipe 5 Interrupt Disable */
#define ATHSOTG_HSTIDR_PEP_6 (0x1u << 14) /* Pipe 6 Interrupt Disable */
#define ATHSOTG_HSTIDR_PEP_7 (0x1u << 15) /* Pipe 7 Interrupt Disable */
#define ATHSOTG_HSTIDR_PEP_8 (0x1u << 16) /* Pipe 8 Interrupt Disable */
#define ATHSOTG_HSTIDR_PEP_9 (0x1u << 17) /* Pipe 9 Interrupt Disable */
#define ATHSOTG_HSTIDR_PEP_10 (0x1u << 18) /* Pipe 10 Interrupt Disable */
#define ATHSOTG_HSTIDR_PEP_11 (0x1u << 19) /* Pipe 11 Interrupt Disable */
#define ATHSOTG_HSTIDR_DMA_1 (0x1u << 25) /* DMA Channel 1 Interrupt Disable */
#define ATHSOTG_HSTIDR_DMA_2 (0x1u << 26) /* DMA Channel 2 Interrupt Disable */
#define ATHSOTG_HSTIDR_DMA_3 (0x1u << 27) /* DMA Channel 3 Interrupt Disable */
#define ATHSOTG_HSTIDR_DMA_4 (0x1u << 28) /* DMA Channel 4 Interrupt Disable */
#define ATHSOTG_HSTIDR_DMA_5 (0x1u << 29) /* DMA Channel 5 Interrupt Disable */
#define ATHSOTG_HSTIDR_DMA_6 (0x1u << 30) /* DMA Channel 6 Interrupt Disable */
#define ATHSOTG_HSTIDR_DMA_7 (0x1u << 31) /* DMA Channel 7 Interrupt Disable */

/* Host Global Interrupt Enable Register */
#define ATHSOTG_HSTIER 0x0418
#define ATHSOTG_HSTIER_DCONNIES (0x1u << 0) /* Device Connection Interrupt Enable */
#define ATHSOTG_HSTIER_DDISCIES (0x1u << 1) /* Device Disconnection Interrupt Enable */
#define ATHSOTG_HSTIER_RSTIES (0x1u << 2) /* USB Reset Sent Interrupt Enable */
#define ATHSOTG_HSTIER_RSMEDIES (0x1u << 3) /* Downstream Resume Sent Interrupt Enable */
#define ATHSOTG_HSTIER_RXRSMIES (0x1u << 4) /* Upstream Resume Received Interrupt Enable */
#define ATHSOTG_HSTIER_HSOFIES (0x1u << 5) /* Host Start of Frame Interrupt Enable */
#define ATHSOTG_HSTIER_HWUPIES (0x1u << 6) /* Host Wake-Up Interrupt Enable */
#define ATHSOTG_HSTIER_PEP_0 (0x1u << 8) /* Pipe 0 Interrupt Enable */
#define ATHSOTG_HSTIER_PEP_1 (0x1u << 9) /* Pipe 1 Interrupt Enable */
#define ATHSOTG_HSTIER_PEP_2 (0x1u << 10) /* Pipe 2 Interrupt Enable */
#define ATHSOTG_HSTIER_PEP_3 (0x1u << 11) /* Pipe 3 Interrupt Enable */
#define ATHSOTG_HSTIER_PEP_4 (0x1u << 12) /* Pipe 4 Interrupt Enable */
#define ATHSOTG_HSTIER_PEP_5 (0x1u << 13) /* Pipe 5 Interrupt Enable */
#define ATHSOTG_HSTIER_PEP_6 (0x1u << 14) /* Pipe 6 Interrupt Enable */
#define ATHSOTG_HSTIER_PEP_7 (0x1u << 15) /* Pipe 7 Interrupt Enable */
#define ATHSOTG_HSTIER_PEP_8 (0x1u << 16) /* Pipe 8 Interrupt Enable */
#define ATHSOTG_HSTIER_PEP_9 (0x1u << 17) /* Pipe 9 Interrupt Enable */
#define ATHSOTG_HSTIER_PEP_10 (0x1u << 18) /* Pipe 10 Interrupt Enable */
#define ATHSOTG_HSTIER_PEP_11 (0x1u << 19) /* Pipe 11 Interrupt Enable */
#define ATHSOTG_HSTIER_DMA_1 (0x1u << 25) /* DMA Channel 1 Interrupt Enable */
#define ATHSOTG_HSTIER_DMA_2 (0x1u << 26) /* DMA Channel 2 Interrupt Enable */
#define ATHSOTG_HSTIER_DMA_3 (0x1u << 27) /* DMA Channel 3 Interrupt Enable */
#define ATHSOTG_HSTIER_DMA_4 (0x1u << 28) /* DMA Channel 4 Interrupt Enable */
#define ATHSOTG_HSTIER_DMA_5 (0x1u << 29) /* DMA Channel 5 Interrupt Enable */
#define ATHSOTG_HSTIER_DMA_6 (0x1u << 30) /* DMA Channel 6 Interrupt Enable */
#define ATHSOTG_HSTIER_DMA_7 (0x1u << 31) /* DMA Channel 7 Interrupt Enable */

/* Host Pipe Register */
#define ATHSOTG_HSTPIP 0x0041C
#define ATHSOTG_HSTPIP_PEN0 (0x1u << 0) /* Pipe 0 Enable */
#define ATHSOTG_HSTPIP_PEN1 (0x1u << 1) /* Pipe 1 Enable */
#define ATHSOTG_HSTPIP_PEN2 (0x1u << 2) /* Pipe 2 Enable */
#define ATHSOTG_HSTPIP_PEN3 (0x1u << 3) /* Pipe 3 Enable */
#define ATHSOTG_HSTPIP_PEN4 (0x1u << 4) /* Pipe 4 Enable */
#define ATHSOTG_HSTPIP_PEN5 (0x1u << 5) /* Pipe 5 Enable */
#define ATHSOTG_HSTPIP_PEN6 (0x1u << 6) /* Pipe 6 Enable */
#define ATHSOTG_HSTPIP_PEN7 (0x1u << 7) /* Pipe 7 Enable */
#define ATHSOTG_HSTPIP_PEN8 (0x1u << 8) /* Pipe 8 Enable */
#define ATHSOTG_HSTPIP_PRST0 (0x1u << 16) /* Pipe 0 Reset */
#define ATHSOTG_HSTPIP_PRST1 (0x1u << 17) /* Pipe 1 Reset */
#define ATHSOTG_HSTPIP_PRST2 (0x1u << 18) /* Pipe 2 Reset */
#define ATHSOTG_HSTPIP_PRST3 (0x1u << 19) /* Pipe 3 Reset */
#define ATHSOTG_HSTPIP_PRST4 (0x1u << 20) /* Pipe 4 Reset */
#define ATHSOTG_HSTPIP_PRST5 (0x1u << 21) /* Pipe 5 Reset */
#define ATHSOTG_HSTPIP_PRST6 (0x1u << 22) /* Pipe 6 Reset */
#define ATHSOTG_HSTPIP_PRST7 (0x1u << 23) /* Pipe 7 Reset */
#define ATHSOTG_HSTPIP_PRST8 (0x1u << 24) /* Pipe 8 Reset */

/* Host Frame Number Register */
#define ATHSOTG_HSTFNUM 0x0420
#define ATHSOTG_HSTFNUM_MFNUM_Pos 0
#define ATHSOTG_HSTFNUM_MFNUM_Msk (0x7u << ATHSOTG_HSTFNUM_MFNUM_Pos) /* Micro Frame Number */
#define ATHSOTG_HSTFNUM_MFNUM(value) ((ATHSOTG_HSTFNUM_MFNUM_Msk & ((value) << ATHSOTG_HSTFNUM_MFNUM_Pos)))
#define ATHSOTG_HSTFNUM_FNUM_Pos 3
#define ATHSOTG_HSTFNUM_FNUM_Msk (0x7ffu << ATHSOTG_HSTFNUM_FNUM_Pos) /* Frame Number */
#define ATHSOTG_HSTFNUM_FNUM(value) ((ATHSOTG_HSTFNUM_FNUM_Msk & ((value) << ATHSOTG_HSTFNUM_FNUM_Pos)))
#define ATHSOTG_HSTFNUM_FLENHIGH_Pos 16
#define ATHSOTG_HSTFNUM_FLENHIGH_Msk (0xffu << ATHSOTG_HSTFNUM_FLENHIGH_Pos) /* Frame Length */
#define ATHSOTG_HSTFNUM_FLENHIGH(value) ((ATHSOTG_HSTFNUM_FLENHIGH_Msk & ((value) << ATHSOTG_HSTFNUM_FLENHIGH_Pos)))

/* Host Address 1 Register */
#define ATHSOTG_HSTADDR1 0x0424
#define ATHSOTG_HSTADDR1_HSTADDRP0_Pos 0
#define ATHSOTG_HSTADDR1_HSTADDRP0_Msk (0x7fu << ATHSOTG_HSTADDR1_HSTADDRP0_Pos) /* USB Host Address */
#define ATHSOTG_HSTADDR1_HSTADDRP0(value) ((ATHSOTG_HSTADDR1_HSTADDRP0_Msk & ((value) << ATHSOTG_HSTADDR1_HSTADDRP0_Pos)))
#define ATHSOTG_HSTADDR1_HSTADDRP1_Pos 8
#define ATHSOTG_HSTADDR1_HSTADDRP1_Msk (0x7fu << ATHSOTG_HSTADDR1_HSTADDRP1_Pos) /* USB Host Address */
#define ATHSOTG_HSTADDR1_HSTADDRP1(value) ((ATHSOTG_HSTADDR1_HSTADDRP1_Msk & ((value) << ATHSOTG_HSTADDR1_HSTADDRP1_Pos)))
#define ATHSOTG_HSTADDR1_HSTADDRP2_Pos 16
#define ATHSOTG_HSTADDR1_HSTADDRP2_Msk (0x7fu << ATHSOTG_HSTADDR1_HSTADDRP2_Pos) /* USB Host Address */
#define ATHSOTG_HSTADDR1_HSTADDRP2(value) ((ATHSOTG_HSTADDR1_HSTADDRP2_Msk & ((value) << ATHSOTG_HSTADDR1_HSTADDRP2_Pos)))
#define ATHSOTG_HSTADDR1_HSTADDRP3_Pos 24
#define ATHSOTG_HSTADDR1_HSTADDRP3_Msk (0x7fu << ATHSOTG_HSTADDR1_HSTADDRP3_Pos) /* USB Host Address */
#define ATHSOTG_HSTADDR1_HSTADDRP3(value) ((ATHSOTG_HSTADDR1_HSTADDRP3_Msk & ((value) << ATHSOTG_HSTADDR1_HSTADDRP3_Pos)))

/* Host Address 2 Register */
#define ATHSOTG_HSTADDR2 0x0428
#define ATHSOTG_HSTADDR2_HSTADDRP4_Pos 0
#define ATHSOTG_HSTADDR2_HSTADDRP4_Msk (0x7fu << ATHSOTG_HSTADDR2_HSTADDRP4_Pos) /* USB Host Address */
#define ATHSOTG_HSTADDR2_HSTADDRP4(value) ((ATHSOTG_HSTADDR2_HSTADDRP4_Msk & ((value) << ATHSOTG_HSTADDR2_HSTADDRP4_Pos)))
#define ATHSOTG_HSTADDR2_HSTADDRP5_Pos 8
#define ATHSOTG_HSTADDR2_HSTADDRP5_Msk (0x7fu << ATHSOTG_HSTADDR2_HSTADDRP5_Pos) /* USB Host Address */
#define ATHSOTG_HSTADDR2_HSTADDRP5(value) ((ATHSOTG_HSTADDR2_HSTADDRP5_Msk & ((value) << ATHSOTG_HSTADDR2_HSTADDRP5_Pos)))
#define ATHSOTG_HSTADDR2_HSTADDRP6_Pos 16
#define ATHSOTG_HSTADDR2_HSTADDRP6_Msk (0x7fu << ATHSOTG_HSTADDR2_HSTADDRP6_Pos) /* USB Host Address */
#define ATHSOTG_HSTADDR2_HSTADDRP6(value) ((ATHSOTG_HSTADDR2_HSTADDRP6_Msk & ((value) << ATHSOTG_HSTADDR2_HSTADDRP6_Pos)))
#define ATHSOTG_HSTADDR2_HSTADDRP7_Pos 24
#define ATHSOTG_HSTADDR2_HSTADDRP7_Msk (0x7fu << ATHSOTG_HSTADDR2_HSTADDRP7_Pos) /* USB Host Address */
#define ATHSOTG_HSTADDR2_HSTADDRP7(value) ((ATHSOTG_HSTADDR2_HSTADDRP7_Msk & ((value) << ATHSOTG_HSTADDR2_HSTADDRP7_Pos)))

/* Host Address 3 Register */
#define ATHSOTG_HSTADDR3 0x042C
#define ATHSOTG_HSTADDR3_HSTADDRP8_Pos 0
#define ATHSOTG_HSTADDR3_HSTADDRP8_Msk (0x7fu << ATHSOTG_HSTADDR3_HSTADDRP8_Pos) /* USB Host Address */
#define ATHSOTG_HSTADDR3_HSTADDRP8(value) ((ATHSOTG_HSTADDR3_HSTADDRP8_Msk & ((value) << ATHSOTG_HSTADDR3_HSTADDRP8_Pos)))
#define ATHSOTG_HSTADDR3_HSTADDRP9_Pos 8
#define ATHSOTG_HSTADDR3_HSTADDRP9_Msk (0x7fu << ATHSOTG_HSTADDR3_HSTADDRP9_Pos) /* USB Host Address */
#define ATHSOTG_HSTADDR3_HSTADDRP9(value) ((ATHSOTG_HSTADDR3_HSTADDRP9_Msk & ((value) << ATHSOTG_HSTADDR3_HSTADDRP9_Pos)))

/* Host Pipe Configuration Register (n = 0) */
#define ATHSOTG_HSTPIPCFG 0x500
#define ATHSOTG_HSTPIPCFG_ALLOC (0x1u << 1) /* Pipe Memory Allocate */
#define ATHSOTG_HSTPIPCFG_PBK_Pos 2
#define ATHSOTG_HSTPIPCFG_PBK_Msk (0x3u << ATHSOTG_HSTPIPCFG_PBK_Pos) /* Pipe Banks */
#define ATHSOTG_HSTPIPCFG_PBK(value) ((ATHSOTG_HSTPIPCFG_PBK_Msk & ((value) << ATHSOTG_HSTPIPCFG_PBK_Pos)))
#define   ATHSOTG_HSTPIPCFG_PBK_1_BANK (0x0u << 2) /* Single-bank pipe */
#define   ATHSOTG_HSTPIPCFG_PBK_2_BANK (0x1u << 2) /* Double-bank pipe */
#define   ATHSOTG_HSTPIPCFG_PBK_3_BANK (0x2u << 2) /* Triple-bank pipe */
#define ATHSOTG_HSTPIPCFG_PSIZE_Pos 4
#define ATHSOTG_HSTPIPCFG_PSIZE_Msk (0x7u << ATHSOTG_HSTPIPCFG_PSIZE_Pos) /* Pipe Size */
#define ATHSOTG_HSTPIPCFG_PSIZE(value) ((ATHSOTG_HSTPIPCFG_PSIZE_Msk & ((value) << ATHSOTG_HSTPIPCFG_PSIZE_Pos)))
#define   ATHSOTG_HSTPIPCFG_PSIZE_8_BYTE (0x0u << 4) /* 8 bytes */
#define   ATHSOTG_HSTPIPCFG_PSIZE_16_BYTE (0x1u << 4) /* 16 bytes */
#define   ATHSOTG_HSTPIPCFG_PSIZE_32_BYTE (0x2u << 4) /* 32 bytes */
#define   ATHSOTG_HSTPIPCFG_PSIZE_64_BYTE (0x3u << 4) /* 64 bytes */
#define   ATHSOTG_HSTPIPCFG_PSIZE_128_BYTE (0x4u << 4) /* 128 bytes */
#define   ATHSOTG_HSTPIPCFG_PSIZE_256_BYTE (0x5u << 4) /* 256 bytes */
#define   ATHSOTG_HSTPIPCFG_PSIZE_512_BYTE (0x6u << 4) /* 512 bytes */
#define   ATHSOTG_HSTPIPCFG_PSIZE_1024_BYTE (0x7u << 4) /* 1024 bytes */
#define ATHSOTG_HSTPIPCFG_PTOKEN_Pos 8
#define ATHSOTG_HSTPIPCFG_PTOKEN_Msk (0x3u << ATHSOTG_HSTPIPCFG_PTOKEN_Pos) /* Pipe Token */
#define ATHSOTG_HSTPIPCFG_PTOKEN(value) ((ATHSOTG_HSTPIPCFG_PTOKEN_Msk & ((value) << ATHSOTG_HSTPIPCFG_PTOKEN_Pos)))
#define   ATHSOTG_HSTPIPCFG_PTOKEN_SETUP (0x0u << 8) /* SETUP */
#define   ATHSOTG_HSTPIPCFG_PTOKEN_IN (0x1u << 8) /* IN */
#define   ATHSOTG_HSTPIPCFG_PTOKEN_OUT (0x2u << 8) /* OUT */
#define ATHSOTG_HSTPIPCFG_AUTOSW (0x1u << 10) /* Automatic Switch */
#define ATHSOTG_HSTPIPCFG_PTYPE_Pos 12
#define ATHSOTG_HSTPIPCFG_PTYPE_Msk (0x3u << ATHSOTG_HSTPIPCFG_PTYPE_Pos) /* Pipe Type */
#define ATHSOTG_HSTPIPCFG_PTYPE(value) ((ATHSOTG_HSTPIPCFG_PTYPE_Msk & ((value) << ATHSOTG_HSTPIPCFG_PTYPE_Pos)))
#define   ATHSOTG_HSTPIPCFG_PTYPE_CTRL (0x0u << 12) /* Control */
#define   ATHSOTG_HSTPIPCFG_PTYPE_ISO (0x1u << 12) /* Isochronous */
#define   ATHSOTG_HSTPIPCFG_PTYPE_BLK (0x2u << 12) /* Bulk */
#define   ATHSOTG_HSTPIPCFG_PTYPE_INTRPT (0x3u << 12) /* Interrupt */
#define ATHSOTG_HSTPIPCFG_PEPNUM_Pos 16
#define ATHSOTG_HSTPIPCFG_PEPNUM_Msk (0xfu << ATHSOTG_HSTPIPCFG_PEPNUM_Pos) /* Pipe Endpoint Number */
#define ATHSOTG_HSTPIPCFG_PEPNUM(value) ((ATHSOTG_HSTPIPCFG_PEPNUM_Msk & ((value) << ATHSOTG_HSTPIPCFG_PEPNUM_Pos)))
#define ATHSOTG_HSTPIPCFG_INTFRQ_Pos 24
#define ATHSOTG_HSTPIPCFG_INTFRQ_Msk (0xffu << ATHSOTG_HSTPIPCFG_INTFRQ_Pos) /* Pipe Interrupt Request Frequency */
#define ATHSOTG_HSTPIPCFG_INTFRQ(value) ((ATHSOTG_HSTPIPCFG_INTFRQ_Msk & ((value) << ATHSOTG_HSTPIPCFG_INTFRQ_Pos)))
#define ATHSOTG_HSTPIPCFG_PINGEN (0x1u << 20) /* Ping Enable */
#define ATHSOTG_HSTPIPCFG_BINTERVAL_Pos 24
#define ATHSOTG_HSTPIPCFG_BINTERVAL_Msk (0xffu << ATHSOTG_HSTPIPCFG_BINTERVAL_Pos) /* Binterval Parameter for the Bulk-Out/Ping Transaction */
#define ATHSOTG_HSTPIPCFG_BINTERVAL(value) ((ATHSOTG_HSTPIPCFG_BINTERVAL_Msk & ((value) << ATHSOTG_HSTPIPCFG_BINTERVAL_Pos)))

/* Host Pipe Status Register (n = 0) */
#define ATHSOTG_HSTPIPISR 0x530
#define ATHSOTG_HSTPIPISR_RXINI (0x1u << 0) /* Received IN Data Interrupt */
#define ATHSOTG_HSTPIPISR_TXOUTI (0x1u << 1) /* Transmitted OUT Data Interrupt */
#define ATHSOTG_HSTPIPISR_TXSTPI (0x1u << 2) /* Transmitted SETUP Interrupt */
#define ATHSOTG_HSTPIPISR_PERRI (0x1u << 3) /* Pipe Error Interrupt */
#define ATHSOTG_HSTPIPISR_NAKEDI (0x1u << 4) /* NAKed Interrupt */
#define ATHSOTG_HSTPIPISR_OVERFI (0x1u << 5) /* Overflow Interrupt */
#define ATHSOTG_HSTPIPISR_RXSTALLDI (0x1u << 6) /* Received STALLed Interrupt */
#define ATHSOTG_HSTPIPISR_SHORTPACKETI (0x1u << 7) /* Short Packet Interrupt */
#define ATHSOTG_HSTPIPISR_DTSEQ_Pos 8
#define ATHSOTG_HSTPIPISR_DTSEQ_Msk (0x3u << ATHSOTG_HSTPIPISR_DTSEQ_Pos) /* Data Toggle Sequence */
#define   ATHSOTG_HSTPIPISR_DTSEQ_DATA0 (0x0u << 8) /* Data0 toggle sequence */
#define   ATHSOTG_HSTPIPISR_DTSEQ_DATA1 (0x1u << 8) /* Data1 toggle sequence */
#define ATHSOTG_HSTPIPISR_NBUSYBK_Pos 12
#define ATHSOTG_HSTPIPISR_NBUSYBK_Msk (0x3u << ATHSOTG_HSTPIPISR_NBUSYBK_Pos) /* Number of Busy Banks */
#define   ATHSOTG_HSTPIPISR_NBUSYBK_0_BUSY (0x0u << 12) /* */
#define   ATHSOTG_HSTPIPISR_NBUSYBK_1_BUSY (0x1u << 12) /* 1 busy bank */
#define   ATHSOTG_HSTPIPISR_NBUSYBK_2_BUSY (0x2u << 12) /* 2 busy banks */
#define   ATHSOTG_HSTPIPISR_NBUSYBK_3_BUSY (0x3u << 12) /* 3 busy banks */
#define ATHSOTG_HSTPIPISR_CURRBK_Pos 14
#define ATHSOTG_HSTPIPISR_CURRBK_Msk (0x3u << ATHSOTG_HSTPIPISR_CURRBK_Pos) /* Current Bank */
#define   ATHSOTG_HSTPIPISR_CURRBK_BANK0 (0x0u << 14) /* Current bank is bank0 */
#define   ATHSOTG_HSTPIPISR_CURRBK_BANK1 (0x1u << 14) /* Current bank is bank1 */
#define   ATHSOTG_HSTPIPISR_CURRBK_BANK2 (0x2u << 14) /* Current bank is bank2 */
#define ATHSOTG_HSTPIPISR_RWALL (0x1u << 16) /* Read/Write Allowed */
#define ATHSOTG_HSTPIPISR_CFGOK (0x1u << 18) /* Configuration OK Status */
#define ATHSOTG_HSTPIPISR_PBYCT_Pos 20
#define ATHSOTG_HSTPIPISR_PBYCT_Msk (0x7ffu << ATHSOTG_HSTPIPISR_PBYCT_Pos) /* Pipe Byte Count */
#define ATHSOTG_HSTPIPISR_UNDERFI (0x1u << 2) /* Underflow Interrupt */
#define ATHSOTG_HSTPIPISR_CRCERRI (0x1u << 6) /* CRC Error Interrupt */

/* Host Pipe Clear Register (n = 0) */
#define ATHSOTG_HSTPIPICR 0x560
#define ATHSOTG_HSTPIPICR_RXINIC (0x1u << 0) /* Received IN Data Interrupt Clear */
#define ATHSOTG_HSTPIPICR_TXOUTIC (0x1u << 1) /* Transmitted OUT Data Interrupt Clear */
#define ATHSOTG_HSTPIPICR_TXSTPIC (0x1u << 2) /* Transmitted SETUP Interrupt Clear */
#define ATHSOTG_HSTPIPICR_NAKEDIC (0x1u << 4) /* NAKed Interrupt Clear */
#define ATHSOTG_HSTPIPICR_OVERFIC (0x1u << 5) /* Overflow Interrupt Clear */
#define ATHSOTG_HSTPIPICR_RXSTALLDIC (0x1u << 6) /* Received STALLed Interrupt Clear */
#define ATHSOTG_HSTPIPICR_SHORTPACKETIC (0x1u << 7) /* Short Packet Interrupt Clear */
#define ATHSOTG_HSTPIPICR_UNDERFIC (0x1u << 2) /* Underflow Interrupt Clear */
#define ATHSOTG_HSTPIPICR_CRCERRIC (0x1u << 6) /* CRC Error Interrupt Clear */

/* Host Pipe Set Register (n = 0) */
#define ATHSOTG_HSTPIPIFR 0x590
#define ATHSOTG_HSTPIPIFR_RXINIS (0x1u << 0) /* Received IN Data Interrupt Set */
#define ATHSOTG_HSTPIPIFR_TXOUTIS (0x1u << 1) /* Transmitted OUT Data Interrupt Set */
#define ATHSOTG_HSTPIPIFR_TXSTPIS (0x1u << 2) /* Transmitted SETUP Interrupt Set */
#define ATHSOTG_HSTPIPIFR_PERRIS (0x1u << 3) /* Pipe Error Interrupt Set */
#define ATHSOTG_HSTPIPIFR_NAKEDIS (0x1u << 4) /* NAKed Interrupt Set */
#define ATHSOTG_HSTPIPIFR_OVERFIS (0x1u << 5) /* Overflow Interrupt Set */
#define ATHSOTG_HSTPIPIFR_RXSTALLDIS (0x1u << 6) /* Received STALLed Interrupt Set */
#define ATHSOTG_HSTPIPIFR_SHORTPACKETIS (0x1u << 7) /* Short Packet Interrupt Set */
#define ATHSOTG_HSTPIPIFR_NBUSYBKS (0x1u << 12) /* Number of Busy Banks Set */
#define ATHSOTG_HSTPIPIFR_UNDERFIS (0x1u << 2) /* Underflow Interrupt Set */
#define ATHSOTG_HSTPIPIFR_CRCERRIS (0x1u << 6) /* CRC Error Interrupt Set */

/* Host Pipe Mask Register (n = 0) */
#define ATHSOTG_HSTPIPIMR 0x5C0
#define ATHSOTG_HSTPIPIMR_RXINE (0x1u << 0) /* Received IN Data Interrupt Enable */
#define ATHSOTG_HSTPIPIMR_TXOUTE (0x1u << 1) /* Transmitted OUT Data Interrupt Enable */
#define ATHSOTG_HSTPIPIMR_TXSTPE (0x1u << 2) /* Transmitted SETUP Interrupt Enable */
#define ATHSOTG_HSTPIPIMR_PERRE (0x1u << 3) /* Pipe Error Interrupt Enable */
#define ATHSOTG_HSTPIPIMR_NAKEDE (0x1u << 4) /* NAKed Interrupt Enable */
#define ATHSOTG_HSTPIPIMR_OVERFIE (0x1u << 5) /* Overflow Interrupt Enable */
#define ATHSOTG_HSTPIPIMR_RXSTALLDE (0x1u << 6) /* Received STALLed Interrupt Enable */
#define ATHSOTG_HSTPIPIMR_SHORTPACKETIE (0x1u << 7) /* Short Packet Interrupt Enable */
#define ATHSOTG_HSTPIPIMR_NBUSYBKE (0x1u << 12) /* Number of Busy Banks Interrupt Enable */
#define ATHSOTG_HSTPIPIMR_FIFOCON (0x1u << 14) /* FIFO Control */
#define ATHSOTG_HSTPIPIMR_PDISHDMA (0x1u << 16) /* Pipe Interrupts Disable HDMA Request Enable */
#define ATHSOTG_HSTPIPIMR_PFREEZE (0x1u << 17) /* Pipe Freeze */
#define ATHSOTG_HSTPIPIMR_RSTDT (0x1u << 18) /* Reset Data Toggle */
#define ATHSOTG_HSTPIPIMR_UNDERFIE (0x1u << 2) /* Underflow Interrupt Enable */
#define ATHSOTG_HSTPIPIMR_CRCERRE (0x1u << 6) /* CRC Error Interrupt Enable */

/* Host Pipe Enable Register (n = 0) */
#define ATHSOTG_HSTPIPIER 0x5F0
#define ATHSOTG_HSTPIPIER_RXINES (0x1u << 0) /* Received IN Data Interrupt Enable */
#define ATHSOTG_HSTPIPIER_TXOUTES (0x1u << 1) /* Transmitted OUT Data Interrupt Enable */
#define ATHSOTG_HSTPIPIER_TXSTPES (0x1u << 2) /* Transmitted SETUP Interrupt Enable */
#define ATHSOTG_HSTPIPIER_PERRES (0x1u << 3) /* Pipe Error Interrupt Enable */
#define ATHSOTG_HSTPIPIER_NAKEDES (0x1u << 4) /* NAKed Interrupt Enable */
#define ATHSOTG_HSTPIPIER_OVERFIES (0x1u << 5) /* Overflow Interrupt Enable */
#define ATHSOTG_HSTPIPIER_RXSTALLDES (0x1u << 6) /* Received STALLed Interrupt Enable */
#define ATHSOTG_HSTPIPIER_SHORTPACKETIES (0x1u << 7) /* Short Packet Interrupt Enable */
#define ATHSOTG_HSTPIPIER_NBUSYBKES (0x1u << 12) /* Number of Busy Banks Enable */
#define ATHSOTG_HSTPIPIER_PDISHDMAS (0x1u << 16) /* Pipe Interrupts Disable HDMA Request Enable */
#define ATHSOTG_HSTPIPIER_PFREEZES (0x1u << 17) /* Pipe Freeze Enable */
#define ATHSOTG_HSTPIPIER_RSTDTS (0x1u << 18) /* Reset Data Toggle Enable */
#define ATHSOTG_HSTPIPIER_UNDERFIES (0x1u << 2) /* Underflow Interrupt Enable */
#define ATHSOTG_HSTPIPIER_CRCERRES (0x1u << 6) /* CRC Error Interrupt Enable */

/* Host Pipe Disable Register (n = 0) */
#define ATHSOTG_HSTPIPIDR 0x620
#define ATHSOTG_HSTPIPIDR_RXINEC (0x1u << 0) /* Received IN Data Interrupt Disable */
#define ATHSOTG_HSTPIPIDR_TXOUTEC (0x1u << 1) /* Transmitted OUT Data Interrupt Disable */
#define ATHSOTG_HSTPIPIDR_TXSTPEC (0x1u << 2) /* Transmitted SETUP Interrupt Disable */
#define ATHSOTG_HSTPIPIDR_PERREC (0x1u << 3) /* Pipe Error Interrupt Disable */
#define ATHSOTG_HSTPIPIDR_NAKEDEC (0x1u << 4) /* NAKed Interrupt Disable */
#define ATHSOTG_HSTPIPIDR_OVERFIEC (0x1u << 5) /* Overflow Interrupt Disable */
#define ATHSOTG_HSTPIPIDR_RXSTALLDEC (0x1u << 6) /* Received STALLed Interrupt Disable */
#define ATHSOTG_HSTPIPIDR_SHORTPACKETIEC (0x1u << 7) /* Short Packet Interrupt Disable */
#define ATHSOTG_HSTPIPIDR_NBUSYBKEC (0x1u << 12) /* Number of Busy Banks Disable */
#define ATHSOTG_HSTPIPIDR_FIFOCONC (0x1u << 14) /* FIFO Control Disable */
#define ATHSOTG_HSTPIPIDR_PDISHDMAC (0x1u << 16) /* Pipe Interrupts Disable HDMA Request Disable */
#define ATHSOTG_HSTPIPIDR_PFREEZEC (0x1u << 17) /* Pipe Freeze Disable */
#define ATHSOTG_HSTPIPIDR_UNDERFIEC (0x1u << 2) /* Underflow Interrupt Disable */
#define ATHSOTG_HSTPIPIDR_CRCERREC (0x1u << 6) /* CRC Error Interrupt Disable */

/* Host Pipe IN Request Register (n = 0) */
#define ATHSOTG_HSTPIPINRQ 0x650
#define ATHSOTG_HSTPIPINRQ_INRQ_Pos 0
#define ATHSOTG_HSTPIPINRQ_INRQ_Msk (0xffu << ATHSOTG_HSTPIPINRQ_INRQ_Pos) /* IN Request Number before Freeze */
#define ATHSOTG_HSTPIPINRQ_INRQ(value) ((ATHSOTG_HSTPIPINRQ_INRQ_Msk & ((value) << ATHSOTG_HSTPIPINRQ_INRQ_Pos)))
#define ATHSOTG_HSTPIPINRQ_INMODE (0x1u << 8) /* IN Request Mode */

/* Host Pipe Error Register (n = 0) */
#define ATHSOTG_HSTPIPERR 0x680
#define ATHSOTG_HSTPIPERR_DATATGL (0x1u << 0) /* Data Toggle Error */
#define ATHSOTG_HSTPIPERR_DATAPID (0x1u << 1) /* Data PID Error */
#define ATHSOTG_HSTPIPERR_PID (0x1u << 2) /* Data PID Error */
#define ATHSOTG_HSTPIPERR_TIMEOUT (0x1u << 3) /* Time-Out Error */
#define ATHSOTG_HSTPIPERR_CRC16 (0x1u << 4) /* CRC16 Error */
#define ATHSOTG_HSTPIPERR_COUNTER_Pos 5
#define ATHSOTG_HSTPIPERR_COUNTER_Msk (0x3u << ATHSOTG_HSTPIPERR_COUNTER_Pos) /* Error Counter */
#define ATHSOTG_HSTPIPERR_COUNTER(value) ((ATHSOTG_HSTPIPERR_COUNTER_Msk & ((value) << ATHSOTG_HSTPIPERR_COUNTER_Pos)))

/* Host DMA Channel Next Descriptor Address Register */
#define ATHSOTG_HSTDMANXTDSC_NXT_DSC_ADD_Pos 0
#define ATHSOTG_HSTDMANXTDSC_NXT_DSC_ADD_Msk (0xffffffffu << ATHSOTG_HSTDMANXTDSC_NXT_DSC_ADD_Pos) /* Next Descriptor Address */
#define ATHSOTG_HSTDMANXTDSC_NXT_DSC_ADD(value) ((ATHSOTG_HSTDMANXTDSC_NXT_DSC_ADD_Msk & ((value) << ATHSOTG_HSTDMANXTDSC_NXT_DSC_ADD_Pos)))

/* Host DMA Channel Address Register */
#define ATHSOTG_HSTDMAADDRESS_BUFF_ADD_Pos 0
#define ATHSOTG_HSTDMAADDRESS_BUFF_ADD_Msk (0xffffffffu << ATHSOTG_HSTDMAADDRESS_BUFF_ADD_Pos) /* Buffer Address */
#define ATHSOTG_HSTDMAADDRESS_BUFF_ADD(value) ((ATHSOTG_HSTDMAADDRESS_BUFF_ADD_Msk & ((value) << ATHSOTG_HSTDMAADDRESS_BUFF_ADD_Pos)))

/* Host DMA Channel Control Register */
#define ATHSOTG_HSTDMACONTROL_CHANN_ENB (0x1u << 0) /* Channel Enable Command */
#define ATHSOTG_HSTDMACONTROL_LDNXT_DSC (0x1u << 1) /* Load Next Channel Transfer Descriptor Enable Command */
#define ATHSOTG_HSTDMACONTROL_END_TR_EN (0x1u << 2) /* */
#define ATHSOTG_HSTDMACONTROL_END_B_EN (0x1u << 3) /* End of Buffer Enable Control */
#define ATHSOTG_HSTDMACONTROL_END_TR_IT (0x1u << 4) /* End of Transfer Interrupt Enable */
#define ATHSOTG_HSTDMACONTROL_END_BUFFIT (0x1u << 5) /* End of Buffer Interrupt Enable */
#define ATHSOTG_HSTDMACONTROL_DESC_LD_IT (0x1u << 6) /* Descriptor Loaded Interrupt Enable */
#define ATHSOTG_HSTDMACONTROL_BURST_LCK (0x1u << 7) /* Burst Lock Enable */
#define ATHSOTG_HSTDMACONTROL_BUFF_LENGTH_Pos 16
#define ATHSOTG_HSTDMACONTROL_BUFF_LENGTH_Msk (0xffffu << ATHSOTG_HSTDMACONTROL_BUFF_LENGTH_Pos) /* */
#define ATHSOTG_HSTDMACONTROL_BUFF_LENGTH(value) ((ATHSOTG_HSTDMACONTROL_BUFF_LENGTH_Msk & ((value) << ATHSOTG_HSTDMACONTROL_BUFF_LENGTH_Pos)))

/* Host DMA Channel Status Register */
#define ATHSOTG_HSTDMASTATUS_CHANN_ENB (0x1u << 0) /* Channel Enable Status */
#define ATHSOTG_HSTDMASTATUS_CHANN_ACT (0x1u << 1) /* Channel Active Status */
#define ATHSOTG_HSTDMASTATUS_END_TR_ST (0x1u << 4) /* End of Channel Transfer Status */
#define ATHSOTG_HSTDMASTATUS_END_BF_ST (0x1u << 5) /* End of Channel Buffer Status */
#define ATHSOTG_HSTDMASTATUS_DESC_LDST (0x1u << 6) /* Descriptor Loaded Status */
#define ATHSOTG_HSTDMASTATUS_BUFF_COUNT_Pos 16
#define ATHSOTG_HSTDMASTATUS_BUFF_COUNT_Msk (0xffffu << ATHSOTG_HSTDMASTATUS_BUFF_COUNT_Pos) /* Buffer Byte Count */
#define ATHSOTG_HSTDMASTATUS_BUFF_COUNT(value) ((ATHSOTG_HSTDMASTATUS_BUFF_COUNT_Msk & ((value) << ATHSOTG_HSTDMASTATUS_BUFF_COUNT_Pos)))

/* General Control Register */
#define ATHSOTG_CTRL 0x0800
#define ATHSOTG_CTRL_RDERRE (0x1u << 4) /* Remote Device Connection Error Interrupt Enable */
#define ATHSOTG_CTRL_VBUSHWC (0x1u << 8) /* VBUS Hardware Control */
#define ATHSOTG_CTRL_FRZCLK (0x1u << 14) /* Freeze USB Clock */
#define ATHSOTG_CTRL_USBE (0x1u << 15) /* ATHS Enable */
#define ATHSOTG_CTRL_UIMOD (0x1u << 25) /* ATHS Mode */
#define   ATHSOTG_CTRL_UIMOD_HOST (0x0u << 25) /* The module is in USB Host mode. */
#define   ATHSOTG_CTRL_UIMOD_DEVICE (0x1u << 25) /* The module is in USB Device mode. */

/* General Status Register */
#define ATHSOTG_SR 0x0804
#define ATHSOTG_SR_RDERRI (0x1u << 4) /* */
#define ATHSOTG_SR_VBUSRQ (0x1u << 9) /* */
#define ATHSOTG_SR_SPEED_Pos 12
#define ATHSOTG_SR_SPEED_Msk (0x3u << ATHSOTG_SR_SPEED_Pos) /* */
#define   ATHSOTG_SR_SPEED_FULL_SPEED (0x0u << 12) /* Full-Speed mode */
#define   ATHSOTG_SR_SPEED_HIGH_SPEED (0x1u << 12) /* High-Speed mode */
#define   ATHSOTG_SR_SPEED_LOW_SPEED (0x2u << 12) /* Low-Speed mode */
#define ATHSOTG_SR_CLKUSABLE (0x1u << 14) /* UTMI Clock Usable */

/* General Status Clear Register */
#define ATHSOTG_SCR 0x0808
#define ATHSOTG_SCR_RDERRIC (0x1u << 4) /* Remote Device Connection Error Interrupt Clear */
#define ATHSOTG_SCR_VBUSRQC (0x1u << 9) /* VBUS Request Clear */

/* General Status Set Register */
#define ATHSOTG_SFR 0x080C
#define ATHSOTG_SFR_RDERRIS (0x1u << 4) /* Remote Device Connection Error Interrupt Set */
#define ATHSOTG_SFR_VBUSRQS (0x1u << 9) /* VBUS Request Set */

struct athsotg_softc {
	struct usb_bus sc_bus;
	struct resource *sc_io_res;
	struct resource *sc_irq_res;
	bus_space_tag_t sc_io_tag;
	bus_space_handle_t sc_io_hdl;
	void *sc_intr_hdl;
};

#define	ATHSOTG_READ_4(sc, reg) \
    bus_space_read_4((sc)->sc_io_tag, (sc)->sc_io_hdl, reg)

#define	ATHSOTG_WRITE_4(sc, reg, data) \
    bus_space_write_4((sc)->sc_io_tag, (sc)->sc_io_hdl, reg, data)

usb_error_t athsotg_init(struct athsotg_softc *sc);
void	athsotg_uninit(struct athsotg_softc *sc);
driver_intr_t athsotg_interrupt;

#endif /* _ATHSOTG_H_ */
