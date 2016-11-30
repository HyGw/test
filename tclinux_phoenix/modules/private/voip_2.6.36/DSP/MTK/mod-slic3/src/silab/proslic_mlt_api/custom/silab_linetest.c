/*
** Example MLT Test Implementations
**
** Note:  mdelay(10)() is generic psuedo-code representing the 
**        polling interval to be implemented by the user.
**
*/


/* 
**
** Example Hazardous/Foreign Voltages Test
**
*/
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/timer.h>
#include <linux/proc_fs.h>

#include "_TDI.h"
#include "TDI.h"
#include "proslic.h"
#include "proslic_mlt.h"

static ProSLICMLTType ProSLICMLT;

/* mtk08176_20150824 */
static proslicMonitorType inline_monitor;
static int testId;

//#define SILICON_LT_DEBUG


int Silicon_LineTestStart(char *buf,  proslicChanType_ptr *pProslic)
{
	int teststatus=RC_MLT_TEST_RUNNING;
	_TDI_devicelinetest_t* linetest=(_TDI_devicelinetest_t*)buf;
	int32 temp; 

	testId=linetest->testid;/* mtk08176_20150824 */

	ProSLICMLT.pProslic=pProslic;

	ProSLIC_mlt_init_ren_cal(&ProSLICMLT);
	ProSLIC_SetLinefeedStatus(ProSLICMLT.pProslic, LF_OPEN);

	MLTClearSettings(&ProSLICMLT);
	MLTClearResults(&ProSLICMLT);
	
	switch(linetest->testid)
	{
		case 1:
		case 2:
			teststatus=voltagesTest(&ProSLICMLT);
			break;
		case 3:
			teststatus=resistiveFaultsTest(&ProSLICMLT);
			break;
		case 4:		
			teststatus=receiverOffhookTest(&ProSLICMLT);
			break;
		case 5:
			receiverOffhookTest(&ProSLICMLT);			
			if(ProSLICMLT.roh.rohTrue==RC_MLT_ROH_NOFAULT)
			{
				teststatus=renTest(&ProSLICMLT);
			}
			else if(temp==RC_MLT_ROH_FAIL_ROH)
			{
				printk("Exit REN test because Receiver off-hook[phone is off-hook]\n");
			}
			else
				printk ("Exit REN test because Resistive fault[t-r is short,  Tip or Ring  connect to GND or other high voltage\n");
					
			break;
			case 6:
			teststatus=capFaultsTest(&ProSLICMLT);
			break;
		case 7:
			receiverOffhookTest(&ProSLICMLT);
			if(ProSLICMLT.roh.rohTrue==RC_MLT_ROH_NOFAULT)
			{
				teststatus=renCapTest(&ProSLICMLT);
			}
			else if(temp==RC_MLT_ROH_FAIL_ROH)
			{
				printk("Exit REN test because Receiver off-hook[phone is off-hook]\n");
			}
			else
			{
				printk ("Exit REN test because Resistive fault[t-r is short,  Tip or Ring  connect to GND or other high voltage\n");
			}	
			break;
                /* mtk08176_20150824 */
		case 8:
			ProSLIC_SetLinefeedStatus(pProslic,LF_FWD_ACTIVE);
			mdelay(10);
			ProSLIC_LineMonitor(pProslic,&inline_monitor);

			teststatus=RC_MLT_TEST_COMPLETE;
		
		default:
			break;
	}

	return teststatus;
}

int* Silicon_getLtTestResult(void)
{		
        /* mtk08176_20150824 */
	if(testId==8)
		return (&inline_monitor);
	else
		return (&ProSLICMLT);
}

