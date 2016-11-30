//added by racing
#ifndef TC3162L2
#define TC3162L2
#endif
#ifndef BIG_ENDIAN
#define BIG_ENDIAN
#endif
#define USB_COST_DOWN_VERSION
//#define USE_USB_NEXT_LINK_PTR
//#define FAST_MBUF_RELEASE
#define DRIVER_NAME "tc3162_udc"
#define EP0_NAME "ep0"
#define EP1_NAME "ep1in-bulk"
#define EP2_NAME "ep2out-bulk"
#define EP3_NAME "ep3in-int"
#define NO_STATUS 0
#define STATUS 1
#define ACK_STATUS 2
#define NOACK_STATUS 3

#define	TC3162_UDC_NUM_ENDPOINTS	4
#define MAX_FIFO_NUM 10
#define MAX_FIFO_HISTORY_NUM 1024
#define EP0_FIFO_SIZE	0x08
#define BULK_FIFO_SIZE	((unsigned)64)
#define INT_FIFO_SIZE	((unsigned)8)
#define USB_MAX_CTRL_ENDP_IN_QUE_SIZE	8
#define USB_CTRL_BUF_SIZE 	0x100

#define   BULK_IN     0
#define   BULK_OUT    1

//#define NO_GADGET 1
#define GADGET_ETHER 1
#define GADGET_FILE_STORAGE 2

#define TRUE 1
#define FALSE 0

#define NONE 0
#define TEMPORARY 1
#define ORIGINAL 2

#define ON 1
#define OFF 0

#define SET_CONF 1
#define NOT_SET_CONF 0

#ifdef  USB_COST_DOWN_VERSION
  #define USB_DEVICE_PARAMETER_NUM    4
#else
  #define USB_DEVICE_PARAMETER_NUM    19
#endif

#define   USB_SOFTWARE    0
#define   USB_HARDWARE    1

#define   USB_BULK_ISO_IN_DESCP_NUM   32//64
#define   USB_BULK_ISO_OUT_DESCP_NUM  16//32

#define littleToBig_32(n)(\
	((((u32)(n))>>24)&0x000000ff)|\
	((((u32)(n))>>8)&0x0000ff00)|\
	((((u32)(n))<<8)&0x00ff0000)|\
	((((u32)(n))<<24)&0xff000000))

#define littleToBig_16(n)(\
	((((u16)(n))>>8)&0x00ff)|\
	((((u16)(n))<<8)&0xff00))

#define NONE_STAGE 0
#define CTRL_MSG_RCVD 1
#define FIRST_OUT_MESG_RCVE 2

#define USB_EP0_PACKET_SIZE       0x08
#define USB_EP1_PACKET_SIZE  	  0x0040
#define USB_EP2_PACKET_SIZE  	  0x0040
#define USB_EP1_ALT0_PACKET_SIZE  0x0000
#define USB_EP2_ALT0_PACKET_SIZE  0x0000
#define USB_EP1_ALT1_PACKET_SIZE  0x0040
#define USB_EP2_ALT1_PACKET_SIZE  0x0040
#define USB_EP1_ALT2_PACKET_SIZE  0x0080
#define USB_EP2_ALT2_PACKET_SIZE  0x0080
#define USB_EP1_ALT3_PACKET_SIZE  0x0100
#define USB_EP2_ALT3_PACKET_SIZE  0x0100
#define USB_EP1_ALT4_PACKET_SIZE  0x0200
#define USB_EP2_ALT4_PACKET_SIZE  0x0200
#define USB_EP1_ALT5_PACKET_SIZE  0x0040
#define USB_EP2_ALT5_PACKET_SIZE  0x0040
#define USB_EP3_PACKET_SIZE       0x08

#define   usbRegWrite4Byte(addr,val)  ( *(volatile u32 *)(addr)  = val )
#define   usbRegWrite2Byte(addr,val)  ( *(volatile uint16 *)(addr)  = val )
#define   usbRegWrite1Byte(addr,val)  ( *(volatile uint8 *)(addr)   = val )
#define   usbRegRead4Byte(addr,val)   ( val = *(volatile u32 *)(addr)  )
#define   usbRegRead2Byte(addr,val)   ( val = *(volatile uint16 *)(addr))
#define   usbRegRead1Byte(addr,val)   ( val = *(volatile uint8 *)(addr) )

/* defined RNDIS and MAC Driver*/
#define RNDISDriver	1
#define MacDriver	2
#define MACFORTRENCHUP	3

struct tc3162_request {
	int index;
	struct usb_request req;
	struct list_head queue;
};

