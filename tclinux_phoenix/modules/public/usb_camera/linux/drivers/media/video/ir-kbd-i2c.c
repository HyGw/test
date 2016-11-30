/*
 *
 * keyboard input driver for i2c IR remote controls
 *
 * Copyright (c) 2000-2003 Gerd Knorr <kraxel@bytesex.org>
 * modified for PixelView (BT878P+W/FM) by
 *      Michal Kochanowicz <mkochano@pld.org.pl>
 *      Christoph Bartelmus <lirc@bartelmus.de>
 * modified for KNC ONE TV Station/Anubis Typhoon TView Tuner by
 *      Ulrich Mueller <ulrich.mueller42@web.de>
 * modified for em2820 based USB TV tuners by
 *      Markus Rechberger <mrechberger@gmail.com>
 * modified for DViCO Fusion HDTV 5 RT GOLD by
 *      Chaogui Zhang <czhang1974@gmail.com>
 * modified for MSI TV@nywhere Plus by
 *      Henry Wong <henry@stuffedcow.net>
 *      Mark Schultz <n9xmj@yahoo.com>
 *      Brian Rogers <brian_rogers@comcast.net>
 * modified for AVerMedia Cardbus by
 *      Oldrich Jedlicka <oldium.pro@seznam.cz>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/i2c-id.h>
#include <linux/workqueue.h>

#include "compat.h"
#include <media/ir-common.h>
#include <media/ir-kbd-i2c.h>

/* ----------------------------------------------------------------------- */
/* insmod parameters                                                       */

static int debug;
module_param(debug, int, 0644);    /* debug level (0,1,2) */

static int hauppauge;
module_param(hauppauge, int, 0644);    /* Choose Hauppauge remote */
MODULE_PARM_DESC(hauppauge, "Specify Hauppauge remote: 0=black, 1=grey (defaults to 0)");


#define MODULE_NAME "ir-kbd-i2c"
#define dprintk(level, fmt, arg...)	if (debug >= level) \
	printk(KERN_DEBUG MODULE_NAME ": " fmt , ## arg)

/* ----------------------------------------------------------------------- */

static int get_key_haup_common(struct IR_i2c *ir, u32 *ir_key, u32 *ir_raw,
			       int size, int offset)
{
	unsigned char buf[6];
	int start, range, toggle, dev, code, ircode;

	/* poll IR chip */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 30)
	if (size != i2c_master_recv(&ir->c,buf,size))
#else
	if (size != i2c_master_recv(ir->c, buf, size))
#endif
		return -EIO;

	/* split rc5 data block ... */
	start  = (buf[offset] >> 7) &    1;
	range  = (buf[offset] >> 6) &    1;
	toggle = (buf[offset] >> 5) &    1;
	dev    =  buf[offset]       & 0x1f;
	code   = (buf[offset+1] >> 2) & 0x3f;

	/* rc5 has two start bits
	 * the first bit must be one
	 * the second bit defines the command range (1 = 0-63, 0 = 64 - 127)
	 */
	if (!start)
		/* no key pressed */
		return 0;
	/*
	 * Hauppauge remotes (black/silver) always use
	 * specific device ids. If we do not filter the
	 * device ids then messages destined for devices
	 * such as TVs (id=0) will get through causing
	 * mis-fired events.
	 *
	 * We also filter out invalid key presses which
	 * produce annoying debug log entries.
	 */
	ircode= (start << 12) | (toggle << 11) | (dev << 6) | code;
	if ((ircode & 0x1fff)==0x1fff)
		/* invalid key press */
		return 0;

	if (dev!=0x1e && dev!=0x1f)
		/* not a hauppauge remote */
		return 0;

	if (!range)
		code += 64;

	dprintk(1,"ir hauppauge (rc5): s%d r%d t%d dev=%d code=%d\n",
		start, range, toggle, dev, code);

	/* return key */
	*ir_key = code;
	*ir_raw = ircode;
	return 1;
}

