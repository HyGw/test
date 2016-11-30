#ifndef _SERVERPACKET_H
#define _SERVERPACKET_H

#include "packet.h"

#if defined (CWMP) && defined(TR111)
#include "dhcpd.h"
//#include <fcntl.h>
#include <unistd.h>
#include "../../../lib/libtcapi.h"
#endif

#if defined(TCSUPPORT_CT_FJ)
int sendOffer(struct dhcpMessage *oldpacket, unsigned int mark);
#else
int sendOffer(struct dhcpMessage *oldpacket);
#endif

int sendNAK(struct dhcpMessage *oldpacket);
#if defined(TCSUPPORT_CT_FJ)
int sendACK(struct dhcpMessage *oldpacket, uint32_t yiaddr, unsigned int mark);
#else
int sendACK(struct dhcpMessage *oldpacket, uint32_t yiaddr);
#endif
#if defined(TCSUPPORT_CT_FJ)
int send_inform(struct dhcpMessage *oldpacket, unsigned int mark);
#else
int send_inform(struct dhcpMessage *oldpacket);
#endif
#if defined(TCSUPPORT_CT_PORTAL_MANAGEMENT)
int write_ip_vendor_files(uint32_t yiaddr, uint8_t matchid);
#endif

#if defined (CWMP) && defined(TR111)
/* where to write the devices info file */
#define DEVICE_INFO_FILE         "/etc/devices.conf"
#define DEVICE_HOSTINDEX_INFO_FILE         "/etc/device_hostindex.conf"
//#define MAX_BUF_SIZE 2048
#define ATTRIBUTE_ACTIVE 0x20

int check_vi(struct dhcpMessage *packet);
void parseDev(struct dhcpMessage *packet, mDev_t *dev);
void insertDevice(struct dhcpMessage *packet);
#if 0
int findDevice(uint8_t clientID[]);
#else
//int findDevice(mDev_t* pdev);
int findDevice(struct dhcpMessage *packet,mDev_t* pdev);
#endif
void deleteDevice(struct dhcpMessage *packet);
void deleteDeviceByMac(uint8_t *macAddr);
unsigned long getDeviceNum(void);
void writeDevicesFile(void);
void writeHostIndexFile(void);
void cwmpShowmDev(void);
void dhcpRefresh(void);
#endif
#if defined(TCSUPPORT_CT_IPTV_NODE)
int findSTBDevice(uint8_t clientID[]);
void insertSTBDevice(struct dhcpMessage *packet);
void deleteSTBDevice(struct dhcpMessage *packet);
void deleteSTBDeviceByMac(uint8_t *macAddr);
void dhcpSTBRefresh(void);
#endif
#endif
