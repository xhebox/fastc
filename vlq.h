#ifndef _VLQ_H
#define _VLQ_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

static inline int to_vlq(uint64_t x, char *out)
{
	int i, j;
	for (i = 9; i > 0; i--)
		if (x & 127ULL << i * 7) break;

	for (j = 0; j <= i; j++)
		out[j] = ((x >> ((i - j) * 7)) & 127) | 128;
 
	out[i] ^= 128;

	return i+1;
}
 
static inline uint64_t from_vlq(char *in)
{
	uint64_t r = 0;
 
	do {
		r = (r << 7) | (uint64_t)(*in & 127);
	} while (*in++ & 128);
 
	return r;
}

#ifdef __cplusplus
}
#endif

#endif
