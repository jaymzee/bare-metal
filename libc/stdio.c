#include <stdio.h>
#include <sys/conio.h>
#include <sys/serial.h>

static FILE stdin_file = {0};
static FILE stdout_file = {1};
static FILE stderr_file = {2};
static FILE console_file = {3};

FILE *stdin = &stdin_file;
FILE *stdout = &stdout_file;
FILE *stderr = &stderr_file;
FILE *console = &console_file;    // screen

int getc(FILE *stream)
{
    if (stream == stdin) {
        return COM_ReadChar();
    }
    return -1;
}

int getchar(void)
{
    return getc(stdin);
}

int fputc(int c, FILE *stream)
{
    char buf[4];

    if (stream == stdout || stream == stderr) {
        COM_WriteChar(c);
    } else if (stream == console) {
        buf[0] = c;
        buf[1] = '\0';
        WriteText(buf);
    } else {
        return -1;
    }
    return c;
}

int fputs(const char *str, FILE *stream)
{
    if (stream == stdout || stream == stderr) {
        COM_WriteString(str);
    } else if (stream == console) {
        WriteText(str);
    } else {
        return -1;
    }
    return 1;
}

int puts(const char *str)
{
    int rv = fputs(str, stdout);
    fputc(10, stdout);
    return rv;
}

int putchar(int c)
{
    return fputc(c, stdout);
}
