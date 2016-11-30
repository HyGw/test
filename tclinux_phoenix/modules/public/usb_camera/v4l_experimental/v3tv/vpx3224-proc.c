/*
 * vpx3224d & vpx3225d video decoder register inspector
 *
 * Copyright (C) 2004 Perry Gilfillan <perrye@linuxmail.org>
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

#include <linux/i2c-proc.h>
#include <linux/proc_fs.h>

static char proc_buf[1000];
static int reg_set = 0;
static int leftovers = 0;
struct proc_dir_entry *proc_entry;

/* -- SENSORS SYSCTL START -- */
#define vpx3224_SYSCTL_PARTNUM		3200
#define vpx3224_SYSCTL_OUTPUT		3201
#define vpx3224_SYSCTL_SYNC_SLICER	3202
#define vpx3224_SYSCTL_BIT_SLICER	3203
#define vpx3224_SYSCTL_BYTE_SLICER	3204
#define vpx3224_SYSCTL_SDT_SELECT	3205
#define vpx3224_SYSCTL_COLOR_PROC	3206

/* -- SENSORS SYSCTL END -- */

static void
vpx3224_partnum(struct i2c_client *client, int operation, int ctl_name,
		int *nrels_mag, long *results)
{
	if (operation == SENSORS_PROC_REAL_INFO)
		*nrels_mag = 0;
	else if (operation == SENSORS_PROC_REAL_READ) {
		results[0] = vpx3224_read(client, R_JEDEC);
		results[1] = (vpx3224_read(client, R_PARTHIGH) << 8) +
		    vpx3224_read(client, R_PARTLOW);
		*nrels_mag = 2;
	} else if (operation == SENSORS_PROC_REAL_WRITE) {
		DEB(printk(KERN_INFO "%s: Warning: write was requested on "
			   "read-only proc file: PartNumber\n",
			   client->name));
	}
}

static void
vpx3224_output(struct i2c_client *client, int operation, int ctl_name,
	       int *nrels_mag, long *results)
{
	if (operation == SENSORS_PROC_REAL_INFO)
		*nrels_mag = 0;

	else if (operation == SENSORS_PROC_REAL_READ) {
		results[0] = (u8) vpx3224_read(client, R_driver_a);
		results[1] = (u8) vpx3224_read(client, R_driver_b);
		results[2] = (u8) vpx3224_read(client, R_oena);
		results[3] = (u8) vpx3224_read(client, R_llc);
		*nrels_mag = 4;
	} else if (operation == SENSORS_PROC_REAL_WRITE) {
		if (*nrels_mag >= 1)
			vpx3224_write(client, R_driver_a, (u8) results[0]);
		if (*nrels_mag >= 2)
			vpx3224_write(client, R_driver_b, (u8) results[1]);
		if (*nrels_mag >= 3)
			vpx3224_write(client, R_oena, (u8) results[2]);
		if (*nrels_mag >= 4)
			vpx3224_write(client, R_llc, (u8) results[3]);
	}
}

static void
vpx3224_sync_slicer(struct i2c_client *client, int operation, int ctl_name,
		    int *nrels_mag, long *results)
{
	if (operation == SENSORS_PROC_REAL_INFO)
		*nrels_mag = 0;

	else if (operation == SENSORS_PROC_REAL_READ) {
		results[0] = (u8) vpx3224_read(client, R_sync_slicer);
		results[1] = (u8) vpx3224_read(client, R_sync_stat) & 0xC0;
		results[2] = (u8) vpx3224_read(client, R_sync_count);
		*nrels_mag = 3;
	} else if (operation == SENSORS_PROC_REAL_WRITE) {
		if (*nrels_mag >= 1)
			vpx3224_write(client, R_sync_slicer,
				      (u8) results[0]);
		if (*nrels_mag >= 2)
			DEB(printk
			    (KERN_INFO
			     "%s: Warning: write was requested on "
			     "read-only register: " STR_fmt "0x%x: \n",
			     client->name,
			     VPX_LABEL(R_sync_stat) R_sync_stat));
		if (*nrels_mag >= 3)
			DEB(printk
			    (KERN_INFO
			     "%s: Warning: write was requested on "
			     "read-only register: " STR_fmt "0x%x: \n",
			     client->name,
			     VPX_LABEL(R_sync_count) R_sync_count));
	}
}

