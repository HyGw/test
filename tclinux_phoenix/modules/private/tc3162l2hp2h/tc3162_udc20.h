
// #ifndef TC3162L2
// 	#define TC3162L2
// #endif
// #ifndef BIG_ENDIAN
// 	#define BIG_ENDIAN
// #endif
 
#define DRIVER_NAME 	"tc3162_udc"
#define EP0_NAME 	"ep0-ctrl"
#define EP1_NAME 	"ep1in-bulk"
#define EP2_NAME 	"ep2out-bulk"
#define EP3_NAME 	"ep3in-int"

#define NO_STATUS 	0
#define STATUS 		1
#define ACK_STATUS 	2
#define NOACK_STATUS 	3

#define	TC3162_UDC_NUM_ENDPOINTS	4
#define CONTROL_MAX_PACKET_SIZE		64
#define HIGH_SPEED_BULK_MAX_PACKET_SIZE	512
#define FULL_SPEED_BULK_MAX_PACKET_SIZE	64
#define INTERRUPT_MAX_PACKET_SIZE	64

#define CONTROL_IN	0
#define CONTROL_OUT	1
#define BULK_IN     	0
#define BULK_OUT    	1

#define GADGET_ETHER 		1
#define GADGET_FILE_STORAGE 	2

#define TRUE 	1
#define FALSE 	0

#define SUCCESS 0
#define FAIL 	-1

#define ON 	1
#define OFF 	0

#define SET_CONF 	1
#define NOT_SET_CONF 	0

#define BOOT		1
#define NOT_BOOT	0

#define   USB_SOFTWARE    0
#define   USB_HARDWARE    1

#define	  SETUP_BUF_SIZE 16 //only use 8 bytes 	

#define   USB_BULK_ISO_IN_DESCP_NUM   32
#define   USB_BULK_ISO_OUT_DESCP_NUM  32

//#define LED_OFF(x)      VPint(CR_GPIO_DATA) |= (1<<(x))
//#define LED_ON(x)       VPint(CR_GPIO_DATA) &= ~(1<<(x))
//#define LED_OEN(x)      do { VPint(CR_GPIO_CTRL) |= (1<<((x)*2)); VPint(CR_GPIO_ODRAIN) |= (1<<(x)); } while(0)
#define GPIO_HIGH(x) do{ (x > 15) ? (VPint(CR_GPIO_CTRL1) |= (1<<(x-16)*2)) : (VPint(CR_GPIO_CTRL) |= (1<<((x)*2))); \
						VPint(CR_GPIO_ODRAIN) |= (1<<(x)); \
						VPint(CR_GPIO_DATA) |= (1<<(x));}while(0)

#define   usbRegWrite4Byte(addr,val)  ( *(volatile u32 *)(addr)  = val )
// #define   usbRegWrite2Byte(addr,val)  ( *(volatile uint16 *)(addr)  = val )
// #define   usbRegWrite1Byte(addr,val)  ( *(volatile uint8 *)(addr)   = val )
#define   usbRegRead4Byte(addr,val)   ( val = *(volatile u32 *)(addr)  )
// #define   usbRegRead2Byte(addr,val)   ( val = *(volatile uint16 *)(addr))
// #define   usbRegRead1Byte(addr,val)   ( val = *(volatile uint8 *)(addr) )

struct tc3162_request {
// 	int index;
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

typedef struct dmaBufferHeader_s{
	dma_addr_t dmaHandle;
	u32 bufSize;
	char reserved[24];
 }dmaBufferHeader_t;

/*******************************************************************
                          MACROs for REGISTER IO
 *******************************************************************/
#define	USB_SET_SETUP_BASE_(x) \
 	*(volatile u32 *)(CR_USB20_SETUP_BASE_REG)  = x;

#define	USB_SET_CONTROL_OUT_BASE_(x) \
 	*(volatile u32 *)(CR_USB20_CONTROL_OUT_BASE_REG)  = x;
	
#define	USB_SET_CONTROL_IN_BASE_(x) \
 	*(volatile u32 *)(CR_USB20_CONTROL_IN_BASE_REG)  = x;
	
#define	USB_SET_INTERRUPT_IN_BASE_(x) \
	*(volatile u32 *)(CR_USB20_INTERRUPT_IN_BASE_REG)  = x;	
 
#define   USB_SET_BULKISO_IN_DESCP_BASE_(x) \
          *(volatile u32 *)(CR_USB20_BULK_IN_DESC_BASE_REG)  = x;

#define   USB_SET_BULKISO_OUT_DESCP_BASE_(x) \
          *(volatile u32 *)(CR_USB20_BULK_OUT_DESC_BASE_REG) = x;
	  
#define   USB_CONTROL_OUT_POLL_DEMAND_(x) \
          x = *(volatile uint32 *)(CR_USB20R_CONTROL_EP_DMA_CTRL_REG) | 0x00000002; \
          *(volatile uint32 *)(CR_USB20R_CONTROL_EP_DMA_CTRL_REG) = x;	  

#define   USB_IN_POLL_DEMAND_(x) \
          *(volatile uint32 *)(CR_USB20_BULK_IN_DMA_POLLING_REG) = x;

#define   USB_OUT_POLL_DEMAND_(x) \
          *(volatile uint32 *)(CR_USB20_BULK_OUT_DMA_POLLING_REG) = x;


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

