/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology	5th	Rd.
 * Science-based Industrial	Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2005, Ralink Technology, Inc.
 *
 * All rights reserved.	Ralink's source	code is	an unpublished work	and	the
 * use of a	copyright notice does not imply	otherwise. This	source code
 * contains	confidential trade secret material of Ralink Tech. Any attempt
 * or participation	in deciphering,	decoding, reverse engineering or in	any
 * way altering	the	source code	is stricitly prohibited, unless	the	prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

    Module Name:
    rtmp_main.c

    Abstract:

    Revision History:
    Who         When            What
    --------    ----------      ----------------------------------------------
    Name        Date            Modification logs
    Paul Lin    2002-11-25      Initial version
*/

#include "rt_config.h"

//unsigned long IrqFlags;

// Global variable, debug level flag
#ifdef DBG
ULONG    RTDebugLevel = RT_DEBUG_TRACE;
#endif

//static dma_addr_t		dma_adapter;

#ifdef TC_SOC //Add polling mode
#define POLLING_MODE_DETECT_INTV  30
extern void os_TCIfQuery (unsigned short query_id, void* result, void* result2);
#endif

// Following information will be show when you run 'modinfo'
// *** If you have a solution for the bug in current version of driver, please mail to me.
// Otherwise post to forum in ralinktech's web site(www.ralinktech.com) and let all users help you. ***
MODULE_AUTHOR("Paul Lin <paul_lin@ralinktech.com>");
MODULE_DESCRIPTION("RT61 Wireless Lan Linux Driver");


//#if LINUX_VERSION_CODE >= 0x20412       // Red Hat 7.3
//MODULE_LICENSE("GPL");
//#endif

static INT __devinit RT61_init_one (
    IN  struct pci_dev              *pPci_Dev,
    IN  const struct pci_device_id  *ent)
{
    INT rc;

    DBGPRINT(RT_DEBUG_TRACE, "===> RT61_init_one\n");

    // wake up and enable device
    if (pci_enable_device (pPci_Dev))
    {
        rc = -EIO;
    }
    else
    {
        rc = RT61_probe(pPci_Dev, ent);
    }

    DBGPRINT(RT_DEBUG_TRACE, "<=== RT61_init_one\n");
    return rc;
}

#if LINUX_VERSION_CODE <= 0x20402       // Red Hat 7.1
static struct net_device *alloc_netdev(int sizeof_priv, const char *mask, void (*setup)(struct net_device *))
{
    struct net_device	*dev;
    INT					alloc_size;

    /* ensure 32-byte alignment of the private area */
    alloc_size = sizeof (*dev) + sizeof_priv + 31;

    dev = (struct net_device *) kmalloc (alloc_size, MEM_ALLOC_FLAG);
    if (dev == NULL)
    {
        DBGPRINT(RT_DEBUG_ERROR, "alloc_netdev: Unable to allocate device memory.\n");
        return NULL;
    }

    memset(dev, 0, alloc_size);

    if (sizeof_priv)
        dev->priv = (void *) (((long)(dev + 1) + 31) & ~31);

    setup(dev);
    strcpy(dev->name,mask);

    return dev;
}
#endif

//
// PCI device probe & initialization function
//
INT __devinit   RT61_probe(
    IN  struct pci_dev              *pPci_Dev, 
    IN  const struct pci_device_id  *ent)
{
    struct  net_device      *net_dev;
    RTMP_ADAPTER            *pAd;
    CHAR                    *print_name;
    INT                     chip_id = (INT) ent->driver_data;
    UMAXLONG                csr_addr;
    INT                     Status;

    DBGPRINT(RT_DEBUG_TRACE, "Driver version-%s\n", DRIVER_VERSION);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
    print_name = pPci_Dev ? pci_name(pPci_Dev) : "rt61";
#else
    print_name = pPci_Dev ? pPci_Dev->slot_name : "rt61";
#endif

#if LINUX_VERSION_CODE <= 0x20402       // Red Hat 7.1
    net_dev = alloc_netdev(sizeof(PRTMP_ADAPTER), "eth%d", ether_setup);
#else
    net_dev = alloc_etherdev(sizeof(PRTMP_ADAPTER));
#endif
    if (net_dev == NULL) 
    {
        DBGPRINT(RT_DEBUG_TRACE, "init_ethernet failed\n");
        goto err_out;
    }

    SET_MODULE_OWNER(net_dev);
        
    if (pci_request_regions(pPci_Dev, print_name))
        goto err_out_free_netdev;

    // Interrupt IRQ number
    net_dev->irq = pPci_Dev->irq;

    // map physical address to virtual address for accessing register
    csr_addr = (UMAXLONG) ioremap(pci_resource_start(pPci_Dev, 0), pci_resource_len(pPci_Dev, 0));
    if (!csr_addr) 
    {
        DBGPRINT(RT_DEBUG_ERROR, "ioremap failed for device %s, region 0x%lX @ 0x%lX\n",
            print_name, (UMAXLONG)pci_resource_len(pPci_Dev, 0), (UMAXLONG)pci_resource_start(pPci_Dev, 0));
        goto err_out_free_res;
    }

	//net_dev->priv = pci_alloc_consistent(pPci_Dev, sizeof(RTMP_ADAPTER), &dma_adapter);
	net_dev->priv = vmalloc(sizeof(RTMP_ADAPTER));

	// Zero init RTMP_ADAPTER
	memset(net_dev->priv, 0, sizeof(RTMP_ADAPTER));

    // Save CSR virtual address and irq to device structure
    net_dev->base_addr = csr_addr;
    pAd = net_dev->priv;
    pAd->CSRBaseAddress = net_dev->base_addr;
    pAd->net_dev = net_dev;
    pAd->PortCfg.MBSSID[MAIN_MBSSID].MSSIDDev = net_dev;

    // Set DMA master
    pci_set_master(pPci_Dev);

    pAd->chip_id = chip_id;
    pAd->pPci_Dev = pPci_Dev;

    // The chip-specific entries in the device structure.
    net_dev->open = RT61_open;
    net_dev->hard_start_xmit = RTMPSendPackets;
    net_dev->stop = RT61_close;
    net_dev->get_stats = RT61_get_ether_stats;

#if WIRELESS_EXT >= 12 && WIRELESS_EXT < 21
    net_dev->get_wireless_stats = RT61_get_wireless_stats;
#endif

    net_dev->set_multicast_list = RT61_set_rx_mode;
    net_dev->do_ioctl = RT61_ioctl;
    net_dev->priv_flags = INT_MAIN;

    {// find available 
        INT     i=0;
        CHAR    slot_name[IFNAMSIZ];
        struct net_device   *device;

        for (i = 0; i < 8; i++)
        {
            sprintf(slot_name, "ra%d", i);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
            device = dev_get_by_name(slot_name);
#else
            for (device = dev_base; device != NULL; device = device->next)
            {
                if (strncmp(device->name, slot_name, 4) == 0)
                {
                    break;
                }
            }
#endif
            if(device == NULL)
				break;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
			else
				dev_put(device);
#endif
        }
        if(i == 8)
        {
            DBGPRINT(RT_DEBUG_ERROR, "No available slot name\n");
            goto err_out_unmap;
        }

        sprintf(net_dev->name, "ra%d", i);
    }

    // Register this device
    Status = register_netdev(net_dev);
    if (Status)
        goto err_out_unmap;

    DBGPRINT(RT_DEBUG_TRACE, "%s: at 0x%lx, VA 0x%lx, IRQ %d. \n", 
        net_dev->name, (UMAXLONG)pci_resource_start(pPci_Dev, 0), (UMAXLONG)csr_addr, pPci_Dev->irq);

    // Set driver data
    pci_set_drvdata(pPci_Dev, net_dev);

#ifdef WSC_SUPPORT
	InitializeWSCTLV();

    init_MUTEX_LOCKED(&(pAd->write_dat_file_semaphore));
    init_completion(&pAd->write_dat_file_notify);
    start_write_dat_file_thread(pAd);
#endif // WSC_SUPPORT //

    return 0;

err_out_unmap:
    iounmap((void *)csr_addr);
    release_mem_region(pci_resource_start(pPci_Dev, 0), pci_resource_len(pPci_Dev, 0));
err_out_free_res:
    pci_release_regions(pPci_Dev);
err_out_free_netdev:
    kfree (net_dev);
err_out:
    return -ENODEV;
}