struct tc3162_ep {
	struct usb_ep ep;
	struct tc3162_udc *dev;

	const struct usb_endpoint_descriptor *desc;
	struct list_head queue;

	u8 bmAttributes;
};

struct tc3162_udc {
	struct usb_gadget gadget;
	struct usb_gadget_driver *driver;
	struct tc3162_ep ep[TC3162_UDC_NUM_ENDPOINTS];

	spinlock_t lock;
};

struct tc3162_task_param {
	struct usb_ep ep;
	struct usb_request req;
};

typedef struct usbFifoData_s {
	u32 word[2];
	u32 ctrl_ep_io_reg;
	u32 rsv;
} usbFifoData_t;

typedef struct usbFifoHistory_s {
	u32 word[2];
} usbFifoHistory_t;

typedef struct usbCtrlBuf_s {
	uint8 data[USB_CTRL_BUF_SIZE];
	u32 cnt;
} usbCtrlBuf_t;

typedef struct dmaBufferHeader_s{
	dma_addr_t dmaHandle;
	u32 bufSize;
	char reserved[24];
}dmaBufferHeader_t;
#if 0
/*******************************************************************
                         TX/RX Buffer Descriptor Pool
 *******************************************************************/

typedef struct usbInMemPool_s {
	uint8 descrPool[USB_BULK_ISO_IN_DESCP_NUM * 16 + 16];	/* Descr pool area */
} usbInMemPool_t;

typedef struct usbOutMemPool_s {
	uint8 descrPool[USB_BULK_ISO_OUT_DESCP_NUM * 16 + 16];	/* Descr pool area */
} usbOutMemPool_t;

#endif
/*******************************************************************
                          MACROs for REGISTER IO
 *******************************************************************/

#define   USB_SET_BULKISO_IN_DESCP_BASE_(x) \
          *(volatile u32 *)(CR_USB_BULKISO_IN_DESCP_BASE_REG)  = x;

#define   USB_SET_BULKISO_OUT_DESCP_BASE_(x) \
          *(volatile u32 *)(CR_USB_BULKISO_OUT_DESCP_BASE_REG) = x;

#define   USB_GET_CONTROL_ENDPOINT_OUT_DATA_(H,L) \
          L = *(volatile u32 *)(CR_USB_CTRL_ENDP_IO_OUT_REG0); \
          H = *(volatile u32 *)(CR_USB_CTRL_ENDP_IO_OUT_REG1);

#define   USB_SET_CONTROL_ENDPOINT_IN_DATA_(H,L) \
          *(volatile u32 *)(CR_USB_CTRL_ENDP_IO_IN_REG0) = L; \
          *(volatile u32 *)(CR_USB_CTRL_ENDP_IO_IN_REG1) = H;

#define   USB_SET_INTERRUPT_ENDPOINT_IN_DATA_(H, L) \
          *(volatile uint32 *)(CR_USB_INTR_IN_ENDP_IN_REG0) = L; \
          *(volatile uint32 *)(CR_USB_INTR_IN_ENDP_IN_REG1) = H;

#define   USB_SET_ISO_ENDP_DELIMITER_(H,L) \
          *(volatile u32 *)(CR_USB_ISO_ENDP_DELIMITER_REG0) = L; \
          *(volatile u32 *)(CR_USB_ISO_ENDP_DELIMITER_REG1) = (H&0x001fffff);

#define   USB_SET_BULKISO_IN_OUT_ENDP_NUM_(IN,OUT) \
          *(volatile u32 *)(CR_USB_BULKISO_INOUT_ENDP_NUM_REG) = \
          ((IN & 0x0000ffff) | ((OUT<<16) & 0xffff0000));

#define   USB_IN_POLL_DEMAND_(x) \
          x = *(volatile uint32 *)(CR_USB_BULKISO_ENDP_DMA_CTRL_REG) | 0x00000010; \
          *(volatile uint32 *)(CR_USB_BULKISO_ENDP_DMA_CTRL_REG) = x;

#define   USB_OUT_POLL_DEMAND_(x) \
          x = *(volatile uint32 *)(CR_USB_BULKISO_ENDP_DMA_CTRL_REG) | 0x00001000; \
          *(volatile uint32 *)(CR_USB_BULKISO_ENDP_DMA_CTRL_REG) = x;



#if !defined (K0_TO_K1)
#define   K0_TO_K1(x)       (((u32)(x)) | 0x20000000)  /* kseg0 to kseg1 */
#endif

