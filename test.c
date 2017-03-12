#include <stdio.h>
#include <string.h>
#include "parser.h"

int main() {
	uint8_t string[] = "NNATCGCNYCCCCNNCCCCCCCCNNATCGNNNNNNNNNNNNNNNNNNKKATCGCCNNATC";
	uint8_t string1[] = "NNATCGCNYCCCCNNCCCCCCCC";
	uint8_t string2[] = "NNATCGNNNNNNNNNNNNNNNNNNKKATCGCCNNATC";
	uint8_t number[40] = { 0 };

	uint8_t mode = 0;
	uint64_t info = 0;
	uint64_t len = fc_estream(&mode, &info, string, number, strlen((char*)string1));
	len += fc_estream(&mode, &info, string2, number+len, strlen((char*)string2));
	len += fc_estream_close(&mode, &info, number+len);

	puts("encode: ");
	for(int a=0; a < 40; a++)
		printf("%x, ", number[a]);
	putchar('\n');
	printf("encode return: %lu\n", len);

	mode = info = 0;
	uint8_t out[100] = { 0 };
	uint64_t ret = fc_dstream(&mode, &info, number, out, len-10);
	printf("decode return: %lu\n", ret + fc_dstream(&mode, &info, number+len-10, out+ret, 10));
	printf("decode: %s\n", out);

	return strcmp((char*)string, (char*)out);
}