#ifdef MBSS_SUPPORT
// Register multiple BSSID IF
VOID RT61_Init_MSSID(
	IN	PRTMP_ADAPTER	pAd,
	IN	struct net_device *net_dev)
{
	INT					index,loop_max;
	struct	net_device  *dev;
	VIRTUAL_ADAPTER     *pMBSSIDAd;
	INT					i = 0;
	CHAR				slot_name[IFNAMSIZ];
	struct net_device	*device;

	if (pAd->flg_mbss_init != 0)
		return;
	/* End of if */
	pAd->flg_mbss_init = 1;

	NICReadEEPROMParameters(pAd);
	loop_max = pAd->PortCfg.BssidNum;

	for(index=FIRST_MBSSID; index<MAX_MBSSID_NUM; index++)
		pAd->PortCfg.MBSSID[index].MSSIDDev = NULL;
	/* End of for */

	// Clear statistics
	for(index=MAIN_MBSSID; index<MAX_MBSSID_NUM; index++)
	{
		pAd->PortCfg.MBSSID[index].RxOkCount = 0;
		pAd->PortCfg.MBSSID[index].TxOkCount = 0;
		pAd->PortCfg.MBSSID[index].ReceivedByteCount = 0;
		pAd->PortCfg.MBSSID[index].TransmittedByteCount = 0;
		pAd->PortCfg.MBSSID[index].RxErrors = 0;
		pAd->PortCfg.MBSSID[index].TxErrors = 0;
		pAd->PortCfg.MBSSID[index].RxDrops = 0;
		pAd->PortCfg.MBSSID[index].TxDrops = 0;
	}
		
	for(index = FIRST_MBSSID; index < loop_max; index++)   // index from 1 is due to exclude the Main-SSID IF. 
	{
#if LINUX_VERSION_CODE <= 0x20402		// Red Hat 7.1
		dev = alloc_netdev(sizeof(VIRTUAL_ADAPTER), "eth%d", ether_setup);
#else
		dev = alloc_etherdev(sizeof(VIRTUAL_ADAPTER));
#endif
		
		{// find available 
			for (i = 0; i < 32; i++)
			{
				sprintf(slot_name, "ra%d", i);
				
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
				device = dev_get_by_name(slot_name);
#else
				for (device = dev_base; device != NULL; device = device->next)
				{
					if (strncmp(device->name, slot_name, 4) == 0)
					{
						break;
					}
				}
#endif
				if(device == NULL)	
					break;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
				else
					dev_put(device);
#endif
			}

			if (i != 32)
			{
				sprintf(dev->name, "ra%d", i);

				DBGPRINT(RT_DEBUG_TRACE, "Register MBSSID IF (ra%d)\n", i);
			}
			else
			{
				DBGPRINT(RT_DEBUG_ERROR, "Ethernet interface number overflow...\n");
				break;
			}
		}
		
		ether_setup(dev);
		pMBSSIDAd = dev->priv;

		pMBSSIDAd->VirtualDev    = dev;
		pMBSSIDAd->RtmpDev       = net_dev;

		NdisMoveMemory(&dev->dev_addr, pAd->PortCfg.MBSSID[index].Bssid, MAC_ADDR_LEN);
       	DBGPRINT(RT_DEBUG_TRACE,"Multiple-IF[%s] MAC=%02x:%02x:%02x:%02x:%02x:%02x\n", dev->name,
	            dev->dev_addr[0], dev->dev_addr[1],
	            dev->dev_addr[2], dev->dev_addr[3],
	            dev->dev_addr[4], dev->dev_addr[5]);

		dev->hard_start_xmit	= VirtualIFSendPackets;
		dev->stop				= VirtualIF_close;
		dev->open				= VirtualIF_open;
		dev->do_ioctl			= VirtualIF_ioctl;
		dev->get_stats			= VirtualIF_get_ether_stats;
		dev->set_multicast_list = VirtualIF_set_rx_mode;
		dev->priv_flags			= INT_MBSSID;

		// Register this device, cannt use register_netdev due to it has been lock before
		register_netdevice(dev);
		pAd->PortCfg.MBSSID[index].MSSIDDev = dev;

		// dev_open(pAd->PortCfg.MBSSID[index].MSSIDDev);
	}
}
#endif /* MBSS_SUPPORT */


#ifdef WDS_SUPPORT
VOID RT61_Init_WDSTAB(
	IN	PRTMP_ADAPTER	pAd)
{
	// Initialize WDS If table.
		int loopIdx;
		UCHAR ValidFlag[MAX_WDS_ENTRY];
		struct net_device *dev[MAX_WDS_ENTRY];

		for (loopIdx=0; loopIdx<MAX_WDS_ENTRY; loopIdx++)
		{
			dev[loopIdx] = pAd->WdsTab.WdsEntry[loopIdx].dev;
			ValidFlag[loopIdx] = pAd->WdsTab.WdsEntry[loopIdx].Valid;
		}
		NdisZeroMemory(&pAd->WdsTab, sizeof(RT_802_11_WDS_TABLE));

		for (loopIdx=0; loopIdx<MAX_WDS_ENTRY; loopIdx++)
		{
			pAd->WdsTab.WdsEntry[loopIdx].dev = dev[loopIdx];
			pAd->WdsTab.WdsEntry[loopIdx].Valid = ValidFlag[loopIdx];
		}
}


// Register WDS interface
VOID RT61_Init_WDS(
	IN	PRTMP_ADAPTER	pAd,
	IN	struct net_device *net_dev)
{
    INT					index;
    struct	net_device  *dev;
    VIRTUAL_ADAPTER     *pWdsAd;
	INT					i = 0;
	CHAR				slot_name[IFNAMSIZ];
	struct net_device	*device;

	if (pAd->flg_wds_init != 0)
		return;
	/* End of if */
	pAd->flg_wds_init = 1;

	//NdisZeroMemory(&pAd->WdsTab, sizeof(RT_802_11_WDS_TABLE));

    for(index = 0; index < MAX_WDS_ENTRY; index++)
    {
#if LINUX_VERSION_CODE <= 0x20402		// Red Hat 7.1
		dev = alloc_netdev(sizeof(VIRTUAL_ADAPTER), "eth%d", ether_setup);
#else
		dev = alloc_etherdev(sizeof(VIRTUAL_ADAPTER));
#endif
		
		{// find available 
			for (i = 0; i < 32; i++)
			{
				sprintf(slot_name, "ra%d", i);
				
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,21)				
				for (device = dev_base; device != NULL; device = device->next)
#else
				for (device = first_net_device(); device != NULL; device = next_net_device(device))
#endif
				{
					if (strncmp(device->name, slot_name, 4) == 0)
					{
						break;
					}
				}
				if(device == NULL)	break;
			}

			if(i != 32)
			{
				sprintf(dev->name, "ra%d", i);

				DBGPRINT(RT_DEBUG_TRACE, "Register WDS(virtual) interface(ra%d)-%02X:%02X:%02X:%02X:%02X:%02X\n", i, pAd->WdsTab.WdsEntry[index].WdsAddr[0],pAd->WdsTab.WdsEntry[index].WdsAddr[1],
					pAd->WdsTab.WdsEntry[index].WdsAddr[2],pAd->WdsTab.WdsEntry[index].WdsAddr[3],pAd->WdsTab.WdsEntry[index].WdsAddr[4],pAd->WdsTab.WdsEntry[index].WdsAddr[5]);
			}
			else
			{
				DBGPRINT(RT_DEBUG_ERROR, "Ethernet interface number overflow...\n");
				break;
			}
		}
		
		ether_setup(dev);
		pWdsAd = dev->priv;

		pWdsAd->VirtualDev  = dev;
		pWdsAd->RtmpDev     = net_dev;

		NdisMoveMemory(&dev->dev_addr, &net_dev->dev_addr, MAC_ADDR_LEN);

		dev->hard_start_xmit    = VirtualIFSendPackets;
		dev->stop               = VirtualIF_close;
		dev->open               = VirtualIF_open;
		dev->do_ioctl           = VirtualIF_ioctl;
		/* net-snmp always check all up interface statistic */
		/* add dev statistic for WDS if define WDS */
		dev->get_stats			= VirtualIF_get_ether_stats;
		dev->priv_flags			= INT_WDS;

		// Register this device
		register_netdevice(dev);
		pAd->WdsTab.WdsEntry[index].dev = dev;

		pAd->WdsTab.WdsEntry[index].CurrTxRate = RATE_11;
        if(pAd->PortCfg.PhyMode == PHY_11B)
            pAd->WdsTab.WdsEntry[index].MaxSupportedRate = RATE_11;
        else
            pAd->WdsTab.WdsEntry[index].MaxSupportedRate = RATE_54;

        // dev_open(pAd->WdsTab.WdsEntry[index].dev);
    }
}
#endif /* WDS_SUPPORT */