#if !defined (K1_TO_K0)
#define   K1_TO_K0(x)       (((u32)(x)) & 0x9fffffff)  /* kseg1 to kseg0 */
#endif

#if !defined (K0_TO_PHYSICAL)
#define   K0_TO_PHYSICAL(x) (((u32)(x)) & 0x1fffffff)  /* kseg0 to physical */
#endif

#if !defined (K1_TO_PHYSICAL)
#define   K1_TO_PHYSICAL(x) (((u32)(x)) & 0x1fffffff)  /* kseg1 to physical */
#endif

#if !defined (PHYSICAL_TO_K0)
#define   PHYSICAL_TO_K0(x) (((u32)(x)) | 0x80000000)  /* physical to kseg0 */
#endif

#if !defined (PHYSICAL_TO_K1)
#define   PHYSICAL_TO_K1(x) (((u32)(x)) | 0xa0000000)  /* physical to kseg1 */
#endif

/*******************************************************************
                     USB SYSTEM CONTROL REGISTER
 *******************************************************************/

typedef union _usb_sys_ctrl_reg
{
    u32  value;
    #ifdef  BIG_ENDIAN
    struct
    {
      unsigned  pio_dma_reset : 1;    // System Reset
      unsigned  rsv           : 30;   // Reserved
      unsigned  big_endian    : 1;    // Big Endian Mode
    } bit;
    #else   // LITTLE_ENDIAN
    struct
    {
      unsigned  big_endian    : 1;    // Big Endian Mode
      unsigned  rsv           : 30;   // Reserved
      unsigned  pio_dma_reset : 1;    // System Reset
    } bit;
    #endif
} usb_sys_ctrl_reg, *pusb_sys_ctrl_reg;

/*******************************************************************
                     USB DEVICE CONTROL REGISTER
 *******************************************************************/

typedef union _usb_dev_ctrl_reg
{
    u32  value;
    #ifdef  BIG_ENDIAN
    struct
    {
      uint8   byte0;
      uint8   byte1;
      uint8   byte2;
      uint8   byte3;                // [31:0] == {byte0, byte1, byte2, byte3}
    } byte;
    struct
    {
      unsigned  udc_dev_connect     : 1;  // UDC Device Connect (RW)
      unsigned  udc_dev_SIE_reset   : 1;  // UDC Device SIE Reset (RW)
      unsigned  udc_dev_resume      : 1;  // UDC Device Resume (RW)
      unsigned  rsv2                : 3;  // Reserved
      unsigned  udc_cfg_read_done   : 1;  // UDC Configuration Dara Load Done (RO)
      unsigned  udc_cfg_writing     : 1;  // UDC Configuration Dara in writing (RO)
      unsigned  udc_cfg_data_port   : 8;  // UDC Alternative Interface Value (RW)
      unsigned  rsv1                : 1;  // Reserved
      unsigned  udc_alt_setting     : 3;  // UDC Alternative Setting Value (RO)
      unsigned  udc_if_value        : 2;  // UDC Interface Value (RO)
      unsigned  udc_cfg_value       : 2;  // UDC Configuration Value (RO)
      unsigned  rsv0                : 1;  // Reserved
      unsigned  udc_dev_addr        : 7;  // UDC Device Address (RO)
    } bit;
    #else   // LITTLE_ENDIAN
    struct
    {
      uint8   byte0;
      uint8   byte1;
      uint8   byte2;
      uint8   byte3;                // [31:0] == {byte3, byte2, byte1, byte0}
    } byte;
    struct
    {
      unsigned  udc_dev_addr        : 7;  // UDC Device Address (RO)
      unsigned  rsv0                : 1;  // Reserved
      unsigned  udc_cfg_value       : 2;  // UDC Configuration Value (RO)
      unsigned  udc_if_value        : 2;  // UDC Interface Value (RO)
      unsigned  udc_alt_setting     : 3;  // UDC Alternative Setting Value (RO)
      unsigned  rsv1                : 1;  // Reserved
      unsigned  udc_cfg_data_port   : 8;  // UDC Alternative Interface Value (RW)
      unsigned  udc_cfg_writing     : 1;  // UDC Configuration Dara in writing (RO)
      unsigned  udc_cfg_read_done   : 1;  // UDC Configuration Dara Load Done (RO)
      unsigned  rsv2                : 3;  // Reserved
      unsigned  udc_dev_resume      : 1;  // UDC Device Resume (RW)
      unsigned  udc_dev_SIE_reset   : 1;  // UDC Device SIE Reset (RW)
      unsigned  udc_dev_connect     : 1;  // UDC Device Connect (RW)
    } bit;
    #endif
} usb_dev_ctrl_reg, *pusb_dev_ctrl_reg;