static void
vpx3224_bit_slicer(struct i2c_client *client, int operation, int ctl_name,
		   int *nrels_mag, long *results)
{
	if (operation == SENSORS_PROC_REAL_INFO)
		*nrels_mag = 0;

	else if (operation == SENSORS_PROC_REAL_READ) {
		results[0] =
		    (u8) vpx3224_read(client, R_soft_slicer) & 0x7F;
		results[1] = (u8) vpx3224_read(client, R_ttx_freql);
		results[2] = (u8) vpx3224_read(client, R_ttx_freqh) & 0x0F;
		results[3] = (u8) vpx3224_read(client, R_filter) & 0x3F;
		results[4] = (u8) vpx3224_read(client, R_data_slicer);
		results[5] = (u8) vpx3224_read(client, R_accu) & 0x3F;
		results[6] = (u8) vpx3224_read(client, R_coeff_rd);
		results[7] = (u8) vpx3224_read(client, R_level_rd);
		*nrels_mag = 8;
	} else if (operation == SENSORS_PROC_REAL_WRITE) {
		if (*nrels_mag >= 1)
			vpx3224_write(client, R_soft_slicer,
				      (u8) results[0] & 0x7F);
		if (*nrels_mag >= 2)
			vpx3224_write(client, R_ttx_freql,
				      (u8) results[1]);
		if (*nrels_mag >= 3)
			vpx3224_write(client, R_ttx_freqh,
				      (u8) results[2] & 0x0F);
		if (*nrels_mag >= 4)
			vpx3224_write(client, R_filter,
				      (u8) results[3] & 0x3F);
		if (*nrels_mag >= 5)
			vpx3224_write(client, R_data_slicer,
				      (u8) results[4]);
		if (*nrels_mag >= 6)
			vpx3224_write(client, R_accu,
				      (u8) results[5] & 0x3F);
		if (*nrels_mag >= 7)
			DEB(printk
			    (KERN_INFO
			     "%s: Warning: write was requested on "
			     "read-only register: " STR_fmt "0x%x: \n",
			     client->name,
			     VPX_LABEL(R_coeff_rd) R_coeff_rd));
		if (*nrels_mag >= 8)
			DEB(printk
			    (KERN_INFO
			     "%s: Warning: write was requested on "
			     "read-only register: " STR_fmt "0x%x: \n",
			     client->name,
			     VPX_LABEL(R_level_rd) R_level_rd));
	}
}

static void
vpx3224_byte_slicer(struct i2c_client *client, int operation, int ctl_name,
		    int *nrels_mag, long *results)
{
	if (operation == SENSORS_PROC_REAL_INFO)
		*nrels_mag = 0;

	else if (operation == SENSORS_PROC_REAL_READ) {
		results[0] = vpx3224_read(client, R_soft_cnt);
		results[1] = vpx3224_read(client, R_standard);
		results[2] = (vpx3224_read(client, R_reference_l) +
			      (vpx3224_read(client, R_reference_m) << 8) +
			      (vpx3224_read(client, R_reference_h) << 16))
		    & 0x00ffffff;
		results[3] =
		    (vpx3224_read(client, R_mask_l) +
		     (vpx3224_read(client, R_mask_m) << 8) +
		     (vpx3224_read(client, R_mask_h) << 16)) & 0x00ffffff;
		results[4] = vpx3224_read(client, R_tolerance) & 0x3f;
		results[5] = vpx3224_read(client, R_out_mode);
		*nrels_mag = 6;
	} else if (operation == SENSORS_PROC_REAL_WRITE) {
		if (*nrels_mag >= 2)
			vpx3224_write(client, R_standard, results[1]);
		if (*nrels_mag >= 3) {
			vpx3224_write(client, R_reference_l,
				      results[2] & 0xff);
			vpx3224_write(client, R_reference_m,
				      (results[2] >> 8) & 0xff);
			vpx3224_write(client, R_reference_h,
				      (results[2] >> 16) & 0xff);
		}
		if (*nrels_mag >= 4) {
			vpx3224_write(client, R_mask_l, results[3] & 0xff);
			vpx3224_write(client, R_mask_m,
				      (results[3] >> 8) & 0xff);
			vpx3224_write(client, R_mask_h,
				      (results[3] >> 16) & 0xff);
		}
		if (*nrels_mag >= 5)
			vpx3224_write(client, R_tolerance,
				      results[4] & 0x3f);
		if (*nrels_mag >= 6)
			vpx3224_write(client, R_out_mode, results[5]);
	}

}