#ifdef APCLI_SUPPORT
// Register APCLI interface
VOID RT61_Init_APCLI(
	IN	PRTMP_ADAPTER	pAd,
	IN	struct net_device *net_dev)
{
    INT					index;
    struct	net_device  *dev;
    VIRTUAL_ADAPTER     *pApCliAd;
	INT					i = 0;
	CHAR				slot_name[IFNAMSIZ];
	struct net_device	*device;
//2007/07/26:KH Add to fix the bug that drivers will create new apcli interface when restart ra0.
if(pAd->ApCliUp)
	return;
pAd->ApCliUp=TRUE;

    for(index = 0; index < MAX_APCLI_ENTRY; index++)
    {
#if LINUX_VERSION_CODE <= 0x20402		// Red Hat 7.1
		dev = alloc_netdev(sizeof(VIRTUAL_ADAPTER), "eth%d", ether_setup);
#else
		dev = alloc_etherdev(sizeof(VIRTUAL_ADAPTER));
#endif
		
		{// find available 
			for (i = 0; i < 32; i++)
			{
				sprintf(slot_name, "apcli%d", i);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
				device = dev_get_by_name(slot_name);
#else
				for (device = dev_base; device != NULL; device = device->next)
				{
					if (strncmp(device->name, slot_name, 6) == 0)
					{
						break;
					}
				}
#endif
				if(device == NULL)
					break;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
				else
					dev_put(device);
#endif
			}

			if(i != 32)
			{
				sprintf(dev->name, "apcli%d", i);

				DBGPRINT(RT_DEBUG_TRACE, "Register APCLI(virtual) interface(apcli%d)-%02X:%02X:%02X:%02X:%02X:%02X\n", i, pAd->ApCliTab.ApCliEntry[index].ApCliBssid[0],pAd->ApCliTab.ApCliEntry[index].ApCliBssid[1],
					pAd->ApCliTab.ApCliEntry[index].ApCliBssid[2],pAd->ApCliTab.ApCliEntry[index].ApCliBssid[3],pAd->ApCliTab.ApCliEntry[index].ApCliBssid[4],pAd->ApCliTab.ApCliEntry[index].ApCliBssid[5]);
			}
			else
			{
				DBGPRINT(RT_DEBUG_ERROR, "Ethernet interface number overflow...\n");
				break;
			}
		}
		
		ether_setup(dev);
		pApCliAd = dev->priv;

		pApCliAd->VirtualDev  = dev;
		pApCliAd->RtmpDev     = net_dev;
      
		// reserve one idle Bssid for ApCli interface.
		COPY_MAC_ADDR(pAd->ApCliTab.ApCliEntry[index].CurrentAddress, pAd->CurrentAddress);
		pAd->ApCliTab.ApCliEntry[index].CurrentAddress[ETH_LENGTH_OF_ADDRESS - 1] =
			(pAd->ApCliTab.ApCliEntry[index].CurrentAddress[ETH_LENGTH_OF_ADDRESS - 1] + pAd->PortCfg.BssidNum) & 0xFF;

		NdisMoveMemory(&dev->dev_addr, pAd->ApCliTab.ApCliEntry[index].CurrentAddress, MAC_ADDR_LEN);
		
		dev->hard_start_xmit    = VirtualIFSendPackets;
		dev->stop               = VirtualIF_close;
		dev->open               = VirtualIF_open;
		dev->do_ioctl           = VirtualIF_ioctl;
		dev->get_stats			= VirtualIF_get_ether_stats;
		dev->priv_flags			= INT_APCLI;
        
		// Register this device
		register_netdevice(dev);
		
		pAd->ApCliTab.ApCliEntry[index].dev = dev;

        if(pAd->PortCfg.PhyMode == PHY_11B)
            pAd->ApCliTab.ApCliEntry[index].MaxSupportedRate = RATE_11;
        else
            pAd->ApCliTab.ApCliEntry[index].MaxSupportedRate = RATE_54;
    
		pAd->ApCliTab.ApCliEntry[index].CurrTxRate = pAd->ApCliTab.ApCliEntry[index].MaxSupportedRate;
// For AP Client support WPS Modification
#ifdef WSC_SUPPORT
        pAd->ApCliTab.ApCliEntry[index].WscControl.EntryApIdx = MIN_NET_DEVICE_FOR_APCLI;
        if (pAd->ApCliTab.ApCliEntry[index].WscControl.WscEnrolleePinCode == 0)
            pAd->ApCliTab.ApCliEntry[index].WscControl.WscEnrolleePinCode = WscGeneratePinCode(pAd, TRUE, 0);
        NdisZeroMemory(pAd->ApCliTab.ApCliEntry[index].WscControl.EntryAddr, MAC_ADDR_LEN);
// For AP Client support WPS Modification
        pAd->ApCliTab.ApCliEntry[index].WscControl.pAd = pAd;
// --
        WscInit(pAd, TRUE, &pAd->ApCliTab.ApCliEntry[index].WscControl);
#endif // WSC_SUPPORT //
// --
	}
}
#endif /* APCLI_SUPPORT */

#ifdef RTL865X_FAST_PATH
BOOLEAN	isFastPathCapable(PRTMP_ADAPTER	pAd)
{
    BOOLEAN	bFastPathCapable;
    int idx;
    	
	bFastPathCapable = TRUE;
	for (idx=0; idx<MAX_MBSSID_NUM; idx++) {
		if (pAd->PortCfg.MBSSID[idx].bWmmCapable) {
			bFastPathCapable = FALSE;
			break;
		}			
	}
	return (bFastPathCapable);
}
#endif 


INT RT61_open(
    IN  struct net_device *net_dev)
{
    PRTMP_ADAPTER   pAd = net_dev->priv;
    INT             status = NDIS_STATUS_SUCCESS;
    UCHAR			i;
#ifdef RTL865X_SOC
	unsigned int	linkid;
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
    if (!try_module_get(THIS_MODULE))
    {
        DBGPRINT(RT_DEBUG_ERROR, "%s: cannot reserve module\n", __FUNCTION__);
        return -1;
    }
#else
    MOD_INC_USE_COUNT;
#endif

	// Wait for hardware stable
	{
		ULONG  MacCsr0, Index = 0;
		
		do
		{
			RTMP_IO_READ32(pAd, MAC_CSR0, &MacCsr0);

			if (MacCsr0 != 0)
				break;
			
			RTMPusecDelay(1000);
		} while (Index++ < 1000);
	}

	// Load 8051 firmware
    status = NICLoadFirmware(pAd);
    if(status != NDIS_STATUS_SUCCESS)
    {
        goto out_module_put;
    }

	RTMPAllocAdapterBlock(pAd);
			
    // 1. Allocate DMA descriptors & buffers
    status = RTMPAllocDMAMemory(pAd);
    if (status != NDIS_STATUS_SUCCESS)
        goto out_module_put;

    //
	// 1.1 Init TX/RX data structures and related parameters
	//
	NICInitTxRxRingAndBacklogQueue(pAd);

    // 2. request interrupt
    // Disable interrupts here which is as soon as possible
    // This statement should never be true. We might consider to remove it later
    if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_ACTIVE))
    {
        NICDisableInterrupt(pAd);
    }

    status = request_irq(pAd->pPci_Dev->irq, &RTMPIsr, SA_SHIRQ, net_dev->name, net_dev);
    if (status)
    {
        goto out_free_dma_memory;
    }
    RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE);

 	
// For AP Client support WPS Modification
// move here before initialize MLME
#ifdef APCLI_SUPPORT
	// Initialize ApCli If table.
//2007/07/26:KH Add to fix the bug that drivers will create new apcli interface when restart ra0.
if(!pAd->ApCliUp)
	NdisZeroMemory(&pAd->ApCliTab, sizeof(RT_802_11_APCLI_TABLE));
#endif
// --
   // initialize MLME
    status = MlmeInit(pAd);
    if(status != NDIS_STATUS_SUCCESS)
    {
        goto out_free_irq;
    }

    // Initialize pAd->UserCfg to manufacture default
    //
	PortCfgInit(pAd);

    pAd->ScanTab.BssEntry = kmalloc(sizeof(BSS_ENTRY)*MAX_LEN_OF_BSS_TABLE, MEM_ALLOC_FLAG);
	if (pAd->ScanTab.BssEntry == NULL)
	{
		DBGPRINT(RT_DEBUG_ERROR, "Can't Allocate BssEntry\n");
		goto out_mlme_halt;
	}

    // Initialize Asics
    NICInitializeAdapter(pAd);

#ifdef WDS_SUPPORT
	RT61_Init_WDSTAB(pAd);
#endif

    // Read RaConfig profile parameters  
    RTMPReadParametersFromFile(pAd);

    // We should read EEPROM for all cases.
	NICReadEEPROMParameters(pAd);

	BuildChannelList(pAd);

	// Set PHY to appropriate mode
	for(i=0; i<pAd->PortCfg.BssidNum;i++)
	{
		RTMPSetPhyMode(pAd, pAd->PortCfg.PhyMode, i);
		RTMPBuildDesireRate(pAd, i,	pAd->PortCfg.MBSSID[i].DesiredRatesIndex);
	}

    NICInitAsicFromEEPROM(pAd);

    net_dev->dev_addr[0] = pAd->CurrentAddress[0];
	net_dev->dev_addr[1] = pAd->CurrentAddress[1];
	net_dev->dev_addr[2] = pAd->CurrentAddress[2];
	net_dev->dev_addr[3] = pAd->CurrentAddress[3];
	net_dev->dev_addr[4] = pAd->CurrentAddress[4];
	net_dev->dev_addr[5] = pAd->CurrentAddress[5];

	// move multiple bss, wds interface create here when insert module
#ifdef MBSS_SUPPORT 
	RT61_Init_MSSID(pAd, net_dev);
#endif 

#ifdef WDS_SUPPORT
	RT61_Init_WDS(pAd, net_dev);
#endif

#ifdef APCLI_SUPPORT
	if (pAd->CurrentAddress[ETH_LENGTH_OF_ADDRESS-1] % 2 == 0)  // add by johnli, fix WPAPSK bug if the MAC address is odd number
	{
		RT61_Init_APCLI(pAd, net_dev);
	}
#endif

#ifdef THREAD_ISR
	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_REMOVE_IN_PROGRESS);
	Rtmp_Init_Thread_Task(pAd);
#endif 

    //
	// Enable Interrupt
	//
	RTMP_IO_WRITE32(pAd, INT_SOURCE_CSR, 0xffffffff);  // clear garbage interrupts
 	NICEnableInterrupt(pAd, 0);

 	RTMP_IO_WRITE32(pAd, TXRX_CSR0, 0x0256b032);       // enable RX of MAC block

#ifdef APCLI_SUPPORT
	APCLIConvertEngineInit();