/*******************************************************************
                   USB CONTROL ENDPOINT CONTROL REGISTER
 *******************************************************************/

typedef union _usb_ctrl_endp_io_ctrl_reg
{
    u32  value;
    #ifdef  BIG_ENDIAN
    struct
    {
      uint8   byte0;
      uint8   byte1;
      uint8   byte2;
      uint8   byte3;
    } byte;
    struct
    {
      unsigned  ctrl_endp_out_Rx_data_cnt : 4;
      unsigned  ctrl_endp_in_Tx_data_cnt  : 4;
      unsigned  ctrl_endp_out_stalled     : 1;
      unsigned  ctrl_endp_in_stalled      : 1;
      unsigned  out_token_Rx_data_owner   : 1;
      unsigned  in_token_Tx_data_owner    : 1;
      unsigned  rsv                       : 2;
      unsigned  ctrl_endp_stall           : 1;
      unsigned  ctrl_endp_enable          : 1;
      unsigned  ctrl_endp_number          : 16;
    } bit;
    #else   // LITTLE_ENDIAN
    struct
    {
      uint8   byte0;
      uint8   byte1;
      uint8   byte2;
      uint8   byte3;
    } byte;
    struct
    {
      unsigned  ctrl_endp_number          : 16;
      unsigned  ctrl_endp_enable          : 1;
      unsigned  ctrl_endp_stall           : 1;
      unsigned  rsv                       : 2;
      unsigned  in_token_Tx_data_owner    : 1;
      unsigned  out_token_Rx_data_owner   : 1;
      unsigned  ctrl_endp_in_stalled      : 1;
      unsigned  ctrl_endp_out_stalled     : 1;
      unsigned  ctrl_endp_in_Tx_data_cnt  : 4;
      unsigned  ctrl_endp_out_Rx_data_cnt : 4;
    } bit;
    #endif
} usb_ctrl_endp_io_ctrl_reg, *pusb_ctrl_endp_io_ctrl_reg;


/*******************************************************************
                   USB BULK/ISO ENDPOINT CONTROL REGISTER
 *******************************************************************/

typedef union _usb_bulkiso_endp_dma_ctrl_reg
{
    u32  value;
    #ifdef  BIG_ENDIAN
    struct
    {
      unsigned  rsv3                        : 17;   // bit 15 ~ 31
      unsigned  bulkiso_out_DMA_reset       : 1;
      unsigned  bulkiso_out_DMA_disable     : 1;
      unsigned  bulkiso_out_DMA_enable      : 1;
      unsigned  bulkiso_out_endp_stalled    : 1;
      unsigned  rsv2                        : 1;    // bit 10
      unsigned  bulkiso_out_endp_stall      : 1;
      unsigned  bulkiso_out_endp_enable     : 1;
      unsigned  rsv1                        : 1;    // bit 7
      unsigned  bulkiso_in_DMA_reset        : 1;
      unsigned  bulkiso_in_DMA_disable      : 1;
      unsigned  bulkiso_in_DMA_enable       : 1;
      unsigned  bulkiso_in_endp_stalled     : 1;
      unsigned  rsv0                        : 1;    // bit 2
      unsigned  bulkiso_in_endp_stall       : 1;
      unsigned  bulkiso_in_endp_enable      : 1;
    } bit;
    #else   // LITTLE_ENDIAN
    struct
    {
      unsigned  bulkiso_in_endp_enable      : 1;
      unsigned  bulkiso_in_endp_stall       : 1;
      unsigned  rsv0                        : 1;    // bit 2
      unsigned  bulkiso_in_endp_stalled     : 1;
      unsigned  bulkiso_in_DMA_enable       : 1;
      unsigned  bulkiso_in_DMA_disable      : 1;
      unsigned  bulkiso_in_DMA_reset        : 1;
      unsigned  rsv1                        : 1;    // bit 7
      unsigned  bulkiso_out_endp_enable     : 1;
      unsigned  bulkiso_out_endp_stall      : 1;
      unsigned  rsv2                        : 1;    // bit 10
      unsigned  bulkiso_out_endp_stalled    : 1;
      unsigned  bulkiso_out_DMA_enable      : 1;
      unsigned  bulkiso_out_DMA_disable     : 1;
      unsigned  bulkiso_out_DMA_reset       : 1;
      unsigned  rsv3                        : 17;   // bit 15 ~ 31
    } bit;
    #endif
} usb_bulkiso_endp_dma_ctrl_reg, *pusb_bulkiso_endp_dma_ctrl_reg;