void MLTClearResults(ProSLICMLTType *pProSLICMLT)
{
	pProSLICMLT->hazVAC.measTG = 0;
	pProSLICMLT->hazVAC.measTR = 0;
	pProSLICMLT->hazVAC.measRG = 0;
	pProSLICMLT->hazVAC.resultsValid = 0;

	pProSLICMLT->hazVDC.measTG = 0;
	pProSLICMLT->hazVDC.measTR = 0;
	pProSLICMLT->hazVDC.measRG = 0;
	pProSLICMLT->hazVDC.resultsValid = 0;
	
	pProSLICMLT->resFaults.measTG = 10000000;
	pProSLICMLT->resFaults.measTR = 10000000;
	pProSLICMLT->resFaults.measRG = 10000000;
	pProSLICMLT->resFaults.resultsValid = 0;

	pProSLICMLT->roh.rohTrue = RC_MLT_ROH_NOFAULT;
    pProSLICMLT->roh.resultsValid = 0;

	pProSLICMLT->ren.renValue = 0;
    pProSLICMLT->ren.resultsValid = 0;
}

/*
** Clears REN cal flag
*/
void MLTClearSettings(ProSLICMLTType *pProSLICMLT)
{
	pProSLICMLT->ren.renCalFlag = 0;
}



int voltagesTest(ProSLICMLTType *pProSLICMLT)
{
ProSLIC_mlt_foreign_voltages_state vState;
int samples = 30;
int done = RC_MLT_TEST_RUNNING;

	/* Call initialization function */
	ProSLIC_mlt_init_foreign_voltages(&vState,samples);

	do
	{
		/* Call MLT Function */
		done = ProSLIC_mlt_foreign_voltages(pProSLICMLT,&vState);

		/* Perform other host tasks/wait for poll period */
		//mdelay(10);
		msleep(10);/* mtk08176_20150824 */
	} while(done == RC_MLT_TEST_RUNNING);

	/* Process Return Code */
	if(done != RC_MLT_TEST_COMPLETE)
	{
		return RC_MLT_ERROR;
		/*
		** Possible Return Codes:
		**      RC_MLT_TEST_RUNNING         - Test in progress
		**		RC_MLT_TEST_COMPLETE		- Test complete with no Errors
		*/
	}
	else
	{
		/* Process Available Results */
		/*
		** Available Results:
		**
		** pProSLICMLT->hazVDC.measTR		TR DC Voltage (mv)
		** pProSLICMLT->hazVDC.measTG		TG DC Voltage (mv)
		** pProSLICMLT->hazVDC.measRG		RG DC Voltage (mv)
		** pProSLICMLT->hazVAC.measTR		TR AC Voltage (Vrms^2/100)
		** pProSLICMLT->hazVAC.measTG		TG AC Voltage (Vrms^2/100)
		** pProSLICMLT->hazVAC.measRG		RG AC Voltage (Vrms^2/100)
		**
		*/
		  #ifdef SILICON_LT_DEBUG
		  printk("	  Hazardous/Foreign Voltages Test\n");
		  printk("	pProSLICMLT->hazVDC.measTR 		  =  %d\n" ,   pProSLICMLT->hazVDC.measTR );
		  printk("	pProSLICMLT->hazVDC.measTG			  =  %d\n" ,   pProSLICMLT->hazVDC.measTG );
		  printk("	 pProSLICMLT->hazVDC.measRG			  =  %d\n" ,   pProSLICMLT->hazVDC.measRG );
		  printk("	pProSLICMLT->hazVAC.measTR	  =  %d\n" ,   pProSLICMLT->hazVAC.measTR );
		  printk("	pProSLICMLT->hazVAC.measTG	  =  %d\n" ,   pProSLICMLT->hazVAC.measTG );
		  printk("	pProSLICMLT->hazVAC.measRG	  =  %d\n" ,   pProSLICMLT->hazVAC.measRG );
		  #endif
		return RC_MLT_TEST_COMPLETE;
	}
}


