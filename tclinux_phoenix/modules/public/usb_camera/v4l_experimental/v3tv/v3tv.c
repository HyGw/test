
#define __NO_VERSION__
#include <linux/config.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <linux/i2c-algo-bit.h>
#include <linux/videodev.h>
#include <linux/video_decoder.h>
#include <linux/spinlock.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <asm/io.h>
#include <asm/pgtable.h>

#include <media/tuner.h>
#include <media/audiochip.h>

//#ifndef I2C_DRIVERID_VPX3224
//#include "vpx-common.h"
//#endif


//#include <linux/i2c.h>


#define UNSET (-1U)

#define V3_VERSION "0.1-Multistandard for V4L2"

/* i2c defines */
/* bit locations in the register */
#define DDC_ENAB	0x00040000
#define DDC_SCL_OUT	0x00080000
#define DDC_SDA_OUT	0x00100000
#define DDC_SCL_IN	0x00200000
#define DDC_SDA_IN	0x00400000
#define I2C_ENAB	0x00800000
#define I2C_SCL_OUT	0x01000000
#define I2C_SDA_OUT	0x02000000
#define I2C_SCL_IN	0x04000000
#define I2C_SDA_IN	0x08000000

/* initialization states */
#define I_i2c	0x2		/* i2c-interface initialized */
#define I_ddc	0x4		/* ddc-interface initialized */
#define I_v4l2	0x8		/* v4l devices registered    */
#define I_irq	0x10		/* irq active */

/* delays */
#define CYCLE_DELAY	10
#define TIMEOUT		50

/* the only registers we use for i2c control*/
#define REG	0x78
#define REG2 	0x70

/* Voodoo avenger register offsets */
#define V3REG_vidTvOutBlankVCount 		0x3c	/* 31:0 R/W TV Out Vertical Active Start/End */
#define V3REG_vidMaxRgbDelta 			0x58	/* 23:0 R/W Maximum delta values for video filtering */
#define V3REG_vidProcCfg 			0x5c	/* 31:0 R/W Video Processor configuration */
#define V3REG_hwCurPatAddr 			0x60	/* 23:0 R/W Cursor Pattern Address */
#define V3REG_hwCurLoc 				0x64	/* 26:0 R/W X and Y location of HW cursor */
#define V3REG_hwCurC0 				0x68	/* 23:0 R/W Hw cursor color 0 */
#define V3REG_hwCurC1 				0x6c	/* 23:0 R/W Hw cursor color 1 */
#define V3REG_vidInFormat 			0x70	/* 31:0 R/W Video In Format */
#define V3REG_vidTvOutBlankHCount 		0x74	/* 31:0 R/W TV Out Horizontal Active Start/End */
#define V3REG_vidSerialParallelPort 		0x78	/* 31:0 R/W Serial and Parallel Ports */
#define V3REG_vidInXDecimDeltas 		0x7c	/* 31:0 R/W Video In horizontal decimation delta 1 & 2. */
#define V3REG_vidInDecimInitErrs 		0x80	/* 31:0 R/W Video In horizontal and vertical decimation initial error term */
#define V3REG_vidInYDecimDeltas 		0x84	/* 31:0 R/W Video In vertical decimation delta 1 & 2 */
#define V3REG_vidPixelBufThold 			0x88	/* 17:0 R/W Video Pixel Buffer Threshold */
#define V3REG_vidChromaMin 			0x8c	/* 31:0 R/W Chroma Key minimum value */
#define V3REG_vidChromaMax 			0x90	/* 31:0 R/W Chroma Key maximum value */
#define V3REG_vidInStatusCurrentLine 		0x94	/* 18:0 R Video In Status and Current Scan line */
#define V3REG_vidScreenSize 			0x98	/* 23:0 R/W Screen resolution */
#define V3REG_vidOverlayStartCoords 		0x9c	/* 31:0 R/W Start Surface Coordinates [31:28] Overlay Start Screen Coordinates */
#define V3REG_vidOverlayEndScreenCoord 		0xa0	/* 23:0 R/W Overlay End Screen Coordinates */
#define V3REG_vidOverlayDudx 			0xa4	/* 19:0 R/W Overlay horizontal magnification factor */
#define V3REG_vidOverlayDudxOffsetSrcWidth 	0xa8	/* 31:0 R/W Overlay horizontal magnification factor initial offset (bit 18:0) Overlay source surface width (bit 31:19) */
#define V3REG_vidOverlayDvdy 			0xac	/* 19:0 R/W Overlay vertical magnification factor */
#define V3REG_vidOverlayDvdyOffset 		0xe0	/* 18:0 R/W Overlay vertical magnification factor initial offset */
#define V3REG_vidDesktopStartAddr 		0xe4	/* 23:0 R/W Desktop start address */
#define V3REG_vidDesktopOverlayStride 		0xe8	/* 31:0 R/W Desktop and Overlay strides (linear or tile) */
#define V3REG_vidInAddr0 			0xec	/* 23:0 R/W Video In Buffer 0 starting address */
#define V3REG_vidInAddr1 			0xf0	/* 23:0 R/W Video In Buffer 1 starting address */
#define V3REG_vidInAddr2 			0xf4	/* 23:0 R/W Video In Buffer 2 starting address */
#define V3REG_vidInStride 			0xf8	/* 14:0 R/W Video In Buffer stride (linear or tile) */
#define V3REG_vidCurrOverlayStartAddr 		0xfc	/* 23:0 R Current overlay start address in use */

/* 2D registers */
#define V3REG_IO_BASE_1	0x100000

#define V3REG_status 		V3REG_IO_BASE_1 + 0x000	/* 31:0 R Avenger status register */
#define V3REG_intrCtrl		V3REG_IO_BASE_1 + 0x004	/* 31:0 R/W Interrupt control and status */
#define V3REG_clip0Min 		V3REG_IO_BASE_1 + 0x008	/* 28:0 R/W Min X & Y clip values when clip select is 0 */
#define V3REG_clip0Max 		V3REG_IO_BASE_1 + 0x00c	/* 28:0 R/W Max X & Y clip values when clip select is 0 */
#define V3REG_dstBaseAddr 	V3REG_IO_BASE_1 + 0x010	/* 23:0 R/W Destination base address */
#define V3REG_dstFormat 	V3REG_IO_BASE_1 + 0x014	/* 17:0 R/W Destination stride and bits per pixel */
#define V3REG_srcColorkeyMin 	V3REG_IO_BASE_1 + 0x018	/* 23:0 R/W Source Colorkey range (min) */
#define V3REG_srcColorkeyMax 	V3REG_IO_BASE_1 + 0x01c	/* 23:0 R/W Source Colorkey range (max) */
#define V3REG_dstColorkeyMin 	V3REG_IO_BASE_1 + 0x020	/* 23:0 R/W Destination Colorkey range (min) */
#define V3REG_dstColorkeyMax 	V3REG_IO_BASE_1 + 0x024	/* 23:0 R/W Destination Colorkey range (max) */
#define V3REG_bresError0 	V3REG_IO_BASE_1 + 0x028	/* 31:0 R/W Initial error for lines, right edges & stretch blt x */
#define V3REG_bresError1 	V3REG_IO_BASE_1 + 0x02c	/* 31:0 R/W Initial error for left poly edges & stretch blt y */
#define V3REG_rop 		V3REG_IO_BASE_1 + 0x030	/* 31:0 R/W 4 Ternary Raster operations */
#define V3REG_srcBaseAddr 	V3REG_IO_BASE_1 + 0x034	/* 23:0 R/W Source base address */
#define V3REG_commandExtra 	V3REG_IO_BASE_1 + 0x038	/* 31:0 R/W Extra control bits */
#define V3REG_lineStipple 	V3REG_IO_BASE_1 + 0x03c	/* 31:0 R/W Monochrome pattern for lines */
#define V3REG_lineStyle 	V3REG_IO_BASE_1 + 0x040	/* 28:0 R/W Style register for lines */
#define V3REG_pattern0Alias 	V3REG_IO_BASE_1 + 0x044	/* 31:0 R/W Alias to colorPattern(0) */
#define V3REG_pattern1Alias 	V3REG_IO_BASE_1 + 0x048	/* 31:0 R/W Alias to colorPattern(1) */
#define V3REG_clip1Min 		V3REG_IO_BASE_1 + 0x04c	/* 28:0 R/W Min X & Y clip values when clip select is 1 */
#define V3REG_clip1Max 		V3REG_IO_BASE_1 + 0x050	/* 28:0 R/W Max X & Y clip values when clip select is 1 */
#define V3REG_srcFormat 	V3REG_IO_BASE_1 + 0x054	/* 18:0 R/W Source stride and bits per pixel */
#define V3REG_srcSize 		V3REG_IO_BASE_1 + 0x058	/* 28:0 R/W Height and width of source for stretch blts */
#define V3REG_srcXY 		V3REG_IO_BASE_1 + 0x05c	/* 28:0 R/W Starting pixel of blt source data */
#define V3REG_colorBack 	V3REG_IO_BASE_1 + 0x060	/* 31:0 R/W Background color */
#define V3REG_colorFore 	V3REG_IO_BASE_1 + 0x064	/* 31:0 R/W Foreground color */
#define V3REG_dstSize 		V3REG_IO_BASE_1 + 0x068	/* 28:0 R/W Destination width and height for blts and rectangle fills */
#define V3REG_dstXY 		V3REG_IO_BASE_1 + 0x06c	/* 28:0 R/W Starting X and Y of destination for blts */
#define V3REG_command 		V3REG_IO_BASE_1 + 0x070	/* 31:0 R/W 2D command mode & control bits */
#define V3REG_launchArea 	V3REG_IO_BASE_1 + 0x080	/* 31:0 R Initiates 2D commands */

/* 3D registers */
#define V3REG_IO_BASE_2	0x200000

#define V3REG_swapbufferCMD	V3REG_IO_BASE_2 + 0x128	/*8:0 FBI W Yes/Yes Execute SWAPBUFFER command  */
#define V3REG_swapPending	V3REG_IO_BASE_2 + 0x24C	/*na FBI W No / No Swap buffer pending          */
#define V3REG_leftOverlayBuf 	V3REG_IO_BASE_2 + 0x250	/*31:0 FBI W No / Yes Left Overlay address      */
#define V3REG_rightOverlayBuf 	V3REG_IO_BASE_2 + 0x254	/*31:0 FBI W No / Yes Right Overlay address     */

/* External variables: Data struct */
struct voodoo_data *voodoo;

/* Global data struct.
 * We can have only a single voodoo3 3500 TV card anyway,
 * because there aren't any dual AGP slot motherboards, right?
 */
struct voodoo_data *voodoo = NULL;

/* Module parameters */
#ifdef INCLUDE_I2C
int ddc = 1;
#endif
int debug = 0;
int model = 0;
int video_nr = -1;
int vbi_nr = -1;
int radio_nr = -1;




/*
 * Many declarations for v3tv drivers.
 *
 * $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/modules/public/usb_camera/v4l_experimental/v3tv/v3tv.c#1 $
 */

/* 3DFX defines */
#ifndef PCI_DEVICE_ID_3DFX_VOODOO3
#define PCI_DEVICE_ID_3DFX_VOODOO3 0x05
#endif

#ifndef VID_HARDWARE_V3TV
#define VID_HARDWARE_V3TV_RADIO 0x49
#define VID_HARDWARE_V3TV	0x50
#endif

/* Different versions of the Voodoo3 3500 TV */
#define VOODOO3_MODEL_NTSC  0x60
#define VOODOO3_MODEL_PAL   0x61
#define VOODOO3_MODEL_SECAM 0x62

#define V3_MAX_I2C_CLIENTS	10

#define VOODOO3_2D_SAVE 0
#define VOODOO3_2D_RESTORE 1

struct voodoo_data {
	/* locking */
//	spinlock_t s_lock;
//	struct semaphore lock;

	struct pci_dev *dev;

	unsigned char *io_base;	/* memory mapped registers */
	unsigned char *fb_base;	/* frame buffer memory     */
#ifdef INCLUDE_I2C
	struct i2c_client *i2c_clients[V3_MAX_I2C_CLIENTS];
#else
	struct i2c_adapter *voodoo_i2c_adapter;
#endif
	struct video_picture picture_settings;
	struct video_window capture_win;
	struct video_buffer fb;

	struct video_device *video_dev;
	struct video_device *radio_dev;
	struct video_device *vbi_dev;

	int buffering;		/* 1 = single, 2=double, 3=triple */

	u32 vidInAddr0;
	u32 vidInAddr1;
	u32 vidInAddr2;
	u32 captureBuf;
	u32 desktopAddr;

	int gbuffers;
	int gbufsize;
	unsigned char *fbuffer;

	int vidOverlayStartX;
	int vidOverlayStartY;
	int vidOverlayOffsetX;
	int vidOverlayOffsetY;

	int vidOverlayEndX;
	int vidOverlayEndY;

	int vidOrigWidth;
	int vidOrigHeight;

	int vidInStride;
	int vidOverlayStride;
	int vidScanlineBytes;

	int desktopStride;
	int desktopWidth;
	int desktopHeight;

	int overlayWidth, overlayHeight;
	int dudx;
	int dvdy;
	int XDecim;
	int YDecim;

	char name[32];
	int model;
	int norm;
	int type;
	int input;
	int tuner_type;
	int channel;
	unsigned long freq;
	unsigned long chroma;

	int initialized;
	int decoder_enabled;
};

struct conf_struct {
	unsigned int reg;
	int value;
	unsigned char endbit, startbit;
};

#define CONF_SIZE(a) (sizeof(a) / sizeof (struct conf_struct))

/* FIXME:
 * The chromakey register has different formats, depending on the desktop color bits.
 * This means, that the chromakey will be interpreted differently in different video modes.
 */

// static struct conf_struct Chromakey_Disable[] =
// {
//      { V3REG_vidProcCfg, 0, 5, 5}
// };

static struct conf_struct voodoo3_init_reg[] = {
	{V3REG_vidInFormat, -1, 0, 0},	/* Reserved - leave defaults                 */
	{V3REG_vidInFormat, 6, 3, 1},	/* 110 = 8bit YCbCr 4:2:2 UYVY               */
	{V3REG_vidInFormat, 0, 4, 4},	/* No De-interlacing                         */
	{V3REG_vidInFormat, -1, 7, 5},	/* Polarities - leave defaults               */
	{V3REG_vidInFormat, 1, 8, 8},	/* Brooktree tv-out                          */
	{V3REG_vidInFormat, 2, 10, 9},	/* VideoIn triple buffering (FIXME:BUGGY???) */
	{V3REG_vidInFormat, 0, 11, 11},	/* VideoInBuffer linear space                */
	{V3REG_vidInFormat, -1, 13, 12},	/* polarities - leave defaults               */
	{V3REG_vidInFormat, -1, 14, 14},	/* VMI-interface enable (not yet)            */
	{V3REG_vidInFormat, 1, 15, 15},	/* TV-Out-interface enable                   */
	{V3REG_vidInFormat, 0, 16, 16},	/* Genlock disable(VMI-slave)                */
	{V3REG_vidInFormat, 0, 17, 17},	/* Use vga timing                            */
	{V3REG_vidInFormat, -1, 18, 18},	/* Genlock source - with bit16=0 no effect   */
	{V3REG_vidInFormat, -1, 19, 19},	/* Use display_ena for driving tv-out        */
	{V3REG_vidInFormat, 0, 20, 20},	/* VideoIn Horizontal Decimation disable     */
	{V3REG_vidInFormat, 0, 21, 21},	/* VideoIn Vertical Decimation disable       */
	{V3REG_vidInFormat, -1, 31, 22},	/* Reserved - leave defaults                 */
	{V3REG_vidSerialParallelPort, 0, 0, 0},	/* VMI parallel port disable                 */
	{V3REG_vidSerialParallelPort, -1, 1, 1},	/* VMI Chip Select (default)                 */
	{V3REG_vidSerialParallelPort, -1, 4, 2},	/* VMI send/rcv data ctrl - leave defaults   */
	{V3REG_vidSerialParallelPort, 0, 5, 5},	/* VMI data output disable                   */
	{V3REG_vidSerialParallelPort, -1, 13, 6},	/* VMI send/rcv data - leave defaults        */
	{V3REG_vidSerialParallelPort, -1, 17, 14},	/* VMI send/rcv addrs - leave defaults       */
#ifdef INCLUDE_I2C
	{V3REG_vidSerialParallelPort, 1, 22, 18},	/* Activate ddc-interface(i2c for monitor)   */
	{V3REG_vidSerialParallelPort, 1, 27, 23},	/* Activate i2c-interface(i2c on card)       */
#else
	{V3REG_vidSerialParallelPort, -1, 22, 18},	/* Activate ddc-interface(i2c for monitor)   */
	{V3REG_vidSerialParallelPort, -1, 27, 23},	/* Activate i2c-interface(i2c on card)       */
#endif
	{V3REG_vidSerialParallelPort, 1, 28, 28},	/* VMI normal operation                      */
	{V3REG_vidSerialParallelPort, 1, 29, 29},	/* GPIO[1] - VMI sync_oe                     */
	{V3REG_vidSerialParallelPort, -1, 30, 30},	/* GPIO[2] - Input only                      */
	{V3REG_vidSerialParallelPort, 1, 31, 31},	/* TV out normal operation                   */
};