/*******************************************************************
                    USB INTERRUPT MASK/STATUS REGISTER
 *******************************************************************/

typedef union _usb_intr_reg
{
    u32  value;
#ifdef  BIG_ENDIAN
    struct
    {
      unsigned  SIE_bus_error                   : 1;
    	unsigned  rsv1                            : 10;
    	unsigned  SIE_bus_host_resume_detected    : 1;
      unsigned  SIE_set_interface_valid         : 1;
      unsigned  SIE_set_configuration_valid     : 1;
      unsigned  SIE_bus_reset_detected          : 1;
      unsigned  SIE_bus_suspend_detected        : 1;
      unsigned  rsv0                            : 2;
      unsigned  intr_in_endp_transfer_nak       : 1;
      unsigned  intr_in_endp_transfer_ack       : 1;
      unsigned  bulk_iso_out_fifo_over_run      : 1;
      unsigned  bulk_iso_out_end_of_descp       : 1;
      unsigned  bulk_iso_out_buffer_not_ready   : 1;
      unsigned  bulk_iso_out_endp_descp_updated : 1;
      unsigned  bulk_iso_in_fifo_under_run      : 1;
      unsigned  bulk_iso_in_end_of_descp        : 1;
      unsigned  bulk_iso_in_endp_descp_updated  : 1;
      unsigned  ctrl_endp_setup_token_decoded   : 1;
      unsigned  ctrl_endp_out_transfer_nak      : 1;
      unsigned  ctrl_endp_out_transfer_ack      : 1;
      unsigned  ctrl_endp_in_transfer_nak       : 1;
      unsigned  ctrl_endp_in_transfer_ack       : 1;
    } bit;
#else   // LITTLE_ENDIAN
    struct
    {
      unsigned  ctrl_endp_in_transfer_ack       : 1;
      unsigned  ctrl_endp_in_transfer_nak       : 1;
      unsigned  ctrl_endp_out_transfer_ack      : 1;
      unsigned  ctrl_endp_out_transfer_nak      : 1;
      unsigned  ctrl_endp_setup_token_decoded   : 1;
      unsigned  bulk_iso_in_endp_descp_updated  : 1;
      unsigned  bulk_iso_in_end_of_descp        : 1;
      unsigned  bulk_iso_in_fifo_under_run      : 1;
      unsigned  bulk_iso_out_endp_descp_updated : 1;
      unsigned  bulk_iso_out_buffer_not_ready   : 1;
      unsigned  bulk_iso_out_end_of_descp       : 1;
      unsigned  bulk_iso_out_fifo_over_run      : 1;
      unsigned  intr_in_endp_transfer_ack       : 1;
      unsigned  intr_in_endp_transfer_nak       : 1;
      unsigned  rsv0                            : 2;
      unsigned  SIE_bus_suspend_detected        : 1;
      unsigned  SIE_bus_reset_detected          : 1;
      unsigned  SIE_set_configuration_valid     : 1;
      unsigned  SIE_set_interface_valid         : 1;
    	unsigned  SIE_bus_host_resume_detected	  : 1;
      unsigned  rsv1                            : 10;
      unsigned  SIE_bus_error                   : 1;
    } bit;
#endif
}  usb_intr_status_reg,    usb_intr_mask_reg,
  *pusb_intr_status_reg,  *pusb_intr_mask_reg;

enum _interrupt_bit_define
{
    USB_CTRL_ENDP_IN_ACK                = 0x00000001,
    USB_CTRL_ENDP_IN_NAK                = 0x00000002,
    USB_CTRL_ENDP_OUT_ACK               = 0x00000004,
    USB_CTRL_ENDP_OUT_NAK               = 0x00000008,
    USB_CTRL_ENDP_SETUP_TOKEN_RCVED     = 0x00000010,   // default on
    USB_BULKISO_ENDP_IN_DESCP_UPDATED   = 0x00000020,   // default on
    USB_BULKISO_ENDP_IN_DESCP_END       = 0x00000040,
    USB_BULKISO_ENDP_IN_FIFO_UNDERRUN   = 0x00000080,
    USB_BULKISO_ENDP_OUT_DESCP_UPDATED  = 0x00000100,   // default on
    USB_BULKISO_ENDP_OUT_BUF_BUSY       = 0x00000200,
    USB_BULKISO_ENDP_OUT_DESCP_END      = 0x00000400,
    USB_BULKISO_ENDP_OUT_FIFO_OVERRUN   = 0x00000800,
    USB_INTR_ENDP_IN_ACK                = 0x00001000,
    USB_INTR_ENDP_IN_NAK                = 0x00002000,
    USB_SIE_BUS_SUSPEND                 = 0x00010000,   // default on
    USB_SIE_BUS_RESET                   = 0x00020000,   // default on
    USB_SET_CONF_COMPLETE               = 0x00040000,   // default on
    USB_SET_INF_COMPLETE                = 0x00080000,   // default on
    USB_SIE_BUS_HOST_RESUME						  = 0x00100000,
    USB_SIE_BUS_ERROR                   = 0x80000000    // default on
};

