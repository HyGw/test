/*****************************************************
 ecmh - Easy Cast du Multi Hub - Common Functions
******************************************************
 $Author: aaron.zhang $
 $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/apps/public/ecmh-2005.02.09/src/common.h#1 $
 $Date: 2016/10/14 $
*****************************************************/

void dolog(int level, const char *fmt, ...);
int huprunning(void);
void savepid(void);
void cleanpid(int i);
