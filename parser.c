#include <stdio.h>
#include "parser.h"

#define MODE_ATCG 1
#define MODE_DEGE 2
#define SYM_INT 0xFF

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

int fc_encode(uint8_t *in, uint8_t *out, uint64_t maxlen)
{
	uint8_t mode = 0;
	int n = 1;

	for (uint8_t *dst = in+maxlen; in < dst; in++) {
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
			if ( mode == MODE_ATCG ) {
				*out |= codes[*in] << (8-n*2);
				if ( 4 == n++ ) {
					n = 1;
					if ( *out != 0xFF ) {
						out++;
					} else {
						mode = MODE_DEGE;
						// SYM_INT == "CCCC" == 0xFF
						*out++ = SYM_INT;
						*out++ = 4;
						*out = 0x3C;
					}
				}
			} else if ( mode == MODE_DEGE ) {
				// we met CCCC
				if ( *in == 'C' && *(in+1) == 'C' && *(in+2) == 'C' && *(in+3) == 'C' ) {
					in += 3;
					if ( (*out >> 4) == 0xF || (*out & 0xF) != codes[*in] ) {
						*++out = 0x3C;
						break;
					}
					*out = ((*out>>4)+3)<<4 | (*out & 0xF);
					break;
				}

				mode = MODE_ATCG;
				*++out = SYM_INT;
				*++out |= codes[*in] << (8-n*2);
				n++;
			} else {
				mode = MODE_ATCG;
				*out++ = MODE_ATCG;

				*out |= codes[*in] << (8-n*2);
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
			if ( mode == MODE_ATCG ) {
				mode = MODE_DEGE;
				if ( n != 1 ) out++;

				*out++ = SYM_INT;
				*out++ = (n == 1) ? 4 : n - 1;
				n = 1;
				*out = codes[*in];
			} else if ( mode == MODE_DEGE ) {
				if ( (*out >> 4) == 0xF || (*out & 0xF) != codes[*in] ) {
					*++out = codes[*in];
					break;
				}
				*out = ((*out>>4)+1)<<4 | (*out & 0xF);
			} else {
				mode = MODE_DEGE;
				*out++ = MODE_DEGE;
				*out = codes[*in];
			}
			break;
		case '\n':
		case ' ':
			break;
		default:
			fprintf(stderr, "\33[31mencoding: unknown character - %c, %x\n\33[39m", *in, *in);
			return -1;
		}
	}

	// write down the number of nucleic in the last array
	if ( mode == MODE_ATCG && n != 1 ) {
		*out++ = SYM_INT;
		*out = n - 1;
	}

	return 0;
}
 
int fc_decode(uint8_t *in, uint8_t *out, uint64_t maxlen)
{
	uint8_t mode = *in++;

	for (uint8_t *dst = in+maxlen-1; in < dst; in++) {
		if ( *in == SYM_INT ) {
			if ( mode == MODE_ATCG ) {
				// back if it's not 4 nucleic, but 3, 2, or 1
				// and skip the control code
				out -= (4 - *++in);
				mode = MODE_DEGE;
			} else {
				mode = MODE_ATCG;
			}
			continue;
		} else if ( mode == MODE_ATCG ) {
			for (int a=0; a < 4; a++)
				*out++ = atcg[*in][a];
		} else if ( mode == MODE_DEGE ) {
			uint8_t code = (*in) & 0xF;
			for (int t=0, c=(*in)>>4; t <= c; t++)
				*out++ = dege[code];
		} else {
			fprintf(stderr, "\33[31mdecoding: unknown mode - %d\n\33[39m", mode);
			return -1;
		}
	}
	return 0;
}
