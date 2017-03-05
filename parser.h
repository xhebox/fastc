#ifndef _FASTC_PARSER_H
#define _FASTC_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#define MODE_ATCG 1
#define MODE_DEGE 2
#define SYM_INT 0xFF
#define MAXLEN 0xFFFFFFFFFFFFFFFF

#include <stdint.h>

// para ( input array, output array, inputlen)
// we do not check if output array is big enough, but it sould be smaller than inputlen
// and if it's a seq like ANANAN, too much fragmentation, it may be bigger than inputlen
extern int fc_encode(uint8_t*, uint8_t*, uint64_t);
extern int fc_decode(uint8_t*, uint8_t*, uint64_t);

#ifdef __cplusplus
}
#endif

#endif
