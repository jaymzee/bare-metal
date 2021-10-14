; ------------------------------------------------------------------------------
; bootloader puts CPU into long mode
; paging is required in long mode
; only the first 16MB is identity mapped to show proof of concept
; assemble with nasm

STACK64_TOP equ 0x1000000	; top of 16MB memory
LOADADDR    equ 0x40000		; address to load program into
GDT         equ 0x2000
GDT_SIZE    equ gdt.end - gdt
GDTR        equ GDT + GDT_SIZE
PML4T       equ 0x4000
PT          equ 0x8000
FBPTBASE    equ 0x28000

%include "cpumode.asm"
%include "bootutil.asm"
%include "vesa.asm"

	bits 16
	section .text.start exec align=16
	global _start
_start:
	mov	si, message
	call	_print

	call	_load_program
	a20enbio

	call	_init_page_tables
	call	_init_fb_page_tables

	mov	si, gdt
	mov	di, GDT
	gdtcpy	GDT_SIZE
	xor	eax, eax
	mov	[PT], eax	; unmap first page (0x00000000-0x00000FFF)
	mov	[PT+4], eax
	mov	di, 0

	mov	ax, 0x4f02	; SetVesaMode
	mov	bx, [0x2100]	; get VESA mode saved earlier
	int	0x10

	cli			; disable interrupts
	clnmi			; disable NMI
	stlgmd			; set long mode bit
	lgdt	[GDTR]		; load the copied version
	jmp	0x08:start64

; ax, cx, dx  clobbered
_load_program:
	push	es
	push	bx
	mov	ah, 2h		; read sectors from drive
	mov	al, 64		; sector count (32K)
	mov	ch, 0		; cylinder
	mov	dh, 0		; head
	mov	cl, 3		; sector
	mov	dl, 0		; drive a (use 80h for 1st HD)
	mov	bx, LOADADDR >> 4
	mov	es, bx
	mov	bx, LOADADDR & 0xFFFF ; es:bx start address of main program
	int	13h
	pop	bx
	pop	es
	ret

; initialize page tables
; eax, ecx, edx  clobbered
_init_page_tables:
	push	edi
	mov	edi, PML4T	; start of page tables
	mov	cr3, edi
	xor	eax, eax
	cld
	mov	ecx, 512 * 2 * 3
	rep	stosd		; clear page tables
	mov	edi, cr3
	mov	eax, 0x1000	; eax = page size
	mov	edx, edi
	add	edx, 0x1003	; edx -> next page table
	mov	[di], edx	; PML4T[0] -> PDPT
	add	edi, eax
	add	edx, eax
	mov	[di], edx	; PDPT[0] -> PDT
	add	edi, eax
	mov	edx, PT | 3	; point to first page table
	mov	ecx, 8
	xor	ebx, ebx	; nx bit cleared
.pdt	mov	[di], edx	; PDT[n] -> nth PT
	mov	[di+4], ebx	; PDT[n] upper bits
	add	edx, eax
	add	di, 8
	loop	.pdt
	mov	edi, PT		; first page table
	mov	edx, 3		; R/W and Present
	mov	ecx, 4096	; identity map first 16MB
.fillpt	mov	[di], edx	; PT[n] = n*4096 + 3
	add	edx, eax
	add	di, 8
	loop	.fillpt
	pop	edi
	ret

_init_fb_page_tables:
	push	ds
	push	es
	push	edi
	cld
	mov	edx, [0x2400 + VbeModeInfo.physbase]
	mov	ax, FBPTBASE >> 4
	mov	ds, ax
	mov	es, ax
	mov	eax, 0x1000	; page size
	mov	edi, 0		; first page table
	or	edx, 3		; R/W and Present
	mov	cx, 4096	; map 16MB
	mov	ebx, 0x00000000
.fillpt	mov	[di], edx	; PT[n] = n*0x1000 + 3
	mov	[di+4], ebx	; set nx bit, clear upper phys addr bits
	add	edx, eax
	add	edi, 8
	loop	.fillpt
	xor	eax,eax
	mov	ds,ax
	mov	es,ax
	mov	eax, 0x1000		; size of page (4K)
	mov	edx, FBPTBASE | 3	; edx points to first fb page table
	mov	di, PML4T + 0x2040	; edi should now point to PDT[8]
	mov	cx, 8
.pdtfb	mov	[di], edx		; write entry
	add	edx, eax
	add	di, 8
	loop	.pdtfb
	pop	edi
	pop	es
	pop	ds
	ret


; 64 bit code that run in long mode

	bits 64
	default rel
	section .text
start64:
	cld			; Direction flag forward
	mov	eax, 0x10	; 0x10 is flat selector for data
	mov	ds, eax
	mov	es, eax
	mov	fs, eax
	mov	gs, eax
	mov	ss, eax
	mov	esp,STACK64_TOP ; Should set ESP to a usable memory location
				; Stack will be grow down from this location
	mov	ebp, 0		; terminate chain of frame pointers
	mov	rax, LOADADDR
	call	rax		; call main()
				; not expect for it to return but if it does:
	cli			;   disable interrupts
.halt	hlt			;   Halt CPU with infinite loop
	jmp	.halt

	section .data
	align 8
gdt:
	dq 0			; First entry is always the Null Descriptor
	; 4gb flat r/executable code descriptor
.code	dw 0xFFFF		; limit 0:15
	dw 0			; base 0:15
	db 0			; base 16:23
	db 0b10011010		; access P DPL S, flags Ex DC R Ac
	db 0b10101111		; flags Gr Sz L, Limit 16:19
	db 0			; base 24:31
	; 4gb flat r/w data descriptor
.data	dw 0xFFFF		; limit 0:15
	dw 0			; base 0:15
	db 0			; base 16:23
	db 0b10010010		; access P DPL S, flags Ex DC W Ac
	db 0b10101111		; flags Gr Sz L, Limit 16:19
	db 0			; base 24:31
.end:

message:
	db `stage 1\r\n`, 0
