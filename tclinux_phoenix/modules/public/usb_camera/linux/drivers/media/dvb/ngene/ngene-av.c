/*
 * ngene-av.c: nGene PCIe bridge driver - DVB video/audio support
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

/* This file provides the support functions for DVB audio/video devices
   (/dev/dvb/adapter0/[video|audio]), not to be confused with V4L2 support */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/io.h>
#include <asm/div64.h>
#include <linux/pci.h>
#include <linux/smp_lock.h>
#include <linux/timer.h>
#include <linux/version.h>
#include <linux/byteorder/generic.h>
#include <linux/firmware.h>
#include <linux/vmalloc.h>
#include "compat.h"

#include "ngene.h"

#if 0

static void *ain_exchange(void *priv, void *buf, u32 len, u32 clock, u32 flags)
{
	struct ngene_channel *chan = priv;
	struct ngene *dev = chan->dev;

	if (dvb_ringbuffer_free(&dev->ain_rbuf) >= len) {
		dvb_ringbuffer_write(&dev->ain_rbuf, buf, len);
		wake_up_interruptible(&dev->ain_rbuf.queue);
	} else
		printk(KERN_INFO DEVICE_NAME ": Dropped ain packet.\n");

	return 0;
}

static void *vcap_exchange(void *priv, void *buf, u32 len, u32 clock, u32 flags)
{

	struct ngene_channel *chan = priv;
	struct ngene *dev = chan->dev;

	if (len >= 1920 * 1080)
		len = 1920 * 1080;
	if (dvb_ringbuffer_free(&dev->vin_rbuf) >= len) {
		dvb_ringbuffer_write(&dev->vin_rbuf, buf, len);
		wake_up_interruptible(&dev->vin_rbuf.queue);
	} else {
		;/*printk(KERN_INFO DEVICE_NAME ": Dropped vcap packet.\n"); */
	}
	return 0;
}

static ssize_t audio_write(struct file *file,
			   const char *buf, size_t count, loff_t *ppos)
{
	return -EINVAL;
}

ssize_t audio_read(struct file *file, char *buf, size_t count, loff_t *ppos)
{
	struct dvb_device *dvbdev = file->private_data;
	struct ngene_channel *chan = dvbdev->priv;
	struct ngene *dev = chan->dev;
	int left;
	int avail;

	left = count;
	while (left) {
		if (wait_event_interruptible(
				dev->ain_rbuf.queue,
				dvb_ringbuffer_avail(&dev->ain_rbuf) > 0) < 0)
			return -EAGAIN;
		avail = dvb_ringbuffer_avail(&dev->ain_rbuf);
		if (avail > left)
			avail = left;
		dvb_ringbuffer_read_user(&dev->ain_rbuf, buf, avail);
		left -= avail;
		buf += avail;
	}
	return count;
}

static int audio_open(struct inode *inode, struct file *file)
{
	struct dvb_device *dvbdev = file->private_data;
	struct ngene_channel *chan = dvbdev->priv;
	struct ngene *dev = chan->dev;
	struct ngene_channel *chan2 = &chan->dev->channel[2];
	int ret;

	ret = dvb_generic_open(inode, file);
	if (ret < 0)
		return ret;
	dvb_ringbuffer_flush(&dev->ain_rbuf);

	chan2->Capture1Length = MAX_AUDIO_BUFFER_SIZE;
	chan2->pBufferExchange = ain_exchange;
	ngene_command_stream_control(chan2->dev, chan2->number, 0x80,
				     SMODE_AUDIO_CAPTURE, 0);
	return ret;
}

static int audio_release(struct inode *inode, struct file *file)
{
	struct dvb_device *dvbdev = file->private_data;
	struct ngene_channel *chan = dvbdev->priv;
	struct ngene *dev = chan->dev;
	struct ngene_channel *chan2 = &chan->dev->channel[2];

	ngene_command_stream_control(dev, 2, 0, 0, 0);
	chan2->pBufferExchange = 0;

	return dvb_generic_release(inode, file);
}

static const struct file_operations audio_fops = {
	.owner   = THIS_MODULE,
	.read    = audio_read,
	.write   = audio_write,
	.open    = audio_open,
	.release = audio_release,
};

static struct dvb_device dvbdev_audio = {
	.priv    = 0,
	.readers = -1,
	.writers = 1,
	.users   = 1,
	.fops    = &audio_fops,
};

static int video_open(struct inode *inode, struct file *file)
{
	struct dvb_device *dvbdev = file->private_data;
	struct ngene_channel *chan = dvbdev->priv;
	struct ngene *dev = chan->dev;
	struct ngene_channel *chan0 = &chan->dev->channel[0];
	int ret;

	ret = dvb_generic_open(inode, file);
	if (ret < 0)
		return ret;
	if ((file->f_flags & O_ACCMODE) != O_RDONLY)
		return ret;
	dvb_ringbuffer_flush(&dev->vin_rbuf);

	chan0->nBytesPerLine = 1920 * 2;
	chan0->nLines = 540;
	chan0->Capture1Length = 1920 * 2 * 540;
	chan0->pBufferExchange = vcap_exchange;
	chan0->itumode = 2;
	ngene_command_stream_control(chan0->dev, chan0->number,
				     0x80, SMODE_VIDEO_CAPTURE, 0);
	return ret;
}