/* 
**
** Example Resistive Faults Test
**
*/
int resistiveFaultsTest(ProSLICMLTType *pProSLICMLT)
{
ProSLIC_mlt_rmeas_state rState;
int done = RC_MLT_TEST_RUNNING;

	/* Call initialization function */
	ProSLIC_mlt_init_resistive_faults(&rState);



	if(pProSLICMLT==NULL)
	{
		return RC_MLT_ERROR;
	}

	
	do
	{
		/* Call MLT Function */
		done = ProSLIC_mlt_resistive_faults(pProSLICMLT,&rState);

		/* Perform other host tasks/wait for poll period */
		//mdelay(10);
		msleep(10);/* mtk08176_20150824 */

	} while(done == RC_MLT_TEST_RUNNING);

	/* Process Return Code */
	if(done != RC_MLT_TEST_COMPLETE)
	{
		return RC_MLT_ERROR;
		/*
		** Possible Return Codes:
		**      RC_MLT_TEST_RUNNING         - Test in progress
		**		RC_MLT_TEST_COMPLETE		- Test complete with no Errors
		*/
	}
	else
	{
		/* Process Available Results */
		/*
		** Available Results:
		**
		** pProSLICMLT->resFaults.measTR		RTR (ohms/10)
		** pProSLICMLT->resFaults.measTG		RTG (ohms/10)
		** pProSLICMLT->resFaults.measRG		RRG (ohms/10)
		**
		*/
		 #ifdef SILICON_LT_DEBUG
		  printk("Resistive Fault Test\n");
		  printk("	 pProSLICMLT->resFaults.measTR			  =  %d\n" ,   pProSLICMLT->resFaults.measTR );
		  printk("	 pProSLICMLT->resFaults.measTG			  =  %d\n" ,   pProSLICMLT->resFaults.measTG );
		  printk("	 pProSLICMLT->resFaults.measRG			  =  %d\n" ,   pProSLICMLT->resFaults.measRG );
		  #endif
		return RC_MLT_TEST_COMPLETE;
	}
}

/* 
**
** Example Receiver Offhook Test
**
*/
int receiverOffhookTest(ProSLICMLTType *pProSLICMLT)
{
ProSLIC_mlt_roh_state rState;
int done = RC_MLT_TEST_RUNNING;

	/* Call initialization function */
	ProSLIC_mlt_init_receiver_offhook(&rState);

	do
	{
		/* Call MLT Function */
		done = ProSLIC_mlt_receiver_offhook(pProSLICMLT,&rState);

		/* Perform other host tasks/wait for poll period */
		//mdelay(10);
		msleep(10);/* mtk08176_20150824 */

	} while(done == RC_MLT_TEST_RUNNING);

	/* Process Return Code */
	if(done != RC_MLT_TEST_COMPLETE)
	{

		return RC_MLT_ERROR;
		/*
		** Possible Return Codes:
		**      RC_MLT_TEST_RUNNING         - Test in progress
		**		RC_MLT_TEST_COMPLETE		- Test complete with no Errors
		*/
	}
	else
	{
		/* Process Available Results */
		/*
		** Available Results:
		**
		** pProSLICMLT->roh.rohTrue		
		**			RC_NONE						No fault or receiver detected
		**			RC_MLT_ROH_FAIL_ROH			Offhook receiver detected
		**			RC_MLT_ROH_FAIL_RESFAULT	Resistive fault detected
		*/
	     #ifdef SILICON_LT_DEBUG 
		printk("receiverOffhookTest \r\n");
		printk("pProSLICMLT->roh.rohTrue=%d\r\n",pProSLICMLT->roh.rohTrue);
		#endif
		return RC_MLT_TEST_COMPLETE;
	}
}


/* 
**
** Example REN Test
**
*/
int renTest(ProSLICMLTType *pProSLICMLT)
{
ProSLIC_mlt_ren_state rState;
int done = RC_MLT_TEST_RUNNING;

	/* Call initialization function */
	ProSLIC_mlt_init_ren(&rState);

	do
	{
		/* Call MLT Function */
		done = ProSLIC_mlt_ren(pProSLICMLT,&rState);

		/* Perform other host tasks/wait for poll period */
		//mdelay(10);
		msleep(10);/* mtk08176_20150824 */

	} while(done == RC_MLT_TEST_RUNNING);

	/* Process Return Code */
	if(done != RC_MLT_TEST_COMPLETE)
	{
		return RC_MLT_ERROR;
		/*
		** Possible Return Codes:
		**      RC_MLT_TEST_RUNNING         - Test in progress
		**		RC_MLT_TEST_COMPLETE		- Test complete with no Errors
		**		RC_MLT_FALSE_RINGTRIP		- Ringtrip occurred during test
		**		RC_MLT_ALARM_ON_RING		- Power/thermal alarm during test
		**		RC_MLT_RINGSTART_ERR		- Ringing failed to start
		*/
	}
	else
	{
		/* Process Available Results */
		/*
		** Available Results:
		**
		** pProSLICMLT->ren.renValue		REN (unitless/10)	
		**
		*/
		 #ifdef SILICON_LT_DEBUG
		printk("	renTest\n");
		printk("	 pProSLICMLT->ren.renValue		  =  %d\n" ,	   pProSLICMLT->ren.renValue );
		#endif
		return RC_MLT_TEST_COMPLETE;
	}
}