#endif

	ApInitialize(pAd);

	if (pAd->PortCfg.bAutoChannelAtBootup || (pAd->PortCfg.Channel == 0))
		pAd->PortCfg.Channel = APAutoSelectChannel(pAd);

	ApStartUp(pAd, TRUE);

#ifdef WSC_SUPPORT
    // <<WCN vista logo>> ++, add by johnli
    NdisZeroMemory(&Wsc_Uuid_Str[0], sizeof(Wsc_Uuid_Str));
    NdisZeroMemory(&Wsc_Uuid_E[0], sizeof(Wsc_Uuid_E));
    WscGenerateUUID(pAd, &Wsc_Uuid_E[0], &Wsc_Uuid_Str[0], 0);
    // <<WCN vista logo>> --
// For AP Client support WPS Modification
//    WscInit(pAd);
        WscInit(pAd, FALSE, &pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl);
// --
    // For Re-Generate Pin-Code Modification
    if (pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.WscEnrolleePinCode == 0)
    	pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.WscEnrolleePinCode = WscGeneratePinCode(pAd, FALSE, 0);
    // --
#endif // WSC_SUPPORT //

#ifdef RTL865X_SOC
	if (devglue_regExtDevice(net_dev->name, CONFIG_8139CP_VID, CONFIG_RTL865XB_WLAN1_PORT, 
						&linkid)) {					
		printk("cannot register external %s device on extPort %d\n", net_dev->name, CONFIG_RTL865XB_WLAN1_PORT);
	} else {		
		printk("register external %s device on extPort %d, id  %d\n", net_dev->name, CONFIG_RTL865XB_WLAN1_PORT, linkid);
	}
	pAd->PortCfg.MBSSID[MAIN_MBSSID].mylinkid = linkid;

#ifdef RTL865X_FAST_PATH
	if (isFastPathCapable(pAd)) {
	rtl865x_extDev_registerUcastTxDev(net_dev);
	rtl865x_extDev_regCallBack(linkid);
	}
#endif

#endif /* RTL865X_SOC */

    // Start net interface tx /rx
    netif_start_queue(net_dev);

    netif_carrier_on(net_dev);
    netif_wake_queue(net_dev);

#ifdef TC_SOC // add polling mode
	if (pAd->PollingModeDetectRunning == FALSE)
	{
	    printk("jiffies=%08lx, POLLING_MODE_DETECT_INTV=%d\r\n", jiffies, POLLING_MODE_DETECT_INTV);
	    pAd->PollingModeDetect.expires = jiffies + POLLING_MODE_DETECT_INTV;
	    add_timer(&pAd->PollingModeDetect);
	}
// init a BH task here
	INIT_WORK(&pAd->PollingDataBH, PollingModeIsr);
#endif

    return 0;

out_mlme_halt:
	MlmeHalt(pAd);
out_free_irq:
	free_irq(net_dev->irq, net_dev);
out_free_dma_memory:
	RTMPFreeDMAMemory(pAd);	
out_module_put:
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
    module_put(THIS_MODULE);
#else
    MOD_DEC_USE_COUNT;
#endif

    return status;
}

/*
    ========================================================================

    Routine Description:
        hard_start_xmit handler

    Arguments:
        skb             point to sk_buf which upper layer transmit
        net_dev         point to net_dev
    Return Value:
        None

    Note:

    ========================================================================
*/

#ifdef RTL865X_FAST_PATH
INT RTMPSendPackets(
    IN  struct sk_buff		*pSkb,
    IN  struct net_device	*net_dev)
{
   /* indicate this packet is from Protocol Stack : for socket buffer free usage */
	RTL865X_SET_PKTTYPE(pSkb, RTL865X_TYPE_PS);
	RTMPSendFastPackets(pSkb, net_dev);		
}
#endif

	

#ifdef RTL865X_FAST_PATH
INT RTMPSendFastPackets(
    IN  struct sk_buff		*pSkb,
    IN  struct net_device	*net_dev)
#else
INT RTMPSendPackets(
    IN  struct sk_buff		*pSkb,
    IN  struct net_device	*net_dev)
#endif
{
    PRTMP_ADAPTER   pAdapter = net_dev->priv;

    DBGPRINT(RT_DEBUG_INFO, "===> RTMPSendPackets\n");

#ifdef RALINK_ATE
	if (pAdapter->ate.Mode != ATE_APSTART)
	{
#ifndef RTL865X_FAST_PATH
		RTMPFreeSkbBuffer(pSkb);
		return 0;
#else
		/* caller free mbuf */
		return 1;
#endif			
	}
#endif

    if (RTMP_TEST_FLAG(pAdapter, fRTMP_ADAPTER_RESET_IN_PROGRESS) ||
    	RTMP_TEST_FLAG(pAdapter, fRTMP_ADAPTER_RADIO_OFF)         ||
		RTMP_TEST_FLAG(pAdapter, fRTMP_ADAPTER_HALT_IN_PROGRESS))
	{
#ifndef RTL865X_FAST_PATH
		// Drop send request since hardware is in reset state
		RTMPFreeSkbBuffer(pSkb);
#else
		/* caller free mbuf */
		return 1;
#endif			
	}
    else
    {
        // initial pSkb->data_len=0, we will use this variable to store data size when fragment(in TKIP)
        // and pSkb->len is actual data len
        //pSkb->data_len = pSkb->len; /* We didn't need asign this becasue skb->len has different means to skb->data_len. */

        // Record that orignal packet source is from protocol layer,so that 
		// later on driver knows how to release this skb buffer
		RTMP_SET_PACKET_SOURCE(pSkb, PKTSRC_NDIS);
        RTMP_SET_PACKET_MOREDATA(pSkb, FALSE);
        pAdapter->RalinkCounters.PendingNdisPacketCount ++;
        
        RTMPSendPacket(pAdapter, pSkb);
    }

    // Dequeue one frame from TxSwQueue[] and process it
    if ((!RTMP_TEST_FLAG(pAdapter, fRTMP_ADAPTER_BSS_SCAN_IN_PROGRESS)) && 
		(!RTMP_TEST_FLAG(pAdapter, fRTMP_ADAPTER_RESET_IN_PROGRESS)) &&
		(!RTMP_TEST_FLAG(pAdapter, fRTMP_ADAPTER_HALT_IN_PROGRESS)))
    {
        // let the subroutine select the right priority Tx software queue
		RTMPDeQueuePacket(pAdapter, FALSE, FALSE);
    }

    return 0;
}

INT VirtualIFSendPackets(
	IN	struct sk_buff *pSkb, 
	IN	struct net_device *dev)
{
    INT             i;
    VIRTUAL_ADAPTER *pVirtualAd = dev->priv;
    RTMP_ADAPTER    *pAd = pVirtualAd->RtmpDev->priv;

    DBGPRINT(RT_DEBUG_INFO, "VirtualIFSendPackets --->\n");

#ifdef RALINK_ATE
	if (pAd->ate.Mode != ATE_APSTART)
	{
		RTMPFreeSkbBuffer(pSkb);
		return 0;
	}
#endif

    if ((RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_BSS_SCAN_IN_PROGRESS)) ||
    	(RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF))          ||
    	(RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_RESET_IN_PROGRESS)))
	{
		RTMPFreeSkbBuffer(pSkb);
		return 0;
	}
	
    if (!(pVirtualAd->RtmpDev->flags & IFF_UP))
    {
        RTMPFreeSkbBuffer(pSkb);
        return 0;
    }
	
#ifdef MBSS_SUPPORT
    for (i = FIRST_MBSSID; i < pAd->PortCfg.BssidNum; i++)
    {
        if (pAd->PortCfg.MBSSID[i].MSSIDDev == dev)
        {
        	pAd->RalinkCounters.PendingNdisPacketCount ++;
        	RTMP_SET_PACKET_SOURCE(pSkb, PKTSRC_NDIS);
        	RTMP_SET_PACKET_MOREDATA(pSkb, FALSE);
            RTMP_SET_PACKET_NET_DEVICE_MBSSID(pSkb, i);
            pSkb->dev = pVirtualAd->RtmpDev;
            dev_queue_xmit(pSkb);
            if (!(*pSkb->data & 0x01))
            DBGPRINT(RT_DEBUG_INFO, "VirtualIFSendPackets(MBSSID) - packet to (ra%d)\n", i);
            return 0;
        }
    }
#endif /* MBSS_SUPPORT */

#ifdef WDS_SUPPORT
    for (i = 0; i < MAX_WDS_ENTRY; i++)
    {
        if ((pAd->WdsTab.WdsEntry[i].Valid == TRUE) && (pAd->WdsTab.WdsEntry[i].dev == dev))
        {
        	pAd->RalinkCounters.PendingNdisPacketCount ++;
        	RTMP_SET_PACKET_SOURCE(pSkb, PKTSRC_NDIS);
        	RTMP_SET_PACKET_MOREDATA(pSkb, FALSE);
            RTMP_SET_PACKET_NET_DEVICE_WDS(pSkb, i);
            pSkb->dev = pVirtualAd->RtmpDev;
            dev_queue_xmit(pSkb);
            DBGPRINT(RT_DEBUG_INFO, "VirtualIFSendPackets(WDS) - packet to (%d)\n", i);
            return 0;
        }
    }
#endif 

