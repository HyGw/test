/*
 * $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/modules/public/usb_camera/v4l_experimental/dpl3518.c#1 $
 *
 * programming the dpl3518a Dolby Pro Logic Processor
 *
 * WARNING: THIS DRIVER WILL LOAD WITHOUT COMPLAINTS EVEN IF A DIFFERENT
 * CHIP IS AT ADDRESS 0x84 (it relies on i2c to make sure that there is a
 * device acknowledging that address)
 *
 *  Copyright (C) 1999 Roland Jansen <roland@lut.rwth-aachen.de>
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
 * USA.
 *
 * ---
 * DOLBY and PRO LOGIC are trademarks of
 * Dolby Laboratories Licensing Corporation.
 */

/* FIXME */
#define DPL_MAJOR  127

#include "compat.h"
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#include <media/id.h>
#include "dpl3518.h"


/* Addresses to scan */
#define I2C_DPL3518       0x84
static unsigned short normal_i2c[] = {
	I2C_DPL3518 >> 1,
	I2C_CLIENT_END};

I2C_CLIENT_INSMOD;

struct dpl3518 {
	struct i2c_client *client;
	int mode;		/* dolby mode */
};

static struct dpl3518 *dpl_device;
static struct i2c_driver driver;
static struct i2c_client client_template;

/* ---------------------------------------------------------------------- */

/* insmod parameters */
static int debug = 0;		/* debug output */
MODULE_PARM(debug, "i");

MODULE_DESCRIPTION("device driver for dpl3518a Dolby Pro Logic Processor");
MODULE_AUTHOR("Roland Jansen");
MODULE_LICENSE("GPL");

#define dprintk     if (debug) printk

/* ---------------------------------------------------------------------- */

#define I2C_DPL3518_MR    0x10	/* write address MODE_REG */
#define I2C_DPL3518_DFP   0x12	/* write address DFP */

/* ----------------------------------------------------------------------- */
/* functions for talking to the dpl3518 Sound processor                   */

static int dpl3518_reset(struct i2c_client *client)
{
	static char reset_off[3] = { 0x00, 0x80, 0x00 };
	static char reset_on[3]  = { 0x00, 0x00, 0x00 };

	i2c_master_send(client,reset_off,3);  /* ignore errors here */
	if (3 != i2c_master_send(client,reset_on, 3)) {
		printk(KERN_ERR "dpl3518: chip reset failed, penguin on i2c bus?\n");
		return -1;
	}
	return 0;
}

static int dpl3518_write(struct i2c_client *client, int dev, int addr, int val)
{
	int err;
	unsigned char buffer[5];

	buffer[0] = dev;
	buffer[1] = addr >> 8;
	buffer[2] = addr &  0xff;
	buffer[3] = val  >> 8;
	buffer[4] = val  &  0xff;

	for (err = 0; err < 3;) {
		if (5 == i2c_master_send(client, buffer, 5))
			break;
		err++;
		printk(KERN_WARNING "dpl3518: I/O error #%d (write 0x%02x/0x%02x)\n",
		       err, dev, addr);
		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout(HZ/10);
	}
	if (3 == err) {
		printk(KERN_WARNING "dpl3518: giving up, reseting chip. Sound will go off, sorry folks :-|\n");
		dpl3518_reset(client);
		return -1;
	}
	return 0;
}

/* ----------------------------------------------------------------------- */

/* in: 0..100 out: 0x00..0x7f */
static int p100_to_7f(int i)
{
	int r;

	r = ((i & 0x7f) * 127 + 1) / 100;
	if (r > 127)
		r = 127;
	if (r < 0)
		r = 0;
	return r;
}

/* ----------------------------------------------------------------------- */

static void dpl3518_ioc_init(struct i2c_client *client)
{
	dprintk("dpl3518 init\n");
	dpl3518_write(client, I2C_DPL3518_MR, 0x0083, 0x0008); /* mode_reg */
	dpl3518_write(client, I2C_DPL3518_DFP, 0x000b, 0x0320); /*I2S1 src: RL */
	dpl3518_write(client, I2C_DPL3518_DFP, 0x0012, 0x0000); /*prescale2: off */
	dpl3518_write(client, I2C_DPL3518_DFP, 0x0016, 0x1000); /*prescale: 0 dB */
	dpl3518_write(client, I2C_DPL3518_DFP, 0x0038, 0x0420); /*I2S2 src: CS */
	dpl3518_write(client, I2C_DPL3518_DFP, 0x0040, 0x0000); /*adaptive,surround */
	dpl3518_write(client, I2C_DPL3518_DFP, 0x0041, 0x0520); /*i2s1, stereo */
	dpl3518_write(client, I2C_DPL3518_DFP, 0x0042, 0x1400); /*delay: 20ms */
	dpl3518_write(client, I2C_DPL3518_DFP, 0x0043, 0x0000); /*inp. Bal. auto */
	dpl3518_write(client, I2C_DPL3518_DFP, 0x0044, 0x4000); /*spatial 50% */
	dpl3518_write(client, I2C_DPL3518_DFP, 0x0045, 0x0000); /*panorama off */
	//dpl3518_write(client,I2C_DPL3518_DFP,0x0045,0x5400); /*panorama 66%*/
	dpl3518_write(client, I2C_DPL3518_DFP, 0x0046, 0x0000); /*reverb off */
}