    struct
    {
      unsigned  system_reset 	: 1;	// System Reset
      unsigned  dma_reset 	: 1;	// DMA Reset
      unsigned  udc_reset 	: 1;	// UDC Reset
      unsigned  rsv2			: 20;	// Reserved
	unsigned  ASIC_FPGA_mode  : 1;   // ASIC / FPGA mode
      unsigned  rsv1        	: 1;   // Reserved
      unsigned  probing_select	: 3;	// probing_select
      unsigned  rsv0          	: 2;	// Reserved
      unsigned	flash_mode	: 1;	// 1 :FLASH; 0 :RNDIS
      unsigned  big_endian    	: 1;	// Big Endian Mode
    } bit;
    
} usb_sys_ctrl_reg, *pusb_sys_ctrl_reg;

/*******************************************************************
                     USB DEVICE CONTROL REGISTER
 *******************************************************************/

typedef union _usb_dev_ctrl_reg
{
    u32  value;

    struct
    {
      uint8   byte0;
      uint8   byte1;
      uint8   byte2;
      uint8   byte3;                // [31:0] == {byte0, byte1, byte2, byte3}
    } byte;
    
    struct
    {
      unsigned  dev_disconnect     	: 1;  // UDC Device Disonnect (RW)
      unsigned  dev_resume      	: 1;  // UDC Device Resume (RW)
      unsigned  rsv2                	: 3;  // Reserved
      unsigned  phy_mode   		: 2;  // UDC phy_mode (RO)
      unsigned  phy_transceiver_select  : 1;  // UDC phy_transceiver select Load Done (RO)
      unsigned  phy_termination_setting : 2;  // UDC phy_termination_setting (RO)
      unsigned  phy_line_state   	: 2;  // UDC phy_line_state (RO)
      unsigned  rsv1                	: 1;  // Reserved
      unsigned  device_speed_detection	: 1;  // UDC Device Speed Detection In progress (RO)
      unsigned  device_speed_enumerated	: 2;  // UDC device_speed_enumerated (RO)
      unsigned  rsv0                	: 4;  // Reserved
      unsigned  device_config_setting   : 4;  // UDC device_config_setting (RO)
      unsigned  device_interface_setting: 4;  // device_interface_setting (RO)
      unsigned  device_alternate_setting: 4;  // UDC device_alternate_setting (RO)
    } bit;

} usb_dev_ctrl_reg, *pusb_dev_ctrl_reg;

/*******************************************************************
                   USB CONTROL ENDPOINT CONTROL REGISTER
 *******************************************************************/
typedef union _usb_ctrl_endp_conf_reg
{
    u32  value;

    struct
    {
      uint8   byte0;
      uint8   byte1;
      uint8   byte2;
      uint8   byte3;
    } byte;
    struct
    {
      	unsigned  rsv1 				: 27;
      	unsigned  ctrl_endp_enable      	: 1;
      	unsigned  ctrl_endp_IN_is_stalled 	: 1;
      	unsigned  ctrl_endp_OUT_is_stalled	: 1;
      	unsigned  rsv0                  	: 1;
	unsigned  ctrl_endp_IN_OUT_stall	: 1;
    } bit;

} usb_ctrl_endp_conf_reg, *pusb_ctrl_endp_conf_reg;