static int get_key_haup(struct IR_i2c *ir, u32 *ir_key, u32 *ir_raw)
{
	return get_key_haup_common (ir, ir_key, ir_raw, 3, 0);
}

static int get_key_haup_xvr(struct IR_i2c *ir, u32 *ir_key, u32 *ir_raw)
{
	return get_key_haup_common (ir, ir_key, ir_raw, 6, 3);
}

static int get_key_pixelview(struct IR_i2c *ir, u32 *ir_key, u32 *ir_raw)
{
	unsigned char b;

	/* poll IR chip */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 30)
	if (1 != i2c_master_recv(&ir->c,&b,1)) {
#else
	if (1 != i2c_master_recv(ir->c, &b, 1)) {
#endif
		dprintk(1,"read error\n");
		return -EIO;
	}
	*ir_key = b;
	*ir_raw = b;
	return 1;
}

static int get_key_pv951(struct IR_i2c *ir, u32 *ir_key, u32 *ir_raw)
{
	unsigned char b;

	/* poll IR chip */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 30)
	if (1 != i2c_master_recv(&ir->c,&b,1)) {
#else
	if (1 != i2c_master_recv(ir->c, &b, 1)) {
#endif
		dprintk(1,"read error\n");
		return -EIO;
	}

	/* ignore 0xaa */
	if (b==0xaa)
		return 0;
	dprintk(2,"key %02x\n", b);

	*ir_key = b;
	*ir_raw = b;
	return 1;
}

static int get_key_fusionhdtv(struct IR_i2c *ir, u32 *ir_key, u32 *ir_raw)
{
	unsigned char buf[4];

	/* poll IR chip */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 30)
	if (4 != i2c_master_recv(&ir->c,buf,4)) {
#else
	if (4 != i2c_master_recv(ir->c, buf, 4)) {
#endif
		dprintk(1,"read error\n");
		return -EIO;
	}

	if(buf[0] !=0 || buf[1] !=0 || buf[2] !=0 || buf[3] != 0)
		dprintk(2, "%s: 0x%2x 0x%2x 0x%2x 0x%2x\n", __func__,
			buf[0], buf[1], buf[2], buf[3]);

	/* no key pressed or signal from other ir remote */
	if(buf[0] != 0x1 ||  buf[1] != 0xfe)
		return 0;

	*ir_key = buf[2];
	*ir_raw = (buf[2] << 8) | buf[3];

	return 1;
}

static int get_key_knc1(struct IR_i2c *ir, u32 *ir_key, u32 *ir_raw)
{
	unsigned char b;

	/* poll IR chip */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 30)
	if (1 != i2c_master_recv(&ir->c,&b,1)) {
#else
	if (1 != i2c_master_recv(ir->c, &b, 1)) {
#endif
		dprintk(1,"read error\n");
		return -EIO;
	}

	/* it seems that 0xFE indicates that a button is still hold
	   down, while 0xff indicates that no button is hold
	   down. 0xfe sequences are sometimes interrupted by 0xFF */

	dprintk(2,"key %02x\n", b);

	if (b == 0xff)
		return 0;

	if (b == 0xfe)
		/* keep old data */
		return 1;

	*ir_key = b;
	*ir_raw = b;
	return 1;
}

