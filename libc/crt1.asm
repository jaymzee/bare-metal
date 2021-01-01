%include "x86_64.asm"

	extern CPUExceptionHandler

%macro	traperr	1
	cli
	push	rbp
	mov	rbp, rsp
	pushall	0			; save all regs
	savisf	rsp, rbp+10h		; fault rip, rflags, rsp, etc.
	mov	rax, [rbp]		; get fault frame pointer
	mov	[rsp+reg.rbp], rax	; save it
	mov	rdi, rsp		; pointer to reg struct
	mov	rsi, %1
	mov	rdx, [rbp+8h]		; fault error code
	call	CPUExceptionHandler
	mov	rax, [rsp+reg.rax]
	mov	rcx, [rsp+reg.rcx]
	mov	rdx, [rsp+reg.rdx]
	mov	rsi, [rsp+reg.rsi]
	mov	rdi, [rsp+reg.rdi]
	mov	r8, [rsp+reg.r8]
	mov	r9, [rsp+reg.r9]
	mov	r10, [rsp+reg.r10]
	mov	r11, [rsp+reg.r11]
	leave				; cleanup frame
	add	rsp, 8			; pop error code so rsp -> ret addr
	iretq
%endmacro

%macro	trap	1
	cli
	push	rbp
	mov	rbp, rsp
	pushall	8			; save all regs and align stack
	savisf	rsp, rbp+8		; fault rip, rflags, rsp, etc.
	mov	rdi, rsp		; pointer to regs struct
	mov	rsi, %1
	xor	rdx, rdx		; no error code so set to 0
	call	CPUExceptionHandler
	mov	rax, [rsp+reg.rax]
	mov	rcx, [rsp+reg.rcx]
	mov	rdx, [rsp+reg.rdx]
	mov	rsi, [rsp+reg.rsi]
	mov	rdi, [rsp+reg.rdi]
	mov	r8, [rsp+reg.r8]
	mov	r9, [rsp+reg.r9]
	mov	r10, [rsp+reg.r10]
	mov	r11, [rsp+reg.r11]
	leave				; cleanup frame
	iretq
%endmacro

	bits 64
	default rel
	section .text

; cpu exceptions

	; Divide-by-zero Error: Fault
	global DivbyzeroHandler
DivbyzeroHandler:
	trap	0x0

	; Debug: Fault/Trap
	global DebugHandler
DebugHandler:
	trap	0x1

	; Non-maskable Interrupt: Interrupt
	global NMIHandler
NMIHandler:
	trap	0x2

	; Breakpoint: Trap
	global BreakpointHandler
BreakpointHandler:
	trap	0x3

	; Overflow: Trap
	global OverflowHandler
OverflowHandler:
	trap	0x4

	; Bound Range Exceeded: Fault
	global BoundRangeHandler
BoundRangeHandler:
	trap	0x5

	; Invalid Opcode: Fault
	global InvalidOpcodeHandler
InvalidOpcodeHandler:
	trap	0x6

	; Double Fault: Abort with error code
	global DoubleFaultHandler
DoubleFaultHandler:
	traperr	0x8

	; Invalid TSS: Fault with error code
	global InvalidTSSHandler
InvalidTSSHandler:
	traperr	0xa

	; SegNotPresent: Fault with error code
	global SegNotPresentHandler
SegNotPresentHandler:
	traperr	0xb

	; StackSegFault: Fault with error code
	global StackSegFaultHandler
StackSegFaultHandler:
	traperr	0xc

	; General Protection Fault: Fault with error code
	global GPFaultHandler
GPFaultHandler:
	traperr	0xd

	; Page Fault: Fault with error code
	global PageFaultHandler
PageFaultHandler:
	traperr	0xe

	; x87 Floating-Point Exception: Fault
	global x87FPExceptHandler
x87FPExceptHandler:
	trap	0x10

	; Alignment Check: Fault with error code
	global AlignCheckHandler
AlignCheckHandler:
	traperr	0x11

	; Machine Check: Abort
	global MachineCheckHandler
MachineCheckHandler:
	trap	0x12

	; SIMD Floating-Point Exception: Fault
	global SIMDFPExceptHandler
SIMDFPExceptHandler:
	trap	0x13

	; Virtualization Exception: Fault
	global VirtExceptHandler
VirtExceptHandler:
	trap	0x14

	; Security Exception: - with error
	global SecurityExceptHandler
SecurityExceptHandler:
	traperr	0x1e


; for testing exceptions

	global CauseGPFault
CauseGPFault:
	push	rax
	mov	eax, ds
	push	rax
	mov	ax, 0x42
	mov	ds, eax			; should cause GP Fault
	pop	rax
	mov	ds, eax
	pop	rax
	ret

	global CausePageFault
CausePageFault:
	mov	[abs 0x442], eax	; First page is not mapped to catch
	ret				; null pointer exceptions.

	global CauseDivbyzero
CauseDivbyzero:
	mov	eax, 0
	div	eax
	ret

	global CauseInvalidOpcode
CauseInvalidOpcode:
	ud2
	ret

	global SystemTimerHandler
SystemTimerHandler:
	push	rax
	mov	al, 0x20
	out	0x20, al		; issue EOI
	add	qword [system_time], 16667
	pop	rax
	iretq

	section .data

	global system_time
system_time:
	dq 0