typedef union _usb_ctrl_endp_ctrl_dma_reg
{
    u32  value;

    struct
    {
      uint8   byte0;
      uint8   byte1;
      uint8   byte2;
      uint8   byte3;
    } byte;
    struct
    {
      unsigned  rsv2 					: 5;
      unsigned  ctrl_endp_in_Tx_data_cnt  		: 11;
      unsigned  rsv1 					: 2;
      unsigned  ctrl_endp_dma_max_burst_size      	: 2;
      unsigned  ctrl_endp_dma_in_drain_threshold	: 2;
      unsigned  ctrl_endp_dma_out_fill_threshold    	: 2;
      unsigned  rsv                       		: 5;
      unsigned  ctrl_in_dma_polling			: 1;
      unsigned  ctrl_out_dma_polling			: 1;
      unsigned  ctrl_endp_dma_enable			: 1;
    } bit;

} usb_ctrl_endp_ctrl_dma_reg, *pusb_ctrl_endp_ctrl_dma_reg;

typedef union _usb_ctrl_endp_ctrl_status_reg
{
    u32  value;

    struct
    {
      uint8   byte0;
      uint8   byte1;
      uint8   byte2;
      uint8   byte3;
    } byte;
    struct
    {
      unsigned  rsv2 				: 5;
      unsigned  ctrl_endp_out_RX_data_cnt  	: 11;
      unsigned  rsv1 				: 5;
      unsigned  ctrl_endp_in_dma_in_progess     : 1;
      unsigned  ctrl_endp_out_dma_in_progess	: 1;
      unsigned  setup_dma_in_progess    	: 1;
      unsigned  ctrl_in_fifo_under_run_error	: 1;
      unsigned  ctrl_in_fifo_under_run    	: 1;
      unsigned  ctrl_in_packet_transmit_ok 	: 1;
      unsigned  ctrl_out_over_size		: 1;
      unsigned  ctrl_out_fifo_over_run		: 1;
      unsigned  ctrl_out_packet_transmit_ok	: 1;
      unsigned  setup_packet_over_size		: 1;
      unsigned  setup_packet_transmit_ok	: 1;
    } bit;

} usb_ctrl_endp_ctrl_status_reg, *pusb_ctrl_endp_ctrl_status_reg;

/*******************************************************************
                   USB BULK/ISO ENDPOINT CONTROL REGISTER
 *******************************************************************/
typedef union _usb_bulk_desc_setup_reg
{
    u32  value;

    struct
    {
      uint8   byte0;
      uint8   byte1;
      uint8   byte2;
      uint8   byte3;
    } byte;
    struct
    {
      	unsigned  rsv3				: 4;
	unsigned  bulk_endp_OUT_desc_offset 	: 4;
	unsigned  rsv2				: 2;
      	unsigned  bulk_endp_OUT_desc_size 	: 6;
	unsigned  rsv1				: 4;
	unsigned  bulk_endp_IN_desc_offset 	: 4;
	unsigned  rsv0				: 2;
      	unsigned  bulk_endp_IN_desc_size 	: 6;
    } bit;

} usb_bulk_endp_desc_setup_reg, *pusb_bulk_endp_desc_setup_reg;


typedef union _usb_bulk_endp_conf_reg
{
    u32  value;

    struct
    {
      uint8   byte0;
      uint8   byte1;
      uint8   byte2;
      uint8   byte3;
    } byte;
    struct
    {
      	unsigned  rsv				: 28;
      	unsigned  bulk_endp_IN_is_stalled 	: 1;
      	unsigned  bulk_endp_IN_stall    	: 1;
      	unsigned  bulk_endp_OUT_is_stalled	: 1;
	unsigned  bulk_endp_OUT_stall		: 1;
    } bit;

} usb_bulk_endp_conf_reg, *pusb_bulk_endp_conf_reg;


typedef union _usb_bulk_endp_dma_ctrl_reg
{
    u32  value;

    struct
    {
      unsigned  rsv1                        		: 18;   
      unsigned  bulk_endp_dma_max_burst_size       	: 2;
      unsigned  bulk_endp_IN_dma_drain_threshold	: 2;
      unsigned  bulk_endp_OUT_dma_fill_threshold      	: 2;
      unsigned  rsv0                        		: 4;    
      unsigned  bulk_endp_IN_endp_enable     		: 1;
      unsigned  bulk_endp_OUT_endp_enable     		: 1;
      unsigned  bulk_endp_IN_dma_enable     		: 1;
      unsigned  bulk_endp_OUT_dma_enable     		: 1;
    } bit;

} usb_bulk_endp_dma_ctrl_reg, *pusb_bulk_endp_dma_ctrl_reg;
/*******************************************************************
                USB BULK DMA DESCRIPTOR STRUCTURE
 *******************************************************************/

