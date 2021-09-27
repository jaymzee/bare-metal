struc VbeInfo
.VbeSignature	resb	4	; "VESA"
.VbeVersion	resw	1	; 0x0300 for VBE 3.0
.OemStringPtr	resw	2	; isa vbeFarPtr
.Capabilities	resb	4
.VideoModePtr	resw	2	; isa vbeFarPtr
.TotalMemory	resw	2	; # of 64KB blocks
.padding	resb	512-20
endstruc

struc VbeModeInfo
.attributes	resw	1
.winA		resb	1
.winB		resb	1
.granularity	resw	1
.winsize	resw	1
.segmentA	resw	1
.segmentB	resw	1
.realFctPtr	resd	1
.pitch		resw	1	; bytes per scanline

.Xres		resw	1
.Yres		resw	1
.Wchar		resb	1
.Ychar		resb	1
.planes		resb	1
.bpp		resb	1
.banks		resb	1
.memory_model	resb	1
.bank_size	resb	1
.image_pages	resb	1
.reserved0	resb	1

.red_mask	resb	1
.red_position	resb	1
.green_mask	resb	1
.green_position	resb	1
.blue_mask	resb	1
.blue_position	resb	1
.rsv_mask	resb	1
.rsv_position	resb	1
.dircolorattrs	resb	1

.physbase	resd	1	; LFB (linear framebuffer) address
.reserved1	resd	1
.reserved2	resw	1
.padding	resb	512-50
endstruc

struc VbeCRTCInfo
.HorizTotal	resw	1
.HorizSyncStart	resw	1
.HorizSyncEnd	resw	1
.VertTotal	resw	1
.VertSyncStart	resw	1
.VertSyncEnd	resw	1
.Flags		resb	1
.PixelClock	resd	1	; units of Hz
.RefreshRate	resw	1	; units of 0.01 Hz
.reserved	resb	40
endstruc

; edi points to VbeInfo
; return value in ax
%macro VBE_GetInfo 0
	mov	ax, 0x4f00
	int	0x10
%endmacro

; edi points to VbeModeInfo
; cx is clobbered
; return value in ax
%macro VBE_GetModeInfo 1
	mov	ax, 0x4f01
	mov	cx, %1
	int	0x10
%endmacro

; VBE_SetVideoMode
; you should or the mode with 0x4000 to make use of the linear frame buffer
; you should set bit 11 of mode to instruct the BIOS to use VbeCRTCInfo
; edi points to VbeCRTCInfo
; bx is clobbered
; return value in ax
%macro VBE_SetVideoMode 1
	mov	ax, 0x4f02
	mov	bx, %1
	int	0x10
%endmacro

%if 0

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
%endif