/*******************************************************************
                USB BULK/ISO DMA DESCRIPTOR STRUCTURE
 *******************************************************************/

typedef struct _usb_DMA_descp
{
  #ifdef  BIG_ENDIAN
  union
  {
    u32  value;
    struct
    {
      unsigned  next_ptr      : 30;   // Next Link Pointer  [31:2]
      unsigned  descp0_rsv    : 1;    // Reserved,          [1]
      unsigned  terminator    : 1;    // T,                 [0]
    } bit;
  } NextLinkPointer;
  union
  {
    u32  value;
    struct
    {
      unsigned  owner           : 1;    // Ownership,                   [31]
      unsigned  ioc             : 1;    // IOC,                         [30]
      unsigned  buf_unvalid     : 1;    // Buffer Not Available,        [29]
      unsigned  retry_cnt       : 2;    // Retry Count,                 [28:27]
      unsigned  data_length     : 11;   // Data Length received in OUT, [26:16]
      unsigned  descp1_rsv0     : 5;    // Reserved,                    [15:11]
      unsigned  buf_length      : 11;   // Buffer Length,               [10:0]
    } bit;
  } DescpCommandStatus;
  u32  BufferPointer;
  u32  RequestPointer;
  /*marked by racing
  mbuf_t  *MBufLinked;
  */
  #else   // LITTLE_ENDIAN
  union
  {
    u32  value;
    struct
    {
      unsigned  terminator    : 1;    // T,                 [0]
      unsigned  descp0_rsv    : 1;    // Reserved,          [1]
      unsigned  next_ptr      : 30;   // Next Link Pointer  [31:2]
    } bit;
  } NextLinkPointer;
  union
  {
    u32  value;
    struct
    {
      unsigned  buf_length      : 11;   // Buffer Length,               [10:0]
      unsigned  descp1_rsv0     : 5;    // Reserved,                    [15:11]
      unsigned  data_length     : 11;   // Data Length received in OUT, [26:16]
      unsigned  retry_cnt       : 2;    // Retry Count,                 [28:27]
      unsigned  buf_unvalid     : 1;    // Buffer Not Available,        [29]
      unsigned  ioc             : 1;    // IOC,                         [30]
      unsigned  owner           : 1;    // Ownership,                   [31]
    } bit;
  } DescpCommandStatus;
  u32  BufferPointer;
  u32  RequestPointer;
  #endif

}	usb_dma_descp, *pusb_dma_descp;


/*******************************************************************
             USB BULK/ISO ENDPOINT CONFIGURATION REGISTER
 *******************************************************************/

typedef union _usb_bulkiso_endp_dma_conf_reg
{
    u32  value;
    #ifdef  BIG_ENDIAN
    struct
    {
      unsigned  bulkiso_type_select         : 1;
      unsigned  rsv1                        : 1;
      unsigned  bulkiso_DMA_burst_size      : 2;
      unsigned  bulkiso_out_threshold       : 2;
      unsigned  bulkiso_in_threshold        : 2;
      unsigned  rsv0                        : 4;
      unsigned  bulkiso_out_max_pkt_size    : 10;
      unsigned  bulkiso_in_max_pkt_size     : 10;
    } bit;
    #else   // LITTLE_ENDIAN
    struct
    {
      unsigned  bulkiso_in_max_pkt_size     : 10;
      unsigned  bulkiso_out_max_pkt_size    : 10;
      unsigned  rsv0                        : 4;
      unsigned  bulkiso_in_threshold        : 2;
      unsigned  bulkiso_out_threshold       : 2;
      unsigned  bulkiso_DMA_burst_size      : 2;
      unsigned  rsv1                        : 1;
      unsigned  bulkiso_type_select         : 1;
    } bit;
    #endif
} usb_bulkiso_endp_dma_conf_reg, *pusb_bulkiso_endp_dma_conf_reg;