typedef struct _usb_DMA_descp
{
  union
  {
    u32  value;
    struct
    {
      unsigned  owner           		: 1;    // Ownership,           	[31]
      unsigned  rsv1				: 11;	//				[30:20]
      unsigned  fifo_overrun_error		: 1;	//Fifo Overrun			[19]
      unsigned  buf_unvalid     		: 1;    // Buffer Not Available,        [18]
      unsigned  retry_cnt       		: 2;    // Retry Count,                 [17:16]
      unsigned  underrun_three_time_error	: 1;    // Undef-run three-times error  [15]
      unsigned  underrun_host_error     	: 1;    // Buffer Not Available,        [14]
      unsigned  rsv0				: 1;	//				[13]
      unsigned  data_length     		: 13;   // Data Length received in OUT, [12:0]
      
    } bit;
  } DescpCommandStatus;
  
  union
  {
    u32  value;
    struct
    {
      unsigned  ioc             	: 1;    // IOC,                         [30]
      unsigned  rsv1			: 14;	//				[30:17]
      unsigned  bulk_IN_zero_transfer   : 1;    // T,                 		[16]
      unsigned  rsv0			: 3;	//				[15:13]
      unsigned  buf_length      	: 13;   // Buffer Length,            	[12:0]
    } bit;
  } BufferLength;
  
  u32  BufferPointer;
  u32  RequestPointer;
 
}usb_dma_descp, *pusb_dma_descp;


/*******************************************************************
                    USB INTERRUPT Enable/STATUS REGISTER
 *******************************************************************/

typedef union _usb_intr_reg
{
    u32  value;

    struct
    {
     	unsigned  rsv                  					: 5;
	unsigned  usb_host_resume_detected         			: 1;//receive real resume signal
	unsigned  usb_bus_resume_detected         			: 1;//means leave suspend
	unsigned  host_set_configuration                  		: 1;
     	unsigned  control_in_aborted_by_host                  		: 1;
     	unsigned  internal_system_bus_error                  		: 1;
      	unsigned  usb_phy_error_detected                   		: 1;
    	unsigned  usb_no_sof_detected                            	: 1;
    	unsigned  usb_bus_reset_detected    				: 1;
      	unsigned  usb_bus_suspend_detected         			: 1;
     	unsigned  control_in_transfer_ack     				: 1;
      	unsigned  control_out_transfer_ack          			: 1;
      	unsigned  setup_dma_done        				: 1;
      	unsigned  control_in_dma_done                            	: 1;
      	unsigned  control_in_dma_request       				: 1;
      	unsigned  control_out_dma_done       				: 1;
      	unsigned  control_out_dma_request      				: 1;
      	unsigned  setup_token_overwrite       				: 1;
      	unsigned  setup_received   					: 1;
      	unsigned  interrupt_in_dma_done 				: 1;
      	unsigned  interrupt_in_dma_request      			: 1;
      	unsigned  bulk_out_fifo_over_run        			: 1;
      	unsigned  bulk_out_end_of_descriptor_updated  			: 1;
      	unsigned  bulk_out_buffer_not_available   			: 1;
      	unsigned  bulk_out_endp_transfer_an_ethernet_packet_compltete	: 1;
      	unsigned  bulk_in_fifo_under_run      				: 1;
      	unsigned  bulk_in_end_of_descriptor_updated       		: 1;
      	unsigned  bulk_in_endp_transfer_an_ethernet_packet_compltete	: 1;
    } bit;

}  usb_intr_status_reg,    usb_intr_enable_reg,
  *pusb_intr_status_reg,  *pusb_intr_enable_reg;

