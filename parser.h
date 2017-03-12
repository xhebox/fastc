#ifndef _FASTC_PARSER_H
#define _FASTC_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#define SYM_INT 0xFF
#define MAXLEN 0xFFFFFFFFFFFFFFFF
// #para ( even number )
// #return ( size of buffer depending on input )
#define MAXOUTLEN(a) (a)*5/2+3

#include <stdint.h>

// #para ( input array, output array, input-count)
// #return len of output
// we do not check if output array is big enough, but it sould be smaller than input-count
// and if it's a seq like ANANAN, which has too many fragmentations, it may be bigger than input-count
// provide MAXOUTLEN(a), it's recommended to init an array
extern uint64_t fc_encode(uint8_t*, uint8_t*, uint64_t);

// #para ( mode, info, input array, output array, input-count)
// #return len of output
// 0 for mode, info for the first time
// and then put previous retval as input over and over again
extern uint64_t fc_estream(uint8_t*, uint64_t*, uint8_t*, uint8_t*, uint64_t);
// #para ( mode, info, output array)
// #return len of output
// put previous retval as input, and close the stream
extern uint64_t fc_estream_close(uint8_t*, uint64_t*, uint8_t*);

// #para ( input array, output array, input-count)
// and for decode, there's no good way to determine buffer
// 4 times to the input is enough in most cases
// #return len of output
extern uint64_t fc_decode(uint8_t*, uint8_t*, uint64_t);
// same as fc_estream*
extern uint64_t fc_dstream(uint8_t*, uint64_t*, uint8_t*, uint8_t*, uint64_t);

#ifdef __cplusplus
}
#endif

#endif
