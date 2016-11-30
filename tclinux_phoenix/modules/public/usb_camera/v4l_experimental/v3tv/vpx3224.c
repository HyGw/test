/*

 * vpx3224d & vpx3225d video decoder driver version 0.0.1
 *
 * Copyright (C) 2004 Perry Gilfillan <perrye@linuxmail.org>
 *
 * Based on work:
 *
 *   Copyright (C) 2001 Laurent Pinchart <lpinchart@freegates.be>
 *       vpx3220a, vpx3216b & vpx3214c video decoder driver version 0.0.1
 *
 *   vpx322x.h - Part of voodoo3500tv v4l drivers
 *
 *   vpx322x.c - Part of voodoo3500tv v4l drivers
 *   Copyright (c) 2000 Juha Valkama <juhisv@surfree.com>
 *
 *   vpx322x-i2c.c - Part of voodoo3500tv v4l drivers
 *   Copyright (c) 2000 Juha Valkama <juhisv@surfree.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/types.h>
#include <linux/slab.h>

#include <linux/byteorder/swab.h>

#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include <linux/videodev.h>
#include <linux/video_decoder.h>

#if 0
static inline int vpx3224_write(struct i2c_client *client,
				u8 reg, u8 value);
static inline int vpx3224_read(struct i2c_client *client, u8 reg);
static int
vpx3224_write_block(struct i2c_client *client,
		    const u8 * data, unsigned int len);
static int vpx3224_fp_status(struct i2c_client *client);
static int
vpx3224_fp_write(struct i2c_client *client, u16 fpaddr, u16 data);
static int vpx3224_fp_read(struct i2c_client *client, u16 fpaddr);
static int
vpx3224_write_fp_block(struct i2c_client *client,
		       const u16 * data, int len);
#endif

static int vpx3224_detect_client(struct i2c_adapter *adapter, int address,
		int kind);

/* Each client has this additional data */

struct vpx3224 {
	int norm;
	int input;
	int enable;
	int bright;
	int contrast;
	int hue;
	int sat;
	int sysctl_id;

	int err;
};

static int debug = 0;

/* Many vpx3224 constants specified below */

#define I2C_VPX3224		0x43

#ifndef I2C_DRIVERID_VPX3224
#define I2C_DRIVERID_VPX3224	0xFF
#endif

#define VPX_VERSION		"0.1-Multistandard"

#define VPX_TIMEOUT_COUNT	10

#define VPX3224_MAX_INPUT	3	/* 3 CVBS, 5 SVHS */
#define VPX3224_MAX_OUTPUT	1	/* 1 CVBS */

#define MICRONAS_INTERMETALL	0xEC
#define VPX3216B		0x4260
#define VPX3214C		0x4280
#define VPX3220A		0x4680
#define VPX3225D                0x7230	/* Chip Identification */
#define VPX3224D		0x7231

#define VPX_PALBGHI             0x00	/* video modes */
#define VPX_NTSCM               0x01
#define VPX_SECAM               0x02
#define VPX_NTSC44              0x03
#define VPX_PALM                0x04
#define VPX_PALN                0x05
#define VPX_PAL60               0x06
#define VPX_NTSCCOMB            0x07

#define FP			0x1000	/* Registers greater than 0x1000 are considerd fp-ram registers */
					/* Subtract 0x1000 from register and use fp-ram access routines */
/* The vpx322x registers */

#define R_JEDEC			0x00	/* r Manufacture ID                             Chip Ident.     */
#define R_PARTLOW		0x01	/* r low-byte of 16-bit part number             Chip Ident.     */
#define R_PARTHIGH		0x02	/* r high-byte of 16-bit part number            Chip Ident.     */
#define R_JEDEC2		0x03

#define R_FPSTA			0x35	/* r FP status                                  FP Interface    */
#define R_FPRD			0x36	/* w FP read                                    FP Interface    */
#define	R_FPWR			0x37	/* w FP write                                   FP Interface    */
#define R_FPDAT			0x38	/* w/r FP data                                  FP Interface    */

/* Output */
#define R_oena			0xF2	/* w Output Enable                              Output          */
#define R_driver_a		0xF8	/* w Pad Driver Strength - TTL Out pads type A  Output          */
#define R_driver_b		0xF9	/* w Pad Driver Strength - TTL Out pads type B  Output          */
#define R_llc			0xAA	/* w Low power mode, LLC mode                   Output          */

/* Data Slicer - vpx3225D only */
/* Sync Slicer */
#define R_sync_slicer		0xC8
#define R_sync_stat		0xB4
#define R_sync_count		0xB5

/* Bit Slicer */
#define R_soft_slicer		0xC0
#define R_ttx_freql		0xC1
#define R_ttx_freqh		0xC2
#define R_filter		0xC5
#define R_data_slicer		0xC6
#define R_accu			0xC7
#define R_coeff_rd		0xB6
#define R_level_rd		0xB7

/* Byte Slicer */
#define R_soft_cnt		0xB3
#define R_standard		0xC9
#define R_reference_l		0xBD
#define R_reference_m		0xBC
#define R_reference_h		0xBB
#define R_mask_l		0xBA
#define R_mask_m		0xB9
#define R_mask_h		0xB8
#define R_tolerance		0xCE
#define R_out_mode		0xCF

/* the vpx322x FP-RAM registers */
/* Standard Selection */
#define RFP_sdt		0x20	/* w Standard select                            Stand. Sel.     */
#define RFP_insel	0x21	/* w Input select                               Stand. Sel.     */
#define RFP_sfif	0x22	/* w start point of active video                Stand. Sel.     */
#define RFP_ldly	0x23	/* w luma/chroma delay adjust                   Stand. Sel.     */

