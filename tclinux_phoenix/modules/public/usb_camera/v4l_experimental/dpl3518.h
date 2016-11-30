/*
 * $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/modules/public/usb_camera/v4l_experimental/dpl3518.h#1 $
 */

#ifndef DPL3518_H
#define DPL3518_H

#include <linux/ioctl.h>

#define DPL_VERSION_CODE 0x0002        /* Version 0.2 */


/* IOCTL numbers */

/* Reset function (just in case...) */
#define DPL_IOC_RESET		_IO('d',1)

/* Get driver version */
#define DPL_IOC_VERSION		_IOR('d',2,int)

/* Dolby Pro Logic modes */
#define DPL_IOC_INIT		_IO('d',3)	/* Must be done first */

#define DPL_IOC_PROLOGIC_INIT	_IO('d',4)
#define DPL_IOC_NOISE_INIT	_IO('d',5)
#define DPL_IOC_THROUGH_INIT	_IO('d',6)

#define DPL_IOC_MODE		_IOW('d',7,int)	/* 0 = Normal */
					/* 1 = Phantom */
					/* 2 = Wide */
					/* 3 = Dolby 3 Stereo */
					/* 4 = Center Off */
					/* 5 = Panorama */
					/* 6 = 3D-Panorama (virtual dolby) */
#define DPL_IOC_GET_MODE 	_IOR('d',7,int)

#define DPL_IOC_NOISE_MODE	_IOW('d',8,int)
#define DPL_IOC_GET_NOISE_MODE	_IOR('d',8,int)

#define DPL_IOC_DELAY		_IOW('d',9,int)        /* 0 .. 15 */
#define DPL_IOC_REVERB		_IOW('d',10,int)       /* 0 .. 100 % */
#define DPL_IOC_PANORAMA	_IOW('d',11,int)       /* 0 .. 100 % */
#define DPL_IOC_SPATIAL		_IOW('d',12,int)       /* 0 .. 100 % */


/* ---------------------------------------------------------------------- */

#endif /* DPL3518_H */