static int get_key_avermedia_cardbus(struct IR_i2c *ir,
				     u32 *ir_key, u32 *ir_raw)
{
	unsigned char subaddr, key, keygroup;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 30)
	struct i2c_msg msg[] = { { .addr = ir->c.addr, .flags = 0,
#else
	struct i2c_msg msg[] = { { .addr = ir->c->addr, .flags = 0,
#endif
				   .buf = &subaddr, .len = 1},
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 30)
				 { .addr = ir->c.addr, .flags = I2C_M_RD,
#else
				 { .addr = ir->c->addr, .flags = I2C_M_RD,
#endif
				  .buf = &key, .len = 1} };
	subaddr = 0x0d;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 30)
	if (2 != i2c_transfer(ir->c.adapter, msg, 2)) {
#else
	if (2 != i2c_transfer(ir->c->adapter, msg, 2)) {
#endif
		dprintk(1, "read error\n");
		return -EIO;
	}

	if (key == 0xff)
		return 0;

	subaddr = 0x0b;
	msg[1].buf = &keygroup;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 30)
	if (2 != i2c_transfer(ir->c.adapter, msg, 2)) {
#else
	if (2 != i2c_transfer(ir->c->adapter, msg, 2)) {
#endif
		dprintk(1, "read error\n");
		return -EIO;
	}

	if (keygroup == 0xff)
		return 0;

	dprintk(1, "read key 0x%02x/0x%02x\n", key, keygroup);
	if (keygroup < 2 || keygroup > 3) {
		/* Only a warning */
		dprintk(1, "warning: invalid key group 0x%02x for key 0x%02x\n",
								keygroup, key);
	}
	key |= (keygroup & 1) << 6;

	*ir_key = key;
	*ir_raw = key;
	return 1;
}

/* ----------------------------------------------------------------------- */

static void ir_key_poll(struct IR_i2c *ir)
{
	static u32 ir_key, ir_raw;
	int rc;

	dprintk(2,"ir_poll_key\n");
	rc = ir->get_key(ir, &ir_key, &ir_raw);
	if (rc < 0) {
		dprintk(2,"error\n");
		return;
	}

	if (0 == rc) {
		ir_input_nokey(ir->input, &ir->ir);
	} else {
		ir_input_keydown(ir->input, &ir->ir, ir_key);
	}
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
static void ir_work(void *data)
#else
static void ir_work(struct work_struct *work)
#endif
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
	struct IR_i2c *ir = data;
#else
	struct IR_i2c *ir = container_of(work, struct IR_i2c, work.work);
#endif
	int polling_interval = 100;

	/* MSI TV@nywhere Plus requires more frequent polling
	   otherwise it will miss some keypresses */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 30)
	if (ir->c.adapter->id == I2C_HW_SAA7134 && ir->c.addr == 0x30)
#else
	if (ir->c->adapter->id == I2C_HW_SAA7134 && ir->c->addr == 0x30)
#endif
		polling_interval = 50;

	ir_key_poll(ir);
	schedule_delayed_work(&ir->work, msecs_to_jiffies(polling_interval));
}

/* ----------------------------------------------------------------------- */

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 30)
static int ir_attach(struct i2c_adapter *adap, int addr,
		     unsigned short flags, int kind);
static int ir_detach(struct i2c_client *client);
static int ir_probe(struct i2c_adapter *adap);

static struct i2c_driver driver = {
	.driver = {
		.name   = "ir-kbd-i2c",
	},
	.attach_adapter = ir_probe,
	.detach_client  = ir_detach,
};

static struct i2c_client client =
{
	.name = "unset",
	.driver = &driver
 };

static int ir_attach(struct i2c_adapter *adap, int addr,
		     unsigned short flags, int kind)
#else
static int ir_probe(struct i2c_client *client, const struct i2c_device_id *id)
#endif
{
	char *ir_codes = NULL;
	const char *name = NULL;
	u64 ir_type = 0;
	struct IR_i2c *ir;
	struct input_dev *input_dev;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 30)
	struct i2c_adapter *adap = client->adapter;
	unsigned short addr = client->addr;
#endif
	int err;

	ir = kzalloc(sizeof(struct IR_i2c),GFP_KERNEL);
	input_dev = input_allocate_device();
	if (!ir || !input_dev) {
		err = -ENOMEM;
		goto err_out_free;
	}

	ir->c = client;
	ir->input = input_dev;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 30)

	ir->c.adapter = adap;
	ir->c.addr    = addr;
	snprintf(ir->c.name, sizeof(ir->c.name), "ir-kbd");

	i2c_set_clientdata(&ir->c, ir);
#else
	i2c_set_clientdata(client, ir);