/* Color Processing */
#define RFP_accref	0x30	/* w ACC reference level (Cr, Cb lev. pic. bus  Color Proc.     */
#define RFP_accb	0x32	/* w ACC multiplier for SECAM Db chroma comp    Color Proc.     */
#define RFP_accr	0x33	/* w ACC multiplier for SECAM Dr chroma comp    Color Proc.     */
#define RFP_kilvl	0x39	/* w amplitude killer level                     Color Proc.     */
#define RFP_kilhy	0x3A	/* w amplitude killer hysteresis                Color Proc.     */
#define RFP_tint	0xDC	/* w NTSC tint angle, +-512 = +-p/4             Color Proc.     */

/* DVCO */
#define RFP_xlck	0xF7	/* w/r crystal oscillator line-locked mode      DVCO            */
#define RFP_dvco	0xF8	/* w crystal oscillator center frequency adjust DVCO            */
#define RFP_adjust	0xF9	/* r crystal oscillator ctr freq adj val        DVCO            */

/* FP Status Register */
#define RFP_gp_ctrl	0x12	/* r/w general purpose control                  Status          */
#define RFP_asr		0x13	/* r standard recognition status                Status          */
#define RFP_nlpf	0xCB	/* r number of lines per field, P/S:312, N:262  Status          */
#define RFP_vcnt	0x15	/* r vertical field counter                     Status          */
#define RFP_sampl	0x74	/* r measured sync amplitude value              Status          */
#define RFP_bampl	0x31	/* r measured burst amplitude                   Status          */
#define RFP_version	0xF0	/* r software version number                    Status          */

/* Macrovision Detection ( version D4 only ) */
#define RFP_mcv_status	0x170
#define RFP_mcv_start	0x171
#define RFP_mcv_stop	0x172

/* Read Table for Window 1 */
#define RFP_vact_delay1 0x10F	/* r Delay of VACT rltv. to HREF during wndw 1  ReadTab1        */

/* Write Table for Window 1 */
#define RFP_vbegin1	0x120	/* w Vertical Begin                             WinLoadTab1     */
#define RFP_vlinesin1	0x121	/* w Vertical LnsIn / Temp.Decim. / FldSel      WinLoadTab1     */
#define RFP_vlinesout1	0x122	/* w Vertical Lines Out                         WinLoadTab1     */
#define RFP_hbeg1	0x123	/* w Horizontal Begin                           WinLoadTab1     */
#define RFP_hlen1	0x124	/* w Horizontal Length                          WinLoadTab1     */
#define RFP_npix1	0x125	/* w Number of Pixels                           WinLoadTab1     */
#define RFP_peaking1	0x126	/* w Selection for peaking / coring             WinLoadTab1     */
#define RFP_brightness1	0x127	/* w Brightness                                 WinLoadTab1     */
#define RFP_contrast1	0x128	/* w Contrast / Noise shaping / Clamping        WinLoadTab1     */

/* Read Table for Window 2 */
#define RFP_vact_delay2 0x11F	/* r Delay of VACT rltv. to HREF during wndw 2  ReadTab2        */

/* Write Table for Window 2 */
#define RFP_vbegin2	0x12A	/* w Vertical Begin                             WinLoadTab2     */
#define RFP_vlinesin2	0x12B	/* w Vertical Lines In                          WinLoadTab2     */
#define RFP_vlinesout2	0x12C	/* w Vertical Lines Out                         WinLoadTab2     */
#define RFP_hbeg2	0x12D	/* w Horizontal Begin                           WinLoadTab2     */
#define RFP_hlen2	0x12E	/* w Horizontal Length                          WinLoadTab2     */
#define RFP_npix2	0x12F	/* w Number of Pixels                           WinLoadTab2     */
#define RFP_peaking2	0x130	/* w Selection for peaking / coring             WinLoadTab2     */
#define RFP_brightness2	0x131	/* w Brightness                                 WinLoadTab2     */
#define RFP_contrast2	0x132	/* w Contrast                                   WinLoadTab2     */

/* Load Table for VBI Window */
#define RFP_start_even	0x134	/* w Start line even field                      VBI-window      */
#define RFP_end_even	0x135	/* w End line even field                        VBI-window      */
#define RFP_start_odd	0x136	/* w Start line odd field                       VBI-window      */
#define RFP_end_odd	0x137	/* w End line odd field                         VBI-window      */
#define RFP_vbicontrol	0x138	/* w Control VBI-Window                         VBI-window      */
#define RFP_slsize	0x139	/* w Slicer Data Size                           VBI-window      */

/* Control Word */
#define RFP_ControlWord	0x140	/* w/r Register for control and latching                        */

/* Info Word */
#define RFP_InfoWord	0x141	/* r Internal status register, do not overwrite                 */

/* Formatter */
#define RFP_format_sel	0x150	/* w Format Selection / Shuffler / PIXCLK-mode  Formatter       */

/* HVREF */
#define RFP_pval_start	0x151	/* w Strt pos of the prgrammble "video active"  HVREF           */
#define RFP_pval_stop	0x152	/* w End pos of the prgrammble "video active"   HVREF           */
#define RFP_refsig	0x153	/* w Length and polarity of HREF, VREF, FIELD   HVREF           */

/* Output Multiplexer */
#define RFP_outmux	0x154	/* w Output Multiplexer / Multi-purpose output  Output Mux.     */

/* Temporal Decimation */
#define RFP_tdecframes	0x157	/* w Num of frams to output within 3000 frames  Temp. Decim.    */

#if defined (CONFIG_V3TV_VERBOSEDEBUG)

#define STR_fmt			"%s "
#define VPX_LABEL(reg)		(reg < vpx_size)   ? vpx_registers[reg]   : "INVALID",
#define VPXFP_LABEL(reg)	(reg < vpxFP_size) ? vpxFP_registers[reg] : "INVALID",

#define vpxFP_size 	0x173
#define vpx_size	0xFA

