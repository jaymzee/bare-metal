; ------------------------------------------------------------------------------
; second stage bootloader enters 32-bit protected mode
; assemble with nasm

STACK32_TOP equ 0x1000000		; top of 16MB memory
LOADADDR    equ 0x10000
GDT         equ 0x2000
GDT_SIZE    equ gdt.end - gdt
GDTR        equ GDT + GDT_SIZE

%include "cpumode.asm"
%include "sys/bootutil.asm"

	section .text.start exec align=16
	global _start
_start:
	xor	ax, ax
	mov	ds, ax
	mov	es, ax
	mov	ss, ax
	mov	bp, ax
	mov	sp, 0x7c00	; initialize ss:sp and ss:bp
	mov	si, greeting
	call	_print
	call	_load_program
	mov	si, greeting2
	call	_print
	a20enbio
	mov	si, gdt
	mov	di, GDT
	gdtcpy GDT_SIZE
	cli			; disable interrupts
	clnmi			; disable NMI
	stprmd			; set protected mode bit
	lgdt	[GDTR]
	jmp	0x08:start32

	section .text

; load the main program into memory
; ax, cx, dx  clobbered
_load_program:
	push	es
	push	bx
	mov	ah, 2h		; read sectors from drive
	mov	al, 32		; sector count (16K)
	mov	ch, 0		; cylinder
	mov	dh, 0		; head
	mov	cl, 2		; sector
	mov	dl, 0		; drive a (use 80h for 1st HD)
	mov	bx, LOADADDR >> 4
	mov	es, bx
	mov	bx, LOADADDR & 0xFFFF ; start address of main program
	int	13h
	pop	bx
	pop	es
	ret

; 32 bit code that will run in 32-bit protected mode

	bits 32

	section .text
	align 4
start32:
	cld			; Direction flag forward
	mov	eax, 0x10	; 0x10 is flat selector for data
	mov	ds, eax
	mov	es, eax
	mov	fs, eax
	mov	gs, eax
	mov	ss, eax
	mov	esp,STACK32_TOP ; Should set ESP to a usable memory location
				; Stack will be grow down from this location
	mov	ebp, 0		; terminate chain of frame pointers
	call	LOADADDR	; it's not expected for main to ever return
				; but just in case:
	cli			;   disable interrupts
.halt	hlt			;   Halt CPU with infinite loop
	jmp	.halt


	section .data

	align 8
gdt:
	dq 0			; First entry is always the Null Descriptor
.code	equ $ - gdt
	; 4gb flat read/executable code descriptor
	dw 0xFFFF		; limit 0:15
	dw 0			; base 0:15
	db 0			; base 16:23
	db 0b10011010		; access P GPL S, Type Ex DC R Ac
	db 0b11001111		; flags Gr Sz L, Limit 16:19
	db 0			; base 24:31
.data	equ $ - gdt
	; 4gb flat read/write data descriptor
	dw 0xFFFF		; limit 0:15
	dw 0			; base 0:15
	db 0			; base 16:23
	db 0b10010010		; access P GPL S Type Ex DC W Ac
	db 0b11001111		; flags Gr Sz L, Limit 16:19
	db 0			; base 24:31
.end:

greeting:
	db `Loading Program...\r\n`, 0
greeting2:
	db `Entering Protected Mode...\r\n`, 0
