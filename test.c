#include <stdio.h>
#include <string.h>
#include "parser.h"

int main() {
	uint8_t string[] = "NNATCGCNYCCCCNNCCCCCCCCNNATCGNNNNNNNNNNNNNNNNNNKKATCGCCNN";
	uint8_t number[40] = { 0 };
	printf("return: %d\n", fc_encode(string, number, strlen((char*)string)));
	printf("%s\n", string);

	uint8_t out[60] = { 0 };
	printf("return: %d\n", fc_decode(number, out, strlen((char*)number)));
	printf("%s\n", out);

	return strcmp((char*)string, (char*)out);
}
