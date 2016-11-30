/*
 * ngene-eeprom.c: nGene PCIe bridge driver - eeprom support
 *
 * Copyright (C) 2005-2007 Micronas
 *
 * Copyright (C) 2008-2009 Ralph Metzler <rjkm@metzlerbros.de>
 *                         Modifications for new nGene firmware,
 *                         support for EEPROM-copying,
 *                         support for new dual DVB-S2 card prototype
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 only, as published by the Free Software Foundation.
 *
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 * Or, point your browser to http://www.gnu.org/copyleft/gpl.html
 */

#if 0
static int copy_eeprom;
module_param(copy_eeprom, int, 0444);
MODULE_PARM_DESC(copy_eeprom, "Copy eeprom.");

#define MICNG_EE_START      0x0100
#define MICNG_EE_END        0x0FF0

#define MICNG_EETAG_END0    0x0000
#define MICNG_EETAG_END1    0xFFFF

/* 0x0001 - 0x000F reserved for housekeeping */
/* 0xFFFF - 0xFFFE reserved for housekeeping */

/* Micronas assigned tags
   EEProm tags for hardware support */

#define MICNG_EETAG_DRXD1_OSCDEVIATION  0x1000  /* 2 Bytes data */
#define MICNG_EETAG_DRXD2_OSCDEVIATION  0x1001  /* 2 Bytes data */

#define MICNG_EETAG_MT2060_1_1STIF      0x1100  /* 2 Bytes data */
#define MICNG_EETAG_MT2060_2_1STIF      0x1101  /* 2 Bytes data */

/* Tag range for OEMs */

#define MICNG_EETAG_OEM_FIRST  0xC000
#define MICNG_EETAG_OEM_LAST   0xFFEF

static int i2c_write_eeprom(struct i2c_adapter *adapter,
			    u8 adr, u16 reg, u8 data)
{
	u8 m[3] = {(reg >> 8), (reg & 0xff), data};
	struct i2c_msg msg = {.addr = adr, .flags = 0, .buf = m,
			      .len = sizeof(m)};

	if (i2c_transfer(adapter, &msg, 1) != 1) {
		dprintk(KERN_ERR DEVICE_NAME ": Error writing EEPROM!\n");
		return -EIO;
	}
	return 0;
}

static int i2c_read_eeprom(struct i2c_adapter *adapter,
			   u8 adr, u16 reg, u8 *data, int len)
{
	u8 msg[2] = {(reg >> 8), (reg & 0xff)};
	struct i2c_msg msgs[2] = {{.addr = adr, .flags = 0,
				   .buf = msg, .len = 2 },
				  {.addr = adr, .flags = I2C_M_RD,
				   .buf = data, .len = len} };

	if (i2c_transfer(adapter, msgs, 2) != 2) {
		dprintk(KERN_ERR DEVICE_NAME ": Error reading EEPROM\n");
		return -EIO;
	}
	return 0;
}

static int ReadEEProm(struct i2c_adapter *adapter,
		      u16 Tag, u32 MaxLen, u8 *data, u32 *pLength)
{
	int status = 0;
	u16 Addr = MICNG_EE_START, Length, tag = 0;
	u8  EETag[3];

	while (Addr + sizeof(u16) + 1 < MICNG_EE_END) {
		if (i2c_read_eeprom(adapter, 0x50, Addr, EETag, sizeof(EETag)))
			return -1;
		tag = (EETag[0] << 8) | EETag[1];
		if (tag == MICNG_EETAG_END0 || tag == MICNG_EETAG_END1)
			return -1;
		if (tag == Tag)
			break;
		Addr += sizeof(u16) + 1 + EETag[2];
	}
	if (Addr + sizeof(u16) + 1 + EETag[2] > MICNG_EE_END) {
		printk(KERN_ERR DEVICE_NAME
		       ": Reached EOEE @ Tag = %04x Length = %3d\n",
		       tag, EETag[2]);
		return -1;
	}
	Length = EETag[2];
	if (Length > MaxLen)
		Length = (u16) MaxLen;
	if (Length > 0) {
		Addr += sizeof(u16) + 1;
		status = i2c_read_eeprom(adapter, 0x50, Addr, data, Length);
		if (!status) {
			*pLength = EETag[2];
			if (Length < EETag[2])
				; /*status=STATUS_BUFFER_OVERFLOW; */
		}
	}
	return status;
}

