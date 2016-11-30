#ifndef DISTRIBUT_H_
#define DISTRIBUT_H_

#include "event.h"
#include "urandom.h"
#include "mobile_manager.h"

#define PRD_COMMON		10000
#define PRD_RESOLVE		10000

#define DISTRI_NOMAL_TRY_MAX	3
#define DISTRI_HB_TRY_MAX	3

int start_distribut_work(void);

#endif /*DISTRIBUT_H_*/
