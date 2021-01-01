#include <stdint.h>

void *memset(void *dest, int val, size_t len);
void *memcpy(void *dest, const void *src, size_t len);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
size_t strlen(const char *str);
char *itoa(int num, int base, int width, char *buf);
char *ltoa(long num, int base, int width, char *buf);
char *strtr(char *str, char repl, char with);
char *strncpy(char *dest, const char *src, size_t n);
