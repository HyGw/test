/*
 * $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/modules/public/usb_camera/v4l_experimental/plx9054.h#1 $
 */

#ifndef _PLX9054_H_
#define _PLX9054_H_

// Definitions for the PLX Configuration space registers.
// All bits numbers are starting from 0.
#define PLX9054_STS_CMD_OFFSET        0x4

#define PLX9054_SUBSYSTEM_ID_OFFSET   0x2C
#define SUBSYSTEM_ID_SHIFT                  16

#define PLX9054_CONF_HOT_SWAP_OFFSET  0x48
#define HOT_SWAP_LED                        0x00080000

#define PLX9054_CONF_VPD_OFFSET       0x4C
#define VPD_ADDRESS_SHIFT                   16
#define VPD_ADDRESS                         0x7FFF0000
#define VPD_FLAG                            0x80000000

#define PLX9054_CONF_VPD_DATA         0x50

// Definitions for the PLX internal registers.
#define PLX9054_LAS0RR_OFFSET         0x00

#define PLX9054_LB0_BASE_ADR          0x04
#define SPACE0_ENABLED                      0x00000001
#define LB_SPACE0_ADDRESS_MASK              0xFFFFFFF0

#define PLX9054_MARBR_OFFSET          0x08
#define DIRECT_SLAVE_LOCK_ENABLE            (1 << 22)     // 22 bit
#define PRIOPITY_MASK                       0x00180000
#define PRIORITY_DMA_0                      0x00100000

//
#define PLX9054_BIGEND_LMISC_PROTAREA 0x0C
#define PROT_AREA_SHIFT                     16
#define PROT_AREA_MASK                      0x007F0000
#define BIGEND_DIRECT_MASTER                0x00000002
#define BIGEND_SPACE_1                      (1 << 5)      // 5 bit
#define BIGEND_DMA0                         (1 << 7)

#define PLX9054_LBRD0_OFFSET          0x18
#define SPACE0_BUSWIDTH8                    0x00000000
#define SPACE0_BUSWIDTH16                   0x00000001
#define SPACE0_BUSWIDTH32                   0x00000003
#define SPACE0_READY_INPUT_ENABLE           0x00000040
#define SPACE0_BTERM_INPUT_ENABLE           0x00000080
#define SPACE0_PREFETCH_DISABLE             0x00000100
#define SPACE0_BURST_ENABLE                 0x01000000
#define SPACE0_PREFETCH_COUNT_ENABLE        0x00000400
#define PCI_TARGET_RETRY_DELAY_MAX          0xF0000000      /* 15 * 4 PCI clocks */


#define EROM_BUSWIDTH8                      0x00000000
#define EROM_BUSWIDTH16                     0x00010000
#define EROM_BUSWIDTH32                     0x00030000
#define EROM_READY_INPUT_ENABLE             0x00400000
#define EROM_BTERM_INPUT_ENABLE             0x00800000
#define EROM_PREFETCH_DISABLE               0x00000200
#define EROM_BURST_ENABLE                   0x04000000
#define EROM_PREFETCH_COUNT_ENABLE          0x00000400

#define PLX9054_LB_PCI_RANGE          0x1C
#define PLX9054_DMRR_OFFSET           PLX9054_LB_PCI_RANGE
#define LB_PCI_RANGE_MASK                   0xFFFF0000

#define PLX9054_LB_BASE_ADR           0x20
#define PLX9054_DMLBAM_OFFSET         PLX9054_LB_BASE_ADR
#define LB_BASE_ADR_MASK                    0xFFFF0000

#define PLX9054_DMLBA1_OFFSET         0x24

#define PLX9054_PCI_BASE_ADR          0x28
#define PLX9054_DMPBAM_OFFSET         PLX9054_PCI_BASE_ADR
#define LB_PCI_MEM_ENABLE                   0x00000001
#define LB_PCI_IO_ENABLE                    0x00000002
#define PCI_PREFETCH_SIZE_INF               0x00001008
#define PCI_KEEP_BUS                        0x00000010
#define USE_PCI_BASE_ADR                    0x00002000
#define PCI_BASE_ADR_MASK                   0xFFFF0000

