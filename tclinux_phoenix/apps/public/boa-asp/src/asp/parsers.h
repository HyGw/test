#ifdef TCSUPPORT_GENERAL_MULTILANGUAGE
#ifndef _PARSERS_H_
#define _PARSERS_H_
#define	NULLKEY		0
#define	SUCCESS		1
#define	FAILURE			0
#define	DUPLICATE		-1
#define	MAX_KEY			32
#define	MAX_VALUE		580
#define	MAX_STREAM	MAX_KEY+MAX_VALUE+4
#define	PATH_LENGTH	32
#define	STRING_PATH	"/userfs/string%d.conf"

typedef unsigned int	uint;

typedef struct _HASH{
	uint	key;
	uint	ioffset;
}HASH;

typedef struct _SParser{
	HASH	*ele;
	uint	count;
	uint	sizeindex;
}SParser;

#endif
#endif
