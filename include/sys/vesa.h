#include <stdint.h>

struct VbeInfo {
    char VbeSignature[4];            // == "VESA"
    uint16_t VbeVersion;             // == 0x0300 for VBE 3.0
    uint16_t OemStringPtr[2];        // isa vbeFarPtr
    uint8_t Capabilities[4];
    uint16_t VideoModePtr[2];        // isa vbeFarPtr
    uint16_t TotalMemory;            // as # of 64KB blocks
    char padding[512 - 20];          // allocation for the remaining data
} __attribute__((packed));

struct VbeModeInfo {
    uint16_t attributes;
    uint8_t winA,winB;
    uint16_t granularity;
    uint16_t winsize;
    uint16_t segmentA, segmentB;
    //VBE_FAR(realFctPtr);
    uint32_t realFctPtr;
    uint16_t pitch; // bytes per scanline

    uint16_t Xres, Yres;
    uint8_t Wchar, Ychar, planes, bpp, banks;
    uint8_t memory_model, bank_size, image_pages;
    uint8_t reserved0;

    uint8_t red_mask, red_position;
    uint8_t green_mask, green_position;
    uint8_t blue_mask, blue_position;
    uint8_t rsv_mask, rsv_position;
    uint8_t directcolor_attributes;

    uint32_t physbase;  // your LFB (Linear Framebuffer) address ;)
    uint32_t reserved1;
    uint16_t reserved2;
    char padding[512 - 50];
} __attribute__((packed));

struct VbeCRTCInfo {
    unsigned short HorizontalTotal;
    unsigned short HorizontalSyncStart;
    unsigned short HorizontalSyncEnd;
    unsigned short VerticalTotal;
    unsigned short VerticalSyncStart;
    unsigned short VerticalSyncEnd;
    unsigned char  Flags;
    unsigned long  PixelClock;      /* units of Hz */
    unsigned short RefreshRate;     /* units of 0.01 Hz */
    unsigned char  reserved[40];
} __attribute__((packed));

static inline uint16_t VBE_GetInfo(struct VbeInfo *vib)
{
    uint16_t result;

    __asm__ __volatile__ (
        "mov $0x4f00,%%ax\n\t"
        "mov %1,%%edi\n\t"
        "int $0x10\n\t"
        "mov %%ax,%0"
        : "=g"(result)
        : ""(vib)
        : "edi", "ax"
    );
    return result;
}

static inline uint16_t VBE_GetModeInfo(struct VbeModeInfo *vmi, uint16_t mode)
{
    uint16_t result;

    __asm__ __volatile__ (
        "mov $0x4f01,%%ax\n\t"
        "mov %2,%%cx\n\t"
        "mov %1,%%edi\n\t"
        "int $0x10\n\t"
        "mov %%ax,%0"
        : "=g"(result)
        : ""(vmi), ""(mode)
        : "edi", "cx", "ax"
    );
    return result;
}

/* VBE_SetVideoMode
 * you should or the mode with 0x4000 to make use of the linear frame buffer
 * you should set bit 11 of mode to instruct the BIOS to use VbeCRTCInfo */
static inline uint16_t VBE_SetVideoMode(struct VbeCRTCInfo *vci, uint16_t mode)
{
    uint16_t result;

    __asm__ __volatile__ (
        "mov $0x4f02,%%ax\n\t"
        "mov %2,%%bx\n\t"
        "mov %1,%%edi\n\t"
        "int $0x10\n\t"
        "mov %%ax,%0"
        : "=g"(result)
        : ""(vci), ""(mode)
        : "edi", "bx", "ax"
    );
    return result;
}
