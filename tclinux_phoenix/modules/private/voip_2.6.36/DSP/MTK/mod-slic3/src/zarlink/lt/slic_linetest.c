/*
 * slic_linetest.c
 *
 *  Created on: Jul 05, 2013
 *      Author: Roger Zhang
 */

#include <vp_api.h>
#include <vp_api_cfg.h>
#include "_TDI.h"
#include "TDI.h"
#include <vp_api_cfg.h>
#include "lt_api.h" 

static int gTestLineNum = 0;
static VpLineCtxType  lineCtx_ptr;
static LtTestIdType testId;
static VpLineCtxType  *gpLineCtxTesting = NULL;
static LtTestCtxType gLtCtx;
static LtTestTempType gLtTemp;
static LtTestResultType gLtResults;
static LtTestAttributesType gLtAtts;


int LtTestStart(VpLineCtxType *lineCtx_ptr,VpDevCtxType *devCtx_ptr,char* buf) {

	_TDI_devicelinetest_t* linetest=(_TDI_devicelinetest_t*)buf;
    LtTestStatusType status = LT_STATUS_TEST_NOT_SUPPORTED;
    uint16 handle;
	bool		  legerityEventFlag = FALSE;
	bool		  legerityEventStatus = FALSE;
   	VpEventType   legerityEventObj;
	int lt_Proc_Cnt=0;

	int Ltstatus=LT_STATUS_RUNNING;	

 	testId=linetest->testid;
	LtRingerInputType RingerInput = {0};
	gLtAtts.inputs.pRingersInp = VP_NULL;

			

	/* set up line test paramters */
	 switch (linetest->testid)
	{		
		case LT_TID_RINGERS:
		{
	
			//LtRingerInputType RingerInput;
			RingerInput.ringerTestType = LT_RINGER_ELECTRONIC_PHNE_TEST;
			RingerInput.vRingerTest = 56000;   
			RingerInput.freq = 20000;
			RingerInput.renFactor = 1000000; 
			gLtAtts.inputs.pRingersInp= &RingerInput;
		}	  
		break;

		case LT_TID_CAP:
			gLtAtts.inputs.pCapInp = VP_NULL;
			break;
			
		default:
			break;
		
	}

    /* if a test is already in progress then get out */
    if (gpLineCtxTesting != NULL) {
        printf("Testing already in progress, cannot start another test\n");
        return (-1);
    }
    
    /* set up test paremters for line one, running receiver off hook test. */
    handle = 0xABCD;
    //gLtAtts.inputs.pLineVInp = VP_NULL;
    gLtAtts.criteria.pLintVCrt = VP_NULL;
    gLtAtts.topology.dummyPtr = VP_NULL;
    gLtAtts.calFactors.dummyPtr = VP_NULL;
	// gLtAtts.ltDbgFlag = LT_DBG_ALL;

    /* start the line test */
  //  printf("LtTestStart Starting test Id= %i\n", linetest->testid);
    status = LtStartTest(lineCtx_ptr, testId, handle, &gLtAtts, &gLtTemp, &gLtResults, &gLtCtx);

    if (LT_STATUS_RUNNING == status) {
        gpLineCtxTesting = lineCtx_ptr;		
       // printf("LtTestStart LtStartTest() running...\n");
		}  else {
        /* set the global flag to indicate test is running on the current Line Ctx */
        printf("Error: LtTestStart LtStartTest(status:%i) failed to start\n", status);
        return (-1);
    }

    /* waitting for line test finish */
	 while(gpLineCtxTesting != NULL) {
		lt_Proc_Cnt++;
		if(lt_Proc_Cnt>1000)
		{
			gpLineCtxTesting=NULL;
			break;
		}
		mdelay(10);
		Ltstatus=Lt_eventProcess(lineCtx_ptr,devCtx_ptr,buf);		
	}		
   return Ltstatus;
}