static struct conf_struct Chromakey_Enable[] =
	{ {V3REG_vidProcCfg, 1, 5, 5} };
static struct conf_struct Horizontal_Decimation_Enable[] =
	{ {V3REG_vidInFormat, 1, 20, 20} };	/* Horizontal Decimation enable */
static struct conf_struct Horizontal_Decimation_Disable[] =
	{ {V3REG_vidInFormat, 0, 20, 20} };	/* Horizontal Decimation disable */
static struct conf_struct Vertical_Decimation_Enable[] =
	{ {V3REG_vidInFormat, 1, 21, 21} };	/* Vertical Decimation enable */
static struct conf_struct Vertical_Decimation_Disable[] =
	{ {V3REG_vidInFormat, 0, 21, 21} };	/* Vertical Decimation enable */
static struct conf_struct Horizontal_Magnification_Enable[] =
	{ {V3REG_vidProcCfg, 1, 14, 14} };	/* Horizontal Scaling enable */
static struct conf_struct Horizontal_Magnification_Disable[] =
	{ {V3REG_vidProcCfg, 0, 14, 14} };	/* Horizontal Scaling disable */
static struct conf_struct Vertical_Magnification_Enable[] =
	{ {V3REG_vidProcCfg, 1, 15, 15} };	/* Vertical Scaling enable */
static struct conf_struct Vertical_Magnification_Disable[] =
	{ {V3REG_vidProcCfg, 0, 15, 15} };	/* Vertical Scaling disable */

static struct conf_struct Overlay_Disable[] =
	{ {V3REG_vidProcCfg, 0, 8, 8},
	{V3REG_vidProcCfg, 0, 9, 9} };
static struct conf_struct Overlay_Enable[] = {
	{V3REG_vidProcCfg, -1, 5, 5},	/* Disable Chroma Key                   */
	{V3REG_vidProcCfg, 1, 8, 8},	/* Overlay surface enable               */
	{V3REG_vidProcCfg, 1, 9, 9},	/* Use vidInAddr as overlay buffer      */
	{V3REG_vidProcCfg, 1, 11, 11},	/* Overlay ramdac-clut bypass           */
	{V3REG_vidProcCfg, -1, 13, 13},	/* Overlay clut don't care              */
	{V3REG_vidProcCfg, -1, 14, 14},	/* overlay horizontal scaling           */
	{V3REG_vidProcCfg, -1, 15, 15},	/* overlay vertical scaling             */
	{V3REG_vidProcCfg, 3, 17, 16},	/* overlay filter mode                  */
	{V3REG_vidProcCfg, 6, 23, 21},	/* overlay pixel format (422 UYVY)      */
	{V3REG_vidProcCfg, 0, 25, 25},	/* overlay tile space (0=linear)        */
	{V3REG_vidProcCfg, -1, 30, 30},	/* Reserved                             */
	{V3REG_vidProcCfg, 0, 31, 31},	/* No overlay backend de-interlacing    */
	{V3REG_vidInFormat, 1, 14, 14},	/* VMI interface enable                 */
	{V3REG_vidSerialParallelPort, 1, 0, 0} };	/* VMI host interface enable	*/
static struct conf_struct Overlay_Filter_Off[] = {
	{V3REG_vidProcCfg, 0, 17, 16} };	/* overlay filter OFF */
static struct conf_struct Overlay_Filter_On[] = {
	{V3REG_vidProcCfg, 3, 17, 16} };	/* overlay filter ON */

static struct conf_struct Disable_VMI_Interrupt[] = {
	{V3REG_intrCtrl, 0, 21, 21}, /* VMI Interrupt disable */
	{V3REG_intrCtrl, 0, 23, 23}, /* clear VMI Interrupt generated */
	{V3REG_intrCtrl, 1, 31, 31} }; /* clear PCI interrupt active */

static struct conf_struct VMI_Enable[] = {
	{V3REG_vidInFormat, 1, 14, 14},			/* VMI interafce enable */
	{V3REG_vidSerialParallelPort, 1, 0, 0} };	/* VMI parallel host interface enable */

static struct conf_struct VMI_Disable[] = {
	{V3REG_vidInFormat, 0, 14, 14},			/* VMI interafce disable */
	{V3REG_vidSerialParallelPort, 0, 0, 0} };	/* VMI parallel host interface disable */

//	static struct conf_struct VMI_Disable[] = {
//		{V3REG_vidInFormat, 0, 14, 14},
//		{V3REG_vidSerialParallelPort, 0, 0, 0} };

#if 0
static struct conf_struct Enable_VMI_Interrupt[] = {
	{V3REG_intrCtrl, 1, 21, 21},
	{V3REG_intrCtrl, 0, 23, 23} };
static struct conf_struct Disable_VMI_Interrupt[] = {
	{V3REG_intrCtrl, 0, 21, 21},
	{V3REG_intrCtrl, 0, 23, 23} };
static struct conf_struct Clear_VMI_Interrupt[] = {
	{V3REG_intrCtrl, 0, 23, 23} };
static struct conf_struct Overlay_Setup[] = {
	{V3REG_vidProcCfg, 1, 8, 8},
	{V3REG_vidProcCfg, 1, 9, 9} };
#endif


/* C function prototypes */
void v3tv_call_i2c_client(unsigned int DriverID, unsigned int cmd,
			     void *arg);
int v3tv_i2c_setup(int arg);
// static int v3tv_probe(void);
// int v3tv_init_module(void);
// void v3tv_cleanup_module(void);

// int v3tv_attach_adapter(struct i2c_adapter *adapter);
// int v3tv_detach_client(struct i2c_client *client);

void v3tv_set_chromakey(struct voodoo_data *voodoo);
// void v3tv_calc_scale(struct voodoo_data *voodoo)
// long do_fraction(int numBits, unsigned a, unsigned b);
// void v3tv_update_overlay_setup (struct voodoo_data *voodoo);
void v3tv_set_overlay(struct voodoo_data *voodoo);
void v3tv_overlay_setup(struct voodoo_data *voodoo);
void v3tv_overlay(struct voodoo_data *voodoo, int i);
// int v3tv_MakeRoom(int num);
// void v3tv_2dreg(int i);
void v3tv_capture_frame(struct voodoo_data *voodoo_tv);

// static int v3tv_radio_open (struct video_device *dev, int mode);
// static void v3tv_radio_close (struct video_device *dev);
// static int v3tv_radio_ioctl (struct video_device *dev, unsigned int cmd, void *arg);
// static irqreturn_t v3tv_voodoo_irq(int irq, void *dev_id, struct pt_regs *regs);
void v3tv_set_palette(struct video_picture *picture_settings);
// static int v3tv_voodoo_open(struct video_device *dev,int flags);
void v3tv_voodoo_close(struct video_device *dev);
int v3tv_voodoo_mmap(struct file *file, struct vm_area_struct *vma);
// static long v3tv_voodoo_read(struct video_device *dev, char *buf, unsigned long count, int nonblock);
// static int v3tv_voodoo_ioctl(struct video_device *dev, unsigned int cmd, void *arg);
int v3tv_video_init(struct voodoo_data *voodoo);
void v3tv_video_cleanup(struct voodoo_data *voodoo);

// inline void v3tv_regwrite(unsigned int reg, unsigned int data);
// inline unsigned int v3tv_regread(unsigned int reg);
// static inline unsigned int do_mask(struct conf_struct *conf);
// static inline unsigned int do_value(unsigned int orig, struct conf_struct *conf);
void v3tv_conf(struct conf_struct *conf, int num_entries);
// static inline unsigned long uvirt_to_kva(pgd_t *pgd, unsigned long adr);
// static inline unsigned long uvirt_to_bus(unsigned long adr);
// static inline unsigned long kvirt_to_bus(unsigned long adr);
// inline unsigned long kvirt_to_pa(unsigned long adr);
// void *rvmalloc(signed long size);
// void rvfree(void * mem, signed long size);



/*
 * Here's some pretty complicated stuff...
 * We probably need to start inserting spinlock-stuff here,
 * or in the functions that call these ones.
 *
 * $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/modules/public/usb_camera/v4l_experimental/v3tv/v3tv.c#1 $
 */

inline void v3tv_regwrite(unsigned int reg, unsigned int data)
{
	if (!voodoo || !voodoo->io_base)
		return;

	*((unsigned int *)(voodoo->io_base + reg)) = data;
}

/*
inline static void voodoo3_regor(unsigned int reg, unsigned int data, unsigned int mask)
{
	unsigned int *reg_ptr;

	if (!voodoo || !voodoo->io_base)
		return;

	reg_ptr = (unsigned int *) voodoo->io_base + reg;
	*reg_ptr = (*reg_ptr & ~mask) | (data & mask);
}
*/

inline unsigned int v3tv_regread(unsigned int reg)
{
	if (!voodoo || !voodoo->io_base)
		return 0;

	return *((unsigned int *)(voodoo->io_base + reg));
}

/*******************************/
/* Memory management functions */
/*	these are from bttv    */
/*******************************/

#define MDEBUG(x)	do { } while(0)	/* Debug memory management */

/* [DaveM] I've recoded most of this so that:
 * 1) It's easier to tell what is happening
 * 2) It's more portable, especially for translating things
 *    out of vmalloc mapped areas in the kernel.
 * 3) Less unnecessary translations happen.
 *
 * The code used to assume that the kernel vmalloc mappings
 * existed in the page tables of every process, this is simply
 * not guarenteed.  We now use pgd_offset_k which is the
 * defined way to get at the kernel page tables.
 */

/* Given PGD from the address space's page table, return the kernel
 * virtual mapping of the physical memory mapped at ADR.
 */
#if 0
static inline unsigned long uvirt_to_kva(pgd_t * pgd, unsigned long adr)
{
	unsigned long ret = 0UL;
	pmd_t *pmd;
	pte_t *ptep, pte;

	if (!pgd_none(*pgd)) {
		pmd = pmd_offset(pgd, adr);
		if (!pmd_none(*pmd)) {
			ptep = pte_offset_map(pmd, adr);
			pte = *ptep;
			if (pte_present(pte)) {
				ret =
				    (unsigned long)page_address(pte_page(pte));
				ret |= (adr & (PAGE_SIZE - 1));
			}
		}
	}
	MDEBUG(printk("v3tv: uv2kva(%lx-->%lx)", adr, ret));
	return ret;
}
#endif

static inline unsigned long uvirt_to_kva(pgd_t * pgd, unsigned long adr)
{
	unsigned long ret = 0UL;
	pmd_t *pmd;
	pte_t *ptep, pte;

	if (!pgd_none(*pgd)) {
		pmd = pmd_offset(pgd, adr);
		if (!pmd_none(*pmd)) {
#if defined (pte_offset_map)	/* 2.4-rmap or 2.5-2.6 */
			ptep = pte_offset_map(pmd, adr);
			pte = *ptep;
			pte_unmap(ptep);
#else
#if defined (pte_offset_atomic)	/* 2.4-aa */
			ptep = pte_offset_atomic(pmd, adr);
			pte = *ptep;
			pte_kunmap(ptep);
#else
			ptep = NULL;
			pte = *pte_offset(pmd, adr);
#endif
#endif
			if (pte_present(pte)) {
				ret =
				    (unsigned long)page_address(pte_page(pte));
				ret |= (adr & (PAGE_SIZE - 1));
			}
		}
	}
	MDEBUG(printk("v3tv: uv2kva(%lx-->%lx)", adr, ret));
	return ret;
}

/*
static inline unsigned long uvirt_to_bus(unsigned long adr)
{
	unsigned long kva, ret;

	kva = uvirt_to_kva(pgd_offset(current->mm, adr), adr);
	ret = virt_to_bus((void *)kva);
	MDEBUG(printk("v3tv: uv2b(%lx-->%lx)", adr, ret));
	return ret;
}

static inline unsigned long kvirt_to_bus(unsigned long adr)
{
	unsigned long va, kva, ret;

	va = ((unsigned long) (adr));
	kva = uvirt_to_kva(pgd_offset_k(va), va);
	ret = virt_to_bus((void *)kva);
	MDEBUG(printk("v3tv: kv2b(%lx-->%lx)", adr, ret));
	return ret;
}
*/

/* Here we want the physical address of the memory.
 * This is used when initializing the contents of the
 * area and marking the pages as reserved.
 */
inline unsigned long kvirt_to_pa(unsigned long adr)
{
	unsigned long va, kva, ret;

	va = ((unsigned long) (adr));
	kva = uvirt_to_kva(pgd_offset_k(va), va);
	ret = __pa(kva);
	MDEBUG(printk("v3tv: kv2pa(%lx-->%lx)", adr, ret));
	return ret;
}

#define mem_map_reserve(p)      set_bit(PG_reserved, &((p)->flags))
#define mem_map_unreserve(p)    clear_bit(PG_reserved, &((p)->flags))

void inline *rvmalloc(signed long size)
{
	void *mem;
	unsigned long adr, page;

	mem = vmalloc(size);
	if (mem) {
		memset(mem, 0, size);	/* Clear the ram out, no junk to the user */
		adr = (unsigned long)mem;
		while (size > 0) {
			page = kvirt_to_pa(adr);
			mem_map_reserve(virt_to_page(__va(page)));
			adr += PAGE_SIZE;
			size -= PAGE_SIZE;
		}
	}
	return mem;
}

/*
void inline rvfree(void *mem, signed long size)
{
	unsigned long adr, page;

	if (mem) {
		adr = (unsigned long)mem;
		while (size > 0) {
			page = kvirt_to_pa(adr);
			mem_map_unreserve(virt_to_page(__va(page)));
			adr += PAGE_SIZE;
			size -= PAGE_SIZE;
		}
		vfree(mem);
	}
}
*/

static inline unsigned int do_mask(struct conf_struct *conf)
{
	unsigned int mask = 0;
	int i;
	for (i = conf->startbit; i <= conf->endbit; i++)
		mask |= (1 << i);

	return mask;
}

static inline unsigned int do_value(unsigned int orig, struct conf_struct *conf)
{
	unsigned int mask = 0;

	mask = do_mask(conf);

	return ((conf->value << conf->startbit) & mask) | (orig & (~mask));
}

