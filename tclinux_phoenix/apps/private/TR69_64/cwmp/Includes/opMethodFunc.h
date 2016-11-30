#ifndef _OPMETHODFUNC_H_
#define _OPMETHODFUNC_H_

#include <stdio.h>
#include "Global_res.h"

typedef midware_packet* (*opMethodFunc)(midware_packet *);

extern opMethodFunc cmd_list[40];

#endif

