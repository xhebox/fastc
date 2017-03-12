#include <stdio.h>
#include "parser.h"

#define MODE_ATCG 1
#define MODE_DEGE 2

char codes[128] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0xB, //-
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0x0, //A
	0x6, //B
	0x3, //C
	0x7, //D
	0,0,
	0x2, //G
	0x8, //H
	0,0,
	0x2, //K
	0,
	0x3, //M
	0xA, //N
	0,0,0,
	0x0, //R
	0x4, //S
	0x1, //T
	0x1, //U
	0x9, //V
	0x5, //W
	0,
	0x1, //Y
	0,0,0,0,0,0,0,
	0x0, //a
	0x6, //b
	0x3, //c
	0x7, //d
	0,0,
	0x2, //g
	0x8, //h
	0,0,
	0x2, //k
	0,
	0x3, //m
	0xA, //n
	0,0,0,
	0x0, //r
	0x4, //s
	0x1, //t
	0x1, //u
	0x9, //v
	0x5, //w
	0,
	0x1, //y
	0,0,0,0,0
};

char dege[13] = { 'R', 'Y', 'K', 'M', 'S', 'W', 'B', 'D', 'H', 'V', 'N', '-', 'C' };

char atcg[256][4] = {
{ 'A', 'A', 'A', 'A' }, { 'A', 'A', 'A', 'T' }, { 'A', 'A', 'A', 'G' }, { 'A', 'A', 'A', 'C' },
{ 'A', 'A', 'T', 'A' }, { 'A', 'A', 'T', 'T' }, { 'A', 'A', 'T', 'G' }, { 'A', 'A', 'T', 'C' },
{ 'A', 'A', 'G', 'A' }, { 'A', 'A', 'G', 'T' }, { 'A', 'A', 'G', 'G' }, { 'A', 'A', 'G', 'C' },
{ 'A', 'A', 'C', 'A' }, { 'A', 'A', 'C', 'T' }, { 'A', 'A', 'C', 'G' }, { 'A', 'A', 'C', 'C' },
{ 'A', 'T', 'A', 'A' }, { 'A', 'T', 'A', 'T' }, { 'A', 'T', 'A', 'G' }, { 'A', 'T', 'A', 'C' },
{ 'A', 'T', 'T', 'A' }, { 'A', 'T', 'T', 'T' }, { 'A', 'T', 'T', 'G' }, { 'A', 'T', 'T', 'C' },
{ 'A', 'T', 'G', 'A' }, { 'A', 'T', 'G', 'T' }, { 'A', 'T', 'G', 'G' }, { 'A', 'T', 'G', 'C' },
{ 'A', 'T', 'C', 'A' }, { 'A', 'T', 'C', 'T' }, { 'A', 'T', 'C', 'G' }, { 'A', 'T', 'C', 'C' },
{ 'A', 'G', 'A', 'A' }, { 'A', 'G', 'A', 'T' }, { 'A', 'G', 'A', 'G' }, { 'A', 'G', 'A', 'C' },
{ 'A', 'G', 'T', 'A' }, { 'A', 'G', 'T', 'T' }, { 'A', 'G', 'T', 'G' }, { 'A', 'G', 'T', 'C' },
{ 'A', 'G', 'G', 'A' }, { 'A', 'G', 'G', 'T' }, { 'A', 'G', 'G', 'G' }, { 'A', 'G', 'G', 'C' },
{ 'A', 'G', 'C', 'A' }, { 'A', 'G', 'C', 'T' }, { 'A', 'G', 'C', 'G' }, { 'A', 'G', 'C', 'C' },
{ 'A', 'C', 'A', 'A' }, { 'A', 'C', 'A', 'T' }, { 'A', 'C', 'A', 'G' }, { 'A', 'C', 'A', 'C' },
{ 'A', 'C', 'T', 'A' }, { 'A', 'C', 'T', 'T' }, { 'A', 'C', 'T', 'G' }, { 'A', 'C', 'T', 'C' },
{ 'A', 'C', 'G', 'A' }, { 'A', 'C', 'G', 'T' }, { 'A', 'C', 'G', 'G' }, { 'A', 'C', 'G', 'C' },
{ 'A', 'C', 'C', 'A' }, { 'A', 'C', 'C', 'T' }, { 'A', 'C', 'C', 'G' }, { 'A', 'C', 'C', 'C' },
{ 'T', 'A', 'A', 'A' }, { 'T', 'A', 'A', 'T' }, { 'T', 'A', 'A', 'G' }, { 'T', 'A', 'A', 'C' },
{ 'T', 'A', 'T', 'A' }, { 'T', 'A', 'T', 'T' }, { 'T', 'A', 'T', 'G' }, { 'T', 'A', 'T', 'C' },
{ 'T', 'A', 'G', 'A' }, { 'T', 'A', 'G', 'T' }, { 'T', 'A', 'G', 'G' }, { 'T', 'A', 'G', 'C' },
{ 'T', 'A', 'C', 'A' }, { 'T', 'A', 'C', 'T' }, { 'T', 'A', 'C', 'G' }, { 'T', 'A', 'C', 'C' },
{ 'T', 'T', 'A', 'A' }, { 'T', 'T', 'A', 'T' }, { 'T', 'T', 'A', 'G' }, { 'T', 'T', 'A', 'C' },
{ 'T', 'T', 'T', 'A' }, { 'T', 'T', 'T', 'T' }, { 'T', 'T', 'T', 'G' }, { 'T', 'T', 'T', 'C' },
{ 'T', 'T', 'G', 'A' }, { 'T', 'T', 'G', 'T' }, { 'T', 'T', 'G', 'G' }, { 'T', 'T', 'G', 'C' },
{ 'T', 'T', 'C', 'A' }, { 'T', 'T', 'C', 'T' }, { 'T', 'T', 'C', 'G' }, { 'T', 'T', 'C', 'C' },
{ 'T', 'G', 'A', 'A' }, { 'T', 'G', 'A', 'T' }, { 'T', 'G', 'A', 'G' }, { 'T', 'G', 'A', 'C' },
{ 'T', 'G', 'T', 'A' }, { 'T', 'G', 'T', 'T' }, { 'T', 'G', 'T', 'G' }, { 'T', 'G', 'T', 'C' },
{ 'T', 'G', 'G', 'A' }, { 'T', 'G', 'G', 'T' }, { 'T', 'G', 'G', 'G' }, { 'T', 'G', 'G', 'C' },
{ 'T', 'G', 'C', 'A' }, { 'T', 'G', 'C', 'T' }, { 'T', 'G', 'C', 'G' }, { 'T', 'G', 'C', 'C' },
{ 'T', 'C', 'A', 'A' }, { 'T', 'C', 'A', 'T' }, { 'T', 'C', 'A', 'G' }, { 'T', 'C', 'A', 'C' },
{ 'T', 'C', 'T', 'A' }, { 'T', 'C', 'T', 'T' }, { 'T', 'C', 'T', 'G' }, { 'T', 'C', 'T', 'C' },
{ 'T', 'C', 'G', 'A' }, { 'T', 'C', 'G', 'T' }, { 'T', 'C', 'G', 'G' }, { 'T', 'C', 'G', 'C' },
{ 'T', 'C', 'C', 'A' }, { 'T', 'C', 'C', 'T' }, { 'T', 'C', 'C', 'G' }, { 'T', 'C', 'C', 'C' },
{ 'G', 'A', 'A', 'A' }, { 'G', 'A', 'A', 'T' }, { 'G', 'A', 'A', 'G' }, { 'G', 'A', 'A', 'C' },
{ 'G', 'A', 'T', 'A' }, { 'G', 'A', 'T', 'T' }, { 'G', 'A', 'T', 'G' }, { 'G', 'A', 'T', 'C' },
{ 'G', 'A', 'G', 'A' }, { 'G', 'A', 'G', 'T' }, { 'G', 'A', 'G', 'G' }, { 'G', 'A', 'G', 'C' },
{ 'G', 'A', 'C', 'A' }, { 'G', 'A', 'C', 'T' }, { 'G', 'A', 'C', 'G' }, { 'G', 'A', 'C', 'C' },
{ 'G', 'T', 'A', 'A' }, { 'G', 'T', 'A', 'T' }, { 'G', 'T', 'A', 'G' }, { 'G', 'T', 'A', 'C' },
{ 'G', 'T', 'T', 'A' }, { 'G', 'T', 'T', 'T' }, { 'G', 'T', 'T', 'G' }, { 'G', 'T', 'T', 'C' },
{ 'G', 'T', 'G', 'A' }, { 'G', 'T', 'G', 'T' }, { 'G', 'T', 'G', 'G' }, { 'G', 'T', 'G', 'C' },
{ 'G', 'T', 'C', 'A' }, { 'G', 'T', 'C', 'T' }, { 'G', 'T', 'C', 'G' }, { 'G', 'T', 'C', 'C' },
{ 'G', 'G', 'A', 'A' }, { 'G', 'G', 'A', 'T' }, { 'G', 'G', 'A', 'G' }, { 'G', 'G', 'A', 'C' },
{ 'G', 'G', 'T', 'A' }, { 'G', 'G', 'T', 'T' }, { 'G', 'G', 'T', 'G' }, { 'G', 'G', 'T', 'C' },
{ 'G', 'G', 'G', 'A' }, { 'G', 'G', 'G', 'T' }, { 'G', 'G', 'G', 'G' }, { 'G', 'G', 'G', 'C' },
{ 'G', 'G', 'C', 'A' }, { 'G', 'G', 'C', 'T' }, { 'G', 'G', 'C', 'G' }, { 'G', 'G', 'C', 'C' },
{ 'G', 'C', 'A', 'A' }, { 'G', 'C', 'A', 'T' }, { 'G', 'C', 'A', 'G' }, { 'G', 'C', 'A', 'C' },
{ 'G', 'C', 'T', 'A' }, { 'G', 'C', 'T', 'T' }, { 'G', 'C', 'T', 'G' }, { 'G', 'C', 'T', 'C' },
{ 'G', 'C', 'G', 'A' }, { 'G', 'C', 'G', 'T' }, { 'G', 'C', 'G', 'G' }, { 'G', 'C', 'G', 'C' },
{ 'G', 'C', 'C', 'A' }, { 'G', 'C', 'C', 'T' }, { 'G', 'C', 'C', 'G' }, { 'G', 'C', 'C', 'C' },
{ 'C', 'A', 'A', 'A' }, { 'C', 'A', 'A', 'T' }, { 'C', 'A', 'A', 'G' }, { 'C', 'A', 'A', 'C' },
{ 'C', 'A', 'T', 'A' }, { 'C', 'A', 'T', 'T' }, { 'C', 'A', 'T', 'G' }, { 'C', 'A', 'T', 'C' },
{ 'C', 'A', 'G', 'A' }, { 'C', 'A', 'G', 'T' }, { 'C', 'A', 'G', 'G' }, { 'C', 'A', 'G', 'C' },
{ 'C', 'A', 'C', 'A' }, { 'C', 'A', 'C', 'T' }, { 'C', 'A', 'C', 'G' }, { 'C', 'A', 'C', 'C' },
{ 'C', 'T', 'A', 'A' }, { 'C', 'T', 'A', 'T' }, { 'C', 'T', 'A', 'G' }, { 'C', 'T', 'A', 'C' },
{ 'C', 'T', 'T', 'A' }, { 'C', 'T', 'T', 'T' }, { 'C', 'T', 'T', 'G' }, { 'C', 'T', 'T', 'C' },
{ 'C', 'T', 'G', 'A' }, { 'C', 'T', 'G', 'T' }, { 'C', 'T', 'G', 'G' }, { 'C', 'T', 'G', 'C' },
{ 'C', 'T', 'C', 'A' }, { 'C', 'T', 'C', 'T' }, { 'C', 'T', 'C', 'G' }, { 'C', 'T', 'C', 'C' },
{ 'C', 'G', 'A', 'A' }, { 'C', 'G', 'A', 'T' }, { 'C', 'G', 'A', 'G' }, { 'C', 'G', 'A', 'C' },
{ 'C', 'G', 'T', 'A' }, { 'C', 'G', 'T', 'T' }, { 'C', 'G', 'T', 'G' }, { 'C', 'G', 'T', 'C' },
{ 'C', 'G', 'G', 'A' }, { 'C', 'G', 'G', 'T' }, { 'C', 'G', 'G', 'G' }, { 'C', 'G', 'G', 'C' },
{ 'C', 'G', 'C', 'A' }, { 'C', 'G', 'C', 'T' }, { 'C', 'G', 'C', 'G' }, { 'C', 'G', 'C', 'C' },
{ 'C', 'C', 'A', 'A' }, { 'C', 'C', 'A', 'T' }, { 'C', 'C', 'A', 'G' }, { 'C', 'C', 'A', 'C' },
{ 'C', 'C', 'T', 'A' }, { 'C', 'C', 'T', 'T' }, { 'C', 'C', 'T', 'G' }, { 'C', 'C', 'T', 'C' },
{ 'C', 'C', 'G', 'A' }, { 'C', 'C', 'G', 'T' }, { 'C', 'C', 'G', 'G' }, { 'C', 'C', 'G', 'C' },
{ 'C', 'C', 'C', 'A' }, { 'C', 'C', 'C', 'T' }, { 'C', 'C', 'C', 'G' }, { 'C', 'C', 'C', 'C' }
};

