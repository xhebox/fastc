#ifndef _FASTC_PARSER_H
#define _FASTC_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAXLEN 0xFFFFFFFFFFFFFFFF
#define MAXOUTLEN (MAXLEN)*5/2+1

#include <stdint.h>

// para ( input array, output array, inputlen)
// we do not check if output array is big enough, but it sould be smaller than inputlen
// and if it's a seq like ANANAN, which has too many fragmentations, it may be bigger than inputlen
// provide MAXOUTLEN, but it's not recommened. MAXLEN is suitable for most cases
extern int fc_encode(uint8_t*, uint8_t*, uint64_t);
extern int fc_decode(uint8_t*, uint8_t*, uint64_t);

#ifdef __cplusplus
}
#endif

#endif
