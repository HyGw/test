#ifndef OPERATION_H_
#define OPERATION_H_

#include "event.h"
#include "urandom.h"
#include "mobile_manager.h"

#define PRD_COMMON		10000
#define PRD_RESOLVE		10000

int operate_init(void);
int start_operate_work(void);
int stop_operate_work(void);
void mobile_restore(void);

#endif /*OPERATION_H_*/