const char *vpx_registers[vpx_size] = {
#if __GNUC__ >= 3
	[0 ... vpx_size - 1] = "unknown",
#endif

	[R_JEDEC] = "JEDEC",
	[R_PARTLOW] = "PARTLOW",
	[R_PARTHIGH] = "PARTHIGH",
	[R_JEDEC2] = "JEDEC2",
	[R_FPSTA] = "FPSTA",
	[R_FPRD] = "FPRD",
	[R_FPWR] = "FPWR",
	[R_FPDAT] = "FPDAT",
	[R_llc] = "llc",
	[R_soft_cnt] = "soft_cnt",
	[R_sync_stat] = "sync_stat",
	[R_sync_count] = "sync_count",
	[R_coeff_rd] = "coeff_rd",
	[R_level_rd] = "level_rd",
	[R_soft_slicer] = "soft_slicer",
	[R_ttx_freql] = "ttx_freql",
	[R_ttx_freqh] = "ttx_freqh",
	[R_filter] = "filter",
	[R_data_slicer] = "data_slicer",
	[R_accu] = "accu",
	[R_sync_slicer] = "sync_slicer",
	[R_standard] = "standard",
	[R_reference_l] = "reference_l",
	[R_reference_m] = "reference_m",
	[R_reference_h] = "reference_h",
	[R_mask_l] = "mask_l",
	[R_mask_m] = "mask_m",
	[R_mask_h] = "mask_h",
	[R_tolerance] = "tolerance",
	[R_out_mode] = "out_mode",
	[R_oena] = "oena",
	[R_driver_a] = "driver_a",
	[R_driver_b] = "driver_b"
};

const char *vpxFP_registers[vpxFP_size] = {
#if __GNUC__ >= 3
	[0 ... vpxFP_size - 1] = "unknown",
#endif

	[RFP_gp_ctrl] = "gp_ctrl",
	[RFP_asr] = "asr",
	[RFP_vcnt] = "vcnt",
	[RFP_sdt] = "sdt",
	[RFP_insel] = "insel",
	[RFP_sfif] = "sfif",
	[RFP_ldly] = "ldly",
	[RFP_accref] = "accref",
	[RFP_bampl] = "bampl",
	[RFP_accb] = "accb",
	[RFP_accr] = "accr",
	[RFP_kilvl] = "kilvl",
	[RFP_kilhy] = "kilhy",
	[RFP_sampl] = "sampl",
	[RFP_nlpf] = "nlpf",
	[RFP_tint] = "tint",
	[RFP_version] = "version",
	[RFP_xlck] = "xlck",
	[RFP_dvco] = "dvco",
	[RFP_adjust] = "adjust",
	[RFP_vact_delay1] = "vact_delay1",
	[RFP_vact_delay2] = "vact_delay2",
	[RFP_vbegin1] = "vbegin1",
	[RFP_vlinesin1] = "vlinesin1",
	[RFP_vlinesout1] = "vlinesout1",
	[RFP_hbeg1] = "hbeg1",
	[RFP_hlen1] = "hlen1",
	[RFP_npix1] = "npix1",
	[RFP_peaking1] = "peaking1",
	[RFP_brightness1] = "brightness1",
	[RFP_contrast1] = "contrast1",
	[RFP_vbegin2] = "vbegin2",
	[RFP_vlinesin2] = "vlinesin2",
	[RFP_vlinesout2] = "vlinesout2",
	[RFP_hbeg2] = "hbeg2",
	[RFP_hlen2] = "hlen2",
	[RFP_npix2] = "npix2",
	[RFP_peaking2] = "peaking2",
	[RFP_brightness2] = "brightness2",
	[RFP_contrast2] = "contrast2",
	[RFP_start_even] = "start_even",
	[RFP_end_even] = "end_even",
	[RFP_start_odd] = "start_odd",
	[RFP_end_odd] = "end_odd",
	[RFP_vbicontrol] = "vbicontrol",
	[RFP_slsize] = "slsize",
	[RFP_ControlWord] = "ControlWord",
	[RFP_InfoWord] = "InfoWord",
	[RFP_format_sel] = "format_sel",
	[RFP_pval_start] = "pval_start",
	[RFP_pval_stop] = "pval_stop",
	[RFP_refsig] = "refsig",
	[RFP_outmux] = "outmux",
	[RFP_tdecframes] = "tdecframes",
	[RFP_mcv_status] = "mcv_status",
	[RFP_mcv_start] = "mcv_start",
	[RFP_mcv_stop] = "mcv_stop"
};

#else				/* CONFIG_V3TV_VERBOSEDEBUG not defined */
#define STR_fmt
#define VPX_LABEL(reg)
#define VPXFP_LABEL(reg)
#endif

#define DEB(x) if (debug>=1) x

#if defined (CONFIG_V3TV_VERBOSEDEBUG)
#define DEB2(x) if (debug>=2) x
#define DEB3(x) if (debug>=3) x

#else
#define DEB2(x)
#define DEB3(x)
#endif

/* ---------------------------------------------------------------------- */
/* register intialization values */

static const u16 init_ntsc[] = {
	RFP_tint, 0x00,		/* NTSC tint angle   */
//      RFP_vbegin1, 30,        /* Window 1 vertical - 17 */
	RFP_vbegin1, 23,	/* Window 1 vertical */
	RFP_vlinesin1, 240,	/* Vertical lines in */
	RFP_vlinesout1, 240,	/* Vertical lines out */
	RFP_hbeg1, 0,		/* Horizontal begin  */
	RFP_hlen1, 640,		/* Horizontal length  */
	RFP_npix1, 640,		/* Number of pixels   */
	RFP_sdt, 1,		/* NTSC M,   */
};