static void
vpx3224_sdt_select(struct i2c_client *client, int operation, int ctl_name,
		   int *nrels_mag, long *results)
{
	if (operation == SENSORS_PROC_REAL_INFO)
		*nrels_mag = 0;

	else if (operation == SENSORS_PROC_REAL_READ) {
		results[0] = vpx3224_fp_read(client, RFP_sdt);
		results[1] = vpx3224_fp_read(client, RFP_insel);
		results[2] = vpx3224_fp_read(client, RFP_sfif);
		results[3] = vpx3224_fp_read(client, RFP_ldly);
		*nrels_mag = 4;
	} else if (operation == SENSORS_PROC_REAL_WRITE) {
		if (*nrels_mag >= 1)
			vpx3224_fp_write(client, RFP_sdt, results[0]);
		if (*nrels_mag >= 2)
			vpx3224_fp_write(client, RFP_insel, results[1]);
		if (*nrels_mag >= 3)
			vpx3224_fp_write(client, RFP_sfif, results[2]);
		if (*nrels_mag >= 4)
			vpx3224_fp_write(client, RFP_ldly, results[3]);
	}
}

static void
vpx3224_color_proc(struct i2c_client *client, int operation, int ctl_name,
		   int *nrels_mag, long *results)
{
	if (operation == SENSORS_PROC_REAL_INFO)
		*nrels_mag = 0;

	else if (operation == SENSORS_PROC_REAL_READ) {
		results[0] = vpx3224_fp_read(client, RFP_accref);
		results[1] = vpx3224_fp_read(client, RFP_accb);
		results[2] = vpx3224_fp_read(client, RFP_accr);
		results[3] = vpx3224_fp_read(client, RFP_kilvl);
		results[4] = vpx3224_fp_read(client, RFP_kilhy);
		results[5] = vpx3224_fp_read(client, RFP_tint);
		*nrels_mag = 6;
	} else if (operation == SENSORS_PROC_REAL_WRITE) {
		if (*nrels_mag >= 1)
			vpx3224_fp_write(client, RFP_accref, results[0]);
		if (*nrels_mag >= 2)
			vpx3224_fp_write(client, RFP_accb, results[1]);
		if (*nrels_mag >= 3)
			vpx3224_fp_write(client, RFP_accr, results[2]);
		if (*nrels_mag >= 4)
			vpx3224_fp_write(client, RFP_kilvl, results[3]);
		if (*nrels_mag >= 5)
			vpx3224_fp_write(client, RFP_kilhy, results[4]);
		if (*nrels_mag >= 6)
			vpx3224_fp_write(client, RFP_tint, results[5]);
	}
}

