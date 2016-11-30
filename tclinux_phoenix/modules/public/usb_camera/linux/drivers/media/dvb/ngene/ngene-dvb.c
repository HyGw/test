/*
 * ngene-dvb.c: nGene PCIe bridge driver - DVB functions
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

#define COMMAND_TIMEOUT_WORKAROUND

#if 0
int ngene_stream_control(struct ngene *dev, u8 stream, u8 control, u8 mode,
			 u16 lines, u16 bpl, u16 vblines, u16 vbibpl)
{
	if (!(mode & SMODE_TRANSPORT_STREAM))
		return -EINVAL;

	if (lines * bpl > MAX_VIDEO_BUFFER_SIZE)
		return -EINVAL;

	if ((mode & SMODE_TRANSPORT_STREAM) && (((bpl * lines) & 0xff) != 0))
		return -EINVAL;

	if ((mode & SMODE_VIDEO_CAPTURE) && (bpl & 7) != 0)
		return -EINVAL;

	return ngene_command_stream_control(dev, stream, control, mode, 0);
}
#endif

/****************************************************************************/
/* COMMAND API interface ****************************************************/
/****************************************************************************/
#if 0

static int command_do_ioctl(struct inode *inode, struct file *file,
			    unsigned int cmd, void *parg)
{
	struct dvb_device *dvbdev = file->private_data;
	struct ngene_channel *chan = dvbdev->priv;
	struct ngene *dev = chan->dev;
	int err = 0;

	switch (cmd) {
	case IOCTL_MIC_NO_OP:
		err = ngene_command_nop(dev);
		break;

	case IOCTL_MIC_DOWNLOAD_FIRMWARE:
		break;

	case IOCTL_MIC_I2C_READ:
	{
		MIC_I2C_READ *msg = parg;

		err = ngene_command_i2c_read(dev, msg->I2CAddress >> 1,
					     msg->OutData, msg->OutLength,
					     msg->OutData, msg->InLength, 1);
		break;
	}

	case IOCTL_MIC_I2C_WRITE:
	{
		MIC_I2C_WRITE *msg = parg;

		err = ngene_command_i2c_write(dev, msg->I2CAddress >> 1,
					      msg->Data, msg->Length);
		break;
	}

	case IOCTL_MIC_TEST_GETMEM:
	{
		MIC_MEM *m = parg;

		if (m->Length > 64 * 1024 || m->Start + m->Length > 64 * 1024)
			return -EINVAL;

		/* WARNING, only use this on x86,
		   other archs may not swallow this  */
		err = copy_to_user(m->Data, dev->iomem + m->Start, m->Length);
		break;
	}

	case IOCTL_MIC_TEST_SETMEM:
	{
		MIC_MEM *m = parg;

		if (m->Length > 64 * 1024 || m->Start + m->Length > 64 * 1024)
			return -EINVAL;

		err = copy_from_user(dev->iomem + m->Start, m->Data, m->Length);
		break;
	}

	case IOCTL_MIC_SFR_READ:
	{
		MIC_IMEM *m = parg;

		err = ngene_command_imem_read(dev, m->Address, &m->Data, 1);
		break;
	}

	case IOCTL_MIC_SFR_WRITE:
	{
		MIC_IMEM *m = parg;

		err = ngene_command_imem_write(dev, m->Address, m->Data, 1);
		break;
	}

	case IOCTL_MIC_IRAM_READ:
	{
		MIC_IMEM *m = parg;

		err = ngene_command_imem_read(dev, m->Address, &m->Data, 0);
		break;
	}

	case IOCTL_MIC_IRAM_WRITE:
	{
		MIC_IMEM *m = parg;

		err = ngene_command_imem_write(dev, m->Address, m->Data, 0);
		break;
	}

	case IOCTL_MIC_STREAM_CONTROL:
	{
		MIC_STREAM_CONTROL *m = parg;

		err = ngene_stream_control(dev, m->Stream, m->Control, m->Mode,
					   m->nLines, m->nBytesPerLine,
					   m->nVBILines, m->nBytesPerVBILine);
		break;
	}

	default:
		err = -EINVAL;
		break;
	}
	return err;
}