void v3tv_conf(struct conf_struct *conf, int num_entries)
{
	int i;
	int lastReg = 0;
	unsigned int cur_value = 0;
	spinlock_t v3conf_lock = SPIN_LOCK_UNLOCKED;

	if (num_entries < 1) {
		printk("v3tv: Empty config data in voodoo3_conf()\n");
		return;
	}

	lastReg = conf[0].reg;
	cur_value = v3tv_regread(lastReg);

	if (debug >= 2)
		printk("v3tv: register 0x%x: 0x%x", lastReg, cur_value);

	spin_lock(&v3conf_lock);

	for (i = 0; i < num_entries; i++) {
		if (lastReg != conf[i].reg) {
			/* Write Register with mask */
			v3tv_regwrite(lastReg, cur_value);
			if (debug >= 2)
				printk(" -> 0x%x\n", cur_value);

			/* Set lastreg to point to current reg */
			lastReg = conf[i].reg;
			cur_value = v3tv_regread(lastReg);
			if (debug >= 2)
				printk("v3tv: register 0x%x: 0x%x",
				       lastReg, cur_value);
		}

		if (conf[i].value != -1) {
			cur_value = do_value(cur_value, &conf[i]);
		}
	}

	v3tv_regwrite(lastReg, cur_value);

	spin_unlock(&v3conf_lock);

	if (debug >= 2)
		printk(" -> 0x%x\n", cur_value);
}


/*
 * voodoo-i2c.c - Part of v4l voodoo3 drivers
 * Copyright (c) 2000  Juha Valkama <jusva@yahoo.com>
 *
 * Based on work by (at least) the following people:
 * Frodo Looijaard <frodol@dds.nl>
 * Philip Edelbrock <phil@netroedge.com>
 * Ralph Metzler <rjkm@thp.uni-koeln.de>
 * Mark D. Studebaker <mdsxyz123@yahoo.com>
 *
 * $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/modules/public/usb_camera/v4l_experimental/v3tv/v3tv.c#1 $
 */

int v3tv_get_adapter(struct i2c_adapter *adapter)
{
	if (debug)
		printk(KERN_INFO "v3tv: v3tv_get_adapter trying: %s\n",
			adapter->name);
	if (!(adapter->class & I2C_CLASS_TV_ANALOG))
		return 0;
	if (debug)
		printk(KERN_INFO "v3tv: i2c_attach_client found adapter: %s\n",
			adapter->name);

	if (voodoo->voodoo_i2c_adapter == adapter) {
		if (debug)
			printk(KERN_INFO "v3tv: i2c_attach_client releasing adapter: %s\n",
			       adapter->name);
		module_put(voodoo->voodoo_i2c_adapter->owner);
		voodoo->voodoo_i2c_adapter = NULL;
		return 0;
	}

	voodoo->voodoo_i2c_adapter = adapter;
	if (!try_module_get(adapter->owner)) {
		printk("v3tv: Unable to get module %s\n",
			       adapter->owner->name);
		return -ENODEV;
	}
	printk ("v3tv: i2c dummy driver registered: adapter: 0x%p\n",
		voodoo->voodoo_i2c_adapter);

	return 0;
}

struct i2c_driver voodoo_i2c_driver = {
	.owner		= THIS_MODULE,
	.name		= "v3tv i2c dummy driver",
	.id		= 0,
/* FIXME: the 2.6 kernel does not have _DUMMY, use .detach_adapter */
#ifdef I2C_DF_DUMMY
	.flags		= I2C_DF_DUMMY,
#else
	.flags		= I2C_DF_NOTIFY,
	.detach_adapter	= v3tv_get_adapter,
#endif
	.attach_adapter = v3tv_get_adapter,
	.detach_client = NULL,
	.command = NULL,
};

int v3tv_i2c_setup(int arg)
{
	if (arg) {		/* get i2c adapter pointer */
		i2c_add_driver(&voodoo_i2c_driver);
		if (NULL == voodoo->voodoo_i2c_adapter) {
			printk
			    ("v3tv: i2c registration failed, module not inserted\n");
			return -ENODEV;
		}
	} else {		/* release adapter */
		if (voodoo->voodoo_i2c_adapter) {
			i2c_del_driver(&voodoo_i2c_driver);
		}
	}
	return 0;
}

/*
 * Overrides for Emacs so that we follow Linus's tabbing style.
 * ---------------------------------------------------------------------------
 * Local variables:
 * c-basic-offset: 8
 * End:
 */
/*
 * voodoo-overlay.c
 *
 * Video overlay handling routines for Voodoo3 3500 TV.
 *
 * $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/modules/public/usb_camera/v4l_experimental/v3tv/v3tv.c#1 $
 */

struct video_capture video_standard[] = {
/*         x,    y,   width, height, decimation, flags */
	{0, 0, 768, 288, 0, 0},	/* PAL */
	{0, 0, 640, 240, 0, 0},	/* NTSC */
	{0, 0, 768, 288, 0, 0},	/* SECAM */
	{0, 0, 768, 288, 0, 0}	/* AUTO *//* we should never get here */
//      { 32,   24,     768,    288, 0, 0 },    /* PAL */
//      { 32,   24,     640,    240, 0, 0 },    /* NTSC */
//      { 32,   24,     768,    288, 0, 0 },    /* SECAM */
//      {  0,    0,     768,    288, 0, 0 }     /* AUTO */ /* we should never get here */
};

/* FIXME: Todo stuff.
 * - Background thread to monitor Voodoo3 for setting changes.
 * - Reset video processor on resolution switch (must figure out how to reset first).
 * - Update overlay window position when the virtual desktop scrolls.
 */

/******************************
 * Overlay handling functions *
 *****************************/



void v3tv_set_chromakey(struct voodoo_data *voodoo)
{
	if (debug >= 2)
		printk("v3tv: set_chromakey (0x%lx)\n", voodoo->chroma);

	v3tv_conf(Chromakey_Enable, CONF_SIZE(Chromakey_Enable));

	v3tv_regwrite(V3REG_vidChromaMin, voodoo->chroma);
	v3tv_regwrite(V3REG_vidChromaMax, voodoo->chroma);
}

/* do a fraction a/b in numBits format is 0.numbits*/
long do_fraction(int numBits, unsigned a, unsigned b)
{
	/* This is _not_ the "official" algorithm from voodoo3_spec.pdf
	 * This should work in typical cases.
	 */

	u32 num;

	if ((0xFFFFFFFF << (32 - numBits)) & a) {
		printk("v3tv: Overflow in do_fraction (%d, %d, %d)\n",
		       numBits, a, b);
	}

	num = ((u32) a << numBits) / b;

	if (debug >= 2) {
		printk("v3tv: do_fraction (0x%lx) -> 0x%lx\n",
		       (unsigned long)a << numBits, (unsigned long)num);
	}
	return num;
}

/* Update overlay window size and strides
 * FIXME:
 * This could possibly be made simpler, if we used constantly sized input video.
 * I'll need to get back to it...
 */

void v3tv_calc_scale(struct voodoo_data *voodoo)
{
	if (voodoo->vidOrigWidth <= voodoo->overlayWidth) {
		if (voodoo->vidOrigWidth < voodoo->overlayWidth) {
			voodoo->dudx =
			    do_fraction(20, voodoo->vidOrigWidth,
					voodoo->overlayWidth);
			if (debug)
				printk
				    ("v3tv: Horizontal Magnification %d\n",
				     voodoo->dudx);
		}

		voodoo->vidOverlayStride = voodoo->vidInStride;
	} else {
		voodoo->XDecim = (voodoo->overlayWidth & 0xFFF) |
		    (((voodoo->vidOrigWidth -
		       voodoo->overlayWidth) & 0xFFF) << 16);
		if (debug)
			printk("v3tv: Horizontal Decimation %d\n",
			       voodoo->XDecim);
		voodoo->vidInStride = voodoo->overlayWidth * 2;
		voodoo->vidOverlayStride = voodoo->vidInStride;
	}

	if (voodoo->vidOrigHeight <= voodoo->overlayHeight) {
		if (voodoo->vidOrigHeight < voodoo->overlayHeight) {
			voodoo->dvdy =
			    do_fraction(20, voodoo->vidOrigHeight,
					voodoo->overlayHeight);
			if (debug)
				printk("v3tv: Vertical Magnification %d\n",
				       voodoo->dvdy);
		}
	} else {
		voodoo->YDecim = (voodoo->overlayHeight & 0xFFF) |
		    (((voodoo->vidOrigHeight -
		       voodoo->overlayHeight) & 0xFFF) << 16);
		if (debug)
			printk("v3tv: Vertical Decimation %d\n",
			       voodoo->YDecim);
	}
}

void v3tv_update_overlay_setup(struct voodoo_data *voodoo)
{
	unsigned int value = 0;
	voodoo->dudx = voodoo->dvdy = voodoo->XDecim = voodoo->YDecim = 0;

	if (!voodoo)
		return;

	voodoo->vidInStride = voodoo->vidOrigWidth * 2;
	voodoo->overlayWidth =
	    voodoo->vidOverlayEndX - voodoo->vidOverlayStartX;
	voodoo->overlayHeight =
	    voodoo->vidOverlayEndY - voodoo->vidOverlayStartY;

	voodoo->vidOverlayStride = voodoo->vidInStride;
	voodoo->vidScanlineBytes = voodoo->vidInStride;

	v3tv_calc_scale(voodoo);

	value = voodoo->vidOverlayStartX & 0xFFF;
	value |= ((voodoo->vidOverlayStartY << 12) & 0xFFF000);
	v3tv_regwrite(V3REG_vidOverlayStartCoords, value);

	value = voodoo->vidOverlayEndX & 0xFFF;
	value |= ((voodoo->vidOverlayEndY << 12) & 0xFFF000);
	v3tv_regwrite(V3REG_vidOverlayEndScreenCoord, value);

	value = v3tv_regread(V3REG_vidDesktopOverlayStride);
	value =
	    (value & 0x8000FFFF) | ((voodoo->vidOverlayStride << 16) &
				    0x7FFF0000);
	v3tv_regwrite(V3REG_vidDesktopOverlayStride, value);

	v3tv_regwrite(V3REG_vidInStride, voodoo->vidInStride & 0x7FFF);

	v3tv_regwrite(V3REG_vidOverlayDudxOffsetSrcWidth,
			 voodoo->vidScanlineBytes << 19);

	v3tv_regwrite(V3REG_vidOverlayDudx, voodoo->dudx);
	v3tv_regwrite(V3REG_vidOverlayDvdy, voodoo->dvdy);

	v3tv_regwrite(V3REG_vidInXDecimDeltas, voodoo->XDecim);
	v3tv_regwrite(V3REG_vidInYDecimDeltas, voodoo->YDecim);

	if (debug)
		printk
		    ("v3tv: overlay start x: %d y: %d end x: %d y: %d (W: %d H: %d)\n",
		     voodoo->vidOverlayStartX, voodoo->vidOverlayStartY,
		     voodoo->vidOverlayEndX, voodoo->vidOverlayEndY,
		     voodoo->overlayWidth, voodoo->overlayHeight);
	if (debug)
		printk
		    ("v3tv: Overlay Stride: %d vidInStride: %d Bytes: %d\n",
		     voodoo->vidOverlayStride, voodoo->vidInStride,
		     voodoo->vidScanlineBytes);

	if (voodoo->XDecim) {

		v3tv_conf(Horizontal_Decimation_Enable,
			     CONF_SIZE(Horizontal_Decimation_Enable));
	} else {
		v3tv_conf(Horizontal_Decimation_Disable,
			     CONF_SIZE(Horizontal_Decimation_Disable));
	}

	if (voodoo->YDecim) {
		v3tv_conf(Vertical_Decimation_Enable,
			     CONF_SIZE(Vertical_Decimation_Enable));
	} else {
		v3tv_conf(Vertical_Decimation_Disable,
			     CONF_SIZE(Vertical_Decimation_Disable));
	}

	if (voodoo->dudx) {
		v3tv_conf(Horizontal_Magnification_Enable,
			     CONF_SIZE(Horizontal_Magnification_Enable));
	} else {
		v3tv_conf(Horizontal_Magnification_Disable,
			     CONF_SIZE(Horizontal_Magnification_Disable));
	}

	if (voodoo->dvdy) {
		v3tv_conf(Vertical_Magnification_Enable,
			     CONF_SIZE(Vertical_Magnification_Enable));
	} else {
		v3tv_conf(Vertical_Magnification_Disable,
			     CONF_SIZE(Vertical_Magnification_Disable));
	}
}

void v3tv_set_overlay(struct voodoo_data *voodoo)
{
	voodoo->vidOverlayStartX = voodoo->capture_win.x;
	voodoo->vidOverlayStartY = voodoo->capture_win.y;
	voodoo->vidOverlayEndX =
	    voodoo->capture_win.x + voodoo->capture_win.width;
	voodoo->vidOverlayEndY =
	    voodoo->capture_win.y + voodoo->capture_win.height;
	voodoo->vidOrigWidth = video_standard[voodoo->norm].width;
	voodoo->vidOrigHeight = video_standard[voodoo->norm].height;
	v3tv_update_overlay_setup(voodoo);
}

void v3tv_overlay_setup(struct voodoo_data *voodoo)
{
	unsigned int value = 0;
	static int dstFormat;

	voodoo->vidInStride = video_standard[voodoo->norm].width * 2;

	value = v3tv_regread(V3REG_vidScreenSize);
	voodoo->desktopWidth = value & 0xFFF;
	voodoo->desktopHeight = (value >> 12) & 0xFFF;

	voodoo->desktopAddr =
	    v3tv_regread(V3REG_vidDesktopStartAddr) & 0xFFFFFF;
	voodoo->desktopStride =
	    v3tv_regread(V3REG_vidDesktopOverlayStride) & 0x7FFF;

	/*
	 * Allocate memory for video capture and triple buffering overlay
	 * Start from the top of video memory, but leave 4K left for the hw mouse cursor.
	 * FIXME: We need to make this more intelligent, because the X-server might use
	 * memory differently in the future...
	 */
	voodoo->vidInAddr2 =
	    0xFFF000 -
	    video_standard[voodoo->norm].width *
	    (video_standard[voodoo->norm].height * 2);
	voodoo->vidInAddr1 =
	    voodoo->vidInAddr2 -
	    video_standard[voodoo->norm].width *
	    (video_standard[voodoo->norm].height * 2);
	voodoo->vidInAddr0 =
	    voodoo->vidInAddr1 -
	    video_standard[voodoo->norm].width *
	    (video_standard[voodoo->norm].height * 2);
	voodoo->captureBuf =
	    voodoo->vidInAddr0 -
	    (video_standard[voodoo->norm].width * 2) *
	    (video_standard[voodoo->norm].height * 4) * 3;

	v3tv_regwrite(V3REG_vidInAddr0, voodoo->vidInAddr0);
	v3tv_regwrite(V3REG_vidInAddr1, voodoo->vidInAddr1);
	v3tv_regwrite(V3REG_vidInAddr2, voodoo->vidInAddr2);

	dstFormat = (v3tv_regread(V3REG_dstFormat) >> 16) & 0x07;

	if (debug) {
		printk
		    ("v3tv: %d x %d sized %sbpp desktop at vidmem address 0x%x\n",
		     voodoo->desktopWidth, voodoo->desktopHeight,
		     (dstFormat == 1) ? "8" : (dstFormat ==
					       3) ? "16" : (dstFormat ==
							    4) ? "24"
		     : (dstFormat == 5) ? "32" : "unknown-",
		     voodoo->desktopAddr);
		printk("v3tv: Desktop stride is %d bytes.\n",
		       voodoo->desktopStride);
		printk("v3tv: Video in stride is %d bytes.\n",
		       voodoo->vidInStride);
		printk("v3tv: Capture buffer at vidmem 0x%x\n",
		       voodoo->captureBuf);
		printk("v3tv: Triple buffers at 0x%x, 0x%x, 0x%x\n",
		       voodoo->vidInAddr0, voodoo->vidInAddr1,
		       voodoo->vidInAddr2);
	}

	/* FIXME: Sometimes, the OverlayStride is incorrect. Check for this */
	value = v3tv_regread(V3REG_vidDesktopOverlayStride);
	value =
	    (value & 0x8000FFFF) +
	    ((voodoo->vidInStride << 16) & 0x7FFF0000);

	v3tv_regwrite(V3REG_vidDesktopOverlayStride, value);
	v3tv_regwrite(V3REG_vidOverlayDudxOffsetSrcWidth,
			 (voodoo->vidInStride) << 19);

	v3tv_set_overlay(voodoo);
}

