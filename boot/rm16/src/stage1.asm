	extern Main

	bits 16

	global _start
_start:
	push	greeting
	call	_print
	cli				; disable interrupts
	mov	dword [0x20], timerISR	; install ISR
	sti				; enable interrupts
	call	0:Main
.L1	jmp .L1				; forever

timerISR:
	inc	dword [tick_counter]
	push	ax
	mov	al, 0x20
	out	0x20, al		; issue EOI
	pop	ax
	iret

; void print(char *str)			; print to screen using BIOS INT 10h
_print:
	push	bp
	mov	bp, sp
	push	bx
	push	si

	mov	si, [bp + 4]		; str
	mov	bx, 0x000f		; page number (and color in gfx mode)
	mov	ah, 0Eh			; teletype output
.L1	lodsb
	cmp	al, 0
	je	.done
	int	10h
	jmp	.L1

.done	pop	si
	pop	bx
	leave
	retn	2

greeting:
	db `switch to serial 0 console\r\n`, 0

	global tick_counter
tick_counter:
	dd 0
