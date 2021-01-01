; 16-bit functions and macros intended to be used in real mode

; i386/x86_64 support

; enter 32-bit protected mode
;   eax  clobbered
%macro stprmd 0
	mov	eax, cr0
	or	eax, 1
	mov	cr0, eax	; Set protected mode flag
%endmacro

; enter long mode (64-bit protected mode)
;   eax, ecx  clobbered
%macro stlgmd 0
.en_pae	mov	eax, cr4
	or	eax, 1 << 5	; enable PAE mode paging
	mov	cr4, eax
.set_lm	mov	ecx, 0xC0000080 ; EFER MSR
	rdmsr
	or	eax, 1 << 8	; set LM-bit in the EFER MSR
	wrmsr
.en_ppr	mov	eax, cr0
	or	eax, 1 << 31 | 1; enable paging and protected mode
	mov	cr0, eax
%endmacro

; copy the gdt so that the memory used by the bootloader
; can be reclaimed for something else
;   ds:si  offset to copy the gdt from
;   es:di  offset to copy the gdt to
%macro gdtcpy 1
	push	bx
	mov	bx,di
	mov	cx,%1
	add	bx,cx
	mov	word [bx],%1-1	; write GDTR limit
	mov	word [bx+2],di	; write GDTR offset bits 15..0
	mov	word [bx+4],0	; write GDTR offset bits 31..16
	rep	movsb
	pop	bx
%endmacro
