#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/interrupt.h>
#include <sys/rtc.h>
#include <sys/serial.h>

struct VbeInfoBlock {
   char VbeSignature[4];            // == "VESA"
   uint16_t VbeVersion;             // == 0x0300 for VBE 3.0
   uint16_t OemStringPtr[2];        // isa vbeFarPtr
   uint8_t Capabilities[4];
   uint16_t VideoModePtr[2];        // isa vbeFarPtr
   uint16_t TotalMemory;            // as # of 64KB blocks
   char padding[512 - 20];          // allocation for the remaining data
} __attribute__((packed));

uint16_t GetVbeInfo(struct VbeInfoBlock *vib)
{
    uint16_t result;

    __asm__ __volatile__ (
        "mov $0x4f00,%%ax\n\t"
        "mov %1,%%edi\n\t"
        "int $0x10\n\t"
        "mov %%ax,%0"
        : "=g"(result)
        : "Nd"(vib)
        : "edi"
    );
    return result;
}

void ShowVesaModes(void)
{
    char sbuf[80];
    char nbuf[20];
    struct VbeInfoBlock vbe_info;

    print("Querying VESA Video modes INT 0x10, AX=0x4f00\n");
    int result = GetVbeInfo(&vbe_info);

    if (result == 0x004f) { // data in vbe_info is valid
        int total_memory = vbe_info.TotalMemory << 16;
        uint16_t *modes = (uint16_t *)(vbe_info.VideoModePtr[1] << 16 | vbe_info.VideoModePtr[0]);

        strncpy(sbuf, vbe_info.VbeSignature, 4);
        sbuf[4] = 10;
        sbuf[5] = 0;
        print("Signature: ");
        print(sbuf);
        print("Vbe Version: 0x");
        print(itoa(vbe_info.VbeVersion, 16, 4, nbuf));
        print("\n");
        print("Total Memory: ");
        print(itoa(total_memory, 10, 8, nbuf));
        print("\n");
        print("Video Mode Pointer: 0x");
        print(itoa((int)modes, 16, 8, nbuf));
        print("\n");

        print("Modes:\n");
        for (int i = 0; modes[i] != 0xffff; i++) {
            print(itoa(modes[i], 16, 4, nbuf));
            print("\n");
        }
    } else {
        print("query failure, returned 0x");
        print(itoa(result, 16, 4, nbuf));
        print("\n");
    }
}

void Main()
{
    extern int tick_counter;
    char sbuf[80];
    char nbuf[20];
    struct time now;

    COM_Init();
    ShowVesaModes();
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