static const u16 init_pal[] = {
	RFP_vbegin1, 0,		/* Window 1 vertical begin      */
//      RFP_vbegin1, 23,        /* Window 1 vertical begin      */
	RFP_vlinesin1, 288 + 16,	/* Vertical lines in (16 lines
					 * skipped by the VFE) */
	RFP_vlinesout1, 288 + 16,	/* Vertical lines out (16 lines
					 * skipped by the VFE) */
	RFP_hbeg1, 16,		/* Horizontal begin  */
	RFP_hlen1, 768,		/* Horizontal length */
	RFP_npix1, 784,		/* Number of pixels
				 * Must be >= Horizontal begin + Horizontal length */
	RFP_sdt, 0,		/* PAL B,G,H,I, */
};

static const u16 init_secam[] = {
	RFP_vbegin1, 23 - 16,	/* Window 1 vertical begin */
	RFP_vlinesin1, 288 + 16,	/* Vertical lines in (16 lines
					 * skipped by the VFE) */
	RFP_vlinesout1, 288 + 16,	/* Vertical lines out (16 lines
					 * skipped by the VFE) */
	RFP_hbeg1, 16,		/* Horizontal begin  */
	RFP_hlen1, 768,		/* Horizontal length */
	RFP_npix1, 784,		/* Number of pixels
				 * Must be >= Horizontal begin + Horizontal length */
	RFP_sdt, 2,		/* SECAM,   */
};

static const u16 init_fp[] = {	/* 16 reg-value pairs */
	RFP_format_sel, 1,
	RFP_accref, 0x800,	/* 0x75C, *//* ACC reference */
	RFP_outmux, 0x200,	/* set up output multiplexed 8bit data stream */
	RFP_sfif, 0,		/* picture start position */
	RFP_vbicontrol, 0,	/* disable VBI window */
	RFP_peaking1, 0x100,	/* swap chroma values, Cr Pixels first */
	RFP_vlinesin2, 0xc00,	/* Disable window 2 */
	RFP_brightness1, 0,
	RFP_brightness2, 0,
	RFP_contrast1, 32,
	RFP_contrast2, 32
};

static const unsigned char init_common[] = {
	R_llc, 0,		/* active mode, outputs enabled */
	R_driver_a, 0x24,	/* Port A, PIXCLK, HF# & FE#
				 * stra1 = 100; stra2 = 100 */
	R_driver_b, 0x20,	/* Port B, HREF, VREF, PREF &
				 * strb2 = 100 */
	R_sync_slicer, 0x40,
	R_oena, 0x00,		/* Disable all outputs */
};

/* ---------------------------------------------------------------------- */
/* I2C access functions */

static int vpx3224_write(struct i2c_client *client, u8 reg, u8 value)
{
	int res = 0;

	res = i2c_smbus_write_byte_data(client, reg, value);
	DEB2(printk
	     (KERN_INFO "%s: Write %sreg: " STR_fmt "0x%x; value: 0x%x\n",
	      client->name, (res < 0) ? "failed!! " : "",
	      VPX_LABEL(reg) reg, value));
	return res;
}

static int vpx3224_read(struct i2c_client *client, u8 reg)
{
	int res;

	res = i2c_smbus_read_byte_data(client, reg);
	if (reg != R_FPSTA)
		DEB2(printk
		     (KERN_INFO "%s: Read %sreg: " STR_fmt
		      "0x%x; value: 0x%x\n", client->name,
		      (res < 0) ? "failed!! " : "", VPX_LABEL(reg) reg,
		      res));

	return res;
}

static int
vpx3224_write_block(struct i2c_client *client,
		    const u8 * data, unsigned int len)
{
	u8 reg;
	int res = 0;

	while (len >= 2) {
		reg = *data++;
		res = vpx3224_write(client, reg, *data++);
		if (res < 0)
			break;
		len -= 2;
	}

	return res;
}

/* ---------------------------------------------------------------------- */
/* Fast Processor functions */

static int vpx3224_fp_status(struct i2c_client *client)
{
	int status, i;

	for (i = 0; i < VPX_TIMEOUT_COUNT; i++) {
		status = vpx3224_read(client, R_FPSTA);

		if (status < 4) {
			DEB3(printk(KERN_INFO "%s: FPSTA %s request: %x\n",
				    client->name,
				    (status == 1) ? "write" : (status ==
							       2) ? "read"
				    : "invalid", status));
			return status;
		}
		udelay(10);

		if (need_resched())
			cond_resched();
	}
	printk(KERN_INFO "%s: FPSTA failed: status: %d\n",
	       client->name, status);

	return -1;
}

static int
vpx3224_fp_write(struct i2c_client *client, u16 fpaddr, u16 data)
{
	int res;
	/* Write the 16-bit address to the FPWR register */
	if (i2c_smbus_write_word_data(client, R_FPWR, swab16(fpaddr)) ==
	    -1) {
		printk(KERN_INFO "%s: write to R_FPWR failed!! fpaddr: "
		       STR_fmt "0x%x; data: 0x%x\n",
		       client->name, VPXFP_LABEL(fpaddr) fpaddr,
		       data);
		return -1;
	}

	if (vpx3224_fp_status(client) == -1)
		return -1;

	/* Write the 16-bit data to the FPDAT register */
	res = i2c_smbus_write_word_data(client, R_FPDAT, swab16(data));
	DEB2(printk
	     (KERN_INFO "%s: Write %sfpaddr: " STR_fmt
	      "0x%x; data: 0x%x\n", client->name,
	      (res < 0) ? "write to R_FPDAT failed!! " : "",
	      VPXFP_LABEL(fpaddr) fpaddr, data));

	return res;
}

