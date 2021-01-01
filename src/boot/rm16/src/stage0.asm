; ----------------------------------------------------------------------
; floppy disk stage 0 boot loader
; assemble with nasm

LOADADDR equ 0x7e00

	bits 16
	global _start
_start:
	mov	bp, 0		; initialize ss:sp and ss:bp
	mov	ax, 0
	mov	ss, ax
	mov	sp, 0x7c00

	mov	ah, 2h		; read sectors from drive
	mov	al, 32		; sector count
	mov	ch, 0		; cylinder
	mov	dh, 0		; head
	mov	cl, 2		; sector
	mov	dl, 0		; drive a (use 80h for 1st HD)
	mov	bx, LOADADDR	; start address of next boot loader
	int	13h
	jmp	0:LOADADDR	; jump to 2nd level boot loader