/* 
**
** Example Capacitive Faults Test
**
*/
int capFaultsTest(ProSLICMLTType *pProSLICMLT)
{
ProSLIC_mlt_capacitance_state cState;
int done = RC_MLT_TEST_RUNNING;

	/* Call initialization function */
	ProSLIC_mlt_init_capacitance(&cState);

	do
	{
		/* Call MLT Function */
		done = ProSLIC_mlt_capacitance(pProSLICMLT,&cState);

		/* Perform other host tasks/wait for poll period */
		//mdelay(10);
		msleep(10);/* mtk08176_20150824 */

	} while(done == RC_MLT_TEST_RUNNING);

	/* Process Return Code */
	if(done != RC_MLT_TEST_COMPLETE)
	{
		return RC_MLT_ERROR;
		/*
		** Possible Return Codes:
		**      RC_MLT_TEST_RUNNING         - Test in progress
		**		RC_MLT_TEST_COMPLETE		- Test complete with no Errors
		*/
	}
	else
	{
		/* Process Available Results */
		/*
		** Available Results:
		**
		** pProSLICMLT->capFaults.measTR		CTR (nF/10)
		** pProSLICMLT->capFaults.measTG		CTG (nF/10)
		** pProSLICMLT->capFaults.measRG		CRG (nF/10)
		**
		*/
		  #ifdef SILICON_LT_DEBUG
		  printk("    capFaultsTest\n");
		  printk("	  pProSLICMLT->capFaults.measTR          =  %d\n" ,		 pProSLICMLT->capFaults.measTR );
		  printk("	   pProSLICMLT->capFaults.measTG           =  %d\n" ,		 pProSLICMLT->capFaults.measTG	);
		  printk("	 pProSLICMLT->capFaults.measRG         =  %d\n" ,pProSLICMLT->capFaults.measRG	);
		#endif
		return RC_MLT_TEST_COMPLETE;
	}
}


/* 
**
** Example REN capacitance Test
**
*/
int renCapTest(ProSLICMLTType *pProSLICMLT)
{
ProSLIC_mlt_capacitance_state cState;
int done = RC_MLT_TEST_RUNNING;

	/* Call initialization function */
	ProSLIC_mlt_init_capacitance(&cState);

	do
	{
		/* Call MLT Function */
		done = ProSLIC_mlt_ren_cap(pProSLICMLT,&cState);

		/* Perform other host tasks/wait for poll period */
		//mdelay(10);
		msleep(10);/* mtk08176_20150824 */

	} while(done == RC_MLT_TEST_RUNNING);

	/* Process Return Code */
	if(done != RC_MLT_TEST_COMPLETE)
	{

		return RC_MLT_ERROR;
		/*
		** Possible Return Codes:
		**      RC_MLT_TEST_RUNNING         - Test in progress
		**		RC_MLT_TEST_COMPLETE		- Test complete with no Errors
		*/
	}
	else
	{
		/* Process Available Results */
		/*
		** Available Results:
		**
		** pProSLICMLT->capFaults.measTR		CTR (nF)
		**
		*/
		  #ifdef SILICON_LT_DEBUG
		 printk("    renCapTest\n");
		 printk ("pProSLICMLT->capFaults.measTR2=%d\r\n",pProSLICMLT->capFaults.measTR2);    	
		#endif
		return RC_MLT_TEST_COMPLETE;
	}
}