/*******************************************************************
               USB INTERRUPT IN ENDPOINT CONTROL REGISTER
 *******************************************************************/

typedef union _usb_intr_in_endp_ctrl_reg
{
  u32  value;
  #ifdef  BIG_ENDIAN
  struct
  {
    unsigned  rsv3                      : 4;
    unsigned  intr_in_endp_Tx_data_cnt  : 4;
    unsigned  rsv2                      : 1;
    unsigned  intr_in_endp_stalled      : 1;
    unsigned  rsv1                      : 1;
    unsigned  in_token_Tx_data_owner    : 1;
    unsigned  rsv0                      : 2;
    unsigned  intr_in_endp_stall        : 1;
    unsigned  intr_in_endp_enable       : 1;
    unsigned  intr_in_endp_number       : 16;
  } bit;
  #else
  struct
  {
    unsigned  intr_in_endp_number       : 16;
    unsigned  intr_in_endp_enable       : 1;
    unsigned  intr_in_endp_stall        : 1;
    unsigned  rsv0                      : 2;
    unsigned  in_token_Tx_data_owner    : 1;
    unsigned  rsv1                      : 1;
    unsigned  intr_in_endp_stalled      : 1;
    unsigned  rsv2                    stop_bulk_dma  : 1;
    unsigned  intr_in_endp_Tx_data_cnt  : 4;
    unsigned  rsv3                      : 4;
  } bit;
  #endif
} usb_intr_in_endp_ctrl_reg, *pusb_intr_in_endp_ctrl_reg;


/****************************
 * USB Module Registers *
 ****************************/

#define CR_USB_BASE     0xBFB70000

        // --- System Control Register ---
#define CR_USB_SYS_CTRL_REG           (0x00 | CR_USB_BASE)

        // --- Device Control Register ---
#define CR_USB_DEV_CTRL_REG           (0x04 | CR_USB_BASE)

        // --- Interrupt Status Register ---
#define CR_USB_INTR_STATUS_REG        (0x08 | CR_USB_BASE)

        // --- Interrupt Mask Register ---
#define CR_USB_INTR_MASK_REG          (0x0c | CR_USB_BASE)

        // --- Control Endpoint I/O Mode Control Register ---
#define CR_USB_CTRL_ENDP_IO_CTRL_REG  (0x10 | CR_USB_BASE)

        // --- Control Endpoint I/O Mode OUT Transfer Data Register #00 ---
#define CR_USB_CTRL_ENDP_IO_OUT_REG0  (0x18 | CR_USB_BASE)

        // --- Control Endpoint I/O Mode OUT Transfer Data Register #01 ---
#define CR_USB_CTRL_ENDP_IO_OUT_REG1  (0x1c | CR_USB_BASE)

        // --- Control Endpoint I/O Mode IN Transfer Data Register #00 ---
#define CR_USB_CTRL_ENDP_IO_IN_REG0   (0x20 | CR_USB_BASE)

        // --- Control Endpoint I/O Mode IN Transfer Data Register #01 ---
#define CR_USB_CTRL_ENDP_IO_IN_REG1   (0x24 | CR_USB_BASE)

        // --- Interrupt IN Endpoint Control Register ---
#define CR_USB_INTR_IN_ENDP_CTRL_REG  (0x30 | CR_USB_BASE)

        // --- Interrupt IN Endpoint IN Transfer Data Register #00 ---
#define CR_USB_INTR_IN_ENDP_IN_REG0   (0x38 | CR_USB_BASE)

        // --- Interrupt IN Endpoint IN Transfer Data Register #01 ---
#define CR_USB_INTR_IN_ENDP_IN_REG1   (0x3c | CR_USB_BASE)

        // --- Bulk/ISO OUT Descriptor Pointer Register ---
#define CR_USB_BULKISO_OUT_DESCP_BASE_REG   (0x40 | CR_USB_BASE)

        // --- Bulk/ISO IN Descriptor Pointer Register ---
#define CR_USB_BULKISO_IN_DESCP_BASE_REG    (0x44 | CR_USB_BASE)

        // --- Bulk/ISO IN/OUT Endpoint Number Register ---
#define CR_USB_BULKISO_INOUT_ENDP_NUM_REG   (0x48 | CR_USB_BASE)

        // --- Bulk/ISO Endpoint DMA Control Register ---
#define CR_USB_BULKISO_ENDP_DMA_CTRL_REG    (0x4c | CR_USB_BASE)

        // --- Bulk/ISO Endpoint DMA Configuration Register ---
