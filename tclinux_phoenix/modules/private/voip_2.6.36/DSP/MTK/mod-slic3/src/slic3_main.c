/**
 *	@author mtk02588
 */
 #ifdef __KERNEL__
#include <linux/version.h>
#include <linux/module.h>
#include <asm/tc3162/tc3162.h>
#include "ddr_slic3.h"
#include "slic_func_tbl.h"
#include "pcm_listener.h"
#include "pcmdriver.h"
#include "slic3_proc.h"



extern SLIC_FUNC_TBL slicFuncTab;
#ifdef TCSUPPORT_SLIC_SEPARATE
extern int fxsNum, fxoNum, pcm1ChNum,pcm2ChNum; 
#endif
extern int devNum;
unsigned int spi_interface_type =NORMAL_PCM_SPI_INTERFACE;  /*default 0 = normal PCM+SPI interface; 1 = ZSI interface (Microsemi) ;2=ISI interface(silicon lab)*/
char TypeTmp[20];
char *type = TypeTmp;
extern PCM_FUNC_TBL pcmFuncTab[PCM_NUM];
extern int SLIC_SPI_Reset(void);
extern void (*pSLIC_Reset)(void);
extern int SLIC_Reset(void);

/* Add by mtk06404_20160318 for gpio set */
#define GPIO_ZSI_ISI  (devNum == 1?GPIO_ZSI_ISI_1st:(GPIO_ZSI_ISI_1st|GPIO_ZSI_ISI_2nd))
#define GPIO_SPI_SLIC (devNum == 1?GPIO_SPI_SLIC_1st:(GPIO_SPI_SLIC_1st|GPIO_SPI_SLIC_2nd))

static int slic3_init(void)
{
	uint i=0;
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "init mod-slic3,interface:%s,devNum:%d,gpio:%x\n", type, devNum, GPIO_ZSI_ISI); /* Add by mtk06404_20160318 for gpio set */
	if (!strcmp(type, "ISI")) {
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "ISI\n");
		spi_interface_type =SILICON_ISI_INTERFACE;
		if(isEN751221){
			regWrite(IOMUX_CONTROL1, ((regRead(IOMUX_CONTROL1)&(~0x7d00))|GPIO_ZSI_ISI)); 
			regWrite(PCM_CLK_SOURCE_SELECT,regRead(PCM_CLK_SOURCE_SELECT)&(~PCM_ZSI_CLK_SOURCE)); /* clk source selecet:ZSI,PCM&SPI set bit[2][3] 1, ISI set bit[2][3] 0 */
		}
		for(i=0;i<PCM_NUM;i++){
                    if(pcmFuncTab[i].timeSlotCfgReinit)
                        pcmFuncTab[i].timeSlotCfgReinit(0);
                }
	}else if (!strcmp(type, "ZSI")) {
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "ZSI\n");
		spi_interface_type = MICROMSEMI_ZSI_INTERFACE;
		if(isEN751221){					
			regWrite(IOMUX_CONTROL1,((regRead(IOMUX_CONTROL1)&(~0x7d00))|GPIO_ZSI_ISI));
			regWrite(PCM_CLK_SOURCE_SELECT,regRead(PCM_CLK_SOURCE_SELECT)|PCM_ZSI_CLK_SOURCE);
		}
		for(i=0;i<PCM_NUM;i++){
			if(pcmFuncTab[i].timeSlotCfgReinit)
				pcmFuncTab[i].timeSlotCfgReinit(2);
		}
	}else if (!strcmp(type, "CSI")) {
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "CSI\n");
		spi_interface_type = LANTIQ_CSI_INTERFACE;
		if(isEN751221){
			regWrite(IOMUX_CONTROL1,((regRead(IOMUX_CONTROL1)&(~0x7d00))|GPIO_ZSI_ISI|GPIO_PCM_RESET));/*CSI need gpio2 as pcm reset*/
			regWrite(PCM_CLK_SOURCE_SELECT,regRead(PCM_CLK_SOURCE_SELECT)|PCM_ZSI_CLK_SOURCE);
		}
		for(i=0;i<PCM_NUM;i++){
			if(pcmFuncTab[i].timeSlotCfgReinit)
				pcmFuncTab[i].timeSlotCfgReinit(2);
		}
	}else{
		int gpio_spi_slic = GPIO_SPI_SLIC;
		SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "spi &pcm \n");
#ifdef CZ_DSP_SUPPORT
		/* ZTE use GPIO2 for USB_LED,  slic reset with GPIO9, so clear bit10 */
		gpio_spi_slic &= (~GPIO_PCM_RESET);
#endif
		if(isEN751221){
			regWrite(IOMUX_CONTROL1,((regRead(IOMUX_CONTROL1)&(~0x7d00))|gpio_spi_slic));
			regWrite(PCM_CLK_SOURCE_SELECT,regRead(PCM_CLK_SOURCE_SELECT)|PCM_ZSI_CLK_SOURCE);
		}
		for(i=0;i<PCM_NUM;i++){
			if(pcmFuncTab[i].timeSlotCfgReinit)
				pcmFuncTab[i].timeSlotCfgReinit(0);
		}
	}
#ifdef TCSUPPORT_SLIC_SEPARATE
	slic3_func_uninit(&slicFuncTab);
#if 0 /* Add by mtk06404_20160318 for gpio set */
	if(isEN751221){
		if(devNum ==1){
			if(spi_interface_type==NORMAL_PCM_SPI_INTERFACE)
				regWrite(IOMUX_CONTROL1,(regRead(IOMUX_CONTROL1))&(~GPIO_SPI_SLIC_2nd)); /*release GPIO3  */
			else
				regWrite(IOMUX_CONTROL1,(regRead(IOMUX_CONTROL1))&(~GPIO_ZSI_ISI_2nd)); /*release GPIO4~7  */
		}
	}
#endif    
	//memset(&slicFuncTab, 0, sizeof(slicFuncTab));
#ifdef OSBN_VOIP
	slicFuncTab.SLIC_SEP_getDeviceInfo = SLIC_SEP_getDeviceInfo;
	slicFuncTab.SLIC_SEP_SpiReset = SLIC_SPI_Reset;
#else	
	slicFuncTab.SLIC_SEP_TotalChannelGet = SLIC_SEP_TotalChannelGet;
#endif

#else
	slic3_func_init(&slicFuncTab);
#endif

	pSLIC_Reset = SLIC_Reset;
	slic_proc_init();
	return 0;
}

static void slic3_dest(void)
{
	SLIC_DBG_MSG(_SLIC_DBG_TYPE_INFO, "clean mod-slic3\n");	
	slic3_func_uninit(&slicFuncTab);
	slic_proc_dest();
	return;
}

module_init(slic3_init);
module_exit(slic3_dest);

MODULE_LICENSE("MTK");
#ifdef TCSUPPORT_SLIC_SEPARATE
module_param(fxsNum,  int, S_IRUGO);
module_param(fxoNum, int, S_IRUGO);
module_param(pcm1ChNum, int, S_IRUGO);
module_param(pcm2ChNum, int, S_IRUGO);
#endif
module_param(type, charp, 0);
module_param(devNum, int, S_IRUGO); /* Add by mtk06404_20160318 modify for normal */


#else //for none OS mode
int main()
{
	return 0;
}
#endif

