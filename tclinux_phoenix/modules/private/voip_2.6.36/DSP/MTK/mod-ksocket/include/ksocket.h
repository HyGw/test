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
/**
 *  @file           ksocket.h
 *  @author         Shu-Liang Lee
 *  @description    KSOCKET Header file
 *
 *  @date           2008.03.18
 *  @comments       reference
 *                  ksocket project
 *                  BSD-style socket APIs for kernel 2.6 developers
 *                  
 *                  @2007, China
 *                  @song.xian-guang@hotmail.com (MSN Accounts)
 *                  
 *                  This code is licenced under the GPL
 *                  Feel free to contact me if any questions
 * 
 */

/* 
 * $Log: ksocket.h,v $
 * Revision 1.2  2009/04/01 08:02:28  mtk02202
 * * Add a parameter to kaccept() to let accept can set flags(like O_NONBLOCK)
 *
 * Revision 1.1  2008/04/29 14:06:55  mtk01662
 * *: fit new linux platform architecture
 *
 *
 */
#ifndef __KSOCKET_H__
#define __KSOCKET_H__

/*==========================================================================
 *
 *      Library Inclusion Segment
 *
 *==========================================================================
 */ 
#include <linux/module.h>
#include <linux/string.h>
#include <linux/socket.h>
#include <linux/net.h>
#include <linux/in.h>
#include <net/sock.h>
#include <asm/processor.h>
#include <asm/uaccess.h>

/*==========================================================================
 *
 *      External Variable Segment
 *
 *==========================================================================
 */

/*==========================================================================
 *
 *      Constant and Macro Definition Segment
 *
 *==========================================================================
 */
#define KSOCKET_MAJOR_NUM      250 

#define xLog(a, ...)
 
typedef struct  ksocket_ioctl_param
{
} T_KSOCKET_IOCTL_PARAM;
 
/*==========================================================================
 *
 *      Internal Function Segment
 *
 *==========================================================================
 */

/*==========================================================================
 *
 *      Global Variable Segment
 *
 *==========================================================================
 */

/*==========================================================================
 *
 *      External Function Segment
 *
 *==========================================================================
 */

/*--------------------------------------------------------------------------
 *                                                                          
 *  FUNCTION NAME :                                                         
 *      
 *                                                                          
 *  DESCRIPTION :                                                           
 *
 *  INPUT :                                                                 
 *                                                                          
 *  OUTPUT :                                                                
 *      None                                                                
 *
 *  RETURN :                                                                
 *      None                                                              
 *--------------------------------------------------------------------------
 */
#ifdef __cplusplus
extern "C"
{
#endif

struct socket;
struct sockaddr;
typedef struct socket * ksocket_t;

/* BSD socket APIs prototype declaration */
ksocket_t ksocket(int domain, int type, int protocol);
int kshutdown(ksocket_t socket, int how);
int kclose(ksocket_t socket);

int kbind(ksocket_t socket, struct sockaddr *address, int address_len);
int klisten(ksocket_t socket, int backlog);
int kconnect(ksocket_t socket, struct sockaddr *address, int address_len);
ksocket_t kaccept(ksocket_t socket, struct sockaddr *address, int *address_len, long flags);

ssize_t krecv(ksocket_t socket, void *buffer, size_t length, int flags);
ssize_t ksend(ksocket_t socket, const void *buffer, size_t length, int flags);
ssize_t krecvfrom(ksocket_t socket, void * buffer, size_t length, int flags, struct sockaddr * address, int * address_len);
ssize_t ksendto(ksocket_t socket, void *message, size_t length, int flags, const struct sockaddr *dest_addr, int dest_len);

int kgetsockname(ksocket_t socket, struct sockaddr *address, int *address_len);
int kgetpeername(ksocket_t socket, struct sockaddr *address, int *address_len);
int ksetsockopt(ksocket_t socket, int level, int optname, void *optval, int optlen);
int kgetsockopt(ksocket_t socket, int level, int optname, void *optval, int *optlen);
 
unsigned int inet_addr(char* ip);
char *inet_ntoa(struct in_addr in);
  
#ifdef __cplusplus
}
#endif

#endif  /* __KSOCKET_H__ */