#ifdef APCLI_SUPPORT
    for (i = 0; i < MAX_APCLI_ENTRY; i++)
    {
        if ((pAd->ApCliTab.ApCliEntry[i].Valid == TRUE) && (pAd->ApCliTab.ApCliEntry[i].dev == dev))
        {
        	pAd->RalinkCounters.PendingNdisPacketCount ++;
        	RTMP_SET_PACKET_SOURCE(pSkb, PKTSRC_NDIS);
        	RTMP_SET_PACKET_MOREDATA(pSkb, FALSE);
            RTMP_SET_PACKET_NET_DEVICE_APCLI(pSkb, i);
            pSkb->dev = pVirtualAd->RtmpDev;
            dev_queue_xmit(pSkb);
            DBGPRINT(RT_DEBUG_INFO, "VirtualIFSendPackets(APCLI) - packet to (apcli%d)\n", i);
            return 0;
        }
    }
#endif 

    DBGPRINT(RT_DEBUG_INFO, "VirtualIFSendPackets - needn't to send or net_device not exist.\n");
    RTMPFreeSkbBuffer(pSkb);

    return 0;
}

INT VirtualIF_open(
	IN	struct net_device *dev)
{
	VIRTUAL_ADAPTER	*pVirtualAd = dev->priv;
#ifdef RTL865X_SOC
	RTMP_ADAPTER	*pAd = pVirtualAd->RtmpDev->priv;
	INT				index;
	unsigned int 	linkid;
#endif

	DBGPRINT(RT_DEBUG_TRACE, "%s: ===> VirtualIF_open\n", dev->name);

#ifdef RTL865X_SOC
#ifdef MBSS_SUPPORT
	for(index = FIRST_MBSSID; index < pAd->PortCfg.BssidNum; index++)   // index from 1 is due to exclude the Main-SSID IF. 
	{
		if (pAd->PortCfg.MBSSID[index].MSSIDDev == dev) {
			if (devglue_regExtDevice(dev->name, CONFIG_8139CP_VID, CONFIG_RTL865XB_WLAN1_PORT, 
							&linkid)) {					
				printk("cannot register external %s device on extPort %d\n", dev->name, CONFIG_RTL865XB_WLAN1_PORT);
				return (0);
			} else {		
				printk("register external %s device on extPort %d, id  %d\n", dev->name, CONFIG_RTL865XB_WLAN1_PORT, linkid);
			}
			pAd->PortCfg.MBSSID[index].mylinkid = linkid;
			break;
		}							
	}
#endif /* MBSS_SUPPORT */
#ifdef WDS_SUPPORT
    for (index = 0; index < MAX_WDS_ENTRY; index++)
    {
        if (pAd->WdsTab.WdsEntry[index].dev == dev)
        {
			if (devglue_regExtDevice(dev->name, CONFIG_8139CP_VID, CONFIG_RTL865XB_WLAN1_PORT, 
							&linkid)) {					
				printk("cannot register external %s device on extPort %d\n", dev->name, CONFIG_RTL865XB_WLAN1_PORT);
				return (0);
			} else {		
				printk("register external %s device on extPort %d, id  %d\n", dev->name, CONFIG_RTL865XB_WLAN1_PORT, linkid);
			}
			pAd->WdsTab.WdsEntry[index].mylinkid = linkid;
			break;
        }
    }
#endif /* WDS_SUPPORT */
#ifdef APCLI_SUPPORT
    for (index = 0; index < MAX_APCLI_ENTRY; index++)
    {
        if (pAd->ApCliTab.ApCliEntry[index].dev == dev)
        {
			if (devglue_regExtDevice(dev->name, CONFIG_8139CP_VID, CONFIG_RTL865XB_WLAN1_PORT, 
							&linkid)) {					
				printk("cannot register external %s device on extPort %d\n", dev->name, CONFIG_RTL865XB_WLAN1_PORT);
				return (0);
			} else {		
				printk("register external %s device on extPort %d, id  %d\n", dev->name, CONFIG_RTL865XB_WLAN1_PORT, linkid);
			}
			pAd->ApCliTab.ApCliEntry[index].mylinkid = linkid;
			break;
        }
    }
#endif /* APCLI_SUPPORT */
#endif /* RTL865X_SOC */

#ifdef APCLI_SUPPORT
    if (dev->priv_flags == INT_APCLI)
    {
		UCHAR ifIndex;
		RTMP_ADAPTER *pAd = pVirtualAd->RtmpDev->priv;
        for (ifIndex = 0; ifIndex < MAX_APCLI_ENTRY; ifIndex++)
        {
            if (pAd->ApCliTab.ApCliEntry[ifIndex].dev == dev)
            {
				// indicate the apcli(%d) ready.
                pAd->ApCliTab.ApCliEntry[ifIndex].InterfaceReady = TRUE;

				DBGPRINT(RT_DEBUG_TRACE, "(%s) ApCli interface[%d] startup.\n", __FUNCTION__, ifIndex);
                break;
            }
        }
    }
#endif /* APCLI_SUPPORT */
	netif_start_queue(pVirtualAd->VirtualDev);
	
	return 0;
}

INT VirtualIF_close(
	IN	struct net_device *dev)
{
	VIRTUAL_ADAPTER	*pVirtualAd = dev->priv;

	DBGPRINT(RT_DEBUG_TRACE, "%s: ===> VirtualIF_close\n", dev->name);

#ifdef APCLI_SUPPORT
    if (dev->priv_flags == INT_APCLI)
    {
		UCHAR ifIndex;
		RTMP_ADAPTER *pAd = pVirtualAd->RtmpDev->priv;

		for(ifIndex = 0; ifIndex < MAX_APCLI_ENTRY; ifIndex++)
		{
            if (pAd->ApCliTab.ApCliEntry[ifIndex].dev == dev)
            {
				// indicate the apcli(%d) not ready.
                pAd->ApCliTab.ApCliEntry[ifIndex].InterfaceReady = FALSE;

				// send disconnect-req to sta State Machine.
				if (pAd->ApCliTab.ApCliEntry[ifIndex].Valid)
				{
					StaMlmeEnqueue(pAd, STA_CTRL_STATE_MACHINE, STA_CTRL_DISCONNECT_REQ, 0, NULL, ifIndex);
					MlmeHandler(pAd);
					DBGPRINT(RT_DEBUG_TRACE, "(%s) ApCli interface[%d] startdown.\n", __FUNCTION__, ifIndex);
				}
                break;
            }
		}
    }
#endif

	netif_stop_queue(pVirtualAd->VirtualDev);	
	
	return 0;
}

INT VirtualIF_ioctl(
	IN	struct net_device	*net_dev, 
	IN	OUT	struct ifreq	*rq, 
	IN	INT					cmd)
{
	VIRTUAL_ADAPTER	*pVirtualAd = net_dev->priv;
	RTMP_ADAPTER	*pAd = pVirtualAd->RtmpDev->priv;
	// struct iwreq	*wrq = (struct iwreq *) rq;
	
	if(!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE))
	{
		DBGPRINT(RT_DEBUG_TRACE, "VirtualIF_ioctl(%s)::Network is down!\n", net_dev->name);
		return -ENETDOWN;
	}

	return RT61_ioctl(net_dev, rq, cmd);
}