static void dpl3518_prologic_init(struct i2c_client *client)
{
	dprintk("dpl3518 prologic_init\n");
	dpl3518_write(client, I2C_DPL3518_DFP, 0x000b, 0x0320); /*I2S1 src: RL */
	dpl3518_write(client, I2C_DPL3518_DFP, 0x0038, 0x0420); /*I2S2 src: CS */
	dpl3518_write(client, I2C_DPL3518_DFP, 0x0041, 0x0520); /*i2s1, stereo */
}

static void dpl3518_noise_init(struct i2c_client *client)
{
	dprintk("dpl3518 noise_init\n");
	dpl3518_write(client, I2C_DPL3518_DFP, 0x000b, 0x0320); /*I2S1 src: RL */
	dpl3518_write(client, I2C_DPL3518_DFP, 0x0038, 0x0420); /*I2S2 src: CS */
	dpl3518_write(client, I2C_DPL3518_DFP, 0x0041, 0x0100); /*NOISE */
}

static void dpl3518_through_init(struct i2c_client *client)
{
	dprintk("dpl3518 through_init\n");
	dpl3518_write(client, I2C_DPL3518_DFP, 0x000b, 0x0520); /*I2S1 src: S1 */
	dpl3518_write(client, I2C_DPL3518_DFP, 0x0038, 0x0220); /*I2S2 src: scart*/
	dpl3518_write(client, I2C_DPL3518_DFP, 0x0041, 0x0520); /*i2s1, stereo */
}

static void dpl3518_setmode(struct i2c_client *client, int mode)
{
	dprintk("dpl3518 setmode: %i\n", mode);
	dpl3518_write(client, I2C_DPL3518_DFP, 0x0040, mode << 4);
	dpl_device->mode = mode;
}

static void dpl3518_setnoisemode(struct i2c_client *client, int mode)
{
	static int noise_modes[5] = { 0x0000, 0x01a0, 0x01b0, 0x01c0, 0x01d0 };
	dprintk("dpl3518 setnoisemode: %i\n", mode);
	dpl3518_write(client, I2C_DPL3518_DFP, 0x0041, noise_modes[mode]);
}


static void dpl3518_delay(struct i2c_client *client, int delay)
{
	dprintk("dpl3518 setdelay: %i\n", delay);
	dpl3518_write(client, I2C_DPL3518_DFP, 0x0042, delay << 8);
}

static void dpl3518_reverb(struct i2c_client *client, int rev)
{
	dprintk("dpl3518 setreverb: %i\n", rev);
	dpl3518_write(client, I2C_DPL3518_DFP, 0x0046, p100_to_7f(rev) << 8);
}

static void dpl3518_panorama(struct i2c_client *client, int pan)
{
	dprintk("dpl3518 panorama: %i\n", pan);
	dpl3518_write(client, I2C_DPL3518_DFP, 0x0045, p100_to_7f(pan) << 8);
}

static void dpl3518_spatial(struct i2c_client *client, int spatial)
{
	dprintk("dpl3518 setspatial: %i\n", spatial);
	dpl3518_write(client, I2C_DPL3518_DFP, 0x0045, p100_to_7f(spatial) << 8);
}

/* ----------------------------------------------------------------------- */

