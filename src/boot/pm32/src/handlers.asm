%include "x86.asm"

	extern GPFaultHandlerM
	extern DivbyzeroHandlerM
	extern KeyboardHandlerM

	bits 32
	section .text

	global GPFaultHandler
GPFaultHandler:
	cli
	push	ebp
	mov	ebp, esp
	sub	esp, reg.size
	savregs	esp
	savisf	esp, ebp+8		; save fault eip, cs, flags etc.
	mov	eax, esp		; save address of reg struct
	push	dword [ebp+4]		; 2nd arg: error code
	push	eax			; 1st arg: pointer to reg struct
	call	GPFaultHandlerM
	add	esp, 8
.halt	cli
	hlt
	jmp .halt
	mov	eax, [esp+reg.eax]
	leave
	add	esp, 4			; pop error code
	iret

	global DivbyzeroHandler
DivbyzeroHandler:
	cli
	sub	esp, reg.size
	savregs	esp
	savisf	esp, esp+reg.size
	push	esp			; pointer to reg struct
	call	DivbyzeroHandlerM
	add	esp, 4
.halt	cli
	hlt
	jmp .halt
	mov	eax, [esp+reg.eax]
	add	esp, reg.size
	iret

	global KeyboardHandler
KeyboardHandler:
	push eax
	push ecx
	push edx
	mov	al, 0x20
	out	0x20, al	; issue EOI
	call	KeyboardHandlerM
	pop	edx
	pop	ecx
	pop	eax
	iret

	global TimerHandler
TimerHandler:
	push	eax
	inc	dword [timer_count]
	mov	al, 0x20
	out	0x20, al	; issue EOI
	pop	eax
	iret

	global CauseGPFault
CauseGPFault:
	push	eax
	mov	eax, 0x42
	mov	ds, eax
	pop	eax
	ret

	global CauseDivbyzeroFault
CauseDivbyzeroFault:
	push	eax
	push	edx
	mov	eax, 0
	div	eax
	pop	edx
	pop	eax
	ret

	section .data

	global timer_count
timer_count:
	dd 0

div_by_zero:
	db "division by zero, cpu halted",0
gp_fault:
	db "GP Fault",0
page_fault:
	db "Page Fault",0
