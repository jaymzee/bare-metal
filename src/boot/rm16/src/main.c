#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/interrupt.h>
#include <sys/rtc.h>
#include <sys/serial.h>

void Main()
{
    extern int tick_counter;
    char sbuf[80];
    char nbuf[20];
    struct time now;

    COM_Init();
    print("Real time clock demo\n");
    while (1) {
        print("\ntick count: ");
        print(itoa(tick_counter, 10, 0, sbuf));
        print("\npress a key ");
        int c = getchar();
        print("\nyou pressed: ");
        putchar(c);
        switch (c) {
        case 'e':
            PIC_UnmaskIRQ(0);
            print("\nIRQ 0 enabled");
            break;
        case 'd':
            PIC_MaskIRQ(0);
            print("\nIRQ 0 disabled");
            break;
        case 't':
            RTC_GetTime(&now);
            strcat(itoa(now.hour, 10, 2, sbuf), ":");
            strcat(sbuf, itoa(now.minute, 10, 2, nbuf));
            strcat(sbuf, ":");
            strcat(sbuf, itoa(now.second, 10, 2, nbuf));
            print("\nTime is ");
            print(strtr(sbuf, ' ', '0'));
        }
    }
}
