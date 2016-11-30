
#ifndef _ALIAS_H_
#define _ALIAS_H_

#include <stdio.h>

#define ALIAS_NAME_LEN	128
#define FULL_NAME_LEN	256

typedef struct
{
	char *aliasName;
	char *fullName;
}nameMap;

extern int alias2full(const char *inName, char *outName);
extern int full2alias(const char *inName, char *outName);

#endif