static inline uint64_t encode(uint8_t *mode, uint64_t *info, uint8_t *in, uint8_t *out, uint8_t *dst) {
	uint8_t *start = out;
	int n = 0;
	uint8_t flag = 0;

	if ( *info ) {
		if ( *mode == MODE_ATCG ) {
			*out = (*info)>>24;
			n = ((*info)>>16) & 0xFF;
			flag = ((*info)>>8) & 0xFF;
		} else if ( *mode == MODE_DEGE ) {
			*out = (*info)>>24;
		}
	}

	for (; in < dst; in++) {
		switch (*in) {
		case 'a':
		case 'A':
		case 'u':
		case 'U':
		case 't':
		case 'T':
		case 'g':
		case 'G':
		case 'c':
		case 'C':
			if ( *mode == MODE_ATCG ) {
				*out |= codes[*in] << (6-n*2);
				n++;
				if ( 4 == n ) {
					n = 0;
					if ( *out != 0xFF ) {
						if ( flag != MODE_DEGE ) {
							out++;
						} else {
							*(out+1) = *out;
							*out = SYM_INT;
							out += 2;
						}
					} else {
						*mode = MODE_DEGE;
						// SYM_INT == "CCCC" == 0xFF
						if ( flag != MODE_DEGE ) {
							*out++ = SYM_INT;
							*out++ = 4;
						} else {
							flag = 0;
						}
						*out = 0x3C;
					}
				}
			} else if ( *mode == MODE_DEGE ) {
				flag = MODE_DEGE;
				*mode = MODE_ATCG;
				*++out |= codes[*in] << (6-n*2);
				n++;
			} else {
				*mode = MODE_ATCG;
				*out++ = MODE_ATCG;

				*out |= codes[*in] << (6-n*2);
				n++;
			}
			break;
		case 'r':
		case 'R':
		case 'y':
		case 'k':
		case 'm':
		case 's':
		case 'w':
		case 'b':
		case 'd':
		case 'h':
		case 'v':
		case 'n':
		case 'Y':
		case 'K':
		case 'M':
		case 'S':
		case 'W':
		case 'B':
		case 'D':
		case 'H':
		case 'V':
		case 'N':
		case '-':
			if ( *mode == MODE_ATCG ) {
				*mode = MODE_DEGE;
				if ( n != 0 ) out++;

				*out++ = SYM_INT;
				*out++ = ( n == 0 ) ? 4 : n;
				n = 0;
				*out = codes[*in];
			} else if ( *mode == MODE_DEGE ) {
				if ( (*out >> 4) == 0xF || (*out & 0xF) != codes[*in] ) {
					*++out = codes[*in];
					break;
				}
				*out = ((*out>>4)+1)<<4 | (*out & 0xF);
			} else {
				*mode = MODE_DEGE;
				*out++ = MODE_DEGE;
				*out = codes[*in];
			}
			break;
		default:
			fprintf(stderr, "\33[31mencoding:\33[39m unknown character - %c, %x\n", *in, *in);
			return 0;
		}
	}

	// write down the number of nucleic in the last array
	if ( *mode == MODE_ATCG ) {
		if ( n != 0 ) {
			*info = (*out)<<24;
			*info |= n<<16;
			*info |= flag<<8;
		} else *info = 0;
	} else if ( *mode == MODE_DEGE ) {
		*info = (*out)<<24;
	}

	return out-start;
}

