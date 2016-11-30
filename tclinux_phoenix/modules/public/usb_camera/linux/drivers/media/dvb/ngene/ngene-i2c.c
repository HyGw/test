/*
 * ngene-i2c.c: nGene PCIe bridge driver i2c functions
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

/* FIXME - some of these can probably be removed */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/io.h>
#include <asm/div64.h>
#include <linux/pci.h>
#include <linux/pci_ids.h>
#include <linux/smp_lock.h>
#include <linux/timer.h>
#include <linux/version.h>
#include <linux/byteorder/generic.h>
#include <linux/firmware.h>
#include <linux/vmalloc.h>
#include "compat.h"

#include "ngene.h"

/* Firmware command for i2c operations */
static int ngene_command_i2c_read(struct ngene *dev, u8 adr,
			   u8 *out, u8 outlen, u8 *in, u8 inlen, int flag)
{
	struct ngene_command com;

	com.cmd.hdr.Opcode = CMD_I2C_READ;
	com.cmd.hdr.Length = outlen + 3;
	com.cmd.I2CRead.Device = adr << 1;
	memcpy(com.cmd.I2CRead.Data, out, outlen);
	com.cmd.I2CRead.Data[outlen] = inlen;
	com.cmd.I2CRead.Data[outlen + 1] = 0;
	com.in_len = outlen + 3;
	com.out_len = inlen + 1;

	if (ngene_command(dev, &com) < 0)
		return -EIO;

	if ((com.cmd.raw8[0] >> 1) != adr)
		return -EIO;

	if (flag)
		memcpy(in, com.cmd.raw8, inlen + 1);
	else
		memcpy(in, com.cmd.raw8 + 1, inlen);
	return 0;
}

static int ngene_command_i2c_write(struct ngene *dev, u8 adr,
				   u8 *out, u8 outlen)
{
	struct ngene_command com;

#if 0
	/* Probing by writing 0 bytes does not work */
	if (!outlen)
		outlen++;
#endif

	com.cmd.hdr.Opcode = CMD_I2C_WRITE;
	com.cmd.hdr.Length = outlen + 1;
	com.cmd.I2CRead.Device = adr << 1;
	memcpy(com.cmd.I2CRead.Data, out, outlen);
	com.in_len = outlen + 1;
	com.out_len = 1;

	if (ngene_command(dev, &com) < 0)
		return -EIO;

	if (com.cmd.raw8[0] == 1)
		return -EIO;

	return 0;
}

static void ngene_i2c_set_bus(struct ngene *dev, int bus)
{
	if (!(dev->card_info->i2c_access & 2))
		return;
	if (dev->i2c_current_bus == bus)
		return;

	switch (bus) {
	case 0:
		ngene_command_gpio_set(dev, 3, 0);
		ngene_command_gpio_set(dev, 2, 1);
		break;

	case 1:
		ngene_command_gpio_set(dev, 2, 0);
		ngene_command_gpio_set(dev, 3, 1);
		break;
	}
	dev->i2c_current_bus = bus;
}

static int ngene_i2c_master_xfer(struct i2c_adapter *adapter,
				 struct i2c_msg msg[], int num)
{
	struct ngene_channel *chan =
		(struct ngene_channel *)i2c_get_adapdata(adapter);
	struct ngene *dev = chan->dev;

	down(&dev->i2c_switch_mutex);
	ngene_i2c_set_bus(dev, chan->number);

	if (num == 2 && msg[1].flags & I2C_M_RD && !(msg[0].flags & I2C_M_RD))
		if (!ngene_command_i2c_read(dev, msg[0].addr,
					    msg[0].buf, msg[0].len,
					    msg[1].buf, msg[1].len, 0))
			goto done;

	if (num == 1 && !(msg[0].flags & I2C_M_RD))
		if (!ngene_command_i2c_write(dev, msg[0].addr,
					     msg[0].buf, msg[0].len))
			goto done;
	if (num == 1 && (msg[0].flags & I2C_M_RD))
		if (!ngene_command_i2c_read(dev, msg[0].addr, NULL, 0,
					    msg[0].buf, msg[0].len, 0))
			goto done;

	up(&dev->i2c_switch_mutex);
	return -EIO;

done:
	up(&dev->i2c_switch_mutex);
	return num;
}