struct net_device_stats *VirtualIF_get_ether_stats(
	IN	struct net_device *net_dev)
{
	VIRTUAL_ADAPTER		*pVirtualAd = net_dev->priv;
    RTMP_ADAPTER		*pAd = pVirtualAd->RtmpDev->priv;
	UCHAR				apidx;
    INT					i;
	
	
#ifdef APCLI_SUPPORT
	for (i = 0; i < MAX_APCLI_ENTRY; i++)
    {
        if ((pAd->ApCliTab.ApCliEntry[i].Valid == TRUE) && (pAd->ApCliTab.ApCliEntry[i].dev == net_dev))
        {
        	pAd->stats.rx_packets = pAd->ApCliTab.ApCliEntry[i].RxOkCount;		// total packets received
			pAd->stats.tx_packets = pAd->ApCliTab.ApCliEntry[i].TxOkCount;		// total packets transmitted
	
			pAd->stats.rx_bytes=pAd->ApCliTab.ApCliEntry[i].ReceivedByteCount;	// total bytes received
			pAd->stats.tx_bytes =pAd->ApCliTab.ApCliEntry[i].TransmittedByteCount;	// total bytes transmitted

			pAd->stats.rx_errors = pAd->ApCliTab.ApCliEntry[i].RxErrors;		// bad packets received
			pAd->stats.tx_errors = pAd->ApCliTab.ApCliEntry[i].TxErrors;		// packet transmit problems

			pAd->stats.rx_dropped = pAd->ApCliTab.ApCliEntry[i].RxDrops;		// no space in linux buffers
			pAd->stats.tx_dropped = pAd->ApCliTab.ApCliEntry[i].TxDrops;
			break;
        }
   	}
#endif

#ifdef  WDS_SUPPORT
	for(i=0;i<MAX_WDS_ENTRY;i++)
	{
		if(net_dev == pAd->WdsTab.WdsEntry[i].dev)
	    {
	    	pAd->stats.rx_packets = pAd->WdsTab.WdsEntry[i].RxOkCount;		// total packets received
        	pAd->stats.tx_packets = pAd->WdsTab.WdsEntry[i].TxOkCount;		// total packets transmitted
	
   			pAd->stats.rx_bytes= pAd->WdsTab.WdsEntry[i].ReceivedByteCount;	// total bytes received
			pAd->stats.tx_bytes = pAd->WdsTab.WdsEntry[i].TransmittedByteCount;	// total bytes transmitted

			pAd->stats.rx_errors = pAd->WdsTab.WdsEntry[i].RxErrors;			// bad packets received
			pAd->stats.tx_errors = pAd->WdsTab.WdsEntry[i].TxErrors;			// packet transmit problems

			pAd->stats.rx_dropped = pAd->WdsTab.WdsEntry[i].RxDrops;			// no space in linux buffers
			pAd->stats.tx_dropped = pAd->WdsTab.WdsEntry[i].TxDrops;
	       	break;
		}
	}
#endif

	for (apidx = 0; apidx < pAd->PortCfg.BssidNum; apidx++)
	{
		if (net_dev == pAd->PortCfg.MBSSID[apidx].MSSIDDev)
			break;
	}
	if (apidx == pAd->PortCfg.BssidNum)
		return &pAd->stats;
	else
	{
		DBGPRINT(RT_DEBUG_INFO, "IF(ra%d) VirtualIF_get_ether_stats --->\n", apidx);

		pAd->stats.rx_packets = pAd->PortCfg.MBSSID[apidx].RxOkCount;		// total packets received
		pAd->stats.tx_packets = pAd->PortCfg.MBSSID[apidx].TxOkCount;		// total packets transmitted
	
		pAd->stats.rx_bytes= pAd->PortCfg.MBSSID[apidx].ReceivedByteCount;				// total bytes received
		pAd->stats.tx_bytes = pAd->PortCfg.MBSSID[apidx].TransmittedByteCount;			// total bytes transmitted

		pAd->stats.rx_errors = pAd->PortCfg.MBSSID[apidx].RxErrors;						// bad packets received
		pAd->stats.tx_errors = pAd->PortCfg.MBSSID[apidx].TxErrors;							// packet transmit problems

		pAd->stats.rx_dropped = pAd->PortCfg.MBSSID[apidx].RxDrops;						// no space in linux buffers
		pAd->stats.tx_dropped = pAd->PortCfg.MBSSID[apidx].TxDrops;					// no space available in linux
	}
	
	pAd->stats.multicast = 0;	// multicast packets received
	pAd->stats.collisions = 0;	// Collision packets

	pAd->stats.rx_length_errors = 0;
	pAd->stats.rx_over_errors = pAd->Counters8023.RxNoBuffer;					// receiver ring buff overflow
	pAd->stats.rx_crc_errors = 0;//pAd->WlanCounters.FCSErrorCount;		// recved pkt with crc error
	pAd->stats.rx_frame_errors = pAd->Counters8023.RcvAlignmentErrors;			// recv'd frame alignment error
	pAd->stats.rx_fifo_errors = pAd->Counters8023.RxNoBuffer;					// recv'r fifo overrun
	pAd->stats.rx_missed_errors = 0;											// receiver missed packet
	
	// detailed tx_errors
	pAd->stats.tx_aborted_errors = 0;
	pAd->stats.tx_carrier_errors = 0;
	pAd->stats.tx_fifo_errors = 0;
	pAd->stats.tx_heartbeat_errors = 0;
	pAd->stats.tx_window_errors = 0;
	
	// for cslip etc
	pAd->stats.rx_compressed = 0;
	pAd->stats.tx_compressed = 0;

	return &pAd->stats;
}

VOID VirtualIF_set_rx_mode(
	IN  struct net_device *net_dev)
{
	RT61_set_rx_mode(net_dev);
	return;
}

/*
    ========================================================================

    Routine Description:
        Interrupt handler

    Arguments:
        irq                         interrupt line
        dev_instance                Pointer to net_device
        rgs                         store process's context before entering ISR, 
                                    this parameter is just for debug purpose.

    Return Value:
        VOID

    Note:

    ========================================================================
*/
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
irqreturn_t
#else
VOID
#endif
RTMPIsr(
    IN  INT             irq, 
    IN  VOID            *dev_instance, 
    IN  struct pt_regs  *rgs)
{
    struct net_device		*net_dev = dev_instance;
    PRTMP_ADAPTER			pAdapter = net_dev->priv;
    INT_SOURCE_CSR_STRUC	IntSource;
    MCU_INT_SOURCE_STRUC	McuIntSource;
	UINT32 					int_mask = 0;

    DBGPRINT(RT_DEBUG_INFO, "====> RTMPHandleInterrupt\n");
    //
	// Inital the Interrupt source.
	//
	IntSource.word = 0x00000000L;
	McuIntSource.word = 0x00000000L;

    // 1. Disable interrupt
	if (RTMP_TEST_FLAG(pAdapter, fRTMP_ADAPTER_INTERRUPT_IN_USE) && RTMP_TEST_FLAG(pAdapter, fRTMP_ADAPTER_INTERRUPT_ACTIVE))
	{
		NICDisableInterrupt(pAdapter);
	}

    //
	// Get the interrupt sources & saved to local variable
	//
    RTMP_IO_READ32(pAdapter, MCU_INT_SOURCE_CSR, &McuIntSource.word);
    RTMP_IO_WRITE32(pAdapter, MCU_INT_SOURCE_CSR, McuIntSource.word);
	RTMP_IO_READ32(pAdapter, INT_SOURCE_CSR, &IntSource.word);
	RTMP_IO_WRITE32(pAdapter, INT_SOURCE_CSR, IntSource.word); // write 1 to clear

    //
	// Handle interrupt, walk through all bits
	// Should start from highest priority interrupt
	// The priority can be adjust by altering processing if statement
	//

	// If required spinlock, each interrupt service routine has to acquire
	// and release itself.
	//

#ifndef THREAD_ISR
    if (IntSource.field.MgmtDmaDone)
        RTMPHandleMgmtRingDmaDoneInterrupt(pAdapter);
    
	if (IntSource.field.RxDone)
		RTMPHandleRxDoneInterrupt(pAdapter);

	if (IntSource.field.TxDone)
		RTMPHandleTxDoneInterrupt(pAdapter);

    if (IntSource.word & 0x002f0000)
        RTMPHandleTxRingDmaDoneInterrupt(pAdapter, IntSource);
#else

	int_mask = Rtmp_Thread_Isr(pAdapter, IntSource);

#endif /* THREAD_ISR */


    if (McuIntSource.word & 0xff)
    {
        ULONG  M2hCmdDoneCsr;

#ifdef CARRIER_DETECTION_SUPPORT
		if (pAdapter->PortCfg.CarrierDetect.Enable)
			CarrierDetectionCheck(pAdapter);
#endif // CARRIER_DETECTION_SUPPORT //

        RTMP_IO_READ32(pAdapter, M2H_CMD_DONE_CSR, &M2hCmdDoneCsr);
        RTMP_IO_WRITE32(pAdapter, M2H_CMD_DONE_CSR, 0xffffffff);
		DBGPRINT(RT_DEBUG_INFO, "MCU command done - INT bitmap=0x%02x, M2H mbox=0x%08x\n", McuIntSource.word, M2hCmdDoneCsr);
    }

	if (McuIntSource.field.TBTTExpire)
		RTMPHandleTBTTInterrupt(pAdapter);

//    if (McuIntSource.field.Twakeup)
//        RTMPHandleTwakeupInterrupt(pAdapter);

    // Do nothing if Reset in progress
    if (RTMP_TEST_FLAG(pAdapter, fRTMP_ADAPTER_RESET_IN_PROGRESS) ||
    	RTMP_TEST_FLAG(pAdapter, fRTMP_ADAPTER_RADIO_OFF)         ||
		RTMP_TEST_FLAG(pAdapter, fRTMP_ADAPTER_HALT_IN_PROGRESS))
    {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
        return  IRQ_HANDLED;
#else
        return;
#endif
    }

    //
    // Re-enable the interrupt (disabled in RTMPIsr)
    //
	NICEnableInterrupt(pAdapter, int_mask);

    DBGPRINT(RT_DEBUG_INFO, "<==== RTMPHandleInterrupt\n");
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
    return  IRQ_HANDLED;
#endif
}

#ifdef TC_SOC
/*
    ========================================================================

    Routine Description:
       Trendchip DMT Trainning status detect

    Arguments:
        data                     Point to RTMP_ADAPTER

    Return Value:
        NONE

    Note: 

    ========================================================================
*/
VOID PeriodicPollingModeDetect(
	IN	unsigned long data)
{
  UCHAR modem_status=0; 
  RTMP_ADAPTER *pAd = (RTMP_ADAPTER *)data;

  os_TCIfQuery (0x0002,&modem_status,NULL);
  if ((modem_status==0x08)||(modem_status==0x10))
  {
	// disable enterrupt
	*(volatile unsigned int*)0xbfb40004  &= (~(1<<pAd->pPci_Dev->irq)); 
	schedule_work(&pAd->PollingDataBH);
	//slow down the POLLING MODE DETECT while the dmt in wait init state
  	pAd->PollingModeDetect.expires = jiffies + POLLING_MODE_DETECT_INTV;
  	mod_timer(&pAd->PollingModeDetect,pAd->PollingModeDetect.expires);
  	pAd->PollingModeDetectRunning = TRUE;
	return;
  }
  else
	*(volatile unsigned int*)0xbfb40004 |= (1<<pAd->pPci_Dev->irq); 

  pAd->PollingModeDetect.expires = jiffies + POLLING_MODE_DETECT_INTV;
  mod_timer(&pAd->PollingModeDetect,pAd->PollingModeDetect.expires);
  pAd->PollingModeDetectRunning = TRUE;
  return;
}