static int vpx3224_fp_read(struct i2c_client *client, u16 fpaddr)
{
	int data;

	/* Write the 16-bit address to the FPRD register */
	if (i2c_smbus_write_word_data(client, R_FPRD, swab16(fpaddr)) ==
	    -1) {
		printk(KERN_INFO "%s: write to R_FPRD failed!! fpaddr: "
		       STR_fmt "0x%d\n", client->name,
		       VPXFP_LABEL(fpaddr) fpaddr);
		return -1;
	}

	if (vpx3224_fp_status(client) == -1)
		return -1;

	/* Read the 16-bit data from the FPDAT register */
	data = swab16(i2c_smbus_read_word_data(client, R_FPDAT));
	DEB2(printk
	     (KERN_INFO "%s: Read %sfpaddr: " STR_fmt "0x%x data: 0x%x\n",
	      client->name,
	      (data < 0) ? "read R_FPDAT failed!! " : "",
	      VPXFP_LABEL(fpaddr) fpaddr, data));

	return data;
}

static int
vpx3224_write_fp_block(struct i2c_client *client,
		       const u16 * data, int len)
{
	u16 reg;
	int res = 0;

	while (len > 1) {
		reg = *data++;
		res = vpx3224_fp_write(client, reg, *data++);
		if (res < 0)
			return res;

		len -= 2;
	}

	return res;
}

/* ----------------------------------------------------------------------
 * Client procedures */

static int vpx3224_get_status(struct i2c_client *client)
{
	int res = 0, status, sdt;

	status = vpx3224_fp_read(client, RFP_asr);
	if (status < 0)
		return status;

	sdt = vpx3224_fp_read(client, RFP_sdt);
	if (sdt < 0)
		return sdt;

	DEB(printk(KERN_INFO "%s: status: 0x%x; standard: 0x%x\n",
		   client->name, status, sdt));

	/* bit 2 = 1 == no signal detected */
	/* bit 3 = 1 == color amplitude killer active */
	/* bit 5 = 1 == color ident killer active */
	if ((status & 0x04) == 0) {
		res |= DECODER_STATUS_GOOD;

		DEB(printk(KERN_INFO "%s: Video signal detected.\n",
			   client->name));

		if ((status & 0x08) == 0 || (status & 0x20) == 0)
			res |= DECODER_STATUS_COLOR;
		else
			DEB(printk(KERN_INFO "%s: Color killer active.\n",
				   client->name));

		switch (sdt & 7) {

		case VPX_PALBGHI:
		case VPX_PALM:
		case VPX_PALN:
		case VPX_PAL60:
			res |= DECODER_STATUS_PAL;
			break;

		case VPX_SECAM:
			res |= DECODER_STATUS_SECAM;
			break;

		case VPX_NTSCM:
		case VPX_NTSC44:
		case VPX_NTSCCOMB:
			res |= DECODER_STATUS_NTSC;
			break;
		}
	}
	return res;
}

#if 0
static int vpx3224_auto_norm(struct i2c_client *client)
{
/*	* Auto is not supported by the vpx3224/5d.
	* Scan the valid modes, read RFP_asr for score, pick the highest score.
	* With an NTSC M signal, there is no clear winner... */
	DEB(printk
	    (KERN_INFO "%s: Auto video mode detection not supported.\n",
	     client->name));

	int i, res = 0;
	char *sdt_desc[8] = { "PAL B,G,H,I", "NTSC M", "SECAM", "NTSC44",
		"PAL M", "PAL N", "PAL 60", "NTSC COMB"
	};

	for (i = 0; i < 8; i++) {
		vpx3224_fp_write(client, RFP_sdt, i);
		res = vpx3224_fp_read(client, RFP_asr) & 0x3ff;
		DEB(printk(KERN_INFO "\tRFP_ASR: %s 0x%03x, %s; %s; %s\n",
			   sdt_desc[i], res,
			   (res & 1) ? "vetical lock" : "no v-lock",
			   (res & 2) ? "horizontal lock" : "no h-lock",
			   (res & 4) ? "NO signal detected" :
			   "Signal Detected"));
		DEB(printk
		    (KERN_INFO "\t\t%s; %s\n",
		     (res & 8) ? "color amplitude killer active" :
		     "color signal",
		     (res & 16) ? "amplitude killer disabled" : ""));
		DEB(printk
		    (KERN_INFO "\t\t%s; %s\n",
		     (res & 32) ? "color ident killer active" :
		     "color ident signal",
		     (res & 64) ? "disable ident killer" : ""));
		DEB(printk
		    (KERN_INFO "\t\t%s; %s; %s\n",
		     (res & 128) ? "interlace detected" : "no interlace",
		     (res & 256) ? "no vertical sync" : "v-sync",
		     (res & 512) ? "spurious v-sync" : ""));
	}
	return -EINVAL;
}
#endif

static int vpx3224_set_norm(struct i2c_client *client, int norm)
{
	int res = 0;
	struct vpx3224 *decoder = i2c_get_clientdata(client);
	char *norms[4] = {"PAL", "NTSC", "SECAM", "AUTO"};

	switch (norm) {
	case VIDEO_MODE_NTSC:
		res = vpx3224_write_fp_block(client, init_ntsc,
					     sizeof (init_ntsc) >> 1);
		break;

	case VIDEO_MODE_PAL:
		res = vpx3224_write_fp_block(client, init_pal,
					     sizeof (init_pal) >> 1);
		break;

	case VIDEO_MODE_SECAM:
		res = vpx3224_write_fp_block(client, init_secam,
					     sizeof (init_secam) >> 1);
		break;
/*		case VIDEO_MODE_AUTO:
			norm = vpx3224_auto_norm(client);
			break;
*/
	default:
		return -EINVAL;
	}
	if (!res) {
		decoder->norm = norm;
		vpx3224_fp_write(client, RFP_accref, decoder->sat >> 4);

		DEB(printk(KERN_INFO "%s: norm switched to %s\n",
			   client->name, norms[norm]));
	}
	else
		DEB(printk(KERN_INFO "%s: Unable to set video norm.",
			   client->name));
	return res;
}

