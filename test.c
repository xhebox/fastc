#include <stdio.h>
#include "parser.h"

int main() {
	unsigned char string[] = "NNATCGCNYCCCCNNCCCCCCCCNNATCGNNNNNNNNNNNNNNNNNNKKATCGCCNN";
	uint8_t number[40] = { 0 };
	printf("return: %d\n", fc_encode(string, number, strlen(string)));
	printf("%s\n", string);

	unsigned char out[60] = { 0 };
	printf("return: %d\n", fc_decode(number, out, strlen(number)));
	printf("%s\n", out);

	return strcmp(string, out);
}
