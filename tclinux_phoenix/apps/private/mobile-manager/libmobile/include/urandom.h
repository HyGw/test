#ifndef URANDOM_H_
#define URANDOM_H_

int urandom_init();
void urandom_deinit();
int urandom(void *buffer, size_t len);

#endif /* URANDOM_H_ */