static int vpx3224_dump_register(struct i2c_client *client, int arg)
{
	u16 retval = 0;

	switch (arg) {
	case 0:		/* Chip Identidfication */
		{
			unsigned char jedec;
			u16 pn;

			jedec = vpx3224_read(client, R_JEDEC);
			pn = (vpx3224_read(client, R_PARTHIGH) << 8) +
			    vpx3224_read(client, R_PARTLOW);

			retval = sprintf(proc_buf,
					 "Chip Identification: %s\n"
					 "    Manufacturer ID: 0x%x\n"
					 "        Part Number: 0x%x\n",
					 client->name, jedec,
					 pn);
		}
		break;
	case 1:		/* Output */
		{
			unsigned char driver_a, driver_b, oena, llc;
			unsigned char stra1, stra2, stra3;
			unsigned char strb1, strb2;
			unsigned char aen, ben, clkena, zen, llc2en,
			    oeqdel, latoeq, oeq_dis;
			unsigned char lowpow, iresen, llc2, llc2_pol,
			    slowpow, oldllc;

			driver_a = vpx3224_read(client, R_driver_a);
			stra1 = driver_a & 7;
			stra2 = (driver_a & 0x38) >> 3;
			stra3 = (driver_a & 0xC0) >> 6;
			driver_b = vpx3224_read(client, R_driver_b);
			strb1 = driver_b & 7;
			strb2 = (driver_b & 0x38) >> 3;
			oena = vpx3224_read(client, R_oena);
			aen = oena & 1;
			ben = oena & 2;
			clkena = oena & 4;
			zen = oena & 8;
			llc2en = oena & 16;
			oeqdel = oena & 32;
			latoeq = oena & 64;
			oeq_dis = oena & 128;
			llc = vpx3224_read(client, R_llc);
			lowpow = llc & 3;
			iresen = llc & 4;
			llc2 = llc & 8;
			llc2_pol = llc & 16;
			slowpow = llc & 32;
			oldllc = llc & 64;

			retval = sprintf(proc_buf,
					 "Output: DRIVER_A: 0x%x\n"
					 "\tbit [2:0] Driver strength of Port A[7:0]:           stra1 0x%x\n"
					 "\tbit [5:3] Driver strength of PIXCLK, LLC, and VACT: stra2 0x%x\n"
					 "\tbit [7:6] Additional PIXCLK driver strength:        stra3 0x%x\n"
					 "Output: DRIVER_B: 0x%x\n"
					 "\tbit [2:0] Driver strength of Port B[7:0]:                 strb1 0x%x\n"
					 "\tbit [5:3] Driver strength of HREF, VREF, FIELD, and LLC2: stra2 0x%x\n"
					 "Output Enable: OENA: 0x%x\n"
					 "\tbit [0] aen:\t%s\n"
					 "\tbit [1] ben:\t%s\n"
					 "\tbit [2] clkena\t%s\n"
					 "\tbit [3] zen:\t%s\n"
					 "\tbit [4] llc2en:\t%s\n"
					 "\tbit [5] oeqdel:\t%s\n"
					 "\tbit [6] latoeq:\t%s\n"
					 "\tbit [7] oeq_dis:\t%s\n"
					 "Low power mode, LLC mode: LLC: 0x%x\n"
					 "\tbit [1:0] lowpower:\t%s\n"
					 "\tbit [2]   iresen:\t%s\n"
					 "\tbit [3]   llc2:\t%s\n"
					 "\tbit [4]   llc2_pol: %x:\t%s\n"
					 "\tbit [5]   slowpow:\t%s\n"
					 "\tbit [6]   oldllc:\t%s\n",
					 driver_a, stra1, stra2, stra3,
					 driver_b, strb1, strb2,
					 oena,
					 aen ? "Enable Video Port A" : "Disable Video Port A",
					 ben ? "Enable Video Port B" : "Disable Video Port B",
					 clkena ? "Enable Pixclk Output" : "Disable Pixclk Output",
					 zen ? "Enable HREF, VREF, FIELD, VACT, LLC, LLC2"
						 : "Disbale HREF, VREF, FIELD, VACT, LLC, LLC2",
					 llc2en ? "Enable LLC2 to TDO pin" : "Disable LLC2",
					 oeqdel ? "no delay of OEQ input signal"
						 : "1 LLC cycle delay of ",
					 latoeq ? "latch OEQ input signal with rising edge of LLC"
						 : "don't latch OEQ input signal",
					 oeq_dis ? "disable OEQ pin function"
						 : "enable OEQ pin function",
					 llc,
					 (lowpow == 0) ? "active mode, outputs enabled"
						 : (lowpow == 1) ? "outputs tri-stated, clock divided by 2, i2c full speed"
						 : (lowpow == 2) ? "outputs tri-stated, clock divided by 4, i2c full speed"
						 : (lowpow == 3) ? "outputs tri-stated, clock divided by 8, i2c < 100 kbit/s"
						 : "",
					 iresen ? "i2c reset" : "",
					 llc2 ? "connect LLC2 to TDO pin" : "connect bit[4] to TDO pin",
					 llc2_pol >> 4,
					 !llc2 ? "LLC2 polaroity determined by bit [4]" : "",
					 slowpow ? "reset all slicer registers" : "",
					 oldllc ? "use old llc timing with long hold time"
						 : "use new llc timing with shorter hold time");
		}
		break;
	case 2:
		{		/* Sync Slicer - Hsync Counter */
			unsigned char sync_slicer, sync_stat, sync_cnt;
			unsigned char sync_level, vsw;
			unsigned char vwin, field;

			sync_slicer = vpx3224_read(client, R_sync_slicer);
			sync_level = sync_slicer & 0x7F;
			vsw = sync_slicer & 0x80;
			sync_stat =
			    vpx3224_read(client, R_sync_stat) & 0xC0;
			vwin = sync_stat & 0x40;
			field = sync_stat & 0x80;
			sync_cnt = vpx3224_read(client, R_sync_count);

			retval = sprintf(proc_buf,
					 "Sync Slicer: sync_slicer: 0x%x\n"
					 "\tbit [6:0] binary sync level is compared with binary data: 0x%x\n"
					 "\tbit [7]   vertical sync window: %s\n"
					 "Sync Status: sync_stat: 0x%x\t\tREADONLY\n"
					 "\tbit [6]   vwin: %s\n"
					 "\tbit [7]   field: %s\n"
					 "Hsync Counter: sync_cnt:\t\tREADONLY\n"
					 "\tbit [7:0] number of detected horizontal sync pulses per frame /4: 0x%x\n",
					 sync_slicer,
					 sync_level,
					 vsw ? "disable" : "enable",
					 sync_stat,
					 vwin ? "vert. retrace\tset at line 628/528 (PAL/NTSC)"
						 : "vert. window\treset at line 624/524 (PAL/NTSC)",
					 field ? "field 1\tset at line 624/524 (PAL/NTSC)"
						 : "field 2\treset at line 313/263 (PAL/NTSC)",
					 sync_cnt);
		}
		break;
	case 3:
		{		/* Bit Slicer */
			u8 soft_slicer, ttx_freql, ttx_freqh, filter,
			    data_slicer;
			u8 accu, coeff_rd, level_rd;
			u8 soft_level;
			u16 ttx_freq;
			u8 ttx_phinc;
			u8 coeff;
			u8 reset, dcen, acen, soften, acaden, fltaden;

			soft_slicer = vpx3224_read(client, R_soft_slicer);
			soft_level = soft_slicer & 0x7f;
			ttx_freql = vpx3224_read(client, R_ttx_freql);
			ttx_freqh =
			    vpx3224_read(client, R_ttx_freqh) & 0x0F;
			ttx_freq = (ttx_freql + (ttx_freqh << 8)) & 0x07ff;
			ttx_phinc = ttx_freqh & 0x08;	/* bit 11 */
			filter = vpx3224_read(client, R_filter) & 0x3F;
			coeff = filter;
			data_slicer = vpx3224_read(client, R_data_slicer);
			accu = vpx3224_read(client, R_accu) & 0x3f;
			reset = accu & 1;
			dcen = accu & 2;
			acen = accu & 4;
			soften = accu & 8;
			acaden = accu & 16;
			fltaden = accu & 32;
			coeff_rd = vpx3224_read(client, R_coeff_rd);
			level_rd = vpx3224_read(client, R_level_rd);

			retval = sprintf(proc_buf,
					 "Bit Slicer:\n"
					 "Soft Slicer: soft_slicer: 0x%x\n"
					 "\tbit [6:0]  binary soft slicer level is compared with ABS[data]: 0x%x\n"
					 "Freq: ttx bitslicer frequency: 0x%x %x\n"
					 "\tbit [10:0] Freq: ttx_freq: 0x%x\n"
					 "\tbit [11]   phase inc = %s\n"
					 "Filter Coefficient\n"
					 "\tbit [5:0]  high pass filter coefficient in 2's complement: 0x%x\n"
					 "\t\t   100000 = not allowed; 100001 = -31; 000000 = 0; 011111 = 31\n"
					 "Data Slicer:\n"
					 "\tbit [0:7]  data_slicer: 0x%x\n"
					 "Accumulator Mode: accu: 0x%x\n"
					 "\tbit[0]     %s\n"
					 "\tbit[1]     DC accu %s\n"
					 "\tbit[2]     AC and FLT accu %s\n"
					 "\tbit[3]     soft error correction %s\n"
					 "\tbit[4]     ac adaption %s\n"
					 "\tbit[5]     flt adaption %s\n"
					 "Read Filter Coefficient: coeff_rd: 0x%x\n"
					 "Read Data Slicer Level: level_rd: 0x%x\n",
					 soft_slicer, soft_level,
					 ttx_freqh, ttx_freql, ttx_freq,
					 ttx_phinc ? "Freq*(1+1/8) before framing code\n"
						     "\t\t    Freq*(1+1/16) after framing code"
						 : "Freq",
					 coeff, data_slicer, accu,
					 reset ? "reset DC and AC and FLT accu (one shot)"
						 : "no action",
					 dcen ? "disable" : "enable",
					 acen ? "disable\n\t\t\t(only for VPS and CAPTION and WSS line)"
						 : "enable",
					 soften ? "enable" : "disable",
					 acaden ? "enable" : "disable",
					 fltaden ? "enable" : "disable",
					 coeff_rd, level_rd);
		}
		break;
	case 4:
		{		/* Byte Slicer */
			u8 soft_cnt, standard;
			u32 reference, mask;
			u8 tolerance, out_mode;

			soft_cnt = vpx3224_read(client, R_soft_cnt);
			standard = vpx3224_read(client, R_standard);
			reference = (vpx3224_read(client, R_reference_l) +
				     (vpx3224_read(client, R_reference_m)
				      << 8) + (vpx3224_read(client,
							    R_reference_h)
					       << 16)) & 0x00ffffff;
			mask =
			    (vpx3224_read(client, R_mask_l) +
			     (vpx3224_read(client, R_mask_m) << 8) +
			     (vpx3224_read(client, R_mask_h) << 16)) &
			    0x00ffffff;
			tolerance =
			    vpx3224_read(client, R_tolerance) & 0x3f;
			out_mode = vpx3224_read(client, R_out_mode);

			retval = sprintf(proc_buf,
					 "Byte Slicer:\n"
					 "Soft Error Counter: soft_cnt: 0x%x\n"
					 "Standard: 0x%x\n"
					 "\tbit[0] TTX: %s\n"
					 "\tbit[1] %s mode\n"
					 "\tbit[2] full field %s\n"
					 "\tbit[3] VPS line 16 %s\n"
					 "\tbit[4] WSS line 23 %s\n"
					 "\tbit[5] CAPTION line 21 field 1 %s\n"
					 "\tbit[6] CAPTION line 21 field 2 %s\n"
					 "\tbit[7] horizontal quit mode %s\n"
					 "Clock run-in and framing code reference: 0x%06x\n"
					 "Clock run-in and framing code don't care mask: 0x%06x\n"
					 "Tolerance: 0x%x\n"
					 "\tbit[1:0] maximum number of bit errors in low mask: %d\n"
					 "\tbit[3:2] maximum number of bit errors in mid mask: %d\n"
					 "\tbit[5:4] maximum number of bit errors in high mask: %d\n"
					 "Output Mode: out_mode: 0x%x\n"
					 "\tbit[5:0]  number of data bytes per text "
					 "line including framing code: 0x%x\n"
					 "\tbit[6]    64 byte mode %s\n"
					 "\tbit[7]    data output %s\n",
					 soft_cnt, standard,
					 (standard & 1) ? "enable" : "disable",
					 (standard & 2) ? "PAL" : "NTSC",
					 (standard & 4) ? "enable" : "disable",
					 (standard & 8) ? "enable" : "disable",
					 (standard & 16) ? "enable" : "disable",
					 (standard & 32) ? "enable" : "disable",
					 (standard & 64) ? "enable" : "disable",
					 (standard & 128) ? "enable" : "disable",
					 reference, mask, tolerance,
					 tolerance & 0x03,
					 (tolerance >> 2) & 0x03,
					 (tolerance >> 4) & 0x03, out_mode,
					 out_mode & 0x3f,
					 (out_mode & 64) ? "enable" : "disable",
					 (out_mode & 128) ? "only for text lines"
						 : "for every video line");
		}
		break;
	case 5:
		{		/* Standard Selection */
			u16 sdt, insel, sfif, ldly;
			char standard;

			sdt = vpx3224_fp_read(client, RFP_sdt);
			standard = (sdt & 0x03);

			insel = vpx3224_fp_read(client, RFP_insel);
			sfif = vpx3224_fp_read(client, RFP_sfif);
			ldly = vpx3224_fp_read(client, RFP_ldly);

			retval = sprintf(proc_buf,
					 "Standard Selection:\n"
					 "Standard select: 0x%03x\n"
					 "\tbit[2:0] sdt: 0x%01x: %s\n"
					 "\tbit[3]   sdtmod: %s\n"
					 "\tbit[6]   svhs: SVHS mode %s\n"
					 "\tbit[7]   Options: %s%s%s\n"
					 "Input Selection: insel: 0x%03x\n"
					 "\tbit[1:0]  luma selector: %s\n"
					 "\tbit[2]    chroma selector: %s\n"
					 "\tbit[4:3]  IF compensation: %s\n"
					 "\tbit[6:5]  chroma bandwidth selector: %s\n"
					 "\tbit[7]    SECAM notch filter: %s\n"
					 "\tbit[8]    luma lowpass filter: %s\n"
					 "\tbit[10:9] hpll speed: %s\n"
					 "Picture Start Position: sfif: 0x%03x\n"
					 "Luma/Chroma Delay Adjust: ldly: bit[11:6] 0x%02x\n",
					 sdt, standard,
					 (standard == VPX_PALBGHI) ? "PAL B,G,H,I"
						 : (standard == VPX_NTSCM) ? "NTSC M"
						 : (standard == VPX_SECAM) ? "SECAM"
						 : (standard == VPX_NTSC44) ? "NTSC44"
						 : (standard == VPX_PALM) ? "PAL M"
						 : (standard == VPX_PALN) ? "PAL N"
						 : (standard == VPX_PAL60) ? "PAL 60"
						 : (standard == VPX_NTSCCOMB) ? "NTSC COMB"
						 : "",
					 !(standard & 0x04) ? "MOD standard modifier"
						 : (standard == VPX_NTSC44) ? "NTSC modified to compensated NTSC"
						 : (standard == VPX_NTSCM) ? "NTSC modified to compensated NTSC"
						 : (standard == VPX_NTSCCOMB) ? "NTSCC modified to monochrome 525"
						 : (standard == VPX_SECAM) ? "SECAM modified to monochrome 625"
						 : (standard == VPX_PALBGHI) ? "PAL modified to simple PAL"
						 : (standard == VPX_PALM) ? "PAL modified to simple PAL"
						 : (standard == VPX_PALN) ? "PAL modified to simple PAL"
						 : (standard == VPX_PAL60) ? "PAL modified to simple PAL"
						 : "",
					 (standard & 0x40) ? "on" : "off",
					 (standard & 0x80) ? "no hpll setup; " : "",
					 (standard & 0x100) ? "no vertical setup; " : "",
					 (standard & 0x200) ? "no acc setup" : "", insel,
					 ((insel & 0x03) == 0x00) ? "VIN3"
						 : ((insel & 0x03) == 0x01) ? "VIN2"
						 : ((insel & 0x03) == 0x02) ? "VIN1"
						 : "reserved",
					 (insel & 0x04) ? "CIN" : "VIN1",
					 ((insel & 0x18) == 0x08) ? "6 db/0kt"
						 : ((insel & 0x18) == 0x10) ? "12 db/0kt"
						 : ((insel & 0x18) == 0x18) ? "10 db/Mhz only for SECAM"
						 : "off",
					 ((insel & 0x60) == 0x00) ? "narrow"
						 : ((insel & 0x60) == 0x20) ? "normal"
						 : ((insel & 0x60) == 0x40) ? "broad"
						 : ((insel & 0x60) == 0x60) ? "wide"
						 : "",
					 (insel & 0x80) ? "fixed" : "adaptive",
					 (insel & 0x100) ? "enable" : "disable",
					 ((insel & 0x600) == 0x00) ? "no change"
						 : ((insel & 0x600) == 0x200) ? "terrestrial"
						 : ((insel & 0x600) == 0x400) ? "vcr"
						 : ((insel & 0x600) == 0x600) ? "mixed"
						 : "",
					 sfif,
					 (ldly >> 6) & 0x3f);
		}
		break;
	case 6:
		{		/* Color Processing */
			u16 accref, accb, accr, kilvl, kilhy, tint;

			accref = vpx3224_fp_read(client, RFP_accref);
			accb = vpx3224_fp_read(client, RFP_accb);
			accr = vpx3224_fp_read(client, RFP_accr);
			kilvl = vpx3224_fp_read(client, RFP_kilvl);
			kilhy = vpx3224_fp_read(client, RFP_kilhy);
			tint = vpx3224_fp_read(client, RFP_tint);

			retval = sprintf(proc_buf,
					 "Color Processing:\n"
					 "\tACC reference level: 0x%03x\n"
					 "\tACC multiplier for SECAM Db chroma component: 0x%03x\n"
					 "\tACC multiplier for SECAM Dr chroma component: 0x%03x\n"
					 "\tAmplitude Killer Level: 0x%03x\n"
					 "\tAmplitude Killer Hysteresis: 0x%03x\n"
					 "\tNTSC tint angle: 0x%03x\n",
					 accref, accb, accr, kilvl, kilhy,
					 tint);
		}
		break;
	case 7:
		{		/* DVCO - Digitally Controled Clock Oscillator */
			u16 xlck, dvco, adjust;

			xlck = vpx3224_fp_read(client, RFP_xlck);
			dvco = vpx3224_fp_read(client, RFP_dvco);
			adjust = vpx3224_fp_read(client, RFP_adjust);

			retval = sprintf(proc_buf,
					 "DVCO: Digitally Controled Clock Oscillator\n"
					 "\tcrystal oscillator line-locked mode: 0x%03x\n"
					 "\tcrystal oscillator center frequency adjust: 0x%03x\n"
					 "\tCrystal Oscillator Center Frequency Adjust Value: 0x%03x\n",
					 xlck, dvco, adjust);
		}
		break;
	}
	reg_set++;

	return (retval);
}