int* getLtTestResult(void)
{
		int* testresult=NULL;

		switch (testId)
		{					
			case LT_TID_LINE_V:
			{	
				  /* Test results for the test LT_TID_LINE_V */
					LtLineVResultType *pResultTemp ;
					pResultTemp=&gLtResults.result.lineV;
					testresult=(int*) pResultTemp;
					
					#ifdef LT_DEBUG
				  	printf("   Line Test Result -- LT_TID_LINE_V\n");
					printf("	fltMask 	 =	%d\n" , 				  pResultTemp->fltMask	   )  ;  
					printf("	measStatus	 =	%d\n" , 				  pResultTemp->measStatus  )  ;
					printf("	vAcTip		 =	%ldmVrms\n" , 					  pResultTemp->vAcTip	   )  ;
					printf("	vAcRing 	 =	%ldmVrms\n" , 			  pResultTemp->vAcRing	  )  ;	
					printf("	vAcDiff 	 =	%ldmVrms\n" , 				pResultTemp->vAcDiff	)  ;  
					printf("	vDcTip		 =	%ldmV\n" , 				pResultTemp->vDcTip    )  ;   
					printf("	vDcRing 	 =	%ldmV\n" , 					pResultTemp->vDcRing   )  ;  
					printf("	vDcDiff 	 =	%ldmV\n\n" , 				pResultTemp->vDcDiff	)  ; 
					 //VpSetLineState(pLCtx, VP_LINE_STANDBY);
					#endif
			}	
			break;
	
			case LT_TID_ROH:
			{
				  /* Test results for the test LT_TID_ROH */
				  
				  LtRohResultType *pResultTemp ;														 
				  pResultTemp=&gLtResults.result.roh;
				  testresult=(int*) pResultTemp;
				  
				  #ifdef LT_DEBUG
				  printf("   Line Test Result -- LT_TID_ROH\n");
				  printf("	  fltMask	   =  %d\n" ,					pResultTemp->fltMask	)  ; 
				  printf("	  rLoop1	   =  %ld\n" ,					pResultTemp->rLoop1 	)  ; 
				  printf("	  rLoop2	   =  %ld\n" ,						pResultTemp->rLoop2 	)  ; 
				  printf("	  measStatus   =  %d\n\n" ,				pResultTemp->measStatus )  ;  
				  #endif
			}	 
			break;		

			case LT_TID_RES_FLT:
			{
				  /* Test results for the test LT_TID_RES_FLT */
				  LtResFltResultType *pResultTemp ;
				  pResultTemp=&gLtResults.result.resFlt;
				  testresult=(int*) pResultTemp;
				  #ifdef LT_DEBUG
				  printf("   Line Test Result -- LT_TID_RES_FLT\n ");
				  printf("   fltMask	=  %d\n" , pResultTemp->fltMask	  )  ;	
				  printf("   rtg		=  %ld\n" , pResultTemp->rrg		  )  ;
				  printf("   rrg		=  %ld\n" , pResultTemp->rrg		  )  ;
				  printf("   rtr		=  %ld\n" , pResultTemp->rtr		  )  ;	
				  printf("   rGnd		=  %ld\n" , pResultTemp->rGnd		)  ;  
				  printf("   measStatus	=  %d\n\n" , pResultTemp->measStatus )  ;  
				  #endif
			}	
			//VpSetLineState(, VP_LINE_STANDBY);
			break;	
			
			case LT_TID_RINGERS:
			{
					/* Test results for the test LT_TID_RINGERS */
					LtRingersResultType *pResultTemp ;
					pResultTemp=&gLtResults.result.ringers;
					testresult=(int*) pResultTemp;
					
					#ifdef LT_DEBUG
				   	printf("  Line Test Result -- LT_TID_RINGERS\n ");
					printf("	fltMask			=  %d\n" , 				  pResultTemp->fltMask		   )  ;  
					printf("	measStatus		=  %d\n" , 				  pResultTemp->measStatus	   )  ;
					printf("	ren			=  %ld\n" , 					  pResultTemp->ren			   )  ;
					printf("	rentg			=  %ld\n" , 			  pResultTemp->rentg		  )  ;	
					printf("	renrg			=  %ld\n" , 				pResultTemp->renrg			)  ;  
					printf("	ringerTestType		=  %d\n\n" , 				pResultTemp->ringerTestType)  ;   
					#endif
			}	  
			break;
			
			case LT_TID_DC_FEED_ST:
			{
				  /* Test results for the test LT_TID_DC_FEED_ST */
				  LtDcFeedSTResultType *pResultTemp ;
				  pResultTemp=&gLtResults.result.dcFeedST;
				  testresult=(int*) pResultTemp;
				  
				  #ifdef LT_DEBUG
				  printf("  Line Test Result -- LT_TID_DC_FEED_ST\n");
				  printf("	  fltMask	   =  %d\n" ,		 pResultTemp->fltMask		 );
				  printf("	  measStatus   =  %d\n" ,		 pResultTemp->measStatus	 );
				  printf("	  rTestLoad    =  %ld\n" ,		 pResultTemp->rTestLoad 	 );
				  printf("	  vTestLoad    =  %ld\n" ,		 pResultTemp->vTestLoad 	 );
				  printf("	  iTestLoad    =  %ld\n\n" ,		 pResultTemp->iTestLoad 	 ); 
				  #endif
			}
			break;
			
			case LT_TID_DC_VOLTAGE:
			{
				  /* Test results for the test LT_TID_DC_VOLTAGE */
				  LtDcVoltageResultType *pResultTemp ;
				  pResultTemp=&gLtResults.result.dcVoltage;
				  testresult=(int*) pResultTemp;
				  
				  #ifdef LT_DEBUG
				  printf("   Line Test Result -- LT_TID_DC_VOLTAGE\n");
				  printf("	  fltMask			=  %d\n" ,		  pResultTemp->fltMask			   );
				  printf("	  measStatus		=  %d\n" ,		  pResultTemp->measStatus		   );
				  printf("	  measuredVoltage1	=  %ld\n" ,		  pResultTemp->measuredVoltage1    );
				  printf("	  measuredVoltage2	=  %ld\n\n" ,		  pResultTemp->measuredVoltage2    );
				  #endif
			}
			break;
			
			case LT_TID_RINGING_ST:
			{
				  /* Test results for the test LT_TID_RINGING_ST */
				  LtRingingSTResultType *pResultTemp ;
				  pResultTemp=&gLtResults.result.ringingST;
				  testresult=(int*) pResultTemp;
				  
				  #ifdef LT_DEBUG
				  printf("   Line Test Result -- LT_TID_RINGING_ST\n");
				  printf("	  fltMask		=  %d\n" ,		  pResultTemp->fltMask			   );
				  printf("	  openCktRingVol	=  %ld\n" ,		  pResultTemp->openCktRingVol	   );
				  printf("	  freq			=  %ld\n" ,		  pResultTemp->freq 			   );
				  printf("	  acRload		=  %ld\n" ,		  pResultTemp->acRload			   );
				  printf("	  iRload		=  %ld\n" ,		  pResultTemp->iRload			   );
				  printf("	  measStatus		=  %d\n\n" ,		  pResultTemp->measStatus		   );
				  #endif
//				  ReadReg(0x01,VP880_SIGA_PARAMS_RD, VP880_SIGA_PARAMS_LEN);
//				  VpInitLine(pLCtx, AC_600R_FXS, DC_25MA_CC, RING_20HZ_SINE);
//				  VpInitRing(pLCtx, RING_CAD_STD, VP_PTABLE_NULL);
				//  VpSetLineState(pLCtx, VP_LINE_STANDBY);

			}	  
			break;
			
			case LT_TID_ON_OFF_HOOK_ST:
			{
				  /* Test results for the test LT_TID_ON_OFF_HOOK_ST */
				  LtOnOffHookSTResultType *pResultTemp ;
				  pResultTemp=&gLtResults.result.onOffHookST;
				  testresult=(int*) pResultTemp;
				  
				   #ifdef LT_DEBUG
				  printf("   Line Test Result -- LT_TID_ON_OFF_HOOK_ST\n");
				  printf("	  fltMask	   =  %d\n" ,		 pResultTemp->fltMask		 );
				  printf("	  measStatus   =  %d\n\n" ,		 pResultTemp->measStatus	 ); 
				  #endif
			}	  
			break;	  
			
			case LT_TID_RD_BAT_COND:
			{	
				  /* Test results for the test LT_TID_RD_BAT_COND */
				  LtReadBatCondResultType *pResultTemp ;
				  pResultTemp=&gLtResults.result.readBatCond;
				  testresult=(int*) pResultTemp;
				  
				  #ifdef LT_DEBUG
				  printf("   Line Test Result -- LT_TID_RD_BAT_COND\n");
				  printf("	  fltMask          =  %d\n" ,		 pResultTemp->fltMask		);
				  printf("	  measStatus    =  %d\n" ,		 pResultTemp->measStatus	);
				  printf("	  bat1              =  %ldmV\n" ,		 pResultTemp->bat1			);
				  printf("	  bat2              =  %ldmv\n" ,		 pResultTemp->bat2			);
				  printf("	  bat3              =  %ldmV\n\n" ,		 pResultTemp->bat3			);
				  #endif
			}
				  
			break;	  
			case LT_TID_PRE_LINE_V:
			{	
				  /* Test results for the test LT_TID_PRE_LINE_V */
				  LtPreLineVResultType *pResultTemp ;
				  pResultTemp=&gLtResults.result.preLineV;
				  testresult=(int*) pResultTemp;
				  
				   #ifdef LT_DEBUG
				  printf("   Line Test Result -- LT_TID_PRE_LINE_V\n");
				  printf("	  fltMask	   =  %d\n" ,		 pResultTemp->fltMask		);
				  printf("	  measStatus   =  %d\n\n" ,		 pResultTemp->measStatus	);
				  #endif
			}	  
			break;	

			case LT_TID_CAP:
			{	
				  /* Test results for the test LT_TID_PRE_LINE_V */
				  LtCapResultType *pResultTemp ;
				  pResultTemp=&gLtResults.result.cap;
				  testresult=(int*) pResultTemp;
				  
				  #ifdef LT_DEBUG
				  printf("   fltMask    = %d\n" ,pResultTemp->fltMask	  )  ;	
				  printf("   ctg        = %ld\n" , pResultTemp->ctg		  )  ;
				  printf("   crg        = %ld\n" , pResultTemp->crg		  )  ;
				  printf("   ctr        = %ld\n" , pResultTemp->ctr		  )  ;	
				  #endif
			}	  
			break;	
			case LT_TID_RD_LOOP_COND:
			{	
				  /* Test results for the test LT_TID_RD_BAT_COND */
				  LtRdLoopCondResultType *pResultTemp ;
				  pResultTemp=&gLtResults.result.rdLoopCond;
				  testresult=(int*) pResultTemp;
				  
				  #ifdef LT_DEBUG
				  printf("   Line Test Result -- LT_TID_RD_LOOP_COND\n");
				  printf("	  fltMask         =  %d\n" ,		 pResultTemp->fltMask		);
				  printf("	  measStatus      =  %d\n" ,		 pResultTemp->measStatus	);
				  printf("	  rloop             =  %ld/10ohm\n" ,       pResultTemp->rloop );
				  printf("	  imt             =  %ldmA\n" ,		 pResultTemp->imt			);
				  printf("	  ilg             =  %ldmA\n" ,		 pResultTemp->ilg			);
				  printf("	  vab             =  %ldmV\n" ,		 pResultTemp->vab			);
				  printf("	  vag             =  %ldmV\n" ,		 pResultTemp->vag			);
				  printf("	  vbg             =  %ldmV\n" ,		 pResultTemp->vbg			);
				  printf("	  vbat1           =  %ldmV\n" ,		 pResultTemp->vbat1			);
				  printf("	  vbat2           =  %ldmv\n" ,		 pResultTemp->vbat2			);
				  printf("	  vbat3           =  %ldmV\n\n" ,		 pResultTemp->vbat3			);
				  #endif
			}
			break;
			
			default:
				{
					printf("NO FUNDING testItem\n");
					break;
				}
		}
		
		return testresult;
}