static int dpl3518_attach(struct i2c_adapter *adap, int addr, int kind)
{
	struct dpl3518 *dpl;
	struct i2c_client *client;

	client = kmalloc(sizeof *client,GFP_KERNEL);
	if (!client)
		return -ENOMEM;
	memcpy(client,&client_template,sizeof(struct i2c_client));
	client->adapter = adap;
	client->addr = addr;

	dpl = kmalloc(sizeof *dpl, GFP_KERNEL);
	if (NULL == dpl)
		return -ENOMEM;
	i2c_set_clientdata(client, dpl);
	memset(dpl, 0, sizeof(struct dpl3518));
	dpl->mode = 0;		/* DOLBY_MODE_NONE */
	dpl->client = client;

	if (-1 == dpl3518_reset(client)) {
		kfree(dpl);
		kfree(client);
		dprintk("dpl3518: no chip found\n");
		return -1;
	}

	printk(KERN_INFO "dpl3518: init\n");
	dpl_device = dpl;
	dpl3518_ioc_init(client);

	/* done */
	i2c_attach_client(client);
	return 0;
}

static int dpl3518_probe(struct i2c_adapter *adap)
{
	return i2c_probe(adap, &addr_data, dpl3518_attach);
}

static int dpl3518_detach(struct i2c_client *client)
{
	struct dpl3518 *dpl = i2c_get_clientdata(client);

	dpl3518_reset(client);
	i2c_detach_client(client);

	dpl_device = NULL;
	kfree(dpl);
	kfree(client);
	return 0;
}

static int dpl3518_dev_ioctl(struct inode *inode, struct file *file,
			     unsigned int cmd, unsigned long arg)
{
//        unsigned int minor = MINOR(inode->i_rdev);
	int ret = 0;
	struct i2c_client *client = dpl_device->client;

	switch (cmd) {
	case DPL_IOC_RESET:
		dpl3518_reset(client);
		break;
	case DPL_IOC_VERSION:
		if (put_user(DPL_VERSION_CODE, (int *) arg))
			ret = -EFAULT;
		break;
	case DPL_IOC_INIT:
		dpl3518_ioc_init(client);
		break;
	case DPL_IOC_PROLOGIC_INIT:
		dpl3518_prologic_init(client);
		break;
	case DPL_IOC_NOISE_INIT:
		dpl3518_noise_init(client);
		break;
	case DPL_IOC_THROUGH_INIT:
		dpl3518_through_init(client);
		break;
	case DPL_IOC_MODE:
		dpl3518_setmode(client, (int) arg);
		break;
	case DPL_IOC_GET_MODE:
		if (put_user(dpl_device->mode, (int *) arg))
			ret = -EFAULT;
		break;
	case DPL_IOC_NOISE_MODE:
		dpl3518_setnoisemode(client, (int) arg);
		break;
	case DPL_IOC_DELAY:
		dpl3518_delay(client, (int) arg);
		break;
	case DPL_IOC_REVERB:
		dpl3518_reverb(client, (int) arg);
		break;
	case DPL_IOC_PANORAMA:
		dpl3518_panorama(client, (int) arg);
		break;
	case DPL_IOC_SPATIAL:
		dpl3518_spatial(client, (int) arg);
		break;
	default:
		dprintk("dpl3518_ioctl: default %i\n", (int) arg);
		ret = -EINVAL;
	}
	return ret;
}

static int dpl3518_dev_open(struct inode *inode, struct file *file)
{
	dprintk("dpl3518_dev_open\n");
	return 0;
}

static int dpl3518_dev_release(struct inode *inode, struct file *file)
{
	dprintk("dpl3518_dev_release\n");
	return 0;
}

/* ---------------------------------------------------------------------- */

static struct i2c_driver driver = {
	.owner          = THIS_MODULE,
	.name           = "i2c dpl3518 driver",
	.id             = I2C_DRIVERID_DPL3518,
	.flags          = I2C_DF_NOTIFY,
	.attach_adapter = dpl3518_probe,
	.detach_client  = dpl3518_detach,
};

static struct i2c_client client_template =
{
	I2C_DEVNAME("dpl3518"),
	.id         = -1,
	.driver     = &driver
};

static struct file_operations dpl3518_fops =
{
	.owner          = THIS_MODULE,
	.ioctl		= dpl3518_dev_ioctl,
	.open		= dpl3518_dev_open,
	.release	= dpl3518_dev_release,
};


static int dpl3518_init(void)
{
	if (register_chrdev(DPL_MAJOR, "dpl3518", &dpl3518_fops)) {
		printk("dpl3518: unable to get major %d\n", DPL_MAJOR);
		return -EBUSY;
	}
	i2c_add_driver(&driver);
	return 0;
}

static void dpl3518_fini(void)
{
	i2c_del_driver(&driver);
	unregister_chrdev(DPL_MAJOR, "dpl3518");
}

module_init(dpl3518_init);
module_exit(dpl3518_fini);

/*
 * Local variables:
 * c-basic-offset: 8
 * End:
 */