#endif

	switch(addr) {
	case 0x64:
		name        = "Pixelview";
		ir->get_key = get_key_pixelview;
		ir_type     = IR_TYPE_OTHER;
		ir_codes    = RC_MAP_EMPTY;
		break;
	case 0x4b:
		name        = "PV951";
		ir->get_key = get_key_pv951;
		ir_type     = IR_TYPE_OTHER;
		ir_codes    = RC_MAP_PV951;
		break;
	case 0x18:
	case 0x1f:
	case 0x1a:
		name        = "Hauppauge";
		ir->get_key = get_key_haup;
		ir_type     = IR_TYPE_RC5;
		if (hauppauge == 1) {
			ir_codes    = RC_MAP_HAUPPAUGE_NEW;
		} else {
			ir_codes    = RC_MAP_RC5_TV;
		}
		break;
	case 0x30:
		name        = "KNC One";
		ir->get_key = get_key_knc1;
		ir_type     = IR_TYPE_OTHER;
		ir_codes    = RC_MAP_EMPTY;
		break;
	case 0x6b:
		name        = "FusionHDTV";
		ir->get_key = get_key_fusionhdtv;
		ir_type     = IR_TYPE_RC5;
		ir_codes    = RC_MAP_FUSIONHDTV_MCE;
		break;
	case 0x0b:
	case 0x47:
	case 0x71:
		if (adap->id == I2C_HW_B_CX2388x ||
		    adap->id == I2C_HW_B_CX2341X) {
			/* Handled by cx88-input */
			name = adap->id == I2C_HW_B_CX2341X ? "CX2341x remote"
							    : "CX2388x remote";
			ir_type     = IR_TYPE_RC5;
			ir->get_key = get_key_haup_xvr;
			if (hauppauge == 1) {
				ir_codes    = RC_MAP_HAUPPAUGE_NEW;
			} else {
				ir_codes    = RC_MAP_RC5_TV;
			}
		} else {
			/* Handled by saa7134-input */
			name        = "SAA713x remote";
			ir_type     = IR_TYPE_OTHER;
		}
		break;
	case 0x40:
		name        = "AVerMedia Cardbus remote";
		ir->get_key = get_key_avermedia_cardbus;
		ir_type     = IR_TYPE_OTHER;
		ir_codes    = RC_MAP_AVERMEDIA_CARDBUS;
		break;
	}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 30)
	/* Let the caller override settings */
	if (client->dev.platform_data) {
		const struct IR_i2c_init_data *init_data =
						client->dev.platform_data;

		ir_codes = init_data->ir_codes;
		name = init_data->name;
		if (init_data->type)
			ir_type = init_data->type;

		switch (init_data->internal_get_key_func) {
		case IR_KBD_GET_KEY_CUSTOM:
			/* The bridge driver provided us its own function */
			ir->get_key = init_data->get_key;
			break;
		case IR_KBD_GET_KEY_PIXELVIEW:
			ir->get_key = get_key_pixelview;
			break;
		case IR_KBD_GET_KEY_PV951:
			ir->get_key = get_key_pv951;
			break;
		case IR_KBD_GET_KEY_HAUP:
			ir->get_key = get_key_haup;
			break;
		case IR_KBD_GET_KEY_KNC1:
			ir->get_key = get_key_knc1;
			break;
		case IR_KBD_GET_KEY_FUSIONHDTV:
			ir->get_key = get_key_fusionhdtv;
			break;
		case IR_KBD_GET_KEY_HAUP_XVR:
			ir->get_key = get_key_haup_xvr;
			break;
		case IR_KBD_GET_KEY_AVERMEDIA_CARDBUS:
			ir->get_key = get_key_avermedia_cardbus;
			break;
		}
	}

	/* Make sure we are all setup before going on */
	if (!name || !ir->get_key || !ir_type || !ir_codes) {
		dprintk(1, ": Unsupported device at address 0x%02x\n",
			addr);
		err = -ENODEV;
		goto err_out_free;
	}
#endif

	/* Sets name */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 30)
	snprintf(ir->c.name, sizeof(ir->c.name), "i2c IR (%s)", name);
#else
	snprintf(ir->name, sizeof(ir->name), "i2c IR (%s)", name);