int Lt_eventProcess(VpLineCtxType *lineCtx_ptr,VpDevCtxType *devCtx_ptr,char* buf){
    VpDevCtxType *legerityDev_ptr;
    VpEventType   legerityEventObj;
    bool          legerityEventFlag = FALSE;
    bool          legerityEventStatus = FALSE;
	int ltstatus;
    /*
     * Call the VP API tick
     * and then get status.
     */
    legerityDev_ptr = devCtx_ptr;
   // 
    /*
     * If the legerity EventFlag is active, then parse the legerityEventObj
     * for the events of interest to this TIC driver.
     * For now, do some error checking, to help debug problems.
     */

	VpDeviceInfoType devInfo;

    /* Determine pLineCtx's deviceId */
    devInfo.pLineCtx = lineCtx_ptr;
    devInfo.pDevCtx = NULL;
    VpGetDeviceInfo(&devInfo);

	switch(devInfo.deviceType){
		case VP_DEV_880_SERIES: 
        case VP_DEV_890_SERIES: {
			VpApiTick(legerityDev_ptr, &legerityEventFlag);
			if (TRUE == legerityEventFlag) {
			
	    	legerityEventStatus = VpGetEvent(legerityDev_ptr, &legerityEventObj);

	    	if ((TRUE == legerityEventStatus) &&(VP_STATUS_SUCCESS == legerityEventObj.status))
			{			
				ltstatus=LtTestHandler(&legerityEventObj);
			}
			}
            break;
        }

        case VP_DEV_886_SERIES: 
        case VP_DEV_887_SERIES:{ 
			legerityEventStatus = VpGetEvent(legerityDev_ptr, &legerityEventObj);
			if (TRUE == legerityEventStatus)
			{
			 	ltstatus=LtTestHandler(&legerityEventObj);
			}
			break;
        }

		 default: 
            break;
	}

	return ltstatus;
}