static int command_ioctl(struct inode *inode, struct file *file,
			 unsigned int cmd, unsigned long arg)
{
	void *parg = (void *)arg, *pbuf = NULL;
	char  buf[64];
	int   res = -EFAULT;

	if (_IOC_DIR(cmd) & _IOC_WRITE) {
		parg = buf;
		if (_IOC_SIZE(cmd) > sizeof(buf)) {
			pbuf = kmalloc(_IOC_SIZE(cmd), GFP_KERNEL);
			if (!pbuf)
				return -ENOMEM;
			parg = pbuf;
		}
		if (copy_from_user(parg, (void __user *)arg, _IOC_SIZE(cmd)))
			goto error;
	}
	res = command_do_ioctl(inode, file, cmd, parg);
	if (res < 0)
		goto error;
	if (_IOC_DIR(cmd) & _IOC_READ)
		if (copy_to_user((void __user *)arg, parg, _IOC_SIZE(cmd)))
			res = -EFAULT;
error:
	kfree(pbuf);
	return res;
}

struct page *ngene_nopage(struct vm_area_struct *vma,
			  unsigned long address, int *type)
{
	return 0;
}

static int ngene_mmap(struct file *file, struct vm_area_struct *vma)
{
	struct dvb_device *dvbdev = file->private_data;
	struct ngene_channel *chan = dvbdev->priv;
	struct ngene *dev = chan->dev;

	unsigned long size = vma->vm_end - vma->vm_start;
	unsigned long off = vma->vm_pgoff << PAGE_SHIFT;
	unsigned long padr = pci_resource_start(dev->pci_dev, 0) + off;
	unsigned long psize = pci_resource_len(dev->pci_dev, 0) - off;

	if (size > psize)
		return -EINVAL;

	if (io_remap_pfn_range(vma, vma->vm_start, padr >> PAGE_SHIFT, size,
			       vma->vm_page_prot))
		return -EAGAIN;
	return 0;
}


static int write_uart(struct ngene *dev, u8 *data, int len)
{
	struct ngene_command com;

	com.cmd.hdr.Opcode = CMD_WRITE_UART;
	com.cmd.hdr.Length = len;
	memcpy(com.cmd.WriteUart.Data, data, len);
	com.cmd.WriteUart.Data[len] = 0;
	com.cmd.WriteUart.Data[len + 1] = 0;
	com.in_len = len;
	com.out_len = 0;

	if (ngene_command(dev, &com) < 0)
		return -EIO;

	return 0;
}

static int send_cli(struct ngene *dev, char *cmd)
{
	/* printk(KERN_INFO DEVICE_NAME ": %s", cmd); */
	return write_uart(dev, cmd, strlen(cmd));
}

static int send_cli_val(struct ngene *dev, char *cmd, u32 val)
{
	char s[32];

	snprintf(s, 32, "%s %d\n", cmd, val);
	/* printk(KERN_INFO DEVICE_NAME ": %s", s); */
	return write_uart(dev, s, strlen(s));
}

static int ngene_command_write_uart_user(struct ngene *dev,
					 const u8 *data, int len)
{
	struct ngene_command com;

	dev->tx_busy = 1;
	com.cmd.hdr.Opcode = CMD_WRITE_UART;
	com.cmd.hdr.Length = len;

	if (copy_from_user(com.cmd.WriteUart.Data, data, len))
		return -EFAULT;
	com.in_len = len;
	com.out_len = 0;

	if (ngene_command(dev, &com) < 0)
		return -EIO;

	return 0;
}

static ssize_t uart_write(struct file *file, const char *buf,
			  size_t count, loff_t *ppos)
{
	struct dvb_device *dvbdev = file->private_data;
	struct ngene_channel *chan = dvbdev->priv;
	struct ngene *dev = chan->dev;
	int len, ret = 0;
	size_t left = count;

	while (left) {
		len = left;
		if (len > 250)
			len = 250;
		ret = wait_event_interruptible(dev->tx_wq, dev->tx_busy == 0);
		if (ret < 0)
			return ret;
		ngene_command_write_uart_user(dev, buf, len);
		left -= len;
		buf += len;
	}
	return count;
}

