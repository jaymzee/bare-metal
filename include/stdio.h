#define print(s)    fputs(s, stdout)
#define println(s)  puts(s)

typedef struct FILE {
    int FILENO;
} FILE;

int fputc(int c, FILE *stream);
int fputs(const char *str, FILE *stream);
int puts(const char *str);
int getchar(void);
int putchar(int c);

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;
extern FILE *console;
