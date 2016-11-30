#ifndef __BRIDGE_H__
#define __BRIDGE_H__


typedef struct {

	char cmd[30];
	void (*func)(char **argv);
}cmd_t;

void brtable_show(char **argv);
void brtable_setage(char **argv);
void brtable_clean(char **argv);

static cmd_t cmd[] = {

	{"show", brtable_show},
	{"setage", brtable_setage},
	{"clean", brtable_clean}
};

#endif