static ssize_t ts_write(struct file *file, const char *buf,
			size_t count, loff_t *ppos)
{
	struct dvb_device *dvbdev = file->private_data;
	struct ngene_channel *chan = dvbdev->priv;
	struct ngene *dev = chan->dev;

	if (wait_event_interruptible(dev->tsout_rbuf.queue,
				     dvb_ringbuffer_free
				     (&dev->tsout_rbuf) >= count) < 0)
		return 0;

	dvb_ringbuffer_write(&dev->tsout_rbuf, buf, count);

	return count;
}

static ssize_t uart_read(struct file *file, char *buf,
			 size_t count, loff_t *ppos)
{
	struct dvb_device *dvbdev = file->private_data;
	struct ngene_channel *chan = dvbdev->priv;
	struct ngene *dev = chan->dev;
	int left;
	int wp, rp, avail, len;

	if (!dev->uart_rbuf)
		return -EINVAL;
	if (count > 128)
		count = 128;
	left = count;
	while (left) {
		if (wait_event_interruptible(dev->rx_wq,
					     dev->uart_wp != dev->uart_rp) < 0)
			return -EAGAIN;
		wp = dev->uart_wp;
		rp = dev->uart_rp;
		avail = (wp - rp);

		if (avail < 0)
			avail += UART_RBUF_LEN;
		if (avail > left)
			avail = left;
		if (wp < rp) {
			len = UART_RBUF_LEN - rp;
			if (len > avail)
				len = avail;
			if (copy_to_user(buf, dev->uart_rbuf + rp, len))
				return -EFAULT;
			if (len < avail)
				if (copy_to_user(buf + len, dev->uart_rbuf,
						 avail - len))
					return -EFAULT;
		} else {
			if (copy_to_user(buf, dev->uart_rbuf + rp, avail))
				return -EFAULT;
		}
		dev->uart_rp = (rp + avail) % UART_RBUF_LEN;
		left -= avail;
		buf += avail;
	}
	return count;
}

static const struct file_operations command_fops = {
	.owner   = THIS_MODULE,
	.read    = uart_read,
	.write   = ts_write,
	.ioctl   = command_ioctl,
	.open    = dvb_generic_open,
	.release = dvb_generic_release,
	.poll    = 0,
	.mmap    = ngene_mmap,
};

static struct dvb_device dvbdev_command = {
	.priv    = 0,
	.readers = -1,
	.writers = -1,
	.users   = -1,
	.fops    = &command_fops,
};

#endif

/****************************************************************************/
/* DVB functions and API interface ******************************************/
/****************************************************************************/

static void swap_buffer(u32 *p, u32 len)
{
	while (len) {
		*p = swab32(*p);
		p++;
		len -= 4;
	}
}

void *tsin_exchange(void *priv, void *buf, u32 len, u32 clock, u32 flags)
{
	struct ngene_channel *chan = priv;

#if 0
	printk(KERN_INFO DEVICE_NAME ": tsin %08x %02x %02x %02x %02x\n",
	       len, ((u8 *) buf)[512 * 188], ((u8 *) buf)[0],
	       ((u8 *) buf)[1], ((u8 *) buf)[2]);
#endif

#ifdef COMMAND_TIMEOUT_WORKAROUND
	if (chan->users > 0)
#endif
		dvb_dmx_swfilter(&chan->demux, buf, len);
	return NULL;
}

u8 fill_ts[188] = { 0x47, 0x1f, 0xff, 0x10 };

void *tsout_exchange(void *priv, void *buf, u32 len, u32 clock, u32 flags)
{
	struct ngene_channel *chan = priv;
	struct ngene *dev = chan->dev;
	u32 alen;

	alen = dvb_ringbuffer_avail(&dev->tsout_rbuf);
	alen -= alen % 188;

	if (alen < len)
		FillTSBuffer(buf + alen, len - alen, flags);
	else
		alen = len;
	dvb_ringbuffer_read(&dev->tsout_rbuf, buf, alen);
	if (flags & DF_SWAP32)
		swap_buffer((u32 *)buf, alen);
	wake_up_interruptible(&dev->tsout_rbuf.queue);
	return buf;
}