enum _interrupt_bit_define
{
	BULK_IN_ENDP_TRANSFER_AN_ETHERNET_PACKET_COMPLTETE	= 0x00000001,
	BULK_IN_END_OF_DESCRIPTOR_UPDATED                	= 0x00000002,
	BULK_IN_FIFO_UNDER_RUN               			= 0x00000004,
	BULK_OUT_ENDP_TRANSFER_AN_ETHERNET_PACKET_COMPLTETE 	= 0x00000008,
	BULK_OUT_BUFFER_NOT_AVAILABLE     			= 0x00000010,   
	BULK_OUT_END_OF_DESCRIPTOR_UPDATED   			= 0x00000020,  
	BULK_OUT_FIFO_OVER_RUN       				= 0x00000040,
	INTERRUPT_IN_DMA_REQUEST   				= 0x00000080,
	INTERRUPT_IN_DMA_DONE					= 0x00000100,   
	SETUP_RECEIVED       					= 0x00000200,
	SETUP_TOKEN_OVERWRITE      				= 0x00000400,
	CONTROL_OUT_DMA_REQUEST  				= 0x00000800,
	CONTROL_OUT_DMA_DONE                			= 0x00001000,
	CONTROL_IN_DMA_REQUEST                			= 0x00002000,
	CONTROL_IN_DMA_DONE                	 		= 0x00004000,   
	SETUP_DMA_DONE                   			= 0x00008000,  
	CONTROL_OUT_TRANSFER_ACK               			= 0x00010000,   
	CONTROL_IN_TRANSFER_ACK                			= 0x00020000,   
	USB_BUS_SUSPEND_DETECTED				= 0x00040000,
	USB_BUS_RESET_DETECTED					= 0x00080000,
	USB_NO_SOF_DETECTED                 			= 0x00100000,
	USB_PHY_ERROR_DETECTED					= 0x00200000,
	INTERNAL_SYSTEM_BUS_ERROR				= 0x00400000,
	CONTROL_IN_ABORTED_BY_HOST				= 0x00800000,
	HOST_SET_CONFIGURATION					= 0x01000000,
	USB_BUS_RESUME_DETECTED					= 0x02000000,
	USB_HOST_RESUME_DETECTED				= 0x04000000,
};

/*******************************************************************
               USB INTERRUPT IN ENDPOINT CONTROL REGISTER
 *******************************************************************/
typedef union _usb_intr_in_endp_conf_reg
{
  u32  value;

  struct
  {
    unsigned  rsv0                      : 29;    
    unsigned  intr_in_endp_enable       : 1;
    unsigned  intr_in_endp_is_stalled	: 1;
    unsigned  intr_in_endp_stall       	: 1;
  } bit;
 
} usb_intr_in_endp_conf_reg, *pusb_intr_in_endp_conf_reg;
typedef union _usb_intr_in_endp_ctrl_reg
{
  u32  value;

  struct
  {
    unsigned  rsv3                      	: 5;
    unsigned  intr_in_endp_Tx_data_cnt  	: 11;
    unsigned  rsv2                      	: 2;
    unsigned  intr_endp_dma_max_burst_size      : 2;
    unsigned  intr_endp_dma_in_drain_threshold	: 2;
    unsigned  rsv1                      	: 1;
    unsigned  do_not_reload_intr_data    	: 1;
    unsigned  rsv                       	: 6;
    unsigned  intrl_in_dma_polling		: 1;
    unsigned  intr_endp_dma_enable		: 1;
  } bit;
 
} usb_intr_in_endp_ctrl_reg, *pusb_intr_in_endp_ctrl_reg;

/*******************************************************************
               USB EP CONTROL REGISTER
 *******************************************************************/
typedef union _usb_ep_ctrl_reg
{
  u32  value;

  struct
  {
    unsigned  rsv       			: 2;
    unsigned  max_packet_size       		: 11;
    unsigned  epalternate_setting_number       	: 4;
    unsigned  intferface_number       		: 4;
    unsigned  config_number                     : 4;    
    unsigned  ep_type       			: 2;
    unsigned  ep_direction			: 1;
    unsigned  ep_number       			: 4;
  } bit;
 
} usb_ep_ctrl_reg, *pusb_ep_ctrl_reg;

#if 0
/****************************
 * USB Module Registers *
 ****************************/

#define CR_USB20_BASE     0xBFB70000

// --- System Control Register ---
#define CR_USB20_SYS_CTRL_REG			(0x00 | CR_USB20_BASE)

// --- Device Control Register ---
#define CR_USB20_DEV_CTRL_REG			(0x04 | CR_USB20_BASE)

// --- Interrupt Status Register ---
#define CR_USB20_INTR_STATUS_REG		(0x0c | CR_USB20_BASE)