static inline uint64_t encode_close(uint8_t *mode, uint64_t *info, uint8_t *out) {
	uint8_t *start = out;
	if ( *info ) {
		if ( *mode == MODE_ATCG ) {
			uint8_t flag = ((*info)>>8) & 0xFF;
			if ( flag == MODE_DEGE )
				*out++ = SYM_INT;

			*out++ = (*info)>>24;
			uint8_t n = ((*info)>>16) & 0xFF;
			if ( n != 0 ) {
				*out++ = SYM_INT;
				*out++ = n;
			}
		} else if ( *mode == MODE_DEGE ) {
			*out++ = (*info)>>24;
		}
	}
	return out-start;
}

uint64_t fc_estream(uint8_t *mode, uint64_t *info, uint8_t *in, uint8_t *out, uint64_t count) {
	return encode(mode, info, in, out, in+count);
}

uint64_t fc_estream_close(uint8_t *mode, uint64_t *info, uint8_t *out) {
	return encode_close(mode, info, out);
}

uint64_t fc_encode(uint8_t *in, uint8_t *out, uint64_t count)
{
	uint8_t mode = 0, *modep = &mode;
	uint64_t info = 0, *infop = &info;
	uint64_t len = encode(modep, infop, in, out, in+count);
	return len + encode_close(modep, infop, out+len);
}
 