static int WriteEEProm(struct i2c_adapter *adapter,
		       u16 Tag, u32 Length, u8 *data)
{
	int status = 0;
	u16 Addr = MICNG_EE_START;
	u8 EETag[3];
	u16 tag = 0;
	int retry, i;

	while (Addr + sizeof(u16) + 1 < MICNG_EE_END) {
		if (i2c_read_eeprom(adapter, 0x50, Addr, EETag, sizeof(EETag)))
			return -1;
		tag = (EETag[0] << 8) | EETag[1];
		if (tag == MICNG_EETAG_END0 || tag == MICNG_EETAG_END1)
			return -1;
		if (tag == Tag)
			break;
		Addr += sizeof(u16) + 1 + EETag[2];
	}
	if (Addr + sizeof(u16) + 1 + EETag[2] > MICNG_EE_END) {
		printk(KERN_ERR DEVICE_NAME
		       ": Reached EOEE @ Tag = %04x Length = %3d\n",
		       tag, EETag[2]);
		return -1;
	}

	if (Length > EETag[2])
		return -EINVAL;
	/* Note: We write the data one byte at a time to avoid
	   issues with page sizes. (which are different for
	   each manufacture and eeprom size)
	 */
	Addr += sizeof(u16) + 1;
	for (i = 0; i < Length; i++, Addr++) {
		status = i2c_write_eeprom(adapter, 0x50, Addr, data[i]);

		if (status)
			break;

		/* Poll for finishing write cycle */
		retry = 10;
		while (retry) {
			u8 Tmp;

			msleep(50);
			status = i2c_read_eeprom(adapter, 0x50, Addr, &Tmp, 1);
			if (status)
				break;
			if (Tmp != data[i])
				printk(KERN_ERR DEVICE_NAME
				       "eeprom write error\n");
			retry -= 1;
		}
		if (status) {
			printk(KERN_ERR DEVICE_NAME
			       ": Timeout polling eeprom\n");
			break;
		}
	}
	return status;
}

static void i2c_init_eeprom(struct i2c_adapter *adapter)
{
	u8 tags[] = {0x10, 0x00, 0x02, 0x00, 0x00,
		     0x10, 0x01, 0x02, 0x00, 0x00,
		     0x00, 0x00, 0x00};

	int i;

	for (i = 0; i < sizeof(tags); i++)
		i2c_write_eeprom(adapter, 0x50, 0x0100 + i, tags[i]);
}

int eeprom_read_ushort(struct i2c_adapter *adapter, u16 tag, u16 *data)
{
	int stat;
	u8 buf[2];
	u32 len = 0;

	stat = ReadEEProm(adapter, tag, 2, buf, &len);
	if (stat)
		return stat;
	if (len != 2)
		return -EINVAL;

	*data = (buf[0] << 8) | buf[1];
	return 0;
}

static int eeprom_write_ushort(struct i2c_adapter *adapter, u16 tag, u16 data)
{
	int stat;
	u8 buf[2];

	buf[0] = data >> 8;
	buf[1] = data & 0xff;
	stat = WriteEEProm(adapter, tag, 2, buf);
	if (stat)
		return stat;
	return 0;
}

int i2c_dump_eeprom(struct i2c_adapter *adapter, u8 adr)
{
	u8 buf[64];
	int i;

	if (i2c_read_eeprom(adapter, adr, 0x0000, buf, sizeof(buf))) {
		printk(KERN_ERR DEVICE_NAME ": No EEPROM?\n");
		return -1;
	}
	for (i = 0; i < sizeof(buf); i++) {
		if (!(i & 15))
			printk(KERN_DEBUG "\n");
		printk(KERN_DEBUG "%02x ", buf[i]);
	}
	printk("\n");

	return 0;
}

int i2c_copy_eeprom(struct i2c_adapter *adapter, u8 adr, u8 adr2)
{
	u8 buf[64];
	int i;

	if (i2c_read_eeprom(adapter, adr, 0x0000, buf, sizeof(buf))) {
		printk(KERN_ERR DEVICE_NAME ": No EEPROM?\n");
		return -1;
	}
	buf[36] = 0xc3;
	buf[39] = 0xab;
	for (i = 0; i < sizeof(buf); i++) {
		i2c_write_eeprom(adapter, adr2, i, buf[i]);
		msleep(10);
	}
	return 0;
}

int i2c_check_eeprom(struct i2c_adapter *adapter)
{
	u8 buf[13];

	i2c_dump_eeprom(adapter);

	if (i2c_read_eeprom(adapter, 0x50, 0x0100, buf, sizeof(buf))) {
		printk(KERN_ERR DEVICE_NAME ": No EEPROM?\n");
		return -1;
	}
	if (buf[0] != 0x10 || buf[1] != 0x00) {
		printk(KERN_INFO DEVICE_NAME
		       ": Initializing EEPROM TAG area\n");
		i2c_init_eeprom(adapter);
	}
	return 0;
}

#endif