static int vpx3224_set_input(struct i2c_client *client, int chan)
{
	int sdt;
	struct vpx3224 *decoder = i2c_get_clientdata(client);

	static char *inputs[] = { "Composite", "Tuner", "S-Video" };

	static const unsigned int input[3][2] = {
		/* insel, sdt   RFP_insel: bit 10 = vcr */
		{0x400, 0},	/* VIN3: composite,       sdt.bit[6]=0 */
		{0x001, 0},	/* VIN2: tuner,           sdt.bit[6]=0 */
		{0x406, 1},	/* VIN1: SVHS: Chroma in, sdt.bit[6]=1 */
	};

	if (chan < 0 || chan > 2)
		return -EINVAL;

	decoder->input = chan;

	vpx3224_fp_write(client, RFP_insel, input[decoder->input][0]);

	sdt = vpx3224_fp_read(client, RFP_sdt);
	if (input[decoder->input][1])
		sdt |= 0x40;	/* S-VHS mode */

	vpx3224_fp_write(client, RFP_sdt, sdt & 0x3CF);
	DEB(printk(KERN_INFO "%s: input switched to %s\n",
		   client->name, inputs[decoder->input]));

	return 0;
}

static int vpx3224_set_picture(struct i2c_client *client, void *arg)
{
	struct vpx3224 *decoder = i2c_get_clientdata(client);

	int latch = 0;
	struct video_picture *pic = arg;

	DEB(printk(KERN_INFO "%s:vpx reg-> brightness: 0x%03x "
		   "hue: 0x%03x contrast: 0x%03x colour: 0x%03x\n",
		   client->name,
		   ((s16) pic->brightness - (s16) 0x8000) >> 8,
		   ((s16) pic->hue - (s16) 0x8000) >> 4,
		   pic->contrast >> 10, pic->colour >> 4));

	/* bright, contrast, sat, hue = 0,... ,65535 */
	if (decoder->bright != pic->brightness) {
		latch = 1;
		/* We want -127 to 128 (binary offset: 0 to 255) we get 0-65535 */
		decoder->bright = pic->brightness;
		vpx3224_fp_write(client, RFP_brightness1,
				 ((s16) decoder->bright -
				  (s16) 0x8000) >> 8);
		vpx3224_fp_write(client, RFP_brightness2,
				 ((s16) decoder->bright -
				  (s16) 0x8000) >> 8);
	}

	if (decoder->contrast != pic->contrast) {
		latch = 1;
		/* We want 0 to 64 we get 0-65535 */
		/* Bit 7 and 8 is for noise shaping (decoder->contrast >> 10) + 192) */
		decoder->contrast = pic->contrast;
		vpx3224_fp_write(client, RFP_contrast1,
				 decoder->contrast >> 10);
		vpx3224_fp_write(client, RFP_contrast2,
				 decoder->contrast >> 10);
	}

	if (decoder->sat != pic->colour) {
		latch = 1;
		/* We want 0 to 4096 we get 0-65535 */
		decoder->sat = pic->colour;
		vpx3224_fp_write(client, RFP_accref, decoder->sat >> 4);
	}

	if ((decoder->hue != pic->hue)
	    && (decoder->norm == VIDEO_MODE_NTSC)) {
		latch = 1;
		/* We want -512 to 512 we get 0-65535 */
		decoder->hue = pic->hue;
		vpx3224_fp_write(client, RFP_tint,
				 ((s16) decoder->hue - (s16) 0x8000) >> 4);
	}

	if (latch) {		/* latch WinTable1+2 */
		vpx3224_fp_write(client, RFP_ControlWord,
				 32 + 64 +
				 (vpx3224_fp_read(client, RFP_ControlWord)
				  & 7));
	}

	return 0;
}

#if 0
static int vpx3224_set_window(struct i2c_client *client, void *arg)
{
	struct video_capture *window = arg;

	vpx3224_fp_write(client, RFP_vbegin1, window->y);
	vpx3224_fp_write(client, RFP_vlinesin1, window->height);
	vpx3224_fp_write(client, RFP_vlinesout1, window->height);
	vpx3224_fp_write(client, RFP_hbeg1, window->x);
	vpx3224_fp_write(client, RFP_hlen1, window->width);
	vpx3224_fp_write(client, RFP_npix1, window->width + window->x);

	vpx3224_fp_write(client, RFP_ControlWord,
			 32 + 64 +
			 (vpx3224_fp_read(client, RFP_ControlWord) & 7));

	return 0;
}
#endif

static int vpx3224_init_client(struct i2c_client *client)
{
	int res;
	res =
	    vpx3224_write_block(client, init_common, sizeof (init_common));
	if (res < 0)
		return res;
	res =
	    vpx3224_write_fp_block(client, init_fp, sizeof (init_fp) >> 1);
	if (res < 0)
		return res;

	res = vpx3224_set_input(client, 0);
	if (res < 0)
		return res;

/*	* We cycle through the supported modes
	* and read RFP_asr to see which is valid.
	* restore this line to see output when module loads */
/*	vpx3224_auto_norm(client); */
	res = vpx3224_set_norm(client, VIDEO_MODE_NTSC);
	if (res < 0)
		return res;

	return 0;
}

