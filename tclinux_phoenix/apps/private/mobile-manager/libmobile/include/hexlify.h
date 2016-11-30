#ifndef HEXLIFY_H_
#define HEXLIFY_H_

#include <stdint.h>
#include <stddef.h>

void hexlify(char *dst, const uint8_t *src, size_t len, char sep);
void heXlify(char *dst, const uint8_t *src, size_t len, char sep);
int unhexlify(uint8_t *dst, const char *src, size_t len);

#endif /* HEXLIFY_H_ */
