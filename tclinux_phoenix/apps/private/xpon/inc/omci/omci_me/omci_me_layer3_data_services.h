
#ifndef _LAYER3_DATA_SERVICES_ME_H_
#define _LAYER3_DATA_SERVICES_ME_H_
#include "omci_types.h"
#include "omci_me.h"

typedef struct __pthreadIPHostArgType{
		uint16 tranCorrelatId;
		uint16 meClassId;
		uint8 msgFormat;		//0x0A:baseline format   0X0B:extended format
		uint32 addr;
		uint8 testType;
}pthreadIPHostArgType;

//9.4.1: IP host config data
extern omciAttriDescript_t omciAttriDescriptListIPHostConfigData[];
int32 setIPHostConfigDataIPOptions(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag);
int32 getIPHostConfigDataMACAddr(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag);
int32 getIPHostConfigDataIPAddr(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag);
int32 setIPHostConfigDataIPAddr(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag);
int32 getIPHostConfigDataMask(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag);
int32 setIPHostConfigDataMask(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag);
int32 setIPHostConfigDataGateway(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag);
int32 setIPHostConfigDataPrimaryDNS(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag);
int32 setIPHostConfigDataSecondaryDNS(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag);
int32 getIPHostConfigDataCurAddr(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag);
int32 getIPHostConfigDataCurMask(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag);
int32 getIPHostConfigDataCurGateway(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag);
int32 getIPHostConfigDataCurPrimaryDNS(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag);
int32 getIPHostConfigDataCurSecondaryDNS(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag);
int32 getIPHostConfigDataDomainName(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag);
int32 getIPHostConfigDataHostName(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag);

int omciMeInitForIPHostConfigData(omciManageEntity_t *omciManageEntity_p);

//9.4.2: IP host performance monitoring history data
extern omciAttriDescript_t omciAttriDescriptListIPHostPM[];
int32 setIPHostPMThresholdData(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag);
int32 getIPHostPMICMPErrors(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag);
int32 getIPHostPMDNSErrors(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag);
int32 getIPHostPMIPHostPMDHCPTimeout(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag);
int32 getIPHostPMIPHostPMIPConflict(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag);
int32 getIPHostPMOutOfMemory(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag);
int32 getIPHostPMInternalError(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag);

int omciMeInitForIPHostPM(omciManageEntity_t *omciManageEntity_p);

//9.4.3: TCP/UDP config data
extern omciAttriDescript_t omciAttriDescriptListTcpUdpConfigData[];

int omciMeInitForTcpUdpConfigData(omciManageEntity_t *omciManageEntity_p);

//9.4.4: TCP/UDP performance monitoring history data
extern omciAttriDescript_t omciAttriDescriptListTcpUdpPM[];
int32 setTcpUdpPMThresholdData(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag);
int32 getTcpUdpPMSocketFailed(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag);
int32 getTcpUdpPMListenFailed(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag);
int32 getTcpUdpPMBindFailed(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag);
int32 getTcpUdpPMAcceptFailed(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag);
int32 getTcpUdpPMSelectFailed(char *value, uint8 *meInstantPtr, omciAttriDescript_ptr omciAttribute, uint8 flag);

int omciMeInitForTcpUdpPM(omciManageEntity_t *omciManageEntity_p);

#ifdef TCSUPPORT_VNPTT
int omciMeInitFor250ME(omciManageEntity_t *omciManageEntity_p);
int omciMeInitFor347ME(omciManageEntity_t *omciManageEntity_p);
#endif

#ifdef TCSUPPORT_HUAWEI_OLT_VENDOR_SPECIFIC_ME
int omciMeInitFor350ME(omciManageEntity_t *omciManageEntity_p);
int omciMeInitFor352ME(omciManageEntity_t *omciManageEntity_p);
int omciMeInitFor353ME(omciManageEntity_t *omciManageEntity_p);
int omciMeInitFor367ME(omciManageEntity_t *omciManageEntity_p);
int omciMeInitFor373ME(omciManageEntity_t *omciManageEntity_p);
#ifdef TCSUPPORT_HUAWEI_OLT_VENDOR_SPECIFIC_ME_FOR_INA
int omciMeInitFor370ME(omciManageEntity_t *omciManageEntity_p);
int omciMeInitFor65408ME(omciManageEntity_t *omciManageEntity_p);
int omciMeInitFor65414ME(omciManageEntity_t *omciManageEntity_p);
int omciMeInitFor65425ME(omciManageEntity_t *omciManageEntity_p);
#endif
#endif

#endif