#define CR_USB_BULKISO_ENDP_DMA_CONF_REG    (0x50 | CR_USB_BASE)

        // --- ISO Endpoint Transfer Delimiter Register #00 ---
#define CR_USB_ISO_ENDP_DELIMITER_REG0      (0x58 | CR_USB_BASE)

        // --- ISO Endpoint Transfer Delimiter Register #01 ---
#define CR_USB_ISO_ENDP_DELIMITER_REG1      (0x5c | CR_USB_BASE)

        // --- Vendor ID Register ---
#define CR_USB_VENDOR_ID_REG                (0x68 | CR_USB_BASE)

        // --- Product ID Register ---
#define CR_USB_PRODUCT_ID_REG               (0x6c | CR_USB_BASE)

#define CHK_BUF() pos = begin + index; if (pos < off) { index = 0; begin = pos; }; if (pos > off + count) goto done;
/**************************
 * USB Module Registers *
 **************************/




/*******************************************************************
                         FUNCTION DECLARATION
 *******************************************************************/
static int tc3162_ep_enable (struct usb_ep *_ep, const struct usb_endpoint_descriptor *desc);
static int tc3162_ep_disable (struct usb_ep *_ep);
static struct usb_request * tc3162_ep_alloc_request (struct usb_ep *_ep, gfp_t gfp_flags);
static void tc3162_ep_free_request (struct usb_ep *_ep, struct usb_request *_req);
static void * tc3162_ep_alloc_buffer(struct usb_ep *_ep, unsigned bytes, dma_addr_t *dma, gfp_t gfp_flags);
static void tc3162_ep_free_buffer(struct usb_ep *_ep, void *buf, dma_addr_t dma, unsigned bytes);
static int tc3162_ep_queue(struct usb_ep *_ep, struct usb_request *_req, gfp_t gfp_flags);
static int tc3162_ep_dequeue(struct usb_ep *_ep, struct usb_request *_req);
static int tc3162_ep_set_halt(struct usb_ep *_ep, int value);
static int tc3162_ep_fifo_status(struct usb_ep *_ep);
static void tc3162_ep_fifo_flush(struct usb_ep *_ep);

static int tc3162_get_frame (struct usb_gadget *_gadget);
static int tc3162_wakeup (struct usb_gadget *_gadget);
static int tc3162_set_selfpowered (struct usb_gadget *_gadget, int value);
static int tc3162_pullup (struct usb_gadget *_gadget, int is_on);

static int tc3162_udc_probe(struct platform_device *pdev);
static int tc3162_udc_remove(struct platform_device *pdev);

void usbClearInterruptStatus (u32 status);
static u32 usbSetInterrupt (u32 unmask, int set);
static void usbDevConnect (int connectUSB);
static void usbCtrlEndpInAckTransfer (void);
static void usbReceiveOutPacket(void);
static void rtv_out_ack_data(void);
static int usbCtrlEndpInSend(int transfer_type);
static int usbSendInPacket (struct usb_request *req);
static void usbInPacketSent(void);

void usbInitBulkIso(void);
void *usbInitBulkIsoDMADescp (u32 numOfDescp, int type);
void* usbKmalloc(int size, int gfpFlag);
void usbKfree(void* addr);

void read_ctrl_ep_out_fifo(void);
void get_ctrl_ep_out_fifo_data(u32 *word0, u32 *word1, u32 *ctrl_ep_io_reg_v);
u8 get_next_fifo_len(void);
void dump_fifo_history(void);
void dump_descp_data(void *base_ptr, u32 numOfDescp);
int dump_count(int argc, char *argv[], void *p);
int desc_dump(int argc, char *argv[], void *p);
int fifo_dump(int argc, char *argv[], void *p);
int mem_dump(int argc, char *argv[], void *p);
void dump_data(u8 *data, int length);
void usbCtrlEndpTransfer(int transfer_type,u32 byte7654,u32 byte3210);

static void nop_release (struct device *dev);
void stop_and_clear_ep(void);
void clear_pool_data(void);
void reset_bulk_dma(void);
void reset_queue(void);
void usb_suspend(struct tc3162_udc *udc);
void stop_bulk_dma(void);
void init_parameters(void);
void init_hardware(void);
void init_dbg(void);
void stop_dbg(void);
int usb_stats_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data);
int usb_stats_write_proc(struct file *file, const char *buffer,
		unsigned long count, void *data);
int usb_link_stats_read_proc(char *buf, char **start, off_t off, int count,
        int *eof, void *data);
void usbTimer(unsigned long data);
void clear_mib_count(void);
