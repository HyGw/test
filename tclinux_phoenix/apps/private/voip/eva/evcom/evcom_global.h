/*********************************************************************
* LEGAL DISCLAIMER
*
* (Header of MediaTek Software/Firmware Release or Documentation)
*
* BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY 
* ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
* DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED FROM MEDIATEK AND/OR 
* ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS ONLY. 
* MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR 
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF 
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR 
* NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY 
* WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH 
* MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK 
* SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY
* WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO NOT BE 
* RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S 
* SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
* BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND 
* CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED 
* HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE 
* MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR 
* SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE
* AT ISSUE.
*
* THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN 
* ACCORDANCE WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING
* ITS CONFLICT OF LAWS PRINCIPLES.
**********************************************************************
*/

/*
 *	File:	evcom_global.h
 *	 
 *	Author: PT Chen, Quark Li
 * 		
 *	Last Revision Date: 2011.9.29
 *	 
 *	Description:
 *	EVA Console Manager CLI Commands definition.
 *
 */

#ifndef _EVCOM_GLOBAL_H_
#define _EVCOM_GLOBAL_H_
#include "evcom.h"

command_t gCmdSet[NUM_OF_COMMAND] = {
    { .code = CMD_DSP_INVOKE,
      .name = "DspInvoke",
      .opcode = "di",
      .desc = "Initialize and start the DSP process.",
      .usage = "<dspInvoke | di>",
      .example = "EVCom >> dspInvoke",
      .func = cmdDspInvoke},   
      
    { .code = CMD_DSP_REVOKE,
      .name = "DspRevoke",
      .opcode = "dr",
      .desc = "Terminate DSP process.",
      .usage = "<dsprevoke | dr>",
      .example = "EVCom >> dsprevoke",
      .func = cmdDspRevoke},   
      
    { .code = CMD_DSP_QUERY,
      .name = "DspQuery",
      .opcode = "dq",
      .desc = "Query DSP configuration.",
      .usage = "<dspquery | dq>",
      .example = "EVCom >> dspquery",
      .func = cmdDspQuery},   
                  
    { .code = CMD_DSP_CONFIG_TONE,
      .name = "DspConfigTone",
      .opcode = "dct",
      .desc = "Change DSP tone configuration.",
      #if defined(FIVE_CADENCE_SUPPORT) || defined(CJ_DSP_SUPPORT) 
      .usage = "<dspconfigtone |dct> tone=[tone_id] $attr0=[value] $attr1=[value] ...\n" \
      			"attr: type=<reg | mod>, Note: Regular or Modulated tone. \n" \
      			"attr: For type=reg, <cad0_f0 | cad0_f1 | cad0_f2 | cad0_f3>=[freqency,power] \n" \
      			                    "<cad1_f0 | cad1_f1 | cad1_f2 | cad1_f3>=[freqency,power] \n" \
      			                    "<cad2_f0 | cad2_f1 | cad2_f2 | cad2_f3>=[freqency,power] \n" \
      			                    "<cad3_f0 | cad3_f1 | cad3_f2 | cad3_f3>=[freqency,power] \n" \
      			"attr: For type=mod, cad0_mf=[base_freq,mod_freq,mod_db,mod_depth] \n" \
      			                    "cad1_mf=[base_freq,mod_freq,mod_db,mod_depth] \n" \
      			                    "cad2_mf=[base_freq,mod_freq,mod_db,mod_depth] \n" \
      			                    "cad3_mf=[base_freq,mod_freq,mod_db,mod_depth] \n" \
      			                     
      			"Note: Frequency=0~4000(Hz), Power=-40~0(db) \n" \
      			"attr: <cad0 | cad1 | cad2>=[ontime(ms),offtime(ms),cad_repeat]\n",
      .example = "EVCom >> dspconfigtone tone=0 type=reg cad0_f0=350,-20 cad0_f1=440,-16 cad1_f0=350,-20 cad1_f1=440,-16 cad0=1000,0,10",
      #else 
      .usage = "<dspconfigtone |dct> tone=[tone_id] $attr0=[value] $attr1=[value] ...\n" \
      			"attr: type=<reg | mod>, Note: Regular or Modulated tone. \n" \
      			"attr: For type=reg, <f0 | f1 | f2 | f3>=[freqency,power] \n" \
      			"attr: For type=mod, mf=[base_freq,mod_freq,mod_db,mod_depth] \n"\
      			"Note: Frequency=0~4000(Hz), Power=-40~0(db) \n" \
      			"attr: <cad0 | cad1 | cad2>=[ontime(ms),offtime(ms),cad_repeat]\n",
      .example = "EVCom >> dspconfigtone tone=0 type=reg f0=350,-20 f1=440,-16 cad0=1000,0,10",
      #endif
      .func = cmdDspConfigTone},   

	{ .code = CMD_DSP_CONFIG_CPT,
      .name = "DspConfigCpt",
      .opcode = "dcc",
      .desc = "Change DSP call progress tone detection configuration.",
      .usage = "<dspconfigcpt | dcc> cpt=[tone_type] $attr0=[value] $attr1=[value] ...\n" \
      			"attr: cpt = <dial | ringback | busy | reorder | c1 | c2 | c3 | c4>\n" \
      			"attr: <f0 | f1 >=[freqency,deviation] Note: Frequency=0~4000(Hz)" \
      			" Deviation=Tolerated frequency deviation from base frequency\n" \
      			"attr: <cad0 | cad1 | cad2>=[minMake(ms),maxMake(ms),minBreak(ms),maxBreak(ms)]\n" \
      			"attr: pwr = [power] Note: Power=-40~0(db)\n", 
      .example = "EVCom >> dspconfigcpt cpt=busy f0=480,50 f1=620,50 cad0=450,550,450,550 pwr=-20",
      .func = cmdDspConfigCpt},

	{ .code = CMD_DSP_CONFIG_SIT,
      .name = "DspConfigSit",
      .opcode = "dcs",
      .desc = "Change DSP special information tone detection configuration.",
      .usage = "<dspconfigsit | dcs> $attr0=[value] $attr1=[value] ...\n" \
      			"attr: <f0 | f1 | f2 | f3 | f4 >=[freqency,deviation] Note: Frequency=0~4000(Hz)" \
      			" Deviation=Tolerated frequency deviation from base frequency\n" \
      			"attr: <sd | ld>=[Mintime(ms),Maxtime(ms)] Note: sd=short duration, ld=long duration\n" \
      			"attr: pwr = [power] Note: Power=-40~0(db)\n", 
      .example = "EVCom >> dspconfigsit f0=914,50 f1=985,50 f2=1370,50 f3=1428,50 f4=1776,50 sd=250,300 ld=350,400 pwr=-39",
      .func = cmdDspConfigSit},
                       
    { .code = CMD_CHAN_CONFIG,
      .name = "ChanConfig",
      .opcode = "cc",
      .desc = "Change channel configuration.",
#ifdef CZ_DSP_SUPPORT
      .usage = "<chanconfig | cc> ch=[channel_id] $attr0=[value] $attr1=[value] ...\n" \
      			"attr: det=<dtmf | mdm | cpt | cid | pulse | jb>[+|-], i.e. To enable (Call Progress Tone) detection, det=cpt+ \n" \
      			"attr: ec=<on | off>, i.e. To disable EC, ec=off \n" \
				"attr: ecnlp=<on | off>, i.e. To disable NLP, ecnlp=off \n" \
				"attr: ectaillen=<36|48|60|72>, i.e. To set ec tail length to 36ms, ectaillen=36 \n" \
      			"attr: <tx | rx>=[-20~20], Note: Adjust tx(speakGain)/rx(listenGain) Slic gain level between +/-20db, default at 0db\n" \
      			"attr: <dsptx | dsprx>=[-20~20], Note: Adjust dsptx(speakGain)/dsprx(listenGain) gain level between +/-20db, default at 0db\n" \
      			"attr: <accdev | rejdev | dtmfpower | dtmfdur>=[accept_deviation,reject_deviation,dtmf_power, dtmf_duration]\n " \
      			" Note: accdev=15~35(%) rejdev=25~45(%) rejdev MUST at least 10% larger than accdev\n" \
      			" Note: dtmfpower=-25~-45(dbm) dtmfdur=5-16(unit:5ms) \n",
#else
      .usage = "<chanconfig | cc> ch=[channel_id] $attr0=[value] $attr1=[value] ...\n" \
      			"attr: det=<dtmf | mdm | cpt | cid | pulse | jb>[+|-], i.e. To enable (Call Progress Tone) detection, det=cpt+ \n" \
      			"attr: ec=<on | off>, i.e. To disable EC, ec=off \n" \
      			"attr: <tx | rx>=[-20~20], Note: Adjust tx(speakGain)/rx(listenGain) Slic gain level between +/-20db, default at 0db\n" \
      			"attr: <dsptx | dsprx>=[-20~20], Note: Adjust dsptx(speakGain)/dsprx(listenGain) gain level between +/-20db, default at 0db\n" \
      			"attr: <accdev | rejdev | dtmfpower | dtmfdur>=[accept_deviation,reject_deviation,dtmf_power, dtmf_duration]\n " \
      			" Note: accdev=15~35(%) rejdev=25~45(%) rejdev MUST at least 10% larger than accdev\n" \
      			" Note: dtmfpower=-25~-45(dbm) dtmfdur=5-16(unit:5ms) \n",
#endif
      .example = "EVCom >> chanconfig ch=1 ec=on det=mdm- tx=-3 rx=3",
      .func = cmdChanConfig},   

    { .code = CMD_CHAN_PLAY_TONE,
      .name = "ChanPlayTone",
      .opcode = "cpt",
      .desc = "Generate a tone to the channel.",
      .usage = "<chanplaytone | cpt> ch=[channel_id] tone=[tone_id<,tone_id,...>] rpt=[repeat] dur=[duration]",
      .example = "EVCom >> chanplaytone ch=0 tone=1 rpt=2 dur=2000",
      .func = cmdChanPlayTone},   
	  
    { .code = CMD_CHAN_STOP_TONE,
      .name = "ChanStopTone",
      .opcode = "cst",
      .desc = "Stop a tone to the channel.",
      .usage = "<chanstoptone | cst> ch=[channel_id]",
      .example = "EVCom >> chanstoptone ch=0",
      .func = cmdChanStopTone}, 
	  
    { .code = CMD_CHAN_PLAY_DTMF_TONE,
      .name = "ChanplayDtmfTone",
      .opcode = "cdt",
      .desc = "Generate a DTMF tone to the channel.",
      .usage = "<chanplaydtmftone | cdt> ch=[channel_id] digit=[dtmf_digit] dur=[duration]",
      .example = "EVCom >> chanplaydtmftone ch=0 digit=1 dur=5000",
      .func = cmdChanPlayDtmfTone}, 

    { .code = CMD_CHAN_PLAY_CID,
      .name = "ChanPlayCid",
      .opcode = "cpc",
      .desc = "Generate a caller ID to the channel.",
      .usage = "<chanplaycid | cpc> ch=[channel_id] cid=[number] name=[name] cidp=[power]" \
      		      " cidf=<us | eu | jp | rpas | dtas>, Note: us-BELLCORE_FSK, eu-ETSI_FSK, jp-NTT\n"\
      		      "mwi=<none | set | clear>\n"\
      		      "date=[datetime], Note:datetime format as MMddHHmm,"\
      		      "RPAS timing: T2=200~500(ms),T3=500~800(ms)"\
      		      "DTAS timing: T2=200~500(ms),T4=45~500(ms)",
      .example = "EVCom >> chanplaycid ch=0 cid=1234 name=abcd cidf=us cidp=-10 mwi=none",
      .func = cmdChanPlayCid},  

    { .code = CMD_CHAN_PLAY_CID_T2,
      .name = "ChanPlayCid2",
      .opcode = "cpc2",
      .desc = "Generate a type 2 caller ID with SAS signal to the channel.",
      .usage = "<chanplaycid2 | cpc2> ch=[channel_id] cid=[number] cidp=[fskpower]" \
      		      " cidf=<us | eu | jp>, Note: us-BELLCORE_FSK, eu-ETSI_FSK, jp-NTT\n"\
      		      "date=[datetime], Note:datetime format as MMddHHmm,"\
      		      "timing: T9=0~15(10ms) T10=0~15(10ms) T11=4~5(10ms) T12=6~20(10ms)\n"\
      		      "T13=4~12(10ms) T14=15~16(10ms)(note:T9+T10+T14<=32(10ms))",
      .example = "EVCom >> chanplaycid2 ch=0 cid=1234 cidp=-24 cidf=us mwi=none date=12302010 T9=5 T10=10 T11=4"\
      				"T12=10 T13=8 T14=15",
      .func = cmdChanPlayCid2},

    { .code = CMD_CHAN_PCM_DUMP,
      .name = "ChanPcmDump",
      .opcode = "cpd",
      .desc = "Enable PCM dump for debug.",
      .usage = "<chanpcmdump | cpd> ch=[channel_id] ip=[ip], Note: Set IP as 0.0.0.0 to disable.",
      .example = "EVCom >> chanpcmdump ch=0 ip=192.168.1.2",
      .func = cmdChanPcmDump},
  
    { .code = CMD_CHAN_PLAY_CID_TDTMF,
      .name = "ChanPlayCidDtmf",
      .opcode = "cpcd",
      .desc = "Generate a dtmf type caller ID to the channel.",
      .usage = "<chanplayciddtmf | cpcd> ch=[channel_id] cid=[number]" \
      			"make=40(ms) break=40(ms) dtmfp=[DTMFpower] dtmfT3=350(ms)",
      .example = "EVCom >> chanplayciddtmf ch=0 cid=1234 make=40 break=40 dtmfp=0 dtmfT3=350",
      .func = cmdChanPlayDtmfCid},   

    { .code = CMD_STRM_CONFIG,
      .name = "StrmConfig",
      .opcode = "sc",
      .desc = "Change a stream configuration.",
#ifdef TCSUPPORT_C1_MS
      .usage = "<strmconfig | sc> st=[channel_id:stream_id] $attr0=[value] $attr1=[value] ...\n" \
                  "attr: src=[ip_address:rtp_port:rtcp_port] \n" \
                  "attr: dst=[ip_address:rtp_port:rtcp_port] \n" \
                  "attr: rtptos=<value> \n" \
                  "attr: rtcptos=<value> \n" \
                  //"attr: codec=<g711a | g711u | g722 | g726 | g729 | t38> \n" \ //isabella_g7231
		          "attr: codec=<g711a | g711u | g722 | g7231 | g726 | g729 | t38> \n" \ 
		          "attr: ulcodec=<g711a | g711u | g722 | g7231 | g726 | g729 | t38> \n" \ 
		          "attr: redundat=<on | off \n" \ 
		          "attr: redundant_pt=<value>,g726_pt=<value>,amr_pt=<value> \n"\
                  "attr: ulpt=<10 | 20 | 30 | 40> \n" \
                  "attr: dlpt=<10 | 20 | 30 | 40> \n" \
                  "attr: ulbr=<6300 | 5300> Note:bitrate for g7231. \n" \
                  "attr: dtmfr=<on | off> \n" \
                  "attr: dtmfrm=<on | off> \n" \
                  "attr: dtmful_pt=<value>\n" \
                  "attr: dtmfdl_pt=<value>\n" \
                  "attr: vad=<on | off> \n" \
                  "attr: cng=<on | off> \n" \
                  "attr: scomp=<on | off> \n" \
                  "attr: plc=<on | off> \n" \
                  "attr: dir=<sr | so | ro | in> Note: sr=send+receive, so=send only, ro=receive only, in=inactive\n"\
                  "attr: jb=[<a | f>, jb_init_size(ms), jb_max_size(ms)] Note: a=adaptive, f=fixed, JB size is between 0~800 ms\n"\
                  "attr: ecm=<on | off>",
#else
	.usage = "<strmconfig | sc> st=[channel_id:stream_id] $attr0=[value] $attr1=[value] ...\n" \
		          "attr: src=[ip_address:rtp_port:rtcp_port] \n" \
		          "attr: dst=[ip_address:rtp_port:rtcp_port] \n" \
		          "attr: rtptos=<value> \n" \
		          "attr: rtcptos=<value> \n" \
			      //"attr: codec=<g711a | g711u | g722 | g726 | g729 | t38> \n" \ //isabella_g7231
          		  "attr: codec=<g711a | g711u | g722 | g7231 | g726 | g729 | t38|vbd-pcma|vbd-pcmu> \n" \ 
          		  "attr: ulcodec=<g711a | g711u | g722 | g7231 | g726 | g729 | t38|vbd-pcma|vbd-pcmu> \n" \ 
         		  "attr: redundat=<on | off \n" \ 
         		  "attr: redundant_pt=<value>,g726_pt=<value>,amr_pt=<value> \n"\
        		  "attr: ulpt=<10 | 20 | 30 | 40> \n" \
        		  "attr: dlpt=<10 | 20 | 30 | 40> \n" \
        		  "attr: dtmfr=<on | off> \n" \
        		  "attr: dtmfrm=<on | off> \n" \
        		  "attr: vad=<on | off> \n" \
        		  "attr: cng=<on | off> \n" \
        		  "attr: scomp=<on | off> \n" \
        		  "attr: plc=<on | off> \n" \
        		  "attr: dir=<sr | so | ro | in> Note: sr=send+receive, so=send only, ro=receive only, in=inactive\n"\
        		  "attr: jb=[<a | f>, jb_init_size(ms), jb_max_size(ms)] Note: a=adaptive, f=fixed, JB size is between 0~800 ms\n"\
        		  "attr: ecm=<on | off>",
#endif

    #ifdef TCSUPPORT_C1_MS
	.example = "EVCom >> strmconfig st=0:0 src=192.168.1.1:5566:5567 dst=192.168.1.2:5566:5567 " \
      			"rtptos=46 rtcptos=46 codec=g711a ulcodec=g711a redundant_pt=102 dtmful_pt=97 dtmfdl_pt=98 ptime=20 dtmfr=on dtmfrm=off vad=on cng=on scomp=on dir=sr plc=on jb=f,500,700 ",
    #else
	.example = "EVCom >> strmconfig st=0:0 src=192.168.1.1:5566:5567 dst=192.168.1.2:5566:5567 " \
      			"rtptos=46 rtcptos=46 codec=g711a ulcodec=g711a redundant_pt=102 ptime=20 dtmfr=on dtmfrm=off vad=on cng=on scomp=on dir=sr plc=on jb=f,500,700",/*use either scomp or VAD/CNG*/
    #endif     			
      .func = cmdStrmConfig},  

    { .code = CMD_STRM_START,
      .name = "StrmStart",
      .opcode = "ss",
      .desc = "Start a streaming process.",
      .usage = "<strmstart | ss> st=[channel_id:stream_id]",
      .example = "EVCom >> strmstart st=0:0",
      .func = cmdStrmStart},   
      
    { .code = CMD_STRM_STOP,
      .name = "StrmstoP",
      .opcode = "sp",
      .desc = "Stop a streaming process.",
      .usage = "<strmstop | sp> st=[channel_id:stream_id]",
      .example = "EVCom >> strmstop st=0:0",
      .func = cmdStrmStop},   
      
    { .code = CMD_STRM_SEND_DTMFR,
      .name = "StrmSendDtmfr",
      .opcode = "ssd",
      .desc = "Send DTMF relay (RFC2833/4733) packet to the stream.",
      .usage = "<strmsenddtmfr | ssd> st=[channel_id:stream_id] " \
		      "dtmf=<[0~9] | * | # | [a~d]> dur=[duration(ms), dur >= 10ms]",
      .example = "EVCom >> strmsenddtmfr st=0:0 dtmf=1 dur=500",
      .func = cmdStrmSendDtmfr},   
      
    { .code = CMD_STRM_PLAY_TONE,
      .name = "StrmPlayTone",
      .opcode = "spt",
      .desc = "Generate tone to the stream.",
      .usage = "<strmplaytone | spt> st=[channel_id:stream_id] tone=[tone_id<,tone_id,...>] rpt=[repeat]",
      .example = "EVCom >> strmplaytone st=0:0 tone=1 rpt=5",
      .func = cmdStrmPlayTone}, 
   { .code = CMD_STRM_PLAY_DTMF,
      .name = "StrmPlayDtmf",
      .opcode = "spd",
      .desc = "Generate dtmf to the stream.",
      .usage = "<strmplaydtmf | spd> ch=[channel_id] digit=[dtmf_digit] dur=[duration]",
      .example = "EVCom >> strmplaydtmf ch=0 digit=1 dur=5000",
      .func = cmdStrmPlayDtmf}, 

	{ .code = CMD_STRM_QUERY_MEDIA_INFO,
      .name = "StrmQueryMediaInfo",
      .opcode = "sqm",
      .desc = "Query Media Info to the stream.",
      .usage = "<StrmQueryMediaInfo | sqm> st=[channel_id:stream_id] ",
      .example = "EVCom >> StrmQueryMediaInfo st=0:0",
      .func = cmdStrmQueryMediaInfo},

	{ .code = CMD_STRM_RESET_MEDIA_INFO,
      .name = "StrmResetMediaInfo",
      .opcode = "srm",
      .desc = "Reset Media Info to the stream.",
      .usage = "<StrmResetMediaInfo | srm> st=[channel_id:stream_id]",
      .example = "EVCom >> StrmResetMediaInfo st=0:0",
      .func = cmdStrmResetMediaInfo},
      
    { .code = CMD_INFC_QUERY,
      .name = "InfcQuery",
      .opcode = "iq",
      .desc = "Query interface configuration.",
      .usage = "<infcquery | iq> if=[interface_id]",
      .example = "EVCom >> infcquery if=0",
      .func = cmdInfcQuery},   
  
    { .code = CMD_INFC_CONFIG_LINE,
      .name = "InfcConfigLine",
      .opcode = "icl",
      .desc = "Change interface line state, only works for FXS interface.",
      .usage = "<infcconfigline | icl> if=[interface_id] line=<down | fwd | rev | ring | sleep>",
      .example = "EVCom >> infcconfigline if=0 line=fwd",
      .func = cmdInfcConfigLine},   
  
    { .code = CMD_INFC_CONFIG_HOOK,
      .name = "InfcConfigHook",
      .opcode = "ich",
      .desc = "Change interface hook state, only works for FXO interface.",
      .usage = "<infcconfighook | ich> if=[interface_id] hook=<on | off | flash | pulse1~pulse20>",
      .example = "EVCom >> infcconfighook if=2 hook=flash",
      .func = cmdInfcConfigHook},   

   { .code = CMD_INFC_HW_PROF,
      .name = "InfcConfigHwProf",
      .opcode = "ichp",
      .desc = "Change country code",
      .usage = "<InfcConfigHwProf | ichp> if=[interface_id] HwProf=<0| 1 | 2 | 3 |...>",
      .example = "EVCom >> infcconfighwprof if=2 HwProf=2",
      .func = cmdInfcConfigHwProf}, 

    { .code = CMD_INFC_CONFIG_HOOKTS,
      .name = "InfcConfighookTs",
      .opcode = "ict",
      .desc = "Change interface hook time detection threshold. Min_flash, Max_flash, Min_release, "\
      			"pulseDetSeizeMax/Min, pulseDetReleaseMax/Min only works for FXS. "\
      			"autoFlashDur, pulseGenSeize, pulseGenRelease only works for FXO.",
      .usage = "<infcconfighookts | ict> if=[interface_id] fmin=[Min_flash] fmax=[Max_flash] rmin=[Min_release] smin=[Min_seize]" \
      			"flash=[autoFlashDur] pgs=[pulseGenSeize] pgr=[pulseGenRelease] " \
      			"pdsmax=[pulseDetSeizeMax] pdsmin=[pulseDetSeizeMin] pdrmax=[pulseDetReleaseMax] pdrbin=[pulseDetReleaseMin] pdd=[pulseDetEnable[on|off]]"\
      			", Note: Hook release time(ms) to meet each hook state",
      .example = "EVCom >> infcconfighookts if=0 fmin=250 fmax=600 rmin=700 smin=300 flash=200 pgs=70 pgr=70 "\
      			"pdsmax=350 pdsmin=20 pdrmax=100 pdrmin=40 pdd=on",
      .func = cmdInfcConfigHookTs},   
  
    { .code = CMD_INFC_CONFIG_RING,
      .name = "InfcConfigRing",
      .opcode = "icr",
      .desc = "Change interface ring profile configuration.",
      .usage =  "<infcconfigring | icr> if=[interface_id] $attr0=[value] $attr1=[value] ...\n" \
      			"attr: <cad0 | cad1 | cad2>=[ontime(ms),offtime(ms)], i.e. cad0=500,200\n" \
      			"attr: dur=[duration(ms)]," \
      			"attr: cid=[number], i.e. cid=1234\n" \
      			"attr: cidp=[pwr lv], i.e. cidp=-9\n" \
       			"attr: cidf=<us | eu | jp | dtas | rpas | dtmf>, i.e.cidf=us Note: us-BELLCORE_FSK,eu-ETSI_FSK, jp-NTT rpas-ETSI_RPAS dtas-ETSI_DTAS\n" \
      			"attr: cidb=[ring break], i.e. cidb=1 Note: Send CID at 1st ring break - between 1st and 2nd ring\n"\
      			"attr: cidt=[CID wait time(ms)], i.e. cidbt=100 Note: CID will wait 100ms to send after ring break\n"\
			    "attr: mwi=[none |set|clear], i.e. none, no mwi info; set, activate unread message indicator; clear, deactivate unread message indicator\n",
      .example = "EVCom >> infcconfigring if=0 dur=500 cad0=500,1000 cad1=0,0 cad2=0,0 cidf=us \n"
                 "cidb=0 cidt=100 cid=1234  cidp=-9",
      .func = cmdInfcConfigRing},   
  
    { .code = CMD_INFC_RING,
      .name = "InfcRing",
      .opcode = "ir",
      .desc = "Control interface ringing, only work for FXS type interface.",
      .usage = "<infcring | ir> if=[interface_id] dur=[duration(ms)], Note: duration is optional.",
      .example = "EVCom >> infcring if=0",
      .func = cmdInfcRing},   
  
/* added by mtk07059_20140610 ,  */
    { .code = CMD_PCM_LOOPBACK,
      .name = "PcmLoopback",
      .opcode = "pl",
      .desc = "Pcm loopback test switch.",
      .usage = "<PcmLoopback | pl> switch=<on | off>\n",
      .example = "EVCom >> PcmLoopback switch=on \n",
      .func = cmdPcmLoopback},   
    /*[OSBNB00041138] ADD by peter.jiang@20141126, support rtp/dsp loopback*/
    { .code = CMD_RTP_LOOPBACK,
      .name = "RtpLoopback",
      .opcode = "rl",
      .desc = "Rtp loopback test switch.",
      .usage = "<RtpLoopback | rl> ch=[channel_id] switch=<on | off>\n",
      .example = "EVCom >> RtpLoopback ch=0 switch=on \n",
      .func = cmdRtpLoopback},  
    { .code = CMD_DSP_LOOPBACK,
      .name = "DspLoopback",
      .opcode = "dl",
      .desc = "Dsp loopback test switch.",
      .usage = "<DspLoopback | dl> ch=[channel_id] switch=<on | off>\n",
      .example = "EVCom >> DspLoopback ch=0 switch=on \n",
      .func = cmdDspLoopback},  
    { .code = CMD_DSP_REPLACEDATA,
      .name = "DspReplaceData",
      .opcode = "drd",
      .desc = "Dsp replace data test config.",
      .usage = "<DspReplaceData | drd> ch=[channel_id] enable=<on | off> dir=<tx |rx> stage=<replace stage,even value 0,2,4...16> filepath=[replace file path]\n",
      .example = "EVCom >> DspReplaceData ch=0 enable=on dir=rx stage=0 filepath=/tmp/a.pcm\n",
      .func = cmdDspReplaceData}, 
/* ADD by mtk08176_20150508 , support line test.*/
    { .code = CMD_INFC_LINE_TEST,
      .name = "InfcLineTest",
      .opcode = "ilt",
      .desc = "Start line test.",
      .usage = "<InfcLineTest | ilt> if=[interface_id] testid=[test_items_id,zarlink:1~4,7,10~16 siliconlab:1~8].\n",
      .example = "EVCom >> InfcLineTest if=0 testid=1 \n",
      .func = cmdInfcLineTest},  
/*[OSBNB00045371] ADD by mtk07059_20150423 , support ring parameters set. */
    { .code = CMD_INFC_RING_PARAMS,
      .name = "InfcRingParams",
      .opcode = "irp",
      .desc = "Set ring parameters.",
      .usage = "<InfcRingParams | irp> if=[interface_id] prof=<sine | trap> freq=[frequence(mHz)] amp=[amplitude(mV)] dcb=[dcBias(mV) \ 
      			rtt=[ringTripThreshold(uA)]  ampslab=[amplitudeSlab(V)]\n",
      .example = "EVCom >> InfcRingParams if=0 prof=sine freq=25000 amp=45000 dcb=0 rtt=25000 ampslab=60\n", /* added by mtk08176_20150627  */
      .func = cmdInfcRingParams},  
/*[OSBNB00047036] ADD by mtk07059_20150616 , support dc feed parameters set. */
    { .code = CMD_INFC_DCFEED_PARAMS,
      .name = "InfcDCFeedParams",
      .opcode = "idp",
      .desc = "Set dc feed parameters.",
      .usage = "<InfcDcFeedParams | idp> if=[interface_id] ila=[current(uA)] ilaSlab=[current(mA)]\n",	/* added by mtk08176_20150627  */
      .example = "EVCom >>InfcDcFeedParams if=0 ila=25000 ilaSlab=20\n", /* added by mtk07059_20150616  */
      .func = cmdInfcDcFeedParams}, 
/* ADD by mtk08176_20150824, China Unicom inside and outside line test */
    { .code = CMD_INFC_CULINETEST_PARAMS,
      .name = "InfcCULineTest",
      .opcode = "iult",
      .desc = "China Unicom inside and outside line test.",
      .usage = "<InfcCULineTest | iult> if=[interface_id]\n",
      .example = "EVCom >>InfcCULineTest if=0\n",
      .func = cmdInfcCULineTest},

    { .code = CMD_QUIT,
      .name = "Quit",
      .opcode = "q",
      .desc = "Shutdown DSP process and leave this application.",
      .usage = "<quit | q>",
      .example = "EVCom >> quit",
      .func = cmdQuit},   
};

#endif

