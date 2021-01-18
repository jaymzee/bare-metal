#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/io.h>
#include <sys/serial.h>

uint8_t *ptr = (uint8_t *)0xfd000000;

void map_page_tables(void)
{
    uint64_t *pte;

    pte = 0x10000;
    *pte = 0xfd000000;
}

int main(int argc, char *argv[], char *envp[])
{
    println("Hi resolution graphics in long mode (64-bit) demo");
    while (1) {
        print("\npress a key ");
        int c = getchar();
        print("\nyou pressed: ");
        putchar(c);
    }
}
