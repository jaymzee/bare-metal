; functions/macros intended to be used in 16-bit real mode

; ISA architecture

; disable NMI
;   al  clobbered
%macro clnmi 0
	in	al, 0x70
	or	al, 0x80
	out	0x70, al
%endmacro

; enable NMI
;   al  clobbered
%macro stnmi 0
	in	al, 0x70
	and	al, 0x7f
	out	0x70, al
%endmacro

; Enable A20 using System Control Port A
;   uses fast method as proof of concept and may not work on all hardware
;   ax  clobbered
%macro a20enscp 0
	cli			; disable interrupts
	in	al, 0x92	; Read System Control Port A
	test	al, 0x02	; Test current a20 value (bit 1)
	jnz	%%skip		; If already 1 skip a20 enable
	or	al, 0x02	; Set a20 bit (bit 1) to 1
	and	al, 0xfe	; Always write a zero to bit 0 to avoid
				;     a fast reset into real mode
	out	0x92, al	; Enable a20
%%skip	sti			; reenable interrupts
%endmacro

; Enable A20 using BIOS method
%macro a20enbio 0
	mov	ax,2401h
	int	15h
%endmacro

	bits 16
	section .text

; print to screen using calls to BIOS INT 10h
;   si  pointer to null terminated string
;   ax  clobbered
	global _print
_print:
	push	bx
	cld
	mov	bx, 0x000f	; page number (and color in gfx mode)
	mov	ah, 0Eh		; teletype output
.L1	lodsb
	test	al, al
	je	.break
	int	10h
	jmp	.L1
.break	pop	bx
	ret