static int vpx3224_check_partnum(struct i2c_client *client, int kind)
{
	u8 id;
	u16 pn;

	if (kind < 0) {
		/* Check for manufacture ID and part number */
		id = vpx3224_read(client, R_JEDEC);
		if (id != MICRONAS_INTERMETALL) {
			printk(KERN_INFO
			       "%s: Wrong manufacturer ID (0x%02x)\n",
			       THIS_MODULE->name, id);
			return -1;
		}

		pn = (vpx3224_read(client, R_PARTHIGH) << 8) +
		    vpx3224_read(client, R_PARTLOW);
		switch (pn) {
		case VPX3220A:
			printk(KERN_INFO
			       "%s: vpx3220a not supported (0x%x)  Try the vpx3220 driver.\n",
			       THIS_MODULE->name, pn);
			return -1;
			break;
		case VPX3216B:
			printk(KERN_INFO
			       "%s: vpx3216b not supported (0x%x)  Try the vpx3220 driver.\n",
			       THIS_MODULE->name, pn);
			return -1;
			break;
		case VPX3214C:
			printk(KERN_INFO
			       "%s: vpx3214c not supported (0x%x)  Try the vpx3220 driver.\n",
			       THIS_MODULE->name, pn);
			return -1;
			break;
		case VPX3225D:
			snprintf(client->name,
				 sizeof (client->name) - 1, "vpx3225d");
			break;
		case VPX3224D:
			snprintf(client->name,
				 sizeof (client->name) - 1, "vpx3224d");
			break;
		default:
			printk(KERN_INFO
			       "%s: Wrong part number (0x%04x)\n",
			       client->name, pn);
			return -1;
		}
	} else {
		snprintf(client->name, sizeof (client->name) - 1,
			 "forced vpx32xx");
	}
	return 0;
}

static int
vpx3224_command(struct i2c_client *client, unsigned int cmd, void *arg)
{
	struct vpx3224 *decoder = i2c_get_clientdata(client);
	int res = 0;

	switch (cmd) {
	case 0:{
			return vpx3224_init_client(client);
			break;
		}

	case DECODER_GET_CAPABILITIES:{
			struct video_decoder_capability *cap = arg;

			DEB(printk
			    (KERN_INFO "%s: DECODER_GET_CAPABILITIES\n",
			     client->name));

			cap->flags = VIDEO_DECODER_PAL
			    | VIDEO_DECODER_NTSC | VIDEO_DECODER_SECAM;
/*				   | VIDEO_DECODER_AUTO ; // see vpx3224_auto_norm */
			cap->inputs = VPX3224_MAX_INPUT;
			cap->outputs = VPX3224_MAX_OUTPUT;
			break;
		}

	case DECODER_GET_STATUS:{
			DEB(printk(KERN_INFO "%s: DECODER_GET_STATUS\n",
				   client->name));
			res = vpx3224_get_status(client);
			*(int *) arg = res;
			break;
		}

	case DECODER_SET_NORM:{
			DEB(printk(KERN_INFO "%s: DECODER_SET_NORM %d\n",
				   client->name, *(int *) arg));
			res = vpx3224_set_norm(client, *(int *) arg);
			break;
		}

	case DECODER_SET_INPUT:{
			DEB(printk(KERN_INFO "%s: DECODER_SET_INPUT\n",
				   client->name));
			res = vpx3224_set_input(client, *(int *) arg);
			break;
		}

	case DECODER_SET_OUTPUT:{
			int *iarg = arg;

			DEB(printk(KERN_INFO "%s: DECODER_SET_OUTPUT\n",
				   client->name));
			/* not much choice of outputs */
			if (iarg != 0)
				return -EINVAL;
			break;
		}

	case DECODER_ENABLE_OUTPUT:{
			if (decoder->enable != *(int *) arg) {
				decoder->enable = *(int *) arg;
				DEB(printk
				    (KERN_INFO
				     "%s: DECODER_ENABLE_OUTPUT: %s\n",
				     client->name,
				     (decoder->enable) ?
					"Enable" : "Disable"));
				res = vpx3224_write(client, R_oena,
					(decoder->enable) ? 0x0F : 0);
			}
			break;
		}

	case DECODER_SET_PICTURE:{
			DEB(printk(KERN_INFO "%s: DECODER_SET_PICTURE\n",
				   client->name));
			res = vpx3224_set_picture(client, arg);
			break;
		}
#if 0
	case DECODER_SET_WINDOW:{
			DEB(printk( KERN_INFO "%s: DECODER_SET_WINDOW\n",
				client->name));
			res = vpx3224_set_window(client, arg);
			break;
		}
#endif
	default:
		return -EINVAL;
	}
	return res;
}

/* -----------------------------------------------------------------------
 * Client managment code */

/*
 * Generic i2c probe data
 */
static unsigned short normal_i2c[] = { I2C_VPX3224, I2C_CLIENT_END };
// static unsigned short normal_i2c_range[] = { I2C_CLIENT_END };
static unsigned short probe[2] = { I2C_CLIENT_END, I2C_CLIENT_END };
// static unsigned short probe_range[2] = { I2C_CLIENT_END, I2C_CLIENT_END };
static unsigned short ignore[2] = { I2C_CLIENT_END, I2C_CLIENT_END };
// static unsigned short ignore_range[2] = { I2C_CLIENT_END, I2C_CLIENT_END };
// static unsigned short force[2] = { I2C_CLIENT_END, I2C_CLIENT_END };

static struct i2c_client_address_data addr_data = {
	.normal_i2c = normal_i2c,
//	.normal_i2c_range = normal_i2c_range,
	.probe = probe,
//	.probe_range = probe_range,
	.ignore = ignore,
//	.ignore_range = ignore_range,
};

/* Maintain a count of registered clients */
static int vpx3224_i2c_id = 0;

static int vpx3224_attach_adapter(struct i2c_adapter *adapter)
{
	int res = 0;

	if (adapter->class & I2C_CLASS_TV_ANALOG)
		res = i2c_probe(adapter, &addr_data, vpx3224_detect_client);
	DEB2(printk(KERN_INFO "%s: i2c_probe of adapter: %s returned %d\n",
		    THIS_MODULE->name, adapter->name, res));

	return res;		/* == vpx3224_i2c_id */
}

