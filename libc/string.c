#include <string.h>

void *
memcpy(void *dest, const void *src, size_t len)
{
    char *d = dest;
    const char *s = src;
    while (len-- > 0) {
        *d++ = *s++;
    }
    return dest;
}

void *
memset(void *dest, int val, size_t len)
{
    char *p = dest;
    while (len-- > 0) {
        *p++ = val;
    }
    return dest;
}

char *
strcpy(char *dest, const char *src)
{
    return memcpy(dest, src, strlen(src) + 1);
}

char *
strncpy(char *dest, const char *src, size_t n)
{
    size_t i;

    for (i = 0; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for (; i < n; i++)
        dest[i] = '\0';
    return dest;
}

char *
strcat(char *dest, const char *src)
{
    strcpy(dest + strlen(dest), src);
    return dest;
}

size_t
strlen(const char *str)
{
    int len = 0;
    while (*str++) {
        len++;
    }
    return len;
}

static void
reverse(char *str, int length)
{
    char *end = str + length - 1;
    while (str < end) {
        char ch = *str;
        *str++ = *end;
        *end-- = ch;
    }
}

char *
itoa(int num, int base, int width, char *str)
{
    int i = 0;
    int isNegative = 0;
    if (num == 0) {
        str[i++] = '0';
        goto finish;
    }
    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10) {
        isNegative = 1;
        num = -num;
    }
    unsigned int unum = num;
    while (unum != 0) {
        unsigned int r = unum % base;
        str[i++] = (r > 9) ? (r - 10) + 'a' : r + '0';
        unum /= base;
    }
    if (isNegative) {
        str[i++] = '-';
    }
finish:
    while (i < width) {
        if (base == 10)
            str[i++] = ' ';
        else
            str[i++] = '0';
    }
    if (base != 10 && width > 0) {
        i = width;
    }
    str[i] = '\0';
    reverse(str, i);
    return str;
}

char *
ltoa(long num, int base, int width, char *str)
{
    int i = 0;
    int isNegative = 0;
    if (num == 0) {
        str[i++] = '0';
        goto finish;
    }
    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10) {
        isNegative = 1;
        num = -num;
    }
    unsigned long unum = num;
    while (unum != 0) {
        unsigned long r = unum % base;
        str[i++] = (r > 9) ? (r - 10) + 'a' : r + '0';
        unum /= base;
    }
    if (isNegative) {
        str[i++] = '-';
    }
finish:
    while (i < width) {
        if (base == 10)
            str[i++] = ' ';
        else
            str[i++] = '0';
    }
    if (base != 10 && width > 0) {
        i = width;
    }
    str[i] = '\0';
    reverse(str, i);
    return str;
}

char *
strtr(char *str, char repl, char with)
{
    for (char *p = str; *p != 0; p++) {
        if (*p == repl) {
            *p = with;
        }
    }
    return str;
}