// INTCSR0 register
#define PLX9054_INT_CNTRL_STS         0x68
#define LSERR_ENABLE                        0x00000003    // 1 and 2 bit.
#define PCI_INT_ENABLE                      0x00000100    // 8 bit
#define PCI_DOORBELL_ENABLE                 0x00000200    // 9 bit
#define PCI_ABORT_INT_ENABLE                (1<<10)       // 10 bit
#define LOCAL_INT_INPUT_ENABLE              0x00000800    // 11 bit
#define RETRY_ABORT_ENABLE                  (1<<12)       // 12 bit.
#define LOCAL_INT_ACTIVE                    0x00008000    // 15 bit
#define LOCAL_INT_OUTPUT_ENABLE             0x00010000    // 16 bit
#define LOCAL_DOORBELL_ENABLE               0x00020000    // 17 bit
#define DMA0_INT_ENABLE                     0x00040000    // 18 bit
#define DMA1_INT_ENABLE                     0x00080000    // 19 bit
#define DMA0_INT_ACTIVE                     0x00200000    // 21 bit
#define DMA1_INT_ACTIVE                     0x00400000


#define PLX9054_CNTRL_OFFSET          0x6C
#define SERIAL_EEPROM_CLK                   0x01000000
#define SERIAL_EEPROM_DATA                  0x04000000
#define SERIAL_EEPROM_CS                    0x02000000
#define GP_OUTPUT                           0x00010000    // 16 bit
#define GP_INPUT                            (1 << 17)     // 17 bit
#define GP_OUTPUT_ENABLE                    0x00040000
//#define SERIAL_EEPROM_PESENT


// DMAMODE0 register.
#define PLX9054_DMA0_MODE             0x80
#define DMA0_LB_WIDTH_16                    0x1
#define DMA0_LB_WIDTH_32                    0x2
#define DMA0_LB_WIDTH_MASK                  0x3
#define DMA0_BTERM_ENABLE                   (1 << 7)      // 7 bit
#define DMA0_LOCAL_BURST_ENABLE             (1 << 8)      // 8 bit
#define DMA0_DONE_ENABLE                    (1 << 10)     // 10 bit
#define DMA0_HOLD_LOCAL_ADRESS              (1 << 11)     // 11 bit
#define DMA0_DEMAND_MODE                    (1 << 12)     // 12 bit
#define DMA0_FAST_TERMINATION               (1 << 15)     // 15 bit
#define DMA0_INT_TO_PCI                     (1 << 17)     // 17 bit


// DMAPADR0 register
#define PLX9054_DMA0_PCI_ADDRESS      0x84


// DMALADR0 register
#define PLX9054_DMA0_LB_ADDRESS       0x88


// DMASIZ0 register
#define PLX9054_DMA0_SIZE             0x8C


// DMADPR0 register
#define PLX9054_DMA0_DESCR_PTR        0x90
#define DMA0_DESCRIPTOR_INT_ENABLE          (1 << 2)      // 2 bit
#define DMA0_LOCAL_TO_PCI_TRANSFER          (1 << 3)      // 3 bit


#define PLX9054_DMA1_MODE             0x94
#define DMA1_DONE_ENABLE                    0x00000400    // 10 bit
#define DMA1_INT_TO_PCI                     0x00020000    // 18 bit


#define PLX9054_DMA1_DESCR_PTR        0xA4
#define DMA1_INT_TERM_CNT_ENABLE            0x00000004

// DMACSR0 register
#define PLX9054_DMA_CMD_STS           0xA8
#define DMA0_ENABLE                         1             // 0 bit
#define DMA0_START                          (1 << 1)      // 1 bit
#define DMA0_ABORT                          (1 << 2)      // 2 bit
#define DMA0_CLEAR_INT                      (1 << 3)      // 3 bit
#define DMA0_TRANSFER_DONE                  (1 << 4)      // 4 bit

#define DMA1_CLEAR_INT                      0x00000800
#define DMA1_TRANSFER_DONE                  0x00001000

#define PLX9054_LAS1RR_OFFSET         0xF0

#define PLX9054_LB1_BASE_ADR          0xF4
#define PLX9054_LAS1BA_OFFSET         PLX9054_LB1_BASE_ADR
#define SPACE1_ENABLED                      0x00000001
#define LB_SPACE1_ADDRESS_MASK              0xFFFFFFF0

#define PLX9054_LBRD1_OFFSET          0xF8
#define SPACE1_BUSWIDTH8                    0x00000000
#define SPACE1_BUSWIDTH16                   0x00000001
#define SPACE1_BUSWIDTH32                   0x00000003
#define SPACE1_READY_INPUT_ENABLE           0x00000040
#define SPACE1_BTERM_INPUT_ENABLE           0x00000080
#define SPACE1_PREFETCH_DISABLE             0x00000200
#define SPACE1_BURST_ENABLE                 0x00000100
#define SPACE1_PREFETCH_COUNT_ENABLE        0x00000400


#endif