VOID PollingModeIsr(struct work_struct *work)
{
  	RTMP_ADAPTER *pAd = container_of(work, RTMP_ADAPTER, PollingDataBH);
	struct net_device *net_dev = pAd->net_dev;
	RTMPIsr( 0,  net_dev, NULL);
}
#endif

#if WIRELESS_EXT >= 12
/*
    ========================================================================

    Routine Description:
        get wireless statistics

    Arguments:
        net_dev                     Pointer to net_device

    Return Value:
        struct iw_statistics

    Note:
        This function will be called when query /proc

    ========================================================================
*/
long rt_abs(long arg)	{	return (arg<0)? -arg : arg;}
struct iw_statistics *RT61_get_wireless_stats(
    IN  struct net_device *net_dev)
{
    RTMP_ADAPTER *pAd = net_dev->priv;

    DBGPRINT(RT_DEBUG_TRACE, "RT61_get_wireless_stats --->\n");

    // TODO: All elements are zero before be implemented

    pAd->iw_stats.status = 0;                       // Status - device dependent for now

    pAd->iw_stats.qual.qual = 0;//pAd->Mlme.RoamCqi;            // link quality (%retries, SNR, %missed beacons or better...)
    pAd->iw_stats.qual.level = abs(pAd->PortCfg.LastRssi);   // signal level (dBm)
    pAd->iw_stats.qual.level += 256 - pAd->BbpRssiToDbmDelta;   // signal level (dBm)

    if (pAd->LatchRfRegs.Channel <= 14)
       pAd->iw_stats.qual.noise = (pAd->BbpWriteLatch[17] > pAd->BbpTuning.R17UpperBoundG) ? pAd->BbpTuning.R17UpperBoundG : ((ULONG) pAd->BbpWriteLatch [17]);           // noise level (dBm)
    else
       pAd->iw_stats.qual.noise = (pAd->BbpWriteLatch[17] > pAd->BbpTuning.R17UpperBoundA) ? pAd->BbpTuning.R17UpperBoundA : ((ULONG) pAd->BbpWriteLatch [17]);           // noise level (dBm)
    pAd->iw_stats.qual.noise += 256 - 143;
    //pAd->iw_stats.qual.noise = 0;           // noise level (dBm)
    pAd->iw_stats.qual.updated = 1;     // Flags to know if updated

    pAd->iw_stats.discard.nwid = 0;     // Rx : Wrong nwid/essid
    pAd->iw_stats.miss.beacon = 0;      // Missed beacons/superframe

    // pAd->iw_stats.discard.code, discard.fragment, discard.retries, discard.misc has counted in other place

    return &pAd->iw_stats;
}
#endif

/*
    ========================================================================

    Routine Description:
        return ethernet statistics counter

    Arguments:
        net_dev                     Pointer to net_device

    Return Value:
        net_device_stats*

    Note:

    ========================================================================
*/
struct net_device_stats *RT61_get_ether_stats(
    IN  struct net_device *net_dev)
{
    RTMP_ADAPTER *pAd = net_dev->priv;

    DBGPRINT(RT_DEBUG_INFO, "RT61_get_ether_stats --->\n");
#if 1
    pAd->stats.rx_packets = pAd->PortCfg.MBSSID[MAIN_MBSSID].RxOkCount;        // total packets received
    pAd->stats.tx_packets = pAd->PortCfg.MBSSID[MAIN_MBSSID].TxOkCount;     // total packets transmitted

    pAd->stats.rx_bytes = pAd->PortCfg.MBSSID[MAIN_MBSSID].ReceivedByteCount;             // total bytes received
    pAd->stats.tx_bytes = pAd->PortCfg.MBSSID[MAIN_MBSSID].TransmittedByteCount;         // total bytes transmitted

    pAd->stats.rx_errors = pAd->PortCfg.MBSSID[MAIN_MBSSID].RxErrors;                          // bad packets received
    pAd->stats.tx_errors = pAd->PortCfg.MBSSID[MAIN_MBSSID].TxErrors;	//pAd->Counters8023.TxErrors;                          // packet transmit problems

    pAd->stats.rx_dropped = pAd->PortCfg.MBSSID[MAIN_MBSSID].RxDrops;		//pAd->Counters8023.RxNoBuffer;                       // no space in linux buffers
    pAd->stats.tx_dropped = pAd->PortCfg.MBSSID[MAIN_MBSSID].TxDrops;		//pAd->WlanCounters.FailedCount;                  // no space available in linux

    pAd->stats.multicast = pAd->WlanCounters.MulticastReceivedFrameCount;   // multicast packets received
    pAd->stats.collisions = pAd->Counters8023.OneCollision + pAd->Counters8023.MoreCollisions;  // Collision packets

    pAd->stats.rx_length_errors = 0;
    pAd->stats.rx_over_errors = pAd->Counters8023.RxNoBuffer;                   // receiver ring buff overflow
    pAd->stats.rx_crc_errors = 0;//pAd->WlanCounters.FCSErrorCount;     // recved pkt with crc error
    pAd->stats.rx_frame_errors = pAd->Counters8023.RcvAlignmentErrors;          // recv'd frame alignment error
    pAd->stats.rx_fifo_errors = pAd->Counters8023.RxNoBuffer;                   // recv'r fifo overrun
    pAd->stats.rx_missed_errors = 0;                                            // receiver missed packet

    // detailed tx_errors
    pAd->stats.tx_aborted_errors = 0;
    pAd->stats.tx_carrier_errors = 0;
    pAd->stats.tx_fifo_errors = 0;
    pAd->stats.tx_heartbeat_errors = 0;
    pAd->stats.tx_window_errors = 0;

    // for cslip etc
    pAd->stats.rx_compressed = 0;
    pAd->stats.tx_compressed = 0;
#endif
    return &pAd->stats;
}

/*
    ========================================================================

    Routine Description:
        Set to filter multicast list

    Arguments:
        net_dev                     Pointer to net_device

    Return Value:
        VOID

    Note:

    ========================================================================
*/
VOID RT61_set_rx_mode(
    IN  struct net_device *net_dev)
{
	INT i;
	VIRTUAL_ADAPTER *pVirtualAd;
	RTMP_ADAPTER *pAd = net_dev->priv;
	UCHAR mfilterAddr[MAC_ADDR_LEN];

	// determine this ioctl command is comming from which interface.
	if (net_dev->priv_flags == INT_MAIN)
	{
		pAd= net_dev->priv;
	}
	else if ((net_dev->priv_flags == INT_MBSSID)
			|| (net_dev->priv_flags == INT_APCLI))
	{
		pVirtualAd = net_dev->priv;
		pAd = pVirtualAd->RtmpDev->priv;
	}

	/* Note: do not reorder, GCC is clever about common statements. */
	if ((net_dev->flags & IFF_PROMISC)
		|| (net_dev->flags & IFF_ALLMULTI))
	{
		/* Unconditionally log net taps. */
		DBGPRINT (RT_DEBUG_INFO, "%s: Promiscuous or Accept-All_Multicase mode enabled.\n", net_dev->name);
	} else if (net_dev->flags & IFF_UP)
	{
		struct dev_mc_list *mclist;

#ifdef IGMP_SNOOP_SUPPORT
			MulticastFilterTableUpdate(pAd->pMulticastFilterTable,MCAT_FILTER_LLC);
#endif // IGMP_SNOOP_SUPPORT //

		for (i = 0, mclist = net_dev->mc_list; mclist && i < net_dev->mc_count;
			i++, mclist = mclist->next)
		{
			COPY_MAC_ADDR(mfilterAddr, mclist->dmi_addr);

#ifdef IGMP_SNOOP_SUPPORT
			//MulticastFilterTableUpdate(pAd->pMulticastFilterTable,MCAT_FILTER_LLC);
			MulticastFilterTableInsertEntry(pAd->pMulticastFilterTable, mfilterAddr, NULL, net_dev, MCAT_FILTER_LLC,IGMPV1V2);
#endif // IGMP_SNOOP_SUPPORT //
			DBGPRINT(RT_DEBUG_INFO, "%s (%2x:%2x:%2x:%2x:%2x:%2x)\n",
				net_dev->name, mfilterAddr[0], mfilterAddr[1], mfilterAddr[2],
				mfilterAddr[3], mfilterAddr[4], mfilterAddr[5]);
		}
	}
	return;
}

//
// Close driver function
//
INT RT61_close(
    IN  struct net_device *net_dev)
{
    RTMP_ADAPTER    *pAd = net_dev->priv;
    INT				index;
    // LONG            ioaddr = net_dev->base_addr;

    DBGPRINT(RT_DEBUG_TRACE, "===> RT61_close\n");

#ifdef TC_SOC // add polling mode
	pAd->PollingModeDetectRunning = FALSE;
	del_timer_sync(&pAd->PollingModeDetect);
#endif

#ifdef THREAD_ISR
	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_REMOVE_IN_PROGRESS);
	Rtmp_Kill_Thread_Task(pAd);
