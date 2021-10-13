#ifndef STDLIB_H_INCLUDED
#define STDLIB_H_INCLUDED

#include <stddef.h>

#define NULL 0
#define RAND_MAX 0x7FFFFFFF

int atoi(char *string);
long atol(char *string);
int rand_r(unsigned int *seedp);
void *malloc(size_t nbytes);
void free(void *ptr);

#endif