// --- Interrupt ENABLE Register ---
#define CR_USB20_INTR_ENABLE_REG          	(0x10 | CR_USB20_BASE)

// --- Interrupt Timing Control Register ---
#define CR_USB20_INTR_TIMING_CONTROL_REG	(0x14 | CR_USB20_BASE)

// --- SETUP Receive Data Buffer Register ---
#define CR_USB20_SETUP_BASE_REG          	(0x20 | CR_USB20_BASE)

// --- Control OUT Receive Data Buffer Pointer Register ---
#define CR_USB20_CONTROL_OUT_BASE_REG		(0x24 | CR_USB20_BASE)

// --- Control IN Receive Data Buffer Pointer Register ---
#define CR_USB20_CONTROL_IN_BASE_REG		(0x28 | CR_USB20_BASE)

// --- Control IN Receive Data Buffer Pointer Register ---
#define CR_USB20_CONTROL_CONF_REG		(0x2c | CR_USB20_BASE)

// --- CONTROL Endpoint DMA Transfer Control Register ---
#define CR_USB20R_CONTROL_EP_DMA_CTRL_REG	(0x30 | CR_USB20_BASE)

// --- CONTROL Endpoint DMA Transfer Status Register ---
#define CR_USB20_CONTROL_EP_DMA_STATUS_REG	(0x34 | CR_USB20_BASE)

// --- INTERRUPT IN Transmit Data Buffer Pointer  Register ---
#define CR_USB20_INTERRUPT_IN_BASE_REG		(0x38 | CR_USB20_BASE)

// --- INTERRUPT IN Configuration   Register ---
#define CR_USB20_INTERRUPT_IN_CONF_REG		(0x3c | CR_USB20_BASE)

// --- INTERRUPT IN Endpoint DMA Transfer Control Register ---
#define CR_USB20_INTERRUPT_IN_DMA_CTRL_REG	(0x40 | CR_USB20_BASE)

// --- INTERRUPT IN Endpoint DMA Transfer Status Register ---
#define CR_USB20_INTERRUPT_IN_EP_DMA_STATUS_REG	(0x44 | CR_USB20_BASE)

// --- Bulk/Ctrl/Intr IN/OUT Underrun/Overrun Error Counter  Register ---
#define CR_USB20_STATUS_COUNT_REG		(0x48 | CR_USB20_BASE)

// --- BULK OUT Endpoint Transfer DMA Polling Demand Control Register ---
#define CR_USB20_BULK_OUT_DMA_POLLING_REG	(0x60 | CR_USB20_BASE)

// --- BULK IN Endpoint Transfer DMA Polling Demand Control Register ---
#define CR_USB20_BULK_IN_DMA_POLLING_REG	(0x64 | CR_USB20_BASE)

// --- Bulk OUT Endpoint Transfer Dscriptor Base Address Register ---
#define CR_USB20_BULK_OUT_DESC_BASE_REG		(0x68 | CR_USB20_BASE)

// --- Bulk IN Endpoint Transfer Dscriptor Base Address Register ---
#define CR_USB20_BULK_IN_DESC_BASE_REG		(0x6c | CR_USB20_BASE)

// --- Bulk OUT/IN Endpoint Transfer Dscriptor Rinf Size/Offset Register ---
#define CR_USB20_BULK_DESC_SIZE_OFFSET_REG	(0x70 | CR_USB20_BASE)

// --- Bulk OUT/IN Endpoint Configuration Register ---
#define CR_USB20_BULK_EP_CONF_REG		(0x74 | CR_USB20_BASE)

// --- Bulk OUT/IN Endpoint DMA Transfer Control Register ---
#define CR_USB20_BULK_EP_DMA_CTRL_REG          	(0x78 | CR_USB20_BASE)

// --- Bulk OUT/IN Endpoint DMA Transfer Status Register ---
#define CR_USB20_BULK_EP_DMA_STATUS_REG         (0x7c | CR_USB20_BASE)

// --- UDC Setup Command Address Register ---
#define CR_USB20_UDC_SETUP_COMMAND_ADDR_REG	(0x80 | CR_USB20_BASE)

// --- UDC Control Endpoint Information Register ---
#define CR_USB20_UDC_CTRL_EP_INFO_REG          	(0x84 | CR_USB20_BASE)

// --- UDC BULK IN Endpoint Information Register ---
#define CR_USB20_UDC_BULK_IN_EP_INFO_REG    	(0x88 | CR_USB20_BASE)

