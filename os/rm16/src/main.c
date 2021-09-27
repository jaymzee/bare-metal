#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/interrupt.h>
#include <sys/rtc.h>
#include <sys/serial.h>
#include <sys/vesa.h>

void ShowVesaModes(void)
{
    char sbuf[80];
    char nbuf[20];
    struct VbeInfo vbe_info;

    print("Querying VESA Video modes INT 0x10, AX=0x4f00\n");
    int gi_status = VBE_GetInfo(&vbe_info);

    if (gi_status == 0x004f) { // data in vbe_info is valid
        int total_memory = vbe_info.TotalMemory << 16;
        uint16_t *modes = (uint16_t *)(vbe_info.VideoModePtr[1] << 16 | vbe_info.VideoModePtr[0]);

        strncpy(sbuf, vbe_info.VbeSignature, 4);
        sbuf[4] = 0;
        print("Signature: ");
        puts(sbuf);
        print("Vbe Version: 0x");
        puts(itoa(vbe_info.VbeVersion, 16, 4, nbuf));
        print("Total Memory: ");
        puts(itoa(total_memory, 10, 8, nbuf));
        print("Video Mode Pointer: 0x");
        puts(itoa((int)modes, 16, 8, nbuf));

        print("Modes:\n");
        for (int i = 0; modes[i] != 0xffff; i++) {
            struct VbeModeInfo inf;
            int gmi_status = VBE_GetModeInfo(&inf, modes[i]);
            if (gmi_status != 0x004f)
                continue;

            // Check if this is a graphics mode with linear frame buffer support
            if ((inf.attributes & 0x90) != 0x90)
                continue;

            // Check if this is a packed pixel or direct color mode
            if ( inf.memory_model != 4 && inf.memory_model != 6 )
                continue;

            print("mode: 0x");
            print(itoa(modes[i], 16, 3, nbuf));
            print("  res: ");
            print(itoa(inf.Xres, 10, 4, nbuf));
            print(" x ");
            print(itoa(inf.Yres, 10, 4, nbuf));
            print(" x ");
            print(itoa(inf.bpp, 10, 2, nbuf));
            print("  fbuf: 0x");
            print(itoa(inf.physbase, 16, 8, nbuf));
            if (inf.memory_model == 4) {
                print("  packed-pixel");
            } else {
                print("  direct-color");
            }
            print("\n");

            if (i % 16 == 4) {
                getchar();
            }
        }
    } else {
        print("VBE GetInfo failed, returned 0x");
        puts(itoa(gi_status, 16, 4, nbuf));
    }
}

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
            break;
        case 'v':
            ShowVesaModes();
            break;
        case 's':
            VBE_SetVideoMode(NULL, 0x4000 | 0x118);
            break;
        }
    }
}
