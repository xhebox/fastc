#ifndef _FASTC_ERR_H
#define _FASTC_ERR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#define ERR(fmt, ...) fprintf(stderr, "\33[31mERR:\33[39m " fmt, ##__VA_ARGS__);
#define INFO(fmt, ...) fprintf(stdout, "\33[32mINFO:\33[39m " fmt, ##__VA_ARGS__);
#define DIE(fmt, ...) do { ERR(fmt, ##__VA_ARGS__); exit(1); } while(0)

#ifdef __cplusplus
}
#endif

#endif