static int video_release(struct inode *inode, struct file *file)
{
	struct dvb_device *dvbdev = file->private_data;
	struct ngene_channel *chan = dvbdev->priv;
	struct ngene *dev = chan->dev;
	struct ngene_channel *chan0 = &chan->dev->channel[0];

	ngene_command_stream_control(dev, 0, 0, 0, 0);
	chan0->pBufferExchange = 0;

	return dvb_generic_release(inode, file);
}

static ssize_t video_write(struct file *file,
			   const char *buf, size_t count, loff_t *ppos)
{
	return -EINVAL;
}

ssize_t video_read(struct file *file, char *buf, size_t count, loff_t *ppos)
{
	struct dvb_device *dvbdev = file->private_data;
	struct ngene_channel *chan = dvbdev->priv;
	struct ngene *dev = chan->dev;
	int left, avail;

	left = count;
	while (left) {
		if (wait_event_interruptible(
				dev->vin_rbuf.queue,
				dvb_ringbuffer_avail(&dev->vin_rbuf) > 0) < 0)
			return -EAGAIN;
		avail = dvb_ringbuffer_avail(&dev->vin_rbuf);
		if (avail > left)
			avail = left;
		dvb_ringbuffer_read_user(&dev->vin_rbuf, buf, avail);
		left -= avail;
		buf += avail;
	}
	return count;
}

/* Why is this not exported from dvb_core ?!?! */

static int dvb_usercopy2(struct inode *inode, struct file *file,
			 unsigned int cmd, unsigned long arg,
			 int (*func)(struct inode *inode, struct file *file,
				     unsigned int cmd, void *arg))
{
	char sbuf[128];
	void *mbuf = NULL;
	void *parg = NULL;
	int  err   = -EINVAL;

	/*  Copy arguments into temp kernel buffer  */
	switch (_IOC_DIR(cmd)) {
	case _IOC_NONE:
		/*
		 * For this command, the pointer is actually an integer
		 * argument.
		 */
		parg = (void *)arg;
		break;
	case _IOC_READ: /* some v4l ioctls are marked wrong ... */
	case _IOC_WRITE:
	case (_IOC_WRITE | _IOC_READ):
		if (_IOC_SIZE(cmd) <= sizeof(sbuf)) {
			parg = sbuf;
		} else {
			/* too big to allocate from stack */
			mbuf = kmalloc(_IOC_SIZE(cmd), GFP_KERNEL);
			if (NULL == mbuf)
				return -ENOMEM;
			parg = mbuf;
		}

		err = -EFAULT;
		if (copy_from_user(parg, (void __user *)arg, _IOC_SIZE(cmd)))
			goto out;
		break;
	}

	/* call driver */
	err = func(inode, file, cmd, parg);
	if (err == -ENOIOCTLCMD)
		err = -EINVAL;

	if (err < 0)
		goto out;

	/*  Copy results into user buffer  */
	switch (_IOC_DIR(cmd)) {
	case _IOC_READ:
	case (_IOC_WRITE | _IOC_READ):
		if (copy_to_user((void __user *)arg, parg, _IOC_SIZE(cmd)))
			err = -EFAULT;
		break;
	}

out:
	kfree(mbuf);
	return err;
}

static int video_do_ioctl(struct inode *inode, struct file *file,
			  unsigned int cmd, void *parg)
{
	struct dvb_device *dvbdev = file->private_data;
	struct ngene_channel *chan = dvbdev->priv;
	struct ngene *dev = chan->dev;
	int ret = 0;
	unsigned long arg = (unsigned long)parg;

	switch (cmd) {
	case VIDEO_SET_STREAMTYPE:
		switch (arg) {
		case VIDEO_CAP_MPEG2:
			/* printk(KERN_INFO DEVICE_NAME ": setting MPEG2\n"); */
			send_cli(dev, "vdec mpeg2\n");
			break;
		case VIDEO_CAP_AVC:
			/* printk(KERN_INFO DEVICE_NAME ": setting H264\n"); */
			send_cli(dev, "vdec h264\n");
			break;
		case VIDEO_CAP_VC1:
			/* printk(KERN_INFO DEVICE_NAME ": setting VC1\n"); */
			send_cli(dev, "vdec vc1\n");
			break;
		default:
			ret = -EINVAL;
			break;
		}
		break;
	default:
		ret = -ENOIOCTLCMD;
		return -EINVAL;
	}
	return ret;
}

static int video_ioctl(struct inode *inode, struct file *file,
		       unsigned int cmd, unsigned long arg)
{
	return dvb_usercopy2(inode, file, cmd, arg, video_do_ioctl);
}

static const struct file_operations video_fops = {
	.owner   = THIS_MODULE,
	.read    = video_read,
	.write   = video_write,
	.open    = video_open,
	.release = video_release,
	.ioctl   = video_ioctl,
};

static struct dvb_device dvbdev_video = {
	.priv    = 0,
	.readers = -1,
	.writers = 1,
	.users   = -1,
	.fops    = &video_fops,
};
#endif