#endif
	ir->ir_codes = ir_codes;

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 30)
	/* register i2c device
	 * At device register, IR codes may be changed to be
	 * board dependent.
	 */
	err = i2c_attach_client(&ir->c);
	if (err)
		goto err_out_nodetach;

	/* If IR not supported or disabled, unregisters driver */
	if (ir->get_key == NULL) {
		err = -ENODEV;
		goto err_out_free;
	}

	/* Phys addr can only be set after attaching (for ir->c.dev) */
#endif
	snprintf(ir->phys, sizeof(ir->phys), "%s/%s/ir0",
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 30)
		 dev_name(&ir->c.adapter->dev),
		 dev_name(&ir->c.dev));
#else
		 dev_name(&adap->dev),
		 dev_name(&client->dev));
#endif

	/* init + register input device */
	err = ir_input_init(input_dev, &ir->ir, ir_type);
	if (err < 0)
		goto err_out_free;

	input_dev->id.bustype = BUS_I2C;
	input_dev->name       = ir->name;
	input_dev->phys       = ir->phys;

	err = ir_input_register(ir->input, ir->ir_codes, NULL, MODULE_NAME);
	if (err)
		goto err_out_free;

	printk(MODULE_NAME ": %s detected at %s [%s]\n",
	       ir->input->name, ir->input->phys, adap->name);

	/* start polling via eventd */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
	INIT_DELAYED_WORK(&ir->work, ir_work, ir);
#else
	INIT_DELAYED_WORK(&ir->work, ir_work);
#endif
	schedule_delayed_work(&ir->work, 0);

	return 0;

 err_out_free:
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 30)
	i2c_detach_client(&ir->c);

 err_out_nodetach:
#endif
	kfree(ir);
	return err;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 30)
static int ir_detach(struct i2c_client *client)
#else
static int ir_remove(struct i2c_client *client)
#endif
{
	struct IR_i2c *ir = i2c_get_clientdata(client);

	/* kill outstanding polls */
	cancel_delayed_work_sync(&ir->work);

	/* unregister device */
	ir_input_unregister(ir->input);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 30)
	i2c_detach_client(&ir->c);
#endif

	/* free memory */
	kfree(ir);
	return 0;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 30)
