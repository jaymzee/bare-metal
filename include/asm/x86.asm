struc reg
.eax	resd	1	; 0
.ecx	resd	1	; 4
.edx	resd	1	; 8
.ebx	resd	1	; 12
.esp	resd	1	; 16
.ebp	resd	1	; 20
.esi	resd	1	; 24
.edi	resd	1	; 28
.eflags	resd	1	; 32
.eip	resd	1	; 36
.cr0	resd	1	; 40
.cr2	resd	1	; 44
.cr3	resd	1	; 48
.cr4	resd	1	; 52
.es 	resw	1	; 56
.cs 	resw	1	; 58
.ss 	resw	1	; 60
.ds 	resw	1	; 62
.fs 	resw	1	; 64
.gs 	resw	1	; 66
.size	equ	($ + 3) & 0xFFFC	; 4 byte alignment
endstruc

; eax (rax) is clobbered but it is saved first
%macro	savregs	1
	mov	[%1+reg.eax], eax
	mov	[%1+reg.ecx], ecx
	mov	[%1+reg.edx], edx
	mov	[%1+reg.ebx], ebx
	mov	[%1+reg.esp], esp
	mov	[%1+reg.ebp], ebp
	mov	[%1+reg.esi], esi
	mov	[%1+reg.edi], edi
	mov	eax, cr0
	mov	[%1+reg.cr0], eax
	mov	eax, cr2
	mov	[%1+reg.cr2], eax
	mov	eax, cr3
	mov	[%1+reg.cr3], eax
	mov	eax, cr4
	mov	[%1+reg.cr4], eax
	mov	eax, es
	mov	[%1+reg.es], ax
	mov	eax, cs
	mov	[%1+reg.cs], ax
	mov	eax, ss
	mov	[%1+reg.ss], ax
	mov	eax, ds
	mov	[%1+reg.ds], ax
	mov	eax, fs
	mov	[%1+reg.fs], ax
	mov	eax, gs
	mov	[%1+reg.gs], ax
%endmacro

; save interrupt stack frame
;   saves info about the faulting code to register save area
;   ebp is the frame pointer and should be initialized first
;   eax is clobbered
%macro savisf 2
	mov	eax, [%2+0]		; fault rip
	mov	[%1+reg.eip], eax
	mov	ax, [%2+4]		; fault cs
	mov	[%1+reg.cs], ax
	mov	eax, [%2+8]		; fault eflags
	mov	[%1+reg.eflags], eax
%endmacro