/**
 * Function:  LtTestHandler()
 *
 * Description:  Function is called by ProcessFXSEvent() and keeps the current
 *      test running.
 *
 */
int LtTestHandler(
    char* buf) 
{
	VpEventType *pEvent=(VpEventType*) buf;
	
    LtTestStatusType ltStatus = LT_STATUS_RUNNING;
    LtEventType ltEvent;
    VpTestResultType eventResult;
	char* pResultTemp=VP_NULL;

 
    /* Get results if there are any */
    if(pEvent->hasResults == TRUE) {
        if (VP_STATUS_SUCCESS != VpGetResults(pEvent, &eventResult)) {
            printf("Error: LtTestHandler VpGetResults() :status");
            return (-1);;
        }else {
        	//printf("->LtTestHandler VpGetResults() \n");
        	}
    } 
    /* add pointer to event data */
    ltEvent.pVpEvent = pEvent;
    ltEvent.pResult = (void*)&eventResult;

    /* pass the event (any event) to the LT-API event handler */
    ltStatus = LtEventHandler(&gLtCtx, &ltEvent);

    if (LT_STATUS_DONE == ltStatus) {
        gpLineCtxTesting = NULL;

	//printf("LtTestHandler: Test Complete need to sort the data in gLtResults.\n");	

				
    } else if (LT_STATUS_RUNNING == ltStatus) {

        /* There is really nothing to do here */
       // printf("LtTestHandler: Still Running.\n");
        
    } else {

        /* There has been some sort of error in the test code */
        gpLineCtxTesting = NULL;
        printf("Error: LtEventHandler() something has gone wrong!! Return=%d\n", ltStatus);
    }
    
    return ltStatus;
}