#if 0
static void set_dto(struct ngene_channel *chan, u32 rate)
{
	u64 val = rate * 0x89705f41ULL; /* times val for 2^26 Hz */

	val = ((val >> 25) + 1) >> 1;
	chan->AudioDTOValue = (u32) val;
	/* chan->AudioDTOUpdated=1; */
	/* printk(KERN_INFO DEVICE_NAME ": Setting DTO to %08x\n", val); */
}
#endif


int ngene_start_feed(struct dvb_demux_feed *dvbdmxfeed)
{
	struct dvb_demux *dvbdmx = dvbdmxfeed->demux;
	struct ngene_channel *chan = dvbdmx->priv;
#if 0
	struct ngene *dev = chan->dev;

	if (dev->card_info->io_type[chan->number] & NGENE_IO_TSOUT) {
		switch (dvbdmxfeed->pes_type) {
		case DMX_TS_PES_VIDEO:
			send_cli_val(dev, "vpid", dvbdmxfeed->pid);
			send_cli(dev, "res 1080i50\n");
			/* send_cli(dev, "vdec mpeg2\n"); */
			break;

		case DMX_TS_PES_AUDIO:
			send_cli_val(dev, "apid", dvbdmxfeed->pid);
			send_cli(dev, "start\n");
			break;

		case DMX_TS_PES_PCR:
			send_cli_val(dev, "pcrpid", dvbdmxfeed->pid);
			break;

		default:
			break;
		}

	}
#endif

	if (chan->users == 0) {
#ifdef COMMAND_TIMEOUT_WORKAROUND
		if (!chan->running)
#endif
			set_transfer(chan, 1);
		/* msleep(10); */
	}

	return ++chan->users;
}

int ngene_stop_feed(struct dvb_demux_feed *dvbdmxfeed)
{
	struct dvb_demux *dvbdmx = dvbdmxfeed->demux;
	struct ngene_channel *chan = dvbdmx->priv;
#if 0
	struct ngene *dev = chan->dev;

	if (dev->card_info->io_type[chan->number] & NGENE_IO_TSOUT) {
		switch (dvbdmxfeed->pes_type) {
		case DMX_TS_PES_VIDEO:
			send_cli(dev, "stop\n");
			break;

		case DMX_TS_PES_AUDIO:
			break;

		case DMX_TS_PES_PCR:
			break;

		default:
			break;
		}

	}
#endif

	if (--chan->users)
		return chan->users;

#ifndef COMMAND_TIMEOUT_WORKAROUND
	set_transfer(chan, 0);
#endif

	return 0;
}

int my_dvb_dmx_ts_card_init(struct dvb_demux *dvbdemux, char *id,
			    int (*start_feed)(struct dvb_demux_feed *),
			    int (*stop_feed)(struct dvb_demux_feed *),
			    void *priv)
{
	dvbdemux->priv = priv;

	dvbdemux->filternum = 256;
	dvbdemux->feednum = 256;
	dvbdemux->start_feed = start_feed;
	dvbdemux->stop_feed = stop_feed;
	dvbdemux->write_to_decoder = NULL;
	dvbdemux->dmx.capabilities = (DMX_TS_FILTERING |
				      DMX_SECTION_FILTERING |
				      DMX_MEMORY_BASED_FILTERING);
	return dvb_dmx_init(dvbdemux);
}

int my_dvb_dmxdev_ts_card_init(struct dmxdev *dmxdev,
			       struct dvb_demux *dvbdemux,
			       struct dmx_frontend *hw_frontend,
			       struct dmx_frontend *mem_frontend,
			       struct dvb_adapter *dvb_adapter)
{
	int ret;

	dmxdev->filternum = 256;
	dmxdev->demux = &dvbdemux->dmx;
	dmxdev->capabilities = 0;
	ret = dvb_dmxdev_init(dmxdev, dvb_adapter);
	if (ret < 0)
		return ret;

	hw_frontend->source = DMX_FRONTEND_0;
	dvbdemux->dmx.add_frontend(&dvbdemux->dmx, hw_frontend);
	mem_frontend->source = DMX_MEMORY_FE;
	dvbdemux->dmx.add_frontend(&dvbdemux->dmx, mem_frontend);
	return dvbdemux->dmx.connect_frontend(&dvbdemux->dmx, hw_frontend);
}