void v3tv_overlay(struct voodoo_data *voodoo, int i)
{
	int tempReg;
	if (i == 0) {
		v3tv_conf(Overlay_Disable, CONF_SIZE(Overlay_Disable));
//              voodoo3_conf(Disable_VMI_Interrupt, CONF_SIZE(Disable_VMI_Interrupt));

		tempReg = v3tv_regread(V3REG_intrCtrl);
		if (debug)
			printk("v3tv: intrCtrl: %x\n", tempReg);
		tempReg &= ~(1 << 21);
		if (debug)
			printk("v3tv: write back intrCtrl: %x "
			       "VMI interupts disabled\n", tempReg);
		v3tv_regwrite(V3REG_intrCtrl, tempReg);
	} else {
		v3tv_conf(Overlay_Enable, CONF_SIZE(Overlay_Enable));

		/* Overlay filtering must be turned off, if video processor
		 * is set to 2x mode.  This makes the overlay look bad,
		 * so we disable filtering only when really needed.
		 */
		if (v3tv_regread(V3REG_vidProcCfg) & (1 << 26)) {
			v3tv_conf(Overlay_Filter_Off,
				     CONF_SIZE(Overlay_Filter_Off));
			if (debug)
				printk(KERN_WARNING
				       "v3tv: Overlay filter disabled\n");
		} else {
			v3tv_conf(Overlay_Filter_On,
				     CONF_SIZE(Overlay_Filter_On));
			if (debug)
				printk(KERN_WARNING
				       "v3tv: Overlay filter enabled\n");
		}
//              voodoo3_conf(Enable_VMI_Interrupt, CONF_SIZE(Enable_VMI_Interrupt));
		tempReg = v3tv_regread(V3REG_intrCtrl);
		if (debug)
			printk("v3tv: intrCtrl: %x\n", tempReg);
		tempReg |= (1 << 21);
		tempReg |= (1 << 31);
		if (debug)
			printk("v3tv: write back intrCtrl: %x "
			       "VMI interupts enable; PCI interupt is inactive\n",
			       tempReg);
		v3tv_regwrite(V3REG_intrCtrl, tempReg);

	}
	if (debug)
		printk(KERN_INFO "v3tv: Overlay %s\n",
		       (i) ? "Enabled" : "Disabled");
}

/*
 * Overrides for Emacs so that we follow Linus's tabbing style.
 * ---------------------------------------------------------------------------
 * Local variables:
 * c-basic-offset: 8
 * End:
 */
/*
 * voodoo-capture.c
 *
 * Video capture routines for Voodoo3 3500 TV.
 *
 * $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/modules/public/usb_camera/v4l_experimental/v3tv/v3tv.c#1 $
 */

/* FIXME
 * Here's a loop that is somewhat CPU speed dependent.
 */
/* Make room (wait for it to become available) in the PCI FIFO buffer */
int v3tv_MakeRoom(int num)
{
	int i = 0;

	while ((v3tv_regread(V3REG_status) & 0x1F) < num) {
		if (i++ > 0x100000) {
			printk("v3tv: no room in voodoo3 FIFO: breakout!!!");
			return 0;
		}
	}
	return 1;
}

static int clip0Min;
static int clip0Max;
static int dstBaseAddr;
static int dstFormat;
static int commandExtra;
static int srcFormat;
static int srcSize;
static int srcXY;
static int dstSize;
static int dstXY;
static int command;
static int srcBaseAddr;

/* FIXME
 * This _will_ interfere with the X-server, at least on SMP boxes.
 */
void v3tv_2dreg(int i)
{
	if (i == VOODOO3_2D_SAVE) {
		clip0Min =	v3tv_regread(V3REG_clip0Min);
		clip0Max =	v3tv_regread(V3REG_clip0Max);
		dstBaseAddr =	v3tv_regread(V3REG_dstBaseAddr);
		dstFormat =	v3tv_regread(V3REG_dstFormat);
		commandExtra =	v3tv_regread(V3REG_commandExtra);
		srcFormat =	v3tv_regread(V3REG_srcFormat);
		srcSize =	v3tv_regread(V3REG_srcSize);
		srcXY =		v3tv_regread(V3REG_srcXY);
		dstSize =	v3tv_regread(V3REG_dstSize);
		dstXY =		v3tv_regread(V3REG_dstXY);
		command =	v3tv_regread(V3REG_command);
		srcBaseAddr =	v3tv_regread(V3REG_srcBaseAddr);
	} else if (i == VOODOO3_2D_RESTORE) {
		if (!v3tv_MakeRoom(12))
			printk("Error!!!\n");

		v3tv_regwrite(V3REG_clip0Min, clip0Min);
		v3tv_regwrite(V3REG_clip0Max, clip0Max);
		v3tv_regwrite(V3REG_dstBaseAddr, dstBaseAddr);
		v3tv_regwrite(V3REG_dstFormat, dstFormat);
		v3tv_regwrite(V3REG_commandExtra, commandExtra);
		v3tv_regwrite(V3REG_srcFormat, srcFormat);
		v3tv_regwrite(V3REG_srcSize, srcSize);
		v3tv_regwrite(V3REG_srcXY, srcXY);
		v3tv_regwrite(V3REG_dstSize, dstSize);
		v3tv_regwrite(V3REG_dstXY, dstXY);
		v3tv_regwrite(V3REG_command, command);
		v3tv_regwrite(V3REG_srcBaseAddr, srcBaseAddr);
	}
}

void v3tv_capture_frame(struct voodoo_data *voodoo_tv)
{
	int vidInStatus;
	int isOdd;
	u32 vidBuffer;
	int srcSize;
	int dstSize;
	int cmd;

	vidInStatus = v3tv_regread(V3REG_vidInStatusCurrentLine);
	vidInStatus >>= 16;
	vidInStatus &= 7;

	isOdd = vidInStatus & 4;

	/* Only use odd frames for now */

	switch (vidInStatus & 0x3) {
	case 0:
		/* Finished writing to vidInAddr0 */
//                      if(isOdd)
		vidBuffer = voodoo_tv->vidInAddr0;
//                      else
//                              vidBuffer = voodoo_tv->vidInAddr2;
		break;
	case 1:
		/* Finished writing to vidInAddr1 */
//                      if(isOdd)
		vidBuffer = voodoo_tv->vidInAddr1;
//                      else
//                              vidBuffer = voodoo_tv->vidInAddr0;
		break;
	case 2:
		/* Finished writing to vidInAddr2 */
//                      if(isOdd)
		vidBuffer = voodoo_tv->vidInAddr2;
//                      else
//                              vidBuffer = voodoo_tv->vidInAddr1;
		break;
	default:
		/* Error */
		vidBuffer = 0;
		break;
	}

	if (!vidBuffer)
		return;

	v3tv_2dreg(VOODOO3_2D_SAVE);

	if (!v3tv_MakeRoom(14)) {
		printk("Error2\n");
		return;
	}

	v3tv_set_palette(&voodoo_tv->picture_settings);
	printk("v3tv: capture frame, palette=%d, V3REG_status= %x\n",
	       voodoo_tv->picture_settings.palette, v3tv_regread(V3REG_status));

	v3tv_regwrite(V3REG_srcBaseAddr, (unsigned long)vidBuffer);
	v3tv_regwrite(V3REG_dstBaseAddr, (unsigned long)voodoo_tv->captureBuf);
//      v3tv_regwrite(V3REG_dstBaseAddr, 0);

	srcSize =
	    ((voodoo_tv->
	      vidOrigWidth) & 0xFFF) | (((voodoo_tv->
					  vidOrigHeight) << 16) & 0xFFF0000);
	dstSize =
	    ((voodoo_tv->vidOverlayEndX -
	      voodoo_tv->vidOverlayStartX) & 0xFFF) | (((voodoo_tv->vidOverlayEndY -
						      voodoo_tv->
						      vidOverlayStartY) << 16) &
						    0xFFF0000);

	/* [16:9] = 9 == packed 4:2:2 UYVY */
	v3tv_regwrite(V3REG_srcFormat,
			 (voodoo_tv->vidInStride & 0xFFF) | (9 << 16));

	v3tv_regwrite(V3REG_srcSize, srcSize);
	v3tv_regwrite(V3REG_dstSize, dstSize);
	v3tv_regwrite(V3REG_srcXY, 0);
	v3tv_regwrite(V3REG_dstXY, 0);

//      v3tv_regwrite(V3REG_dstFormat, (2048 & 0xFFF) | (3 << 16));
	v3tv_regwrite(V3REG_clip0Min, 0);
	v3tv_regwrite(V3REG_clip0Max, 0xFFFFFFFF);

	v3tv_regwrite(V3REG_commandExtra, 0);
	cmd = 0xCC000002;
	v3tv_regwrite(V3REG_command, cmd);	/* Command: screen to screen stretch blt */

	v3tv_regwrite(V3REG_launchArea, 0);
	v3tv_2dreg(VOODOO3_2D_RESTORE);
}

#if 0
static int fbuffer_alloc()
{
	if (!voodoo->fbuffer)
		voodoo->fbuffer =
		    (unsigned char *)rvmalloc(voodoo->gbuffers *
					      voodoo->gbufsize);
	else
		printk(KERN_ERR "v4l-v3tv: Double alloc of fbuffer!\n");

	if (!voodoo->fbuffer)
		return -ENOBUFS;
	return 0;
}
#endif

/*
 *	This maps the vmalloced and reserved fbuffer to user space.
 *
 *  FIXME:
 *  - PAGE_READONLY should suffice!?
 *  - remap_page_range is kind of inefficient for page by page remapping.
 *    But e.g. pte_alloc() does not work in modules ... :-(
 */

#if 0
static int do_voodoo_mmap(const char *adr, unsigned long size)
{
	unsigned long start = (unsigned long)adr;
	unsigned long page, pos;

	if (size > voodoo->gbuffers * voodoo->gbufsize)
		return -EINVAL;

	if (!voodoo->fbuffer) {
		if (fbuffer_alloc())
			return -EINVAL;
	}
	pos = (unsigned long)voodoo->fbuffer;
	while (size > 0) {
		page = kvirt_to_pa(pos);
		if (remap_page_range(start, page, PAGE_SIZE, PAGE_SHARED))
			return -EAGAIN;
		start += PAGE_SIZE;
		pos += PAGE_SIZE;
		size -= PAGE_SIZE;
	}
	return 0;
}
#endif

 /*
  * Video4Linux functions for Voodoo3 3500 TV drivers.
  *
  * $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/modules/public/usb_camera/v4l_experimental/v3tv/v3tv.c#1 $
  */

/* External variables: Data struct */
//extern struct voodoo_data *voodoo;
//extern struct video_capture video_standard[];

static struct video_device v3tv_vdev_radio;
static struct video_device v3tv_vdev_video;

static atomic_t users = {
      counter:0
};				/* Common for radio and TV, only one at a time possible */

/* Module parameters */
//extern int debug;

int video_nr;
int radio_nr;
int vbi_nr;

static const char *v4l1_ioctls[] = {
	"?", "CGAP", "GCHAN", "SCHAN", "GTUNER", "STUNER", "GPICT", "SPICT",
	"CCAPTURE", "GWIN", "SWIN", "GFBUF", "SFBUF", "KEY", "GFREQ",
	"SFREQ", "GAUDIO", "SAUDIO", "SYNC", "MCAPTURE", "GMBUF", "GUNIT",
	"GCAPTURE", "SCAPTURE", "SPLAYMODE", "SWRITEMODE", "GPLAYINFO",
	"SMICROCODE", "GVBIFMT", "SVBIFMT" };
#define V4L1_IOCTLS ARRAY_SIZE(v4l1_ioctls)

/* ----------------------------------------------------------------------- */

static struct v4l2_input v3tv_radio_input = {
	.name	=	"Voodoo3 TV 3500 FM Tuner",
	.type	=	V4L2_INPUT_TYPE_TUNER,
	.audioset =	1,
	.tuner	=	1
};

/* ----------------------------------------------------------------------- */

static const struct v4l2_queryctrl no_ctl = {
	.name  = "42",
	.flags = V4L2_CTRL_FLAG_DISABLED,
};
static const struct v4l2_queryctrl v3tv_ctls[] = {
	{
		.id            = V4L2_CID_AUDIO_MUTE,
		.name          = "Mute",
		.minimum       = 0,
		.maximum       = 1,
		.type          = V4L2_CTRL_TYPE_BOOLEAN,
	},{
		.id            = V4L2_CID_AUDIO_VOLUME,
		.name          = "Volume",
		.minimum       = 0,
		.maximum       = 65535,
		.step          = 65535/100,
		.default_value = 65535,
		.type          = V4L2_CTRL_TYPE_INTEGER,
	},{
		.id            = V4L2_CID_AUDIO_BALANCE,
		.name          = "Balance",
		.minimum       = 0,
		.maximum       = 65535,
		.step          = 65535/100,
		.default_value = 32768,
		.type          = V4L2_CTRL_TYPE_INTEGER,
	},{
		.id            = V4L2_CID_AUDIO_BASS,
		.name          = "Bass",
		.minimum       = 0,
		.maximum       = 65535,
		.step          = 65535/100,
		.default_value = 32768,
		.type          = V4L2_CTRL_TYPE_INTEGER,
	},{
		.id            = V4L2_CID_AUDIO_TREBLE,
		.name          = "Treble",
		.minimum       = 0,
		.maximum       = 65535,
		.step          = 65535/100,
		.default_value = 32768,
		.type          = V4L2_CTRL_TYPE_INTEGER,
	}
};

const int V3TV_CTLS = ARRAY_SIZE(v3tv_ctls);