static inline uint64_t decode(uint8_t *mode, uint64_t *info, uint8_t *in, uint8_t *out, uint8_t *dst) {
	uint8_t *start = out;
	if (! *mode ) {
		*mode = *in++;
	}
	if ( (*info & 0xFFFFFFFF00000000) == 0xFFFFFFFF00000000 ) {
		goto read;
	}

	for (; in < dst; in++) {
		if ( *in == SYM_INT ) {
			if ( *mode == MODE_ATCG ) {
				// we met the limit
				if ( in+1 == dst ) {
					*info |= 0xFFFFFFFF00000000;
					continue;
				} else in++;

				// back if it's not 4 nucleic, but 3, 2, or 1
				// and skip the control code
read:
				if ( *info ) {
					for (int a=0; a < 4; a++)
						*out++ = ((*info) >> (24-a*8));
				}
				for(int a=0, b=4-*in; a < b; a++)
					*--out = 0;
				*mode = MODE_DEGE;
				*info = 0;
			} else {
				*mode = MODE_ATCG;
			}
			continue;
		} else if ( *mode == MODE_ATCG ) {
			if ( *info ) {
				for (int a=0; a < 4; a++)
						*out++ = ((*info) >> (24-a*8));
				*info = 0;
			}
			for (int a=0; a < 4; a++)
				*info |= (atcg[*in][a])<<(24-a*8);
		} else if ( *mode == MODE_DEGE ) {
			uint8_t code = (*in) & 0xF;
			for (int t=0, c=(*in)>>4; t <= c; t++)
				*out++ = dege[code];
		} else {
			fprintf(stderr, "\33[31mdecoding\33[39m: unknown mode - %d\n", *mode);
			return 0;
		}
	}
	return out-start;
}

uint64_t fc_dstream(uint8_t *mode, uint64_t *info, uint8_t *in, uint8_t *out, uint64_t count) {
	return decode(mode, info, in, out, in+count);
}

uint64_t fc_decode(uint8_t *in, uint8_t *out, uint64_t count)
{
	uint8_t mode = 0;
	uint64_t info = 0;
	return decode(&mode, &info, in, out, in+count);
}