#if 0
static int ngene_i2c_algo_control(struct i2c_adapter *adap,
				  unsigned int cmd, unsigned long arg)
{
	struct ngene_channel *chan =
		(struct ngene_channel *)i2c_get_adapdata(adap);

	switch (cmd) {
	case IOCTL_MIC_TUN_RDY:
		chan->tun_rdy = 1;
		if (chan->dec_rdy == 1)
			chan->tun_dec_rdy = 1;
		break;

	case IOCTL_MIC_DEC_RDY:
		chan->dec_rdy = 1;
		if (chan->tun_rdy == 1)
			chan->tun_dec_rdy = 1;
		break;

	case IOCTL_MIC_TUN_DETECT:
		{
			int *palorbtsc = (int *)arg;
			*palorbtsc = chan->dev->card_info->ntsc;
			break;
		}

	default:
		break;
	}
	return 0;
}
#endif

static u32 ngene_i2c_functionality(struct i2c_adapter *adap)
{
	return I2C_FUNC_SMBUS_EMUL;
}

static struct i2c_algorithm ngene_i2c_algo = {
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 14)
	.name = "nGene",
#endif
	.master_xfer = ngene_i2c_master_xfer,
#if 0
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 24)
	.algo_control = ngene_i2c_algo_control,
#endif
#endif
	.functionality = ngene_i2c_functionality,
};

int ngene_i2c_init(struct ngene *dev, int dev_nr)
{
	struct i2c_adapter *adap = &(dev->channel[dev_nr].i2c_adapter);

	i2c_set_adapdata(adap, &(dev->channel[dev_nr]));
#ifdef I2C_CLASS_TV_DIGITAL
	adap->class = I2C_CLASS_TV_DIGITAL | I2C_CLASS_TV_ANALOG;
#else
	adap->class = I2C_CLASS_TV_ANALOG;
#endif

	strcpy(adap->name, "nGene");

	adap->algo = &ngene_i2c_algo;
	adap->algo_data = (void *)&(dev->channel[dev_nr]);
	adap->dev.parent = &dev->pci_dev->dev;

	return i2c_add_adapter(adap);
}

#if 0
int i2c_write(struct i2c_adapter *adapter, u8 adr, u8 data)
{
	u8 m[1] = {data};
	struct i2c_msg msg = {.addr = adr, .flags = 0, .buf = m, .len = 1};

	if (i2c_transfer(adapter, &msg, 1) != 1) {
		printk(KERN_ERR DEVICE_NAME
		       ": Failed to write to I2C adr %02x!\n", adr);
		return -1;
	}
	return 0;
}

static int i2c_write_register(struct i2c_adapter *adapter,
			      u8 adr, u8 reg, u8 data)
{
	u8 m[2] = {reg, data};
	struct i2c_msg msg = {.addr = adr, .flags = 0, .buf = m, .len = 2};

	if (i2c_transfer(adapter, &msg, 1) != 1) {
		printk(KERN_ERR DEVICE_NAME
		       ": Failed to write to I2C register %02x@%02x!\n",
		       reg, adr);
		return -1;
	}
	return 0;
}

static int i2c_write_read(struct i2c_adapter *adapter,
			  u8 adr, u8 *w, u8 wlen, u8 *r, u8 rlen)
{
	struct i2c_msg msgs[2] = {{.addr = adr, .flags = 0,
				   .buf = w, .len = wlen},
				  {.addr = adr, .flags = I2C_M_RD,
				   .buf = r, .len = rlen} };

	if (i2c_transfer(adapter, msgs, 2) != 2) {
		printk(KERN_ERR DEVICE_NAME ": error in i2c_write_read\n");
		return -1;
	}
	return 0;
}

static int test_dec_i2c(struct i2c_adapter *adapter, int reg)
{
	u8 data[256] = { reg, 0x00, 0x93, 0x78, 0x43, 0x45 };
	u8 data2[256];
	int i;

	memset(data2, 0, 256);
	i2c_write_read(adapter, 0x66, data, 2, data2, 4);
	for (i = 0; i < 4; i++)
		printk(KERN_DEBUG "%02x ", data2[i]);
	printk(KERN_DEBUG "\n");

	return 0;
}

static int i2c_write_msp_register(struct i2c_adapter *adapter,
				  u8 adr, u8 reg, u16 data)
{
	u8 m[3] = {reg, (data >> 8) & 0xff, data & 0xff};
	struct i2c_msg msg = {.addr = adr, .flags = 0, .buf = m, .len = 3 };

	if (i2c_transfer(adapter, &msg, 1) != 1) {
		printk(KERN_ERR DEVICE_NAME
		       ": Failed to write to I2C register %02x@%02x!\n",
		       reg, adr);
		return -1;
	}
	return 0;
}

#endif