#endif /* TASK_LEVL_ISR */

	for(index = 0; index < MAX_WDS_ENTRY; index++)
	{
		if (pAd->WdsTab.WdsEntry[index].dev)
		{
			dev_close(pAd->WdsTab.WdsEntry[index].dev);
#ifdef RTL865X_SOC
			if (pAd->WdsTab.WdsEntry[index].mylinkid && devglue_unregExtDevice(pAd->WdsTab.WdsEntry[index].mylinkid)) {			
				printk("Cannot unregister link ID %d \n", pAd->WdsTab.WdsEntry[index].mylinkid);
			} 
			pAd->WdsTab.WdsEntry[index].mylinkid = 0;
#endif /* RTL865X_SOC */

		}
	}

#ifdef MBSS_SUPPORT
	for(index = FIRST_MBSSID; index < MAX_MBSSID_NUM; index++)
	{
		if (pAd->PortCfg.MBSSID[index].MSSIDDev)
		{
			dev_close(pAd->PortCfg.MBSSID[index].MSSIDDev);
#ifdef RTL865X_SOC
			if (pAd->PortCfg.MBSSID[index].mylinkid && devglue_unregExtDevice(pAd->PortCfg.MBSSID[index].mylinkid)) {			
				printk("Cannot unregister link ID %d \n", pAd->PortCfg.MBSSID[index].mylinkid);
			} 
			pAd->PortCfg.MBSSID[index].mylinkid = 0;
#endif /* RTL865X_SOC */
		}
	}
#endif /* MBSS_SUPPORT */

#ifdef APCLI_SUPPORT
	for (index = 0; index < MAX_APCLI_ENTRY; index++)
	{
		if (pAd->ApCliTab.ApCliEntry[index].dev)
		{
			pAd->ApCliTab.ApCliEntry[index].Valid = FALSE;
			dev_close(pAd->ApCliTab.ApCliEntry[index].dev);
#ifdef RTL865X_SOC
			if (pAd->ApCliTab.ApCliEntry[index].mylinkid && devglue_unregExtDevice(pAd->ApCliTab.ApCliEntry[index].mylinkid)) {			
				printk("Cannot unregister link ID %d \n", pAd->ApCliTab.ApCliEntry[index].mylinkid);
			} 
			pAd->ApCliTab.ApCliEntry[index].mylinkid = 0;
#endif /* RTL865X_SOC */		
		}	
	}
#endif

    // Stop Mlme state machine
    MlmeHalt(pAd);

    // shutdown Access Point function, release all related resources 
    ApShutdown(pAd);

    netif_stop_queue(net_dev);
    netif_carrier_off(net_dev);

#ifdef RTL865X_SOC	
#ifdef RTL865X_FAST_PATH
	rtlairgo_fast_tx_unregister();
	rtl865x_extDev_unregisterUcastTxDev(net_dev);
#endif
	if (devglue_unregExtDevice(pAd->PortCfg.MBSSID[MAIN_MBSSID].mylinkid)) {
		printk("Cannot unregister link ID %d \n", pAd->PortCfg.MBSSID[MAIN_MBSSID].mylinkid);
	}
#endif /* RTL865X_SOC */

    if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_ACTIVE))
    {
        NICDisableInterrupt(pAd);
    }

    // Disable Rx, register value supposed will remain after reset
    NICIssueReset(pAd);

#ifdef APCLI_SUPPORT
	APCLIConvertEngineExit();
#endif

#ifdef WSC_SUPPORT
// For AP Client support WPS Modification
#ifdef APCLI_SUPPORT
	WscStop(pAd, &pAd->ApCliTab.ApCliEntry[0].WscControl, TRUE);
	if (pAd->ApCliTab.ApCliEntry[0].WscControl.pPubKeyMem)
		vfree(pAd->ApCliTab.ApCliEntry[0].WscControl.pPubKeyMem);
	if (pAd->ApCliTab.ApCliEntry[0].WscControl.pSecKeyMem )
		vfree(pAd->ApCliTab.ApCliEntry[0].WscControl.pSecKeyMem);
#endif // APCLI_SUPPORT //
//	WscStop(pAd);
	WscStop(pAd, &pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl, FALSE);
// --
	if (pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.pPubKeyMem)
		vfree(pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.pPubKeyMem);
	if (pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.pSecKeyMem )
		vfree(pAd->PortCfg.MBSSID[MAIN_MBSSID].WscControl.pSecKeyMem);
#ifndef OLD_DH_KEY
	DH_freeall();
#endif //OLD_DH_KEY
#endif // WSC_SUPPORT //

    // Free IRQ
    if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE))
    {
        // Deregister interrupt function
        free_irq(net_dev->irq, net_dev);
        RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE);
    }

    // Free Ring buffers
    RTMPFreeDMAMemory(pAd);

    kfree(pAd->ScanTab.BssEntry);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
    module_put(THIS_MODULE);
#else
    MOD_DEC_USE_COUNT;
#endif

    return 0;
}

//
// Remove driver function
//
static VOID __devexit RT61_remove_one(
    IN  struct pci_dev  *pPci_Dev)
{
    struct net_device   *net_dev = pci_get_drvdata(pPci_Dev);
    RTMP_ADAPTER        *pAd = net_dev->priv;
    UINT				index;

    DBGPRINT(RT_DEBUG_TRACE, "===> RT61_remove_one\n");

#ifdef WSC_SUPPORT
    if (pAd->write_dat_file_pid >= 0)
    {
        int ret;
        pAd->time_to_die = 1;
        up(&(pAd->write_dat_file_semaphore));
        wmb(); // need to check
		ret = kill_proc (pAd->write_dat_file_pid, SIGTERM, 1);
		if (ret)
		{
			printk (KERN_ERR "%s: unable to signal thread\n", pAd->net_dev->name);
			return;
		}
        wait_for_completion (&pAd->write_dat_file_notify);
    }
#endif // WSC_SUPPORT //

#ifdef APCLI_SUPPORT
    for(index = 0; index < MAX_APCLI_ENTRY; index++)
    {
        if (pAd->ApCliTab.ApCliEntry[index].dev)
	    {
		
	        unregister_netdev(pAd->ApCliTab.ApCliEntry[index].dev);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
	        free_netdev(pAd->ApCliTab.ApCliEntry[index].dev);
#else
	        kfree(pAd->ApCliTab.ApCliEntry[index].dev);
#endif
	    }
    }
#endif 

#ifdef WDS_SUPPORT
    for(index = 0; index < MAX_WDS_ENTRY; index++)
    {
        if (pAd->WdsTab.WdsEntry[index].dev)
	    {
	        unregister_netdev(pAd->WdsTab.WdsEntry[index].dev);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
	        free_netdev(pAd->WdsTab.WdsEntry[index].dev);
#else
	        kfree(pAd->WdsTab.WdsEntry[index].dev);
#endif
	    }
    }
#endif 

#ifdef MBSS_SUPPORT
    for(index = FIRST_MBSSID; index < MAX_MBSSID_NUM; index++)
    {
		if (pAd->PortCfg.MBSSID[index].MSSIDDev != NULL)
    {
        unregister_netdev(pAd->PortCfg.MBSSID[index].MSSIDDev);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
        free_netdev(pAd->PortCfg.MBSSID[index].MSSIDDev);
#else
		kfree(pAd->PortCfg.MBSSID[index].MSSIDDev);
#endif
    }
    }
#endif

    // Unregister network device
    unregister_netdev(net_dev);

    // Unmap CSR base address
    iounmap((char *)(net_dev->base_addr));

    //pci_free_consistent(pAd->pPci_Dev, sizeof(RTMP_ADAPTER), pAd, dma_adapter);
    vfree((void *)pAd);

    // release memory region
    release_mem_region(pci_resource_start(pPci_Dev, 0), pci_resource_len(pPci_Dev, 0));

    // Free pre-allocated net_device memory
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
    free_netdev(net_dev);
#else
	kfree(net_dev);
#endif
}

//
// Ralink PCI device table, include all supported chipsets
//
static struct pci_device_id rt61_pci_tbl[] __devinitdata =
{
    {0x1814, 0x0301, PCI_ANY_ID, PCI_ANY_ID, 0, 0, 0},		//RT2561S
   	{0x1814, 0x0302, PCI_ANY_ID, PCI_ANY_ID, 0, 0, 0},		//RT2561 V2
    {0x1814, 0x0401, PCI_ANY_ID, PCI_ANY_ID, 0, 0, 0},		//RT2661
    {0,}		// terminate list
};

MODULE_DEVICE_TABLE(pci, rt61_pci_tbl);

//
// Our PCI driver structure
//
static struct pci_driver rt61_driver =
{
    name:       "rt61",
    id_table:   rt61_pci_tbl,
    probe:      RT61_init_one,
#if LINUX_VERSION_CODE >= 0x20412 || BIG_ENDIAN == TRUE 
    remove:     __devexit_p(RT61_remove_one),
#else
    remove:     __devexit(RT61_remove_one),
#endif
};

// =======================================================================
// LOAD / UNLOAD sections
// =======================================================================
//
// Driver module load function
//
static INT __init rt61_init_module(VOID)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
	return pci_register_driver(&rt61_driver);
#else
    return pci_module_init(&rt61_driver);
#endif
}

//
// Driver module unload function
//
static VOID __exit rt61_cleanup_module(VOID)
{
    pci_unregister_driver(&rt61_driver);
}

module_init(rt61_init_module);
module_exit(rt61_cleanup_module);