static int ir_probe(struct i2c_adapter *adap)
{

	/* The external IR receiver is at i2c address 0x34 (0x35 for
	   reads).  Future Hauppauge cards will have an internal
	   receiver at 0x30 (0x31 for reads).  In theory, both can be
	   fitted, and Hauppauge suggest an external overrides an
	   internal.

	   That's why we probe 0x1a (~0x34) first. CB
	*/

	static const int probe_bttv[] = { 0x1a, 0x18, 0x4b, 0x64, 0x30, -1};
	static const int probe_saa7134[] = { 0x7a, 0x47, 0x71, 0x2d, -1 };
	static const int probe_em28XX[] = { 0x30, 0x47, -1 };
	static const int probe_cx88[] = { 0x18, 0x6b, 0x71, -1 };
	static const int probe_cx23885[] = { 0x6b, -1 };
	const int *probe;
	struct i2c_msg msg = {
		.flags = I2C_M_RD,
		.len = 0,
		.buf = NULL,
	};
	int i, rc;

	switch (adap->id) {
	case I2C_HW_B_BT848:
		probe = probe_bttv;
		break;
	case I2C_HW_B_CX2341X:
		probe = probe_bttv;
		break;
	case I2C_HW_SAA7134:
		probe = probe_saa7134;
		break;
	case I2C_HW_B_EM28XX:
		probe = probe_em28XX;
		break;
	case I2C_HW_B_CX2388x:
		probe = probe_cx88;
		break;
	case I2C_HW_B_CX23885:
		probe = probe_cx23885;
		break;
	default:
		return 0;
	}

	for (i = 0; -1 != probe[i]; i++) {
		msg.addr = probe[i];
		rc = i2c_transfer(adap, &msg, 1);
		dprintk(1,"probe 0x%02x @ %s: %s\n",
			probe[i], adap->name,
			(1 == rc) ? "yes" : "no");
		if (1 == rc) {
			ir_attach(adap, probe[i], 0, 0);
			return 0;
		}
	}

	/* Special case for MSI TV@nywhere Plus remote */
	if (adap->id == I2C_HW_SAA7134) {
		u8 temp;

		/* MSI TV@nywhere Plus controller doesn't seem to
		   respond to probes unless we read something from
		   an existing device. Weird... */

		msg.addr = 0x50;
		rc = i2c_transfer(adap, &msg, 1);
			dprintk(1, "probe 0x%02x @ %s: %s\n",
			msg.addr, adap->name,
			(1 == rc) ? "yes" : "no");

		/* Now do the probe. The controller does not respond
		   to 0-byte reads, so we use a 1-byte read instead. */
		msg.addr = 0x30;
		msg.len = 1;
		msg.buf = &temp;
		rc = i2c_transfer(adap, &msg, 1);
		dprintk(1, "probe 0x%02x @ %s: %s\n",
			msg.addr, adap->name,
			(1 == rc) ? "yes" : "no");
		if (1 == rc)
			ir_attach(adap, msg.addr, 0, 0);
	}

	/* Special case for AVerMedia Cardbus remote */
	if (adap->id == I2C_HW_SAA7134) {
		unsigned char subaddr, data;
		struct i2c_msg msg[] = { { .addr = 0x40, .flags = 0,
					   .buf = &subaddr, .len = 1},
					 { .addr = 0x40, .flags = I2C_M_RD,
					   .buf = &data, .len = 1} };
		subaddr = 0x0d;
		rc = i2c_transfer(adap, msg, 2);
		dprintk(1, "probe 0x%02x/0x%02x @ %s: %s\n",
			msg[0].addr, subaddr, adap->name,
			(2 == rc) ? "yes" : "no");
		if (2 == rc)
			ir_attach(adap, msg[0].addr, 0, 0);
	}

	/* special case for LifeView FlyDVB Trio */
	if (adap->id == I2C_HW_SAA7134) {
		u8 temp = 0;
		msg.buf = &temp;
		msg.addr = 0x0b;
		msg.len = 1;
		msg.flags = 0;

		/*
		 * send weak up message to pic16C505 chip
		 * @ LifeView FlyDVB Trio
		 */
		if (1 != i2c_transfer(adap, &msg, 1)) {
			dprintk(1, "send wake up byte to pic16C505 failed\n");
		} else {
			msg.flags = I2C_M_RD;
			rc = i2c_transfer(adap, &msg, 1);
			dprintk(1, "probe 0x%02x @ %s: %s\n",
					msg.addr, adap->name,
					(1 == rc) ? "yes" : "no");
			if (1 == rc)
				ir_attach(adap, msg.addr, 0, 0);
		}
		msg.len = 0;
		msg.flags = I2C_M_RD;
	}

	return 0;
}
#else
static const struct i2c_device_id ir_kbd_id[] = {
	/* Generic entry for any IR receiver */
	{ "ir_video", 0 },
	/* IR device specific entries should be added here */
	{ "ir_rx_z8f0811_haup", 0 },
	{ }
};

static struct i2c_driver driver = {
	.driver = {
		.name   = "ir-kbd-i2c",
	},
	.probe          = ir_probe,
	.remove         = ir_remove,
	.id_table       = ir_kbd_id,
};
#endif

/* ----------------------------------------------------------------------- */

MODULE_AUTHOR("Gerd Knorr, Michal Kochanowicz, Christoph Bartelmus, Ulrich Mueller");
MODULE_DESCRIPTION("input driver for i2c IR remote controls");
MODULE_LICENSE("GPL");

static int __init ir_init(void)
{
	return i2c_add_driver(&driver);
}

static void __exit ir_fini(void)
{
	i2c_del_driver(&driver);
}

module_init(ir_init);
module_exit(ir_fini);

/*
 * Overrides for Emacs so that we follow Linus's tabbing style.
 * ---------------------------------------------------------------------------
 * Local variables:
 * c-basic-offset: 8
 * End:
 */