static int get_control(struct voodoo_data *voodoo, struct v4l2_control *c)
{
	struct video_audio va;
	int i;

	for (i = 0; i < V3TV_CTLS; i++)
		if (v3tv_ctls[i].id == c->id)
			break;
	if (i == V3TV_CTLS)
		return -EINVAL;
	if (i >= 4 && i <= 10) {
		memset(&va,0,sizeof(va));
		i2c_clients_command(voodoo->voodoo_i2c_adapter, VIDIOCGAUDIO, &va);
	}
	switch (c->id) {
/*
	case V4L2_CID_BRIGHTNESS:
		c->value = voodoo->bright;
		break;
	case V4L2_CID_HUE:
		c->value = voodoo->hue;
		break;
	case V4L2_CID_CONTRAST:
		c->value = voodoo->contrast;
		break;
	case V4L2_CID_SATURATION:
		c->value = voodoo->saturation;
		break;
*/
	case V4L2_CID_AUDIO_MUTE:
		c->value = (VIDEO_AUDIO_MUTE & va.flags) ? 1 : 0;
		break;
	case V4L2_CID_AUDIO_VOLUME:
		c->value = va.volume;
		break;
	case V4L2_CID_AUDIO_BALANCE:
		c->value = va.balance;
		break;
	case V4L2_CID_AUDIO_BASS:
		c->value = va.bass;
		break;
	case V4L2_CID_AUDIO_TREBLE:
		c->value = va.treble;
		break;
//	case V4L2_CID_AUDIO_LOUDNESS:
//		c->value = voodoo->loudness;
//		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static int set_control(struct voodoo_data *voodoo, struct v4l2_control *c)
{
	struct video_audio va;
	int i;

	for (i = 0; i < V3TV_CTLS; i++)
		if (v3tv_ctls[i].id == c->id)
			break;
	if (i == V3TV_CTLS)
		return -EINVAL;
	if (i >= 4 && i <= 10) {
		memset(&va,0,sizeof(va));
		i2c_clients_command(voodoo->voodoo_i2c_adapter, VIDIOCGAUDIO, &va);
	}
	switch (c->id) {
/*
	case V4L2_CID_BRIGHTNESS:
		bt848_bright(voodoo,c->value);
		break;
	case V4L2_CID_HUE:
		bt848_hue(voodoo,c->value);
		break;
	case V4L2_CID_CONTRAST:
		bt848_contrast(voodoo,c->value);
		break;
	case V4L2_CID_SATURATION:
		bt848_sat(voodoo,c->value);
		break;
*/
	case V4L2_CID_AUDIO_MUTE:
		if (c->value)
			va.flags |= VIDEO_AUDIO_MUTE;
		else
			va.flags &= ~VIDEO_AUDIO_MUTE;
		break;

	case V4L2_CID_AUDIO_VOLUME:
		va.volume = c->value;
		break;
	case V4L2_CID_AUDIO_BALANCE:
		va.balance = c->value;
		break;
	case V4L2_CID_AUDIO_BASS:
		va.bass = c->value;
		break;
	case V4L2_CID_AUDIO_TREBLE:
		va.treble = c->value;
		break;
	default:
		return -EINVAL;
	}
	if (i >= 4 && i <= 10)
		i2c_clients_command(voodoo->voodoo_i2c_adapter,
					 VIDIOCSAUDIO, &va);
	return 0;
}

static void
i2c_vidiocschan(struct voodoo_data *voodoo)
{
	struct video_channel c;

	memset(&c,0,sizeof(c));
	c.norm    = voodoo->norm;
	c.channel = voodoo->input;
	i2c_clients_command(voodoo->voodoo_i2c_adapter, VIDIOCSCHAN, &c);
}

/* ----------------------------------------------------------------------- */

static int v3tv_radio_do_ioctl(struct inode *inode, struct file *file,
			      unsigned int cmd, void *arg)
{
	struct voodoo_data *voodoo_radio = file->private_data;

	switch (cmd) {
	case VIDIOC_ENUMINPUT:
	{
		struct v4l2_input *i = arg;

		printk("v3tv: VIDIOC_ENUMINPUT %d.\n",i->index);
		if( i->index != 0 ) {
			return -EINVAL;
		}
		memcpy(i, &v3tv_radio_input, sizeof(struct v4l2_input));

		return 0;
	}
	case VIDIOC_QUERYCTRL:
	{
		struct v4l2_queryctrl *c = arg;
		int i;

		printk("v3tv: VIDIOC_QUERYCTRL\n");
		if (c->id <  V4L2_CID_BASE ||
		     c->id >= V4L2_CID_LASTP1) /* 'if (( ...' &&
		    (c->id <  V4L2_CID_PRIVATE_BASE ||
		     c->id >= V4L2_CID_PRIVATE_LASTP1)) */
			return -EINVAL;
		for (i = 0; i < V3TV_CTLS; i++)
			if (v3tv_ctls[i].id == c->id)
				break;
		if (i == V3TV_CTLS) {
			*c = no_ctl;
			return 0;
		}
		*c = v3tv_ctls[i];
		if (i >= 4 && i <= 8) {
			struct video_audio va;
			memset(&va,0,sizeof(va));
			i2c_clients_command(voodoo_radio->voodoo_i2c_adapter,
						VIDIOCGAUDIO, &va);
			switch (v3tv_ctls[i].id) {
			case V4L2_CID_AUDIO_VOLUME:
				if (!(va.flags & VIDEO_AUDIO_VOLUME))
					*c = no_ctl;
				break;
			case V4L2_CID_AUDIO_BALANCE:
				if (!(va.flags & VIDEO_AUDIO_BALANCE))
					*c = no_ctl;
				break;
			case V4L2_CID_AUDIO_BASS:
				if (!(va.flags & VIDEO_AUDIO_BASS))
					*c = no_ctl;
				break;
			case V4L2_CID_AUDIO_TREBLE:
				if (!(va.flags & VIDEO_AUDIO_TREBLE))
					*c = no_ctl;
				break;
			}
		}
		return 0;
	}
	case VIDIOC_G_CTRL:
	{
		printk("v3tv: VIDIOC_G_CTRL\n");
		return get_control(voodoo_radio,arg);
	}
	case VIDIOC_S_CTRL:
	{
		printk("v3tv: VIDIOC_S_CTRL\n");
		return set_control(voodoo_radio,arg);
	}
	case VIDIOC_QUERYCAP:
	{
		struct v4l2_capability *cap = arg;

		printk("v3tv: VIDIOC_QUERYCAP\n");

		strcpy(cap->driver,"v3tv radio");
		strcpy(cap->card,"Voodoo3 TV 3500");
		strcpy(cap->bus_info,"AGP Slot 0");
		cap->version = KERNEL_VERSION(0,1,0);
		cap->capabilities =
			V4L2_CAP_TUNER |
			V4L2_CAP_RADIO;
		return 0;
	}
	case VIDIOC_G_TUNER:
	{
		struct video_tuner vt;
		struct video_audio va;
		struct v4l2_tuner *t = arg;

		printk("v3tv: VIDIOC_G_TUNER\n");

		if (0 != t->index)
			return -EINVAL;
//		down(&voodoo_radio->lock);
		memset(t, 0, sizeof(struct v4l2_tuner));
		if (VOODOO3_MODEL_NTSC == voodoo_radio->model)
			strcpy(t->name, "FM Radio");
		else
			strcpy(t->name, "TV Tweaked to FM Radio");
		t->type       = V4L2_TUNER_RADIO;
		/* japan:		76.0 MHz -  89.9 MHz
		 * western europe:	87.5 MHz - 108.0 MHz
		 * russia:		65.0 MHz - 108.0 MHz */
		t->rangelow  = 87 * 16;
		t->rangehigh  = 108 * 16;
		t->capability = //V4L2_TUNER_CAP_LOW |
				V4L2_TUNER_CAP_NORM |
				V4L2_TUNER_CAP_STEREO;
//		t->rxsubchans = V4L2_TUNER_SUB_MONO;
		i2c_clients_command(voodoo_radio->voodoo_i2c_adapter,
				    VIDIOCGTUNER, &vt);
		t->signal = vt.signal;

		/* Hmmm ... */
		/* FIXME: fill capability+audmode */
		memset(&va, 0, sizeof(struct video_audio));
		i2c_clients_command(voodoo_radio->voodoo_i2c_adapter,
				    VIDIOCGAUDIO, &va);
		if(va.mode & VIDEO_SOUND_STEREO) {
			t->audmode     = V4L2_TUNER_MODE_STEREO;
//			t->rxsubchans |= V4L2_TUNER_SUB_STEREO;
		}
		if(va.mode & VIDEO_SOUND_LANG1) {
			t->audmode    = V4L2_TUNER_MODE_LANG1;
//			t->rxsubchans = V4L2_TUNER_SUB_LANG1
//				| V4L2_TUNER_SUB_LANG2;
		}
//		up(&voodoo_radio->lock);
		return 0;
	}
	case VIDIOC_S_TUNER:
	{
		struct v4l2_tuner *t = arg;

		printk("v3tv: VIDIOC_S_TUNER\n");

		if (0 != t->index)
			return -EINVAL;
//		down(&voodoo_radio->lock);
		{
			struct video_audio va;
			memset(&va, 0, sizeof(struct video_audio));
			i2c_clients_command(voodoo_radio->voodoo_i2c_adapter,
						VIDIOCGAUDIO, &va);
			if (t->audmode == V4L2_TUNER_MODE_MONO)
				va.mode = VIDEO_SOUND_MONO;
			else if (t->audmode == V4L2_TUNER_MODE_STEREO)
				va.mode = VIDEO_SOUND_STEREO;
			else if (t->audmode == V4L2_TUNER_MODE_LANG1)
				va.mode = VIDEO_SOUND_LANG1;
			else if (t->audmode == V4L2_TUNER_MODE_LANG2)
				va.mode = VIDEO_SOUND_LANG2;
			i2c_clients_command(voodoo_radio->voodoo_i2c_adapter,
						VIDIOCSAUDIO, &va);
		}
//		up(&voodoo_radio->lock);
		return 0;
	}
	case VIDIOC_G_FREQUENCY:
	{
		struct v4l2_frequency *f = arg;

		printk("v3tv: VIDIOC_G_FREQUENCY\n");

		memset(f,0,sizeof(struct v4l2_frequency));
		f->type = V4L2_TUNER_RADIO;
		f->frequency = voodoo_radio->freq;
		if (copy_to_user(arg, &f, sizeof(struct v4l2_frequency)))
			return -EFAULT;
		return 0;
	}
	case VIDIOC_S_FREQUENCY:
	{
		struct v4l2_frequency *f = arg;
		int zero = 0;

		printk("v3tv: VIDIOC_S_FREQUENCY\n");

		if (unlikely(f->tuner != 0))
			return -EINVAL;
		if (unlikely(f->type != V4L2_TUNER_RADIO))
			return -EINVAL;
#if 0
		if (f->frequency < 45 * 16000) {
//		if (f->frequency < 696000) {
			printk ("v3tv: Radio program does not support the VIDEO_TUNER_LOW flag\n");
			printk("v3tv: Multiplying by 1000 to get %d\n", 1000 * (f->frequency / 16));
			f->frequency *= 1000;
		}
		voodoo_radio->freq = f->frequency/1000;
#endif
		voodoo_radio->freq = f->frequency;
		printk("v3tv: radio frequency %ld\n", voodoo_radio->freq);

//		down(&voodoo_radio->lock);
		i2c_clients_command(voodoo_radio->voodoo_i2c_adapter,
				    AUDC_SET_RADIO, &zero);
		i2c_clients_command(voodoo_radio->voodoo_i2c_adapter,
				    VIDIOCSFREQ, &voodoo_radio->freq);

//		up(&voodoo_radio->lock);
		return 0;
	}
#if 0 // obsolete v4l1 ioctl's
	case VIDIOCGCAP:
		{
			struct video_capability vcap;
			vcap.type = v3tv_vdev_radio.type;
			vcap.channels = 1;
			vcap.audios = 1;
			vcap.maxwidth = 0;
			vcap.minwidth = 0;
			vcap.maxheight = 0;
			vcap.minheight = 0;
			strcpy(vcap.name, v3tv_vdev_radio.name);
			if (copy_to_user(arg, &vcap, sizeof(vcap)))
				return -EFAULT;
			if (debug)
				printk("v3tv: Getting radio capabilities.\n");
			break;
		}
	case VIDIOCGTUNER:
		{
			struct video_tuner vtune;
			if (copy_from_user(&vtune, arg, sizeof(vtune)))
				return -EFAULT;
			if (vtune.tuner)
				return -EINVAL;
			vtune.rangelow = 87 * 16000;
			vtune.rangehigh = 108 * 16000;
			vtune.flags = VIDEO_TUNER_LOW;
			vtune.mode = VIDEO_MODE_AUTO;
			vtune.signal = 0xFFFF;	/* FIXME: We should really check the signal */
			if (voodoo_radio->model == VOODOO3_MODEL_NTSC)
				strcpy(vtune.name, "FM");
			else
				strcpy(vtune.name, "TV Tweaked to FM");

			if (copy_to_user(arg, &vtune, sizeof(vtune)))
				return -EFAULT;
			if (debug)
				printk
				    ("v3tv: Getting radio tuner capabilities.\n");
			break;
		}
	case VIDIOCSTUNER:
		{
			struct video_tuner vtune;
			if (copy_from_user(&vtune, arg, sizeof(vtune)))
				return -EFAULT;
			if (vtune.tuner)
				return -EINVAL;
			if (debug)
				printk("v3tv: Selecting radio tuner 0.\n");
			break;
		}
	case VIDIOCGFREQ:
		{
			if (copy_to_user(arg, &voodoo_radio->freq, sizeof(u32)))
				return -EFAULT;
			break;
		}
	case VIDIOCSFREQ:
		{
			u32 freq;
			int zero = 0;

			if (copy_from_user(&freq, arg, sizeof(u32)))
				return -EFAULT;
			if (freq < 45 * 16000) {
				printk ("v3tv: Radio program does not support the VIDEO_TUNER_LOW flag\n");
				printk("v3tv: Multiplying by 1000 to get %d\n", 1000 * (freq / 16));
				freq *= 1000;
			}

			voodoo_radio->freq = freq/1000;
			/* FIXME. This isn't right!
			 * We should really use the radio mode of the MSP processor,
			 * but apparently the PAL version of the Voodoo3 doesn't
			 * support this. So we use mono FM TV-sound in stead.
			 *
			 * FIXME again. The radio mutes when we change frequency.
			 *
			 * FIXME x 3: Force tweaked radio to use FM mono 5.5. MHz.
			 */
			i2c_clients_command(voodoo->voodoo_i2c_adapter,
						AUDC_SET_RADIO, &zero);
			i2c_clients_command(voodoo->voodoo_i2c_adapter,
						VIDIOCSFREQ, &zero);

			i2c_clients_command(voodoo->voodoo_i2c_adapter,
						AUDC_SET_RADIO, &zero);
			i2c_clients_command(voodoo->voodoo_i2c_adapter, VIDIOCSFREQ,
						&voodoo_radio->freq);

			break;
		}
	case VIDIOCGAUDIO:
		{
			struct video_audio va;

			if (copy_from_user(&va, arg, sizeof(va)))
				return -EFAULT;
			if (va.audio) {
				if (debug)
					printk
					    ("v3tv: FIXME: VIDIOCGAUDIO: Audio=%d\n",
					     va.audio);
				va.audio = 0;
				// return -EINVAL;
			}
			i2c_clients_command(voodoo->voodoo_i2c_adapter,
						VIDIOCGAUDIO, &va);

			va.mode = VIDEO_SOUND_MONO;
			va.flags |= VIDEO_AUDIO_MUTABLE | VIDEO_AUDIO_VOLUME
			    | VIDEO_AUDIO_BASS | VIDEO_AUDIO_TREBLE;
			// FIXME: Add support for these.
			if (copy_to_user(arg, &va, sizeof(va)))
				return -EFAULT;

			break;
		}
	case VIDIOCSAUDIO:
		{
			struct video_audio va;

			if (copy_from_user(&va, arg, sizeof(va)))
				return -EFAULT;
			if (va.audio) {
				if (debug)
					printk
					    ("v3tv: FIXME: VIDIOSAUDIO: Audio=%d\n",
					     va.audio);
				// FIXME. This should really be checked.
				// If only user programs respected it :-(
				return -EINVAL;
			}
			// va.mode = 0;
			// FIXME. Force msp3400 _not_ to change stereo mode.
			// FIXME. See that muting is handled properly.
			i2c_clients_command(voodoo->voodoo_i2c_adapter,
						VIDIOCSAUDIO, &va);

			break;
		}
#endif
	default:
	    if (debug > 1) {
		switch (_IOC_TYPE(cmd)) {
		case 'v':
			printk("v3tv: unsuported ioctl 0x%x (v4l1, VIDIOC%s)\n",
			       cmd, (_IOC_NR(cmd) < V4L1_IOCTLS) ?
			       v4l1_ioctls[_IOC_NR(cmd)] : "???");
			break;
		case 'V':
			printk("v3tv: unsupported ioctl 0x%x (v4l2, %s)\n",
			       cmd,  v4l2_ioctl_names[_IOC_NR(cmd)]);
			break;
		default:
			printk("v3tv: unknown ioctl 0x%x (\?\?\?)\n",
			       cmd);
		}
	    }
	    return -ENOIOCTLCMD;
	}
	return 0;
}

static int v3tv_radio_ioctl(struct inode *inode, struct file *file,
			   unsigned int cmd, unsigned long arg)
{
	return video_usercopy(inode, file, cmd, arg, v3tv_radio_do_ioctl);
}

static int v3tv_radio_open(struct inode *inode, struct file *file)
{
	int minor = iminor(inode);

	if (debug)
		printk(KERN_DEBUG "v3tv: open minor=%d\n",minor);

	if (voodoo->radio_dev->minor != minor)
		return -ENODEV;

	if (debug)
		printk(KERN_DEBUG "v3tv: open called (radio)\n");

//	down(&voodoo->lock);
	if (users.counter) {
		printk("v3tv: Radio device busy\n");
//		up(&voodoo->lock);
		return -EBUSY;
	}
	atomic_inc(&users);
	file->private_data = voodoo;

	voodoo->input = 0;
	i2c_vidiocschan(voodoo);

	if (voodoo->tuner_type != UNSET) {
		struct tuner_setup tun_setup;

		tun_setup.mode_mask = T_RADIO | T_ANALOG_TV;
		tun_setup.type = voodoo->tuner_type;
		tun_setup.addr = ADDR_UNSET;

	i2c_clients_command(voodoo->voodoo_i2c_adapter,
				TUNER_SET_TYPE_ADDR, &tun_setup);
	}

	i2c_clients_command(voodoo->voodoo_i2c_adapter,
				AUDC_SET_RADIO, &voodoo->tuner_type);
//	up(&voodoo->lock);

	try_module_get(THIS_MODULE);
	printk("v3tv: Radio device opened.\n");
	return 0;
}

static int v3tv_radio_release(struct inode *inode, struct file *file)
{
/*	Use these in i2c_clients_command() in future */
//      struct video_device *dev = video_devdata(file);
//      struct voodoo_data *voodoo_radio = dev->priv;

	atomic_dec(&users);

	file->private_data = NULL;

	module_put(THIS_MODULE);

	printk("v3tv: Radio device closed.\n");

	return 0;
}

// static struct wait_queue *capture_wait;
static volatile int capture_ready = 0;

static volatile int irq_counter = 0;
static volatile int idles = 0;
static volatile int timeout = 0;

#define DEBUG_LPT 0x378
#include <asm/delay.h>

static void v3tv_irq_printk(long tempReg)
{
	/* 1111.0000.0000.0000.0001.1111.1011.1111 f0001fbf * page 188
	   1f    * [4]  x1f  = FIFO emptyi

	   0     * [5]  x20  = FIFO busy
	   1     * [6]  x40  = Vertical retrace inactive (1)
	   0     * [7]  x80  = FBI grph eng busy

	   0     * [8]  x100 = TREX busy
	   1     * [9]  x200 = Avenger busy
	   1     * [10] x400 = 2D busy
	   1     * [11] x800 = FIFO 0 busy

	   0     * [12] x1000 = FIFO 1 busy
	   * [27:13] reserved

	   * [30:28] Swap Buffers Pending
	   * [31]    PCI Interupt Generated */

	printk(KERN_WARNING "v3tv: IRQ: Timeout %d : %d : %d\n",
	       timeout, idles, irq_counter);
	printk(KERN_WARNING "v3tv: IRQ: V3REG_status: 0x%lx FIFO: 0x%lx%s%s\n",
	       tempReg, tempReg & 0x1f,
	       (tempReg & 0x20) ? "; FIFO busy" : "",
	       (tempReg & 0x40) ? "; Vertical retrace inactive" :
	       "; Vertical retrace active");
	printk(KERN_WARNING "v3tv: IRQ: %s%s%s%s%s%s\n",
	       (tempReg & 0x80) ? "; FBI grph eng busy" : "",
	       (tempReg & 0x100) ? "; TREX busy" : "",
	       (tempReg & 0x200) ? "; Avenger busy" : "",
	       (tempReg & 0x400) ? "; 2D busy" : "",
	       (tempReg & 0x800) ? "; FIFO 0 busy" : "",
	       (tempReg & 0x1000) ? "; FIFO 1 busy" : "");
	printk(KERN_WARNING "v3tv: IRQ: Swap Buffers Pending: 0x%lx; %s\n",
	       (tempReg >> 28) & 7,
	       (tempReg & 80000000) ? "PCI Interupt Generated" : "");
}

// 2.4.23       void (*handler)(int, void *, struct pt_regs *),
// 2.6.1        irqreturn_t (*handler)(int, void *, struct pt_regs *),

static irqreturn_t v3tv_video_irq(int irq, void *dev_id, struct pt_regs *regs)
{
	long tempReg;
	idles = 0;
	timeout = 0;

	capture_ready = 1;

	irq_counter++;
	if ((irq_counter % 10000) == 0)
		printk(KERN_WARNING "v3tv: IRQ: %d interrupts\n", irq_counter);

	v3tv_regwrite(V3REG_command, 0x100);	/* Immediate NOP */
//      voodoo3_regwrite (V3REG_launchArea, 0);
//      voodoo3_regwrite (V3REG_IO_BASE_2 + 0x120, 0);

	while (((tempReg = v3tv_regread(V3REG_status)) & 0x7BF) != 0x1F) {
		timeout++;
		udelay(1000);
		if (timeout < 10)
			continue;

		v3tv_irq_printk(tempReg);

		timeout = 0;
		idles++;
		if (idles < 3)
			continue;
		else {
/*
	The Voodoo3 may be in an unstable state, so we try to bailout gracefully.
	First disable the VMI Interupt, so we don't get here again.
*/
			capture_ready = 0;

			v3tv_conf(Disable_VMI_Interrupt, CONF_SIZE(Disable_VMI_Interrupt));

/* We're not supposed to call free_irq from interrupt context, leave it till _close */
/*			free_irq(voodoo->dev->irq, voodoo->dev);
			voodoo->initialized &= ~I_irq;

			voodoo->decoder_enabled = 0;
			i2c_clients_command(voodoo->voodoo_i2c_adapter,
				DECODER_ENABLE_OUTPUT, &voodoo->decoder_enabled);

			struct video_audio va;

			i2c_clients_command(voodoo->voodoo_i2c_adapter, VIDIOCGAUDIO, &va);

			va.volume = 0;
			va.balance = 32768;
			i2c_clients_command(voodoo->voodoo_i2c_adapter, VIDIOCSAUDIO, &va);
*/

/* Optionaly disable Video In. */
//			v3tv_conf(VMI_Disable, CONF_SIZE(VMI_Disable));
			break;
		}
	}

	tempReg = v3tv_regread(V3REG_intrCtrl);
	if ((tempReg & (1 << 23)) || (capture_ready == 0)) {
		tempReg &= ~(1 << 23);
		tempReg |= (1 << 31);
		v3tv_regwrite(V3REG_intrCtrl, tempReg);
	}
	return capture_ready;
}

void v3tv_set_palette(struct video_picture *picture_settings)
{
	switch ((v3tv_regread(V3REG_dstFormat) >> 16) & 0x07) {
	case 4:		/* 24bpp */
	case 5:		/* 32 bpp - tdfxfb runs at 32 ?? */
		{
			picture_settings->palette = VIDEO_PALETTE_RGB24;
			picture_settings->depth = 24;
			if (debug)
				printk
				    ("v3tv: palette: VIDEO_PALETTE_RGB24 = 24 bpp\n");
			break;
		}
	case 1:		/* 8bpp - we are making an asumption so things can go forward */
	case 3:		/* 16bpp */
	default:
		{
			picture_settings->palette = VIDEO_PALETTE_RGB565;
			picture_settings->depth = 16;
			if (debug)
				printk
				    ("v3tv: palette: VIDEO_PALETTE_RGB565 = 16 bpp\n");
			break;
		}
	}
}

static int v3tv_video_open(struct inode *inode, struct file *file)
{
	int result;
	int minor = iminor(inode);
	enum v4l2_buf_type type = 0;

	if (debug)
		printk(KERN_DEBUG "v3tv: open minor=%d\n",minor);

	if (voodoo->vbi_dev &&
	    voodoo->vbi_dev->minor == minor)
		type = V4L2_BUF_TYPE_VBI_CAPTURE;
	if (voodoo->video_dev &&
	    voodoo->video_dev->minor == minor)
		type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (!type)
		return -ENODEV;

	if (debug)
		printk(KERN_DEBUG "v3tv: open called (type=%s)\n",
			v4l2_type_names[type]);

	/* Check if we are already in use */
	if (users.counter) {
		printk("v3tv: Video or radio device busy\n");
		return -EBUSY;
	}

	/* Get irq. Interrupt is shared and disabled while processing */
	result = request_irq(voodoo->dev->irq, v3tv_video_irq,
			     SA_SHIRQ | SA_INTERRUPT, "v3tv", voodoo->dev);

	if (result == -EINVAL) {
		printk(KERN_ERR "v3tv: Bad irq number or handler\n");
		return result;
	}

	if (result == -EBUSY) {
		printk(KERN_ERR
		       "v3tv: IRQ %d busy, change your PnP config in BIOS\n",
		       voodoo->dev->irq);
		return result;
	}

	if (result < 0) {
		printk(KERN_ERR
		       "v3tv: Unknown error, code %d when getting interrupt\n",
		       result);
		return -EINVAL;	/* Return something that the kernel knows about, or it might Oops */
	}

	atomic_inc(&users);
	voodoo->initialized |= I_irq;

	if (voodoo->tuner_type != UNSET) {
		struct tuner_setup tun_setup;

		tun_setup.mode_mask = T_RADIO | T_ANALOG_TV;
		tun_setup.type = voodoo->tuner_type;
		tun_setup.addr = ADDR_UNSET;

	i2c_clients_command(voodoo->voodoo_i2c_adapter,
				TUNER_SET_TYPE_ADDR, &tun_setup);
	}

//	struct video_audio va;

//	i2c_clients_command(voodoo->voodoo_i2c_adapter, VIDIOCGAUDIO, &va);

//	va.flags = VIDEO_AUDIO_MUTE;
//	va.volume = 65535;
//	va.balance = 32768;	/* FIXME. This shouldn't be necessary */
//	i2c_clients_command(voodoo->voodoo_i2c_adapter, VIDIOCSAUDIO, &va);

	v3tv_conf(VMI_Enable, CONF_SIZE(VMI_Enable));

	v3tv_set_palette(&voodoo->picture_settings);
	i2c_clients_command(voodoo->voodoo_i2c_adapter, DECODER_SET_PICTURE,
				&voodoo->picture_settings);

	voodoo->decoder_enabled = 1;
	i2c_clients_command(voodoo->voodoo_i2c_adapter, DECODER_ENABLE_OUTPUT,
				&voodoo->decoder_enabled);

	file->private_data = voodoo;
	if (debug)
		printk("v3tv: video device opened.\n");
	return 0;
}

static int v3tv_video_release(struct inode *inode, struct file *file)
{
	struct voodoo_data *voodoo_tv = file->private_data;

	if (voodoo_tv->initialized & I_irq) {
	free_irq(voodoo_tv->dev->irq, voodoo_tv->dev);
		voodoo_tv->initialized &= ~I_irq;
	}

	voodoo_tv->decoder_enabled = 0;
	i2c_clients_command(voodoo_tv->voodoo_i2c_adapter, DECODER_ENABLE_OUTPUT,
				&voodoo_tv->decoder_enabled);

//	struct video_audio va;

//	i2c_clients_command(voodoo->voodoo_i2c_adapter, VIDIOCGAUDIO, &va);

//	va.volume = 0;
//	va.balance = 32768;	/* FIXME. This shouldn't be necessary */
//	i2c_clients_command(voodoo->voodoo_i2c_adapter, VIDIOCSAUDIO, &va);

	v3tv_conf(VMI_Disable, CONF_SIZE(VMI_Disable));

	atomic_dec(&users);

	if (debug)
		printk("v3tv: video device closed\n");

	return 0;
}

/* No poll now

static int v4l_voodoo_poll (struct video_device*dev,
		structfile *file, struct poll_table *wait)
{
	poll_wait(file, &capture_wait, wait);

	if(capture_read)
		return POLLIN|POLLRDNORM;
	return 0;
}
*/

static ssize_t v3tv_video_read(struct file *file, char *data,
			       size_t count, loff_t * ppos)
{
	struct voodoo_data *voodoo_tv = file->private_data;

	v3tv_capture_frame(voodoo_tv);
#if 0
	if(copy_to_user(buf, voodoo_2ddtv->fb_base +
			(unsigned long) voodoo_tv->captureBuf, count))

	if (debug)
		printk( "v3tv: read count=%d type=%s "
			"fb_base=0x%p captureBuf=0x%lx\n",
			(int)count, v4l2_type_names[fh->type], voodoo_tv->fb_base,
			(unsigned long) voodoo_tv->captureBuf);
	return count;
#endif
	return 0;
}

#ifdef JustForRef
static ssize_t bttv_read(struct file *file, char *data,
			 size_t count, loff_t *ppos)
{
	struct bttv_fh *fh = file->private_data;
	int retval = 0;

	if (fh->btv->errors)
		bttv_reinit_bt848(fh->btv);
	dprintk("bttv%d: read count=%d type=%s\n",
		fh->btv->c.nr,(int)count,v4l2_type_names[fh->type]);

	switch (fh->type) {
	case V4L2_BUF_TYPE_VIDEO_CAPTURE:
		if (locked_btres(fh->btv,RESOURCE_VIDEO))
			return -EBUSY;
		retval = videobuf_read_one(file, &fh->cap, data, count, ppos);
		break;
	case V4L2_BUF_TYPE_VBI_CAPTURE:
		if (!check_alloc_btres(fh->btv,fh,RESOURCE_VBI))
			return -EBUSY;
		retval = videobuf_read_stream(file, &fh->vbi, data, count, ppos, 1);
		break;
	default:
		BUG();
	}
	return retval;
}
#endif

/*
static long v3tv_voodoo_read(struct video_device *dev, char *buf,unsigned long count )
{
	struct wait_queue wait = { current, NULL};
	u8 *ptr;
	int len;
	int i;
	add_wait_queue(&capture_wait, &wait);
	while(!capture_ready)
	{
		if(file->flags&O_NDELAY)
		{
			remove_wait_queue(&capture_wait,&wait);
			current->state = TASK_RUNNING;
			return -EWOULDBLOCK;
		}
		if(signal_pending(current))
		{
			remove_wait_queue(&capture_wait,&wait);
			current->state = TASK_RUNNING;
			return -ERESTARTSYS;
		}
		schedule();
		current->state = TASK_INTERRUPTIBLE;
	}

	remove_wait_queue(&capture_wait, &wait);

	current->state = TASK_RUNNING;
	capture_ready = 0;
	ptr = (u8 *)buf;
	len=capture_w * 3 * capture_h;

// 24bit RGB
	if(len>count)
		len = count;
// Doesn't all fit
	for(i=0; i<len; i++)
	{
		put_user(inb (io+IMAGE_DATA),ptr);
		ptr++;
	}
	hardware_restart_capture();
	return i;
}
*/

//int v3tv_voodoo_mmap(struct video_device *dev, const char *adr,
//                    unsigned long size)
int v3tv_voodoo_mmap(struct file *file, struct vm_area_struct *vma)
{
#if 0
	struct video_device *dev = video_devdata(file);
	struct voodoo_data *voodoo_tv = dev->priv;
	int r;

	down(&voodoo_tv->lock);
	r = do_voodoo_mmap(adr, size);
	up(&voodoo_tv->lock);
	return r;
#endif
	return -EINVAL;
}

char *desc_pict_palette[] = {
	[ VIDEO_PALETTE_GREY ]    = "GREY",
	[ VIDEO_PALETTE_HI240 ]   = "HI240",
	[ VIDEO_PALETTE_RGB565 ]  = "RGB565",
	[ VIDEO_PALETTE_RGB24 ]   = "RGB24",
	[ VIDEO_PALETTE_RGB32 ]   = "RGB32",
	[ VIDEO_PALETTE_RGB555 ]  = "RGB555",
	[ VIDEO_PALETTE_YUV422 ]  = "YUV422",
	[ VIDEO_PALETTE_YUYV ]    = "YUYV",
	[ VIDEO_PALETTE_UYVY ]    = "UYVY",
	[ VIDEO_PALETTE_YUV420 ]  = "YUV420",
	[ VIDEO_PALETTE_YUV411 ]  = "YUV411",
	[ VIDEO_PALETTE_RAW ]     = "RAW",
	[ VIDEO_PALETTE_YUV422P ] = "YUV422P",
	[ VIDEO_PALETTE_YUV411P ] = "YUV411P",
	[ VIDEO_PALETTE_YUV420P ] = "YUV420P",
	[ VIDEO_PALETTE_YUV410P ] = "YUV410P",
};

static struct video_channel voodoo_channels[3] = {
	{
		.channel = 0,
		.name = "TV Tuner",
		.tuners = 1,
		.flags = VIDEO_VC_TUNER | VIDEO_VC_AUDIO,
		.type = VIDEO_TYPE_TV
	},
	{
		.channel = 1,
		.name = "Composite input",
		.tuners = 0,
		.flags = VIDEO_VC_AUDIO,
		.type = VIDEO_TYPE_TV
	},
	{
		.channel = 2,
		.name = "S Video input",
		.tuners = 0,
		.flags = VIDEO_VC_AUDIO,
		.type = VIDEO_TYPE_TV
	}
};

static int v3tv_video_do_ioctl(struct inode *inode, struct file *file,
			       unsigned int cmd, void *arg)
{
	struct voodoo_data *voodoo_tv = file->private_data;

	switch (cmd) {
	case VIDIOCGCAP:
		{
			struct video_capability *vcap = arg;

			if (debug)
				printk("v3tv: VIDIOCGCAP\n");

			vcap->type = v3tv_vdev_video.type;
			vcap->channels = 3;
			vcap->audios = 0;	// FIXME: Add audio support.
			vcap->maxwidth = 1280;
			vcap->minwidth = 16;
			vcap->maxheight = 960;
			vcap->minheight = 16;
			strcpy(vcap->name, v3tv_vdev_video.name);

			return 0;
		}
	case VIDIOCGCHAN:
		{
			struct video_channel *vchan = arg;

			if (vchan->channel > 2)
				return -EINVAL;

//			vchan->channel	= voodoo_channels[vchan->channel].channel;
			vchan->tuners	= voodoo_channels[vchan->channel].tuners;
			vchan->flags	= voodoo_channels[vchan->channel].flags;
			vchan->type	= voodoo_channels[vchan->channel].type;
			vchan->norm	= voodoo_tv->norm;
			strcpy(vchan->name, voodoo_channels[vchan->channel].name);

			if (debug)
				printk("v3tv: VIDIOCGCHAN channel=%d %s\n",
					vchan->channel, vchan->name);
			return 0;
		}
	case VIDIOCSCHAN:
		{
			struct video_channel *vchan = arg;
			__u16 sarg = 0;

			if (debug)
				printk
				    ("v3tv: VIDIOCSCHAN: channel=%d, norm=%d\n",
				     vchan->channel, vchan->norm);
			if (vchan->channel > 2)
				return -EINVAL;
			if (vchan->channel == voodoo_tv->input &&
			    vchan->norm == voodoo_tv->norm)
				return 0;
			if ((vchan->norm != VIDEO_MODE_PAL) &&
			    (vchan->norm != VIDEO_MODE_NTSC) &&
			    (vchan->norm != VIDEO_MODE_SECAM))
				return -EINVAL;

			voodoo_tv->channel = vchan->channel;
			voodoo_tv->norm = vchan->norm;
			voodoo_channels[vchan->channel].norm = voodoo_tv->norm;

			i2c_clients_command(voodoo_tv->voodoo_i2c_adapter,
						DECODER_SET_INPUT,
						&vchan->channel);
			i2c_clients_command(voodoo_tv->voodoo_i2c_adapter,
						DECODER_SET_NORM,
						&voodoo_tv->norm);

			v3tv_overlay_setup(voodoo_tv);

			i2c_clients_command(voodoo_tv->voodoo_i2c_adapter,
						VIDIOCSCHAN, &vchan);

			sarg = (voodoo_tv->channel) ? AUDIO_EXTERN_2 :
							AUDIO_TUNER;
			i2c_clients_command(voodoo_tv->voodoo_i2c_adapter,
						AUDC_SET_INPUT, &sarg);

			return 0;
		}
	case VIDIOCGPICT:
		{
			struct video_picture *pic = arg;

			if (debug)
				printk("v3tv: VIDIOCGPICT\n");

			/* Return picture information here */
			pic->brightness	= voodoo_tv->picture_settings.brightness;
			pic->hue	= voodoo_tv->picture_settings.hue;
			pic->colour	= voodoo_tv->picture_settings.contrast;
			pic->contrast	= voodoo_tv->picture_settings.colour;
			pic->whiteness	= voodoo_tv->picture_settings.whiteness;
			pic->depth	= voodoo_tv->picture_settings.depth;
			pic->palette	= voodoo_tv->picture_settings.palette;

			if (debug)
				printk(KERN_INFO
					"v3tv: VIDIOCGPICT, video_picture-> "
					"brightness: 0x%04x hue: 0x%04x "
					"contrast: 0x%04x colour: 0x%04x "
					"w: 0x%04x\n",
					voodoo_tv->picture_settings.brightness,
					voodoo_tv->picture_settings.hue,
					voodoo_tv->picture_settings.contrast,
					voodoo_tv->picture_settings.colour,
					voodoo_tv->picture_settings.whiteness);
			if (debug)
				printk(KERN_INFO
				       "v3tv: VIDIOCGPICT, depth = %d, "
					"palette = %d %s\n",
				       voodoo_tv->picture_settings.depth,
				       voodoo_tv->picture_settings.palette,
				       desc_pict_palette[
					voodoo_tv->picture_settings.palette] );
			return 0;
		}
	case VIDIOCSPICT:
		{
			struct video_picture *pic = arg;

			if (debug)
				printk(KERN_INFO
					"v3tv: VIDIOCSPICT, video_picture-> "
					"brightness: 0x%04x hue: 0x%04x "
					"contrast: 0x%04x colour: 0x%04x "
					"w: 0x%04x\n",
					pic->brightness, pic->hue, pic->contrast,
					pic->colour, pic->whiteness);
			if (debug)
				printk(KERN_INFO
				       "v3tv: VIDIOCSPICT, depth = %d, "
					"palette = %d %s\n",
				       pic->depth, pic->palette,
				       desc_pict_palette[pic->palette]);
#if 1
			/* Set picture parameters here */
			if ((pic->palette != VIDEO_PALETTE_RGB565) &&
			    (pic->palette != VIDEO_PALETTE_RGB24)  &&
			    (pic->palette != VIDEO_PALETTE_RGB32))
				v3tv_set_palette(pic);
#else
			if (pic->palette != voodoo_tv->picture_settings.palette) {
				pic->palette = voodoo_tv->picture_settings.palette;
				printk(KERN_INFO
				       "v3tv: VIDIOCSPICT: pallete not supported\n");
				return -EINVAL;
			}
#endif
			voodoo_tv->picture_settings.brightness	= pic->brightness;
			voodoo_tv->picture_settings.hue		= pic->hue;
			voodoo_tv->picture_settings.contrast	= pic->contrast;
			voodoo_tv->picture_settings.colour	= pic->colour;
			voodoo_tv->picture_settings.whiteness	= pic->whiteness;
			voodoo_tv->picture_settings.depth	= pic->depth;
//			voodoo_tv->picture_settings.palette	= pic->palette;

			i2c_clients_command(voodoo_tv->voodoo_i2c_adapter,
						DECODER_SET_PICTURE,
						&voodoo_tv->picture_settings);
			return 0;
		}

	case VIDIOCGWIN:
		{
			/* return the current capture window here */
			struct video_window *win = arg;

			memset(win,0,sizeof(*win));
			if (debug)
				(printk("v3tv: VIDIOCGWIN: %d, %d, %d, %d\n",
					voodoo_tv->capture_win.x,
					voodoo_tv->capture_win.y,
					voodoo_tv->capture_win.width,
					voodoo_tv->capture_win.height));

			win->x      = voodoo_tv->capture_win.x;
			win->y      = voodoo_tv->capture_win.y;
			win->width  = voodoo_tv->capture_win.width;
			win->height = voodoo_tv->capture_win.height;
			return 0;
		}

		case VIDIOCSWIN:
		{
			struct video_window *win = arg;
			unsigned long chroma;
			unsigned long R, G, B;

			/* set the capture window here */

			if (win->width > 1280 || win->height > 960)
				return -EINVAL;
			if (win->width < 16 || win->height < 16)
				return -EINVAL;

			/* FIXME: an ugly hack to get chroma working */

			R = (win->chromakey & 0xFF0000) >> 16;
			G = (win->chromakey & 0x00FF00) >> 8;
			B = (win->chromakey & 0x0000FF);

			// Stop the seq_fault Divide by zero.
			if (voodoo_tv->fb.width == 0) {
				printk
				    ("v3tv: Frame buffer width is 0! divide by zero in VIDIOCSWIN");
				return -EINVAL;
			}
			switch (voodoo_tv->fb.bytesperline / voodoo_tv->fb.width) {
			case 1:
				R = R >> 5;
				G = G >> 5;
				B = B >> 6;

				chroma = 0;
				chroma |= R & 0x7;
				chroma |= (G << 3) & 0x38;
				chroma |= (B << 6) & 0xC0;

				break;
			case 2:
				R = R >> 3;
				G = G >> 2;
				B = B >> 3;

				chroma = 0;
				chroma |= ((R << 11) & 0xF800);
				chroma |= ((G << 5) & 0x7E0);
				chroma |= (B & 0x1F);

				break;
			default:
				chroma = win->chromakey;
				break;
			}

			voodoo_tv->capture_win.x	= win->x;
			voodoo_tv->capture_win.y	= win->y;
			voodoo_tv->capture_win.width	= win->width;
			voodoo_tv->capture_win.height	= win->height;
			voodoo_tv->capture_win.chromakey = win->chromakey;
			voodoo_tv->chroma = chroma;

			v3tv_set_chromakey(voodoo_tv);
			v3tv_set_overlay(voodoo_tv);

			if (debug)
				printk
				    ("v3tv: VIDIOCSWIN: %d, %d, %d, %d\n",
				     voodoo_tv->capture_win.x,
				     voodoo_tv->capture_win.y,
				     voodoo_tv->capture_win.width,
				     voodoo_tv->capture_win.height);

			return 0;
		}

	case VIDIOCCAPTURE:
		{
			/* turn on/off overlay here */
			int *on = arg;

			if (debug)
				printk("v3tv: VIDIOCCAPTURE %s\n",
				       (*on) ? "Enable" : "Disable");
/*			if (v)
				if(capture_fb.width == 0 || capture_w == 0)
					return -EINVAL;
*/
			v3tv_overlay(voodoo_tv, *on);
			return 0;
		}

	case VIDIOCSFBUF:
		{
			struct video_buffer *fbuf = arg;

			voodoo_tv->fb.base	= fbuf->base;
			voodoo_tv->fb.height	= fbuf->height;
			voodoo_tv->fb.width	= fbuf->width;
			voodoo_tv->fb.depth	= fbuf->depth;
			voodoo_tv->fb.bytesperline = fbuf->bytesperline;

			if (debug)
				printk
				    ("v3tv: VIDIOCSFBUF: base 0x%p height: %d "
				     "width %d depth %d bytesperline %d\n",
				     voodoo_tv->fb.base, voodoo_tv->fb.height,
				     voodoo_tv->fb.width, voodoo_tv->fb.depth,
				     voodoo_tv->fb.bytesperline);
			return 0;
		}
	case VIDIOCGFBUF:
		{
			struct video_buffer *fbuf = arg;

			fbuf->base	= voodoo_tv->fb.base;
			fbuf->height	= voodoo_tv->fb.height;
			fbuf->width	= voodoo_tv->fb.width;
			fbuf->depth	= voodoo_tv->fb.depth;
			fbuf->bytesperline = voodoo_tv->fb.bytesperline;

			return 0;
		}
	case VIDIOCGTUNER:
		{
			struct video_tuner *tuner_info = arg;
			if (debug)
				printk("v3tv: VIDIOCGTUNER\n");

			if (tuner_info->tuner != 0)
				return -EINVAL;

			strcpy(tuner_info->name, "TV Tuner");
			tuner_info->rangelow  = 0;
			tuner_info->rangehigh = 0x7FFFFFFF;
			tuner_info->flags =
				VIDEO_TUNER_PAL | VIDEO_TUNER_NTSC |
				VIDEO_TUNER_SECAM;
			/* XXX VIDEO_TUNER_STEREO_ON   Tuner is seeing stereo, what is RDS and MBS? */
			tuner_info->mode = voodoo_tv->norm;
			/* Get signal strength from tuner */
			i2c_clients_command(voodoo_tv->voodoo_i2c_adapter,
						VIDIOCGTUNER, tuner_info);

			return 0;
		}
	case VIDIOCSTUNER:
		{
			struct video_tuner *tuner_info = arg;

			if (tuner_info->tuner) /* Only tuner 0 */
				return -EINVAL;
			voodoo_tv->norm = tuner_info->mode;
			/* Get signal strength from tuner */
			i2c_clients_command(voodoo_tv->voodoo_i2c_adapter,
						VIDIOCGTUNER, tuner_info);
			i2c_clients_command(voodoo_tv->voodoo_i2c_adapter,
						DECODER_SET_NORM,
						&voodoo_tv->norm);
			return 0;
		}

	case VIDIOCGFREQ:
		{
			if (debug)
				printk("v3tv: VIDIOCGFREQ\n");

			/* get tuner frequency here */
			/* out: int tuner_frequency */
			if (put_user(arg, &voodoo_tv->freq)) {
				if (debug)
					printk("v3tv: VIDIOCGFREQ put_user failed!!\n");
				return -EFAULT;
			}
			return 0;
		}
	case VIDIOCSFREQ:
		{
			unsigned long *freq = arg;
			struct video_channel vchan;

			if (debug)
				printk("v3tv: VIDIOCSFREQ %ld\n", *freq);
			/* set tuner frequency here */
			/* in: int tuner_frequency */

			voodoo_tv->freq = *freq;
			voodoo_tv->channel = 0;	/* Force video input to tuner */
			vchan.channel = 0;

			i2c_clients_command(voodoo_tv->voodoo_i2c_adapter,
						VIDIOCSCHAN, &vchan);
			i2c_clients_command(voodoo_tv->voodoo_i2c_adapter,
						VIDIOCSFREQ, freq);
			return 0;
		}

	case VIDIOCGAUDIO:
		{
			if (debug)
				printk("v3tv: VIDIOCGAUDIO\n");
			i2c_clients_command(voodoo_tv->voodoo_i2c_adapter,
						VIDIOCGAUDIO, (int *)arg);

			return 0;
		}
	case VIDIOCSAUDIO:
		{
			if (debug)
				printk("v3tv: VIDIOCSAUDIO\n");
			i2c_clients_command(voodoo_tv->voodoo_i2c_adapter,
						VIDIOCSAUDIO, (int *)arg);

			return 0;
		}

	case VIDIOCSYNC:
		{
			if (debug)
				printk("v3tv: VIDIOCSYNC not implemented.\n");
			return -EFAULT;
//                      int i;
//                      DECLARE_WAITQUEUE(wait, current);
//
//                      if(copy_from_user((void *)&i,arg,sizeof(int))) {
//				printk("v3tv: VIDIOCSYNC couldn't copy from user.\n");
//                              return -EFAULT;
//			}
//
//                      if (i < 0 || i >= voodoo_tv->gbuffers)
//                              return -EINVAL;
//                      switch (voodoo_tv->gbuf[i].stat)
//                      {
//                              case GBUFFER_UNUSED:
//                                      ret = -EINVAL;
//                                      break;
//                              case GBUFFER_GRABBING:
//                                      add_wait_queue(&voodoo_tv->capq, &wait);
//                                      current->state = TASK_INTERRUPTIBLE;
//                                      while(voodoo_tv->gbuf[i].stat==GBUFFER_GRABBING) {
//                                              schedule();
//                                              if(signal_pending(current)) {
//                                                      remove_wait_queue(&voodoo_tv->capq, &wait);
//                                                      current->state = TASK_RUNNING;
//                                                      return -EINTR;
//                                              }
//                                      }
//                                      remove_wait_queue(&voodoo_tv->capq, &wait);
//                                      current->state = TASK_RUNNING;
//                              /* fall through */
//                              case GBUFFER_DONE:
//                              case GBUFFER_ERROR:
//                                      ret = (voodoo_tv->gbuf[i].stat == GBUFFER_ERROR) ? -EIO : 0;
//                                      voodoo_tv->gbuf[i].stat = GBUFFER_UNUSED;
//                      }
//                      return ret;
		}

	case VIDIOCMCAPTURE:
		{
			if (debug)
				printk
				    ("v3tv: VIDIOCMCAPTURE not implemented.\n");
			return -EFAULT;
//                      struct video_mmap vm;
//                      int ret;
//                      if(copy_from_user((void *) &vm, (void *) arg, sizeof(vm))) {
//				printk("v3tv: VIDIOCMCAPTURE couldn't copy from user.\n");
//                              return -EFAULT;
//			}

//                      down(&voodoo_tv->lock);
//                      ret = vgrab(voodoo_tv, &vm);
//                      up(&voodoo_tv->lock);
//                      return ret;
		}

	case VIDIOCGMBUF:
		{
			if (debug)
				printk("v3tv: VIDIOCMBUF not implemented.\n");
			return -EFAULT;
//                      struct video_mbuf vm;
//                      memset(&vm, 0 , sizeof(vm));

//                      vm.size=gbufsize*gbuffers;
//                      vm.frames=gbuffers;
//                      for (i = 0; i < gbuffers; i++)
//                              vm.offsets[i]=i*gbufsize;
//                      if(copy_to_user((void *)arg, (void *)&vm, sizeof(vm)))
//                              return -EFAULT;
//                      return 0;
		}

	default:
	    if (debug > 1) {
		switch (_IOC_TYPE(cmd)) {
		case 'v':
			printk("v3tv: unsuported ioctl 0x%x (v4l1, VIDIOC%s)\n",
			       cmd, (_IOC_NR(cmd) < V4L1_IOCTLS) ?
			       v4l1_ioctls[_IOC_NR(cmd)] : "???");
			break;
		case 'V':
			printk("v3tv: unsupported ioctl 0x%x (v4l2, %s)\n",
			       cmd,  v4l2_ioctl_names[_IOC_NR(cmd)]);
			break;
		default:
			printk("v3tv: unknown ioctl 0x%x (\?\?\?)\n",
			       cmd);
		}
	    }
	    return -ENOIOCTLCMD;
	}
	return 0;
}

static int v3tv_video_ioctl(struct inode *inode, struct file *file,
			    unsigned int cmd, unsigned long arg)
{
	return video_usercopy(inode, file, cmd, arg, v3tv_video_do_ioctl);
}

static struct file_operations v3tv_vdev_radio_fops = {
	.owner = THIS_MODULE,
	.open = v3tv_radio_open,
	.release = v3tv_radio_release,
	.ioctl = v3tv_radio_ioctl,
	.llseek = no_llseek
};

static struct video_device v3tv_vdev_radio = {
	.owner = THIS_MODULE,
	.name = "Voodoo3 3500 TV radio",
	.type = VID_TYPE_TUNER,
	.hardware = VID_HARDWARE_V3TV_RADIO,	/* FIXME: Define these in a .h file */
	.fops = &v3tv_vdev_radio_fops,
	.minor    = -1,
};

static struct file_operations v3tv_vdev_video_fops = {
	.owner = THIS_MODULE,
	.open = v3tv_video_open,
	.release = v3tv_video_release,
	.ioctl = v3tv_video_ioctl,
	.llseek = no_llseek,
	.read = v3tv_video_read,	/* experimental read */
//      .mmap     = v4l_voodoo_mmap,
//      .poll     = v4l_voodoo_poll,
};

static struct video_device v3tv_vdev_video = {
	.owner = THIS_MODULE,
	.name = "Voodoo3 3500 TV",
	.type =
	    VID_TYPE_CAPTURE | VID_TYPE_TUNER | VID_TYPE_OVERLAY |
	    VID_TYPE_CHROMAKEY | VID_TYPE_SCALES,
	.hardware = VID_HARDWARE_V3TV,	/* temporary id */
	.fops = &v3tv_vdev_video_fops,
	.minor    = -1,
};

static struct video_device v3tv_vdev_vbi = {
	.owner = THIS_MODULE,
	.name = "Voodoo3 3500 VBI Teletext",
	.type     = VID_TYPE_TUNER|VID_TYPE_TELETEXT,
	.hardware = VID_HARDWARE_V3TV,	/* temporary id */
	.fops = &v3tv_vdev_video_fops,
	.minor    = -1,
};

static void v3tv_unregister_video(struct voodoo_data *voodoo)
{
	if (voodoo->video_dev) {
		if (-1 != voodoo->video_dev->minor) {
			printk("v3tv: unregistering device video%d (%d)\n",
				voodoo->video_dev->minor & 0x1f,
				voodoo->video_dev->minor);
			video_unregister_device(voodoo->video_dev);
		}
		voodoo->video_dev = NULL;
	}

	if (voodoo->vbi_dev) {
		if (-1 != voodoo->vbi_dev->minor) {
			printk("v3tv: unregistering device vbi%d (%d)\n",
				voodoo->vbi_dev->minor & 0x1f,
				voodoo->vbi_dev->minor);
			video_unregister_device(voodoo->vbi_dev);
		}
		voodoo->vbi_dev = NULL;
	}

	if (voodoo->radio_dev) {
		if (-1 != voodoo->radio_dev->minor) {
			printk("v3tv: unregistering device radio%d (%d)\n",
				voodoo->radio_dev->minor & 0x1f,
				voodoo->radio_dev->minor);
			video_unregister_device(voodoo->radio_dev);
		}
		voodoo->radio_dev = NULL;
	}
}

/* register video4linux devices */
static int __devinit v3tv_register_video(struct voodoo_data *voodoo)
{
	/* video */
	voodoo->video_dev = &v3tv_vdev_video;
	if (video_register_device(voodoo->video_dev,VFL_TYPE_GRABBER,video_nr)<0)
		goto err;
	printk(KERN_INFO "v3tv: registered device video%d (%d)\n",
	       voodoo->video_dev->minor & 0x1f, voodoo->video_dev->minor);

	/* vbi */
	voodoo->vbi_dev = &v3tv_vdev_vbi;
	if (video_register_device(voodoo->vbi_dev,VFL_TYPE_VBI,vbi_nr)<0)
		goto err;
	printk(KERN_INFO "v3tv: registered device vbi%d (%d)\n",
	       voodoo->vbi_dev->minor & 0x1f, voodoo->vbi_dev->minor);

	/* radio */
	voodoo->radio_dev = &v3tv_vdev_radio;
	if (video_register_device(voodoo->radio_dev, VFL_TYPE_RADIO,radio_nr)<0)
		goto err;
	printk(KERN_INFO "v3tv: registered device radio%d (%d)\n",
	       voodoo->radio_dev->minor & 0x1f, voodoo->radio_dev->minor);

	/* all done */
	return 0;

 err:
	v3tv_unregister_video(voodoo);
	return -1;
}

int v3tv_video_init(struct voodoo_data *voodoo)
{
//	int i;

	switch (voodoo->model) {
	case VOODOO3_MODEL_NTSC:
		printk(KERN_INFO "v3tv: NTSC model.\n");
		snprintf(voodoo->name, sizeof(voodoo->name),
			"Voodoo3 TV 3500 %s", "NTSC");
		voodoo->norm = VIDEO_MODE_NTSC;
		voodoo->tuner_type = TUNER_PHILIPS_NTSC;	/* 2 */
		break;
	case VOODOO3_MODEL_PAL:
		printk(KERN_INFO "v3tv: PAL model.\n");
		snprintf(voodoo->name, sizeof(voodoo->name),
			"Voodoo3 TV 3500 %s", "PAL");
		voodoo->norm = VIDEO_MODE_PAL;
		voodoo->tuner_type = TUNER_Samsung_PAL_TCPM9091PD27;
		break;
	case VOODOO3_MODEL_SECAM:
		printk(KERN_INFO "v3tv: SECAM model.\n");
		snprintf(voodoo->name, sizeof(voodoo->name),
			"Voodoo3 TV 3500 %s", "SECAM");
		voodoo->norm = VIDEO_MODE_SECAM;
		voodoo->tuner_type = TUNER_PHILIPS_SECAM;	/* 3 */
		break;
	default:
		printk(KERN_INFO
		       "v3tv: Unknown model: 0x%x, defaulting to PAL.\n",
		       voodoo->model);
		snprintf(voodoo->name, sizeof(voodoo->name),
			"Voodoo3 TV 3500 model 0x%x", voodoo->model);
		voodoo->norm = VIDEO_MODE_PAL;
		voodoo->tuner_type = TUNER_Samsung_PAL_TCPM9091PD27;
		break;
	}

//	for (i=0;i<3;i++) {
//		voodoo_channels[i].norm = voodoo->norm;
//	}

	if (voodoo->tuner_type != UNSET) {
		struct tuner_setup tun_setup;

		tun_setup.mode_mask = T_RADIO | T_ANALOG_TV;
		tun_setup.type = voodoo->tuner_type;
		tun_setup.addr = ADDR_UNSET;

	i2c_clients_command(voodoo->voodoo_i2c_adapter,
				TUNER_SET_TYPE_ADDR, &tun_setup);
	}

	voodoo->picture_settings.brightness = 0x8000;
	voodoo->picture_settings.hue = 0x8000;
	voodoo->picture_settings.colour = 0x8000;
	voodoo->picture_settings.contrast = 0x8000;
	voodoo->picture_settings.whiteness = 0x8000;
	voodoo->picture_settings.depth = 0;
	voodoo->picture_settings.palette = 0;

	voodoo->vidOverlayStartX = 0;
	voodoo->vidOverlayStartY = 0;
	voodoo->vidOverlayEndX = video_standard[voodoo->norm].width;
	voodoo->vidOverlayEndY = video_standard[voodoo->norm].height * 2;
	voodoo->vidOrigWidth = video_standard[voodoo->norm].width;
	voodoo->vidOrigHeight = video_standard[voodoo->norm].height;

	voodoo->capture_win.x = 0;
	voodoo->capture_win.y = 0;
	voodoo->capture_win.width = video_standard[voodoo->norm].width;
	voodoo->capture_win.height = video_standard[voodoo->norm].height;

	v3tv_register_video(voodoo);

	return 0;
}

void v3tv_video_cleanup(struct voodoo_data *voodoo)
{
	v3tv_unregister_video(voodoo);

	voodoo->initialized &= ~I_v4l2;
}

/*
 * voodoo.c - Part of v4l voodoo3 drivers
 *
 * $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/modules/public/usb_camera/v4l_experimental/v3tv/v3tv.c#1 $
 */

/* Detect whether a Voodoo3 can be found, and map its memory. */
static int v3tv_probe(void)
{
	struct pci_dev *dev = NULL;
	u16 subsys_id;

	while ((dev = pci_find_device(PCI_VENDOR_ID_3DFX,
				      PCI_DEVICE_ID_3DFX_VOODOO3, dev))) {
		subsys_id = dev->subsystem_device;
		if ((subsys_id == VOODOO3_MODEL_NTSC) ||
		    (subsys_id == VOODOO3_MODEL_PAL) ||
		    (subsys_id == VOODOO3_MODEL_SECAM))
			break;
	}

	if (!dev) {
		printk(KERN_INFO "v3tv: No Voodoo3 3500 TV found.\n");
		return -ENODEV;
	}

	printk(KERN_INFO "v3tv: Voodoo3 found.\n");
	if (!model)
		if (debug)
			printk(KERN_INFO
			       "v3tv: Autodetecting Voodoo3 model.\n");
	voodoo->model = (model) ? model : dev->subsystem_device;

	/* map Voodoo3 memory. */
	voodoo->io_base = ioremap_nocache(dev->resource[0].start &
					  PCI_BASE_ADDRESS_MEM_MASK, 0x200400);
	if (!voodoo->io_base)
		return (-ENOMEM);

	voodoo->fb_base = ioremap(dev->resource[1].start &
				  PCI_BASE_ADDRESS_MEM_MASK, 1024 * 1024 * 16);
	if (!voodoo->fb_base) {
		iounmap(voodoo->io_base);
		return -ENOMEM;
	}

	if (debug)
		printk
		    ("v3tv: Registers mapped @ 0x%lx, Frame buffer mapped @ 0x%lx\n",
		     (unsigned long)voodoo->io_base,
		     (unsigned long)voodoo->fb_base);

	voodoo->dev = dev;

	return 0;
}

static void v3tv_cleanup_module(void)
{
	if (voodoo->initialized & I_v4l2)
		v3tv_video_cleanup(voodoo);

	v3tv_i2c_setup(0);

	if (voodoo->io_base)
		iounmap(voodoo->io_base);

	if (voodoo->fb_base)
		iounmap(voodoo->fb_base);

	kfree(voodoo);
}

static int v3tv_init_module(void)
{
	int res;

	printk("v3tv: version %s (%s)\n", V3_VERSION, __DATE__);

	voodoo = kmalloc(sizeof(struct voodoo_data), GFP_KERNEL);
	if (!voodoo)
		return -ENOMEM;

	memset(voodoo, 0, sizeof(struct voodoo_data));	/* Paranoia: Clear data */

	if ((res = v3tv_probe())) {
		printk("v3tv: Voodoo3 not detected, module not inserted.\n");
		v3tv_cleanup_module();
		return res;
	}

//	init_MUTEX(&voodoo->lock);
	/* Configure the chip to initial state */
	v3tv_conf(voodoo3_init_reg, CONF_SIZE(voodoo3_init_reg));

	/* activate i2c */
	if (v3tv_i2c_setup(1))
		return -EINVAL;

	if (v3tv_video_init(voodoo))
		return -EINVAL;

	voodoo->initialized |= I_v4l2;

	/* set up overlay and videobuffers */
	v3tv_overlay_setup(voodoo);

	return 0;
}

#ifdef MODULE
MODULE_AUTHOR("Juha Valkama <juhisv@surfree.com>");
MODULE_DESCRIPTION("Voodoo3 3500 TV Video4Linux driver");
MODULE_LICENSE("GPL");

MODULE_PARM(debug, "i");
MODULE_PARM_DESC(debug, "Debug flag (0, 1 or 2)");
MODULE_PARM(model, "i");
MODULE_PARM_DESC(model, "Voodoo3 submodel ID (0x60, 0x61, or 0x62");
#ifdef INCLUDE_I2C
MODULE_PARM(ddc, "i");
MODULE_PARM_DESC(ddc,
		 "Set to 0 to disable DDC init (if you don't have a DDC monitor)");
#endif

MODULE_PARM(video_nr,"i");
MODULE_PARM(vbi_nr,"i");
MODULE_PARM(radio_nr,"i");

module_init(v3tv_init_module);
module_exit(v3tv_cleanup_module);
#endif				/* MODULE */

/*
 * Overrides for Emacs so that we follow Linus's tabbing style.
 * ---------------------------------------------------------------------------
 * Local variables:
 * c-basic-offset: 8
 * End:
 */