int
vpx3224_read_regs(char *buffer, char **location,
		  off_t offset, int length, int *eof, void *data)
{
#define VPX3224_REG_STOP 7

	int n;

	if (offset == 0) {
		leftovers = 0;
		reg_set = 0;
	}

	if (reg_set >= VPX3224_REG_STOP) {
		*eof = 1;
		return 0;
	}

	if (leftovers) {
		n = leftovers;
		leftovers = 0;
	} else {
		n = vpx3224_dump_register(data, reg_set);
	}

	if (n > length) {
		memcpy(*location = buffer, proc_buf, length);
		memmove(proc_buf, proc_buf + length, n - length);
		leftovers = n - length;
		return length;
	} else {
		memcpy(*location = buffer, proc_buf, n);
		return n;
	}
}

static ctl_table vpx3224_dir_table_template[] = {
	{.ctl_name = vpx3224_SYSCTL_PARTNUM,
	 .procname = "PartNumber",
	 .data = NULL,
	 .maxlen = 0,
	 .mode = 0444,
	 .child = NULL,
	 .proc_handler = &i2c_proc_real,
	 .strategy = &i2c_sysctl_real,
	 .de = NULL,
	 .extra1 = &vpx3224_partnum,
	 .extra2 = NULL},
	{.ctl_name = vpx3224_SYSCTL_OUTPUT,
	 .procname = "Output",
	 .data = NULL,
	 .maxlen = 0,
	 .mode = 0666,
	 .child = NULL,
	 .proc_handler = &i2c_proc_real,
	 .strategy = &i2c_sysctl_real,
	 .de = NULL,
	 .extra1 = &vpx3224_output,
	 .extra2 = NULL},
	{.ctl_name = vpx3224_SYSCTL_SYNC_SLICER,
	 .procname = "SyncSlicer",
	 .data = NULL,
	 .maxlen = 0,
	 .mode = 0666,
	 .child = NULL,
	 .proc_handler = &i2c_proc_real,
	 .strategy = &i2c_sysctl_real,
	 .de = NULL,
	 .extra1 = &vpx3224_sync_slicer,
	 .extra2 = NULL},
	{.ctl_name = vpx3224_SYSCTL_BIT_SLICER,
	 .procname = "BitSlicer",
	 .data = NULL,
	 .maxlen = 0,
	 .mode = 0666,
	 .child = NULL,
	 .proc_handler = &i2c_proc_real,
	 .strategy = &i2c_sysctl_real,
	 .de = NULL,
	 .extra1 = &vpx3224_bit_slicer,
	 .extra2 = NULL},
	{.ctl_name = vpx3224_SYSCTL_BYTE_SLICER,
	 .procname = "ByteSlicer",
	 .data = NULL,
	 .maxlen = 0,
	 .mode = 0666,
	 .child = NULL,
	 .proc_handler = &i2c_proc_real,
	 .strategy = &i2c_sysctl_real,
	 .de = NULL,
	 .extra1 = &vpx3224_byte_slicer,
	 .extra2 = NULL},
	{.ctl_name = vpx3224_SYSCTL_SDT_SELECT,
	 .procname = "StandardSelection",
	 .data = NULL,
	 .maxlen = 0,
	 .mode = 0666,
	 .child = NULL,
	 .proc_handler = &i2c_proc_real,
	 .strategy = &i2c_sysctl_real,
	 .de = NULL,
	 .extra1 = &vpx3224_sdt_select,
	 .extra2 = NULL},
	{.ctl_name = vpx3224_SYSCTL_COLOR_PROC,
	 .procname = "ColorProcessing",
	 .data = NULL,
	 .maxlen = 0,
	 .mode = 0666,
	 .child = NULL,
	 .proc_handler = &i2c_proc_real,
	 .strategy = &i2c_sysctl_real,
	 .de = NULL,
	 .extra1 = &vpx3224_color_proc,
	 .extra2 = NULL},
	{0}
};

void vpx3224_init_proc(struct i2c_client *client)
{
	int i;
	struct vpx3224 *decoder = i2c_get_clientdata(client);

	/* Register a new directory entry with module sensors */
	i = i2c_register_entry
	    (client, client->name, vpx3224_dir_table_template);
	if (i < 0)
		printk(KERN_INFO "%s: could not register "
		       "vpx3224_dir_table_template: err: %d\n",
		       client->name, i);
	else
		decoder->sysctl_id = i;

	proc_entry = create_proc_read_entry
	    (client->name, 0, &proc_root, &vpx3224_read_regs,
	     client);
	if (!proc_entry)
		printk(KERN_INFO "%s: could not create "
		       "/proc/%s\n", client->name,
		       client->name);
}

void vpx3224_del_proc(struct i2c_client *client)
{
	i2c_deregister_entry(((struct vpx3224 *) (client->data))->
			     sysctl_id);

	if (proc_entry)
		remove_proc_entry(client->name, &proc_root);
}
