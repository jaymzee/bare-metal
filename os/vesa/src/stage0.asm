; ----------------------------------------------------------------------
; floppy disk stage 0 boot loader
; assemble with nasm

%include "sys/bootutil.asm"
%include "sys/vesa.asm"

LOADADDR    equ 0x7e00
VESA_MODE   equ 0x4144

	bits 16
	section .text.start exec align=16
	global _start
_start:
	xor	ax, ax
	mov	ds, ax
	mov	es, ax
	mov	ss, ax
	mov	sp, 0x7c00
	mov	bp, 0		; initialize ss:sp and ss:bp

	mov	si, _greeting0
	call	_print

	mov	WORD [0x2100], VESA_MODE
	mov	di, 0x2200
	VbeGetInfo
	mov	di, 0x2400
	VbeGetModeInfo VESA_MODE

	mov	ah, 2h		; read sectors from drive
	mov	al, 1		; sector count
	mov	ch, 0		; cylinder
	mov	dh, 0		; head
	mov	cl, 2		; sector
	mov	dl, 0		; drive a (use 80h for 1st HD)
	mov	bx, LOADADDR	; start address of next boot loader
	int	13h
	jmp	0:LOADADDR	; jump to 2nd level boot loader

_greeting0:
	db	`stage 0\r\n`, 0