// --- UUDC BULK OUT Endpoint Information Register ---
#define CR_USB20_UDC_BULK_OUT_EP_INFO_REG	(0x8c | CR_USB20_BASE)

// --- UDC INTERRUPT IN Endpoint Information Register ---
#define CR_USB20_UDC_INTERRUPT_IN_EP_INFO_REG	(0x90 | CR_USB20_BASE)
#endif

#define CHK_BUF() pos = begin + index; if (pos < off) { index = 0; begin = pos; }; if (pos > off + count) goto done;
/**************************
 * USB Module Registers *
 **************************/

/*******************************************************************
                         FUNCTION DECLARATION
 *******************************************************************/

static int __init tc3162_udc_probe(struct platform_device *pdev);
static int __exit tc3162_udc_remove(struct platform_device *pdev);
static int tc3162_ep_enable (struct usb_ep *_ep, const struct usb_endpoint_descriptor *desc);
static int tc3162_ep_disable (struct usb_ep *_ep);
static int tc3162_ep_fifo_status(struct usb_ep *_ep);
static void tc3162_ep_fifo_flush(struct usb_ep *_ep);
static int tc3162_get_frame (struct usb_gadget *_gadget);
static int tc3162_wakeup (struct usb_gadget *_gadget);
static int tc3162_set_selfpowered (struct usb_gadget *_gadget, int value);
static int tc3162_pullup (struct usb_gadget *_gadget, int is_on);
//static int tc3162_udc_suspend(struct platform_device *dev, pm_message_t state);
//static int tc3162_udc_resume(struct platform_device *dev);
static void nop_release (struct device *dev);

void clear_mib_count(void);
void init_hardware(void);
void usbDevReset (int resetDMA, int resetUDC);
void usbFillDeviceParameter(void);
static u32 usbSetInterrupt (u32 enable, int set);
void* usbKmalloc(int size, int gfpFlag);
void usbKfree(void* addr);
void init_parameters(void);
void usbInitBulk(void);
void *usbInitBulkDMADescp (u32 numOfDescp, int type);
int usbInterruptInit(void);
void usbInitCtrl(void);

static u32 usbSetInterrupt (u32 enable, int set);
static void usbDevConnect (int connectUSB);

static struct usb_request * tc3162_ep_alloc_request (struct usb_ep *_ep, gfp_t gfp_flags);
static void tc3162_ep_free_request (struct usb_ep *_ep, struct usb_request *_req);
static void * tc3162_ep_alloc_buffer(struct usb_ep *_ep, unsigned bytes, dma_addr_t *dma, gfp_t gfp_flags);
static void tc3162_ep_free_buffer(struct usb_ep *_ep, void *buf, dma_addr_t dma, unsigned bytes);
static int tc3162_ep_queue(struct usb_ep *_ep, struct usb_request *_req, gfp_t gfp_flags);
static int tc3162_ep_dequeue(struct usb_ep *_ep, struct usb_request *_req);
static int tc3162_ep_set_halt(struct usb_ep *_ep, int value);
static void tc3162_handle_setup (void);
static void tc3162_handle_control(char direct);
static void tc3162_handle_control_complete(char direct);
static void usbIntrSend(void);
static void usbIntrCompleted(void);
static int handle_bulk_in(struct usb_request *req);
static void handle_bulk_in_complete(void);
static int handle_bulk_out(struct usb_request *req);
static void handle_bulk_out_complete(void);

static void usb_reset(struct tc3162_udc *udc);
static void stop_all_ep(void);
static void clear_pool_data(void);
static void select_speed(void);
static void stop_bulk(void);
void cleanBulk_tasklet(unsigned long data);

void init_dbg(void);
void stop_dbg(void);
void dump_descp_data(void *base_ptr, u32 numOfDescp);
int dump_count(int argc, char *argv[], void *p);
int desc_dump(int argc, char *argv[], void *p);
int recv_dump(int argc, char *argv[], void *p);
void dump_data(u8 *data, int length);
void reset_queue(void);
void error_dump(int len, void *point);

void usbTimer(unsigned long data);
int usb_stats_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data);
int usb_stats_write_proc(struct file *file, const char *buffer,
		unsigned long count, void *data);
int usb_link_stats_read_proc(char *buf, char **start, off_t off, int count,
        int *eof, void *data);