/* call i2c_detach_client to remove client from adapter->clients[] */
static int vpx3224_detach_client(struct i2c_client *client)
{
	struct vpx3224 *decoder = i2c_get_clientdata(client);

	int res;

	res = i2c_detach_client(client);
	DEB2(printk(KERN_INFO "%s: i2c_detach_client returned %d\n",
		    THIS_MODULE->name, res));
	if (res < 0) {
		printk(KERN_ERR "%s: %s: Unable to detach client.\n",
		       THIS_MODULE->name, client->name);
		return res;
	}
	module_put(client->adapter->owner);
	kfree(decoder);
	kfree(client);
	vpx3224_i2c_id--;

	return 0;
}

/* -----------------------------------------------------------------------
 * Driver initialization and cleanup code
 *
 * -----------------------------------------------------------------------
 * I2C driver definition */

static struct i2c_driver vpx3224_i2c_driver = {
	.owner = THIS_MODULE,
	.name = "vpx3224.o",

	.id = I2C_DRIVERID_VPX3224,
	.flags = I2C_DF_NOTIFY,

	.attach_adapter = vpx3224_attach_adapter,
	.detach_client = vpx3224_detach_client,
	.command = vpx3224_command
};

/* vpx3224_detect_client - aka: i2c_client_found_addr_proc *found_proc
 * so called by i2c_probe, res passed back to vpx3224_attach_adapter */
static int vpx3224_detect_client(struct i2c_adapter *adapter, int address,
				 int kind)
{
	int res;
	struct i2c_client *client;
	struct vpx3224 *decoder;

	DEB2(printk
	     (KERN_INFO "%s: i2c_probe found adapter: %s; address: %d\n",
	      THIS_MODULE->name, adapter->name, address));
	/* Check if the adapter supports the needed features */
	res = i2c_check_functionality(adapter,
				      I2C_FUNC_SMBUS_BYTE_DATA |
				      I2C_FUNC_SMBUS_WORD_DATA);
	DEB2(printk
	     (KERN_INFO "%s: i2c_check_functionality returned %d\n",
	      THIS_MODULE->name, res));
	if (res <= 0)
		return -ENOSYS;	/* function not implemented */

	client = kmalloc(sizeof (struct i2c_client), GFP_KERNEL);
	DEB2(printk(KERN_INFO "%s: kmalloc(client)  returned %p\n",
		    THIS_MODULE->name, client));
	if (client == NULL) {
		return -ENOMEM;
	}

	memset(client, 0, sizeof (struct i2c_client));

	vpx3224_i2c_id++;	/* ++ to let vpx3224_init know the client connected */
	client->addr = address;
	client->adapter = adapter;
	client->driver = &vpx3224_i2c_driver;
	client->flags = I2C_CLIENT_ALLOW_USE;

	if (vpx3224_check_partnum(client, kind) < 0) {
		kfree(client);
		vpx3224_i2c_id--;
		return -ENODEV;
	}

	decoder = kmalloc(sizeof (struct vpx3224), GFP_KERNEL);
	DEB2(printk(KERN_INFO "%s: kmalloc(decoder) returned %p\n",
		    THIS_MODULE->name, decoder));
	if (decoder == NULL) {
		kfree(client);
		vpx3224_i2c_id--;	/* -- to let vpx3224_init know the client did not connect */
		return -ENOMEM;
	}

	memset(decoder, 0, sizeof (struct vpx3224));
	i2c_set_clientdata(client, decoder);

	decoder->norm = VIDEO_MODE_NTSC;
	decoder->input = 0;
	decoder->enable = 0;
	decoder->bright = 32768;
	decoder->contrast = 32768;
	decoder->hue = 32768;
	decoder->sat = 32768;

/*      i2c_attach_client adds client to client->adapter->clients[]
	returns:
		-EBUSY
		-ENOMEM         enlarge I2C_CLIENT_MAX */
	res = i2c_attach_client(client);
	DEB2(printk(KERN_INFO "%s: i2c_attach_client returned %d\n",
		    client->name, res));
	if (res < 0) {
		vpx3224_detach_client(client);
		return res;
	}

	res = vpx3224_init_client(client);
	if (res < 0) {
		printk(KERN_INFO "%s: Initialization error!\n",
		       client->name);
		vpx3224_detach_client(client);
		return res;
	}

	printk(KERN_INFO "%s: %s client found at address 0x%x\n",
	       THIS_MODULE->name, client->name, client->addr);

	try_module_get(client->adapter->owner);
	return vpx3224_i2c_id;
}

static void vpx3224_exit(void)
{
	int res;

	printk(KERN_INFO "%s: version " VPX_VERSION
	       " (" __DATE__ "): Exiting.\n", THIS_MODULE->name);

	res = i2c_del_driver(&vpx3224_i2c_driver);

	DEB2(printk(KERN_INFO "%s: i2c_del_driver returnd 0x%02x.\n",
		    THIS_MODULE->name, res));
}

static int vpx3224_init(void)
{
	int err, res = vpx3224_i2c_id;

	printk(KERN_INFO "%s: version " VPX_VERSION
	       " (" __DATE__ ")\n", THIS_MODULE->name);

	/* calls vpx3224_i2c_driver.attach_adapter
	 * ignores attach_adapter return value */
	err = i2c_add_driver(&vpx3224_i2c_driver);
	DEB2(printk(KERN_INFO "%s: i2c_add_driver returned %d\n",
		    THIS_MODULE->name, err));
	if (err < 0) {
		vpx3224_exit();
		return err;
	}

	if (vpx3224_i2c_id == res) {
		printk(KERN_INFO "%s: No device detected.\n",
		       THIS_MODULE->name);
		vpx3224_exit();
		return -ENODEV;
	}

	return 0;
}

module_init(vpx3224_init);
module_exit(vpx3224_exit);

MODULE_PARM(debug, "i");
MODULE_PARM_DESC(debug, "Debug level (0-3");

MODULE_DESCRIPTION("vpx3224d/vpx3225d video encoder driver");
MODULE_AUTHOR("Perry E. Gilfilan <perrye@linuxmail.org");
MODULE_LICENSE("GPL");
