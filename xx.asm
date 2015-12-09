;INT_N  equ 8   ;прерывание таймера (IRQ0)
;INT_N  equ 9   ;прерывание клавиатуры (IRQ1)
INT_N   equ 1Ch ;прерывание таймера BIOS - программный аналог int 8
 
.186
.model small
.stack 2048
 
.code
 
;----- данные ----
;(расположены в секции кода для удобного доступа из обработчика)
 
;адрес предыдущего обработчика прерывания
old_int     label dword
old_int_o   dw ?
old_int_s   dw ?
 
;символ и аттрибут для вывода на экран
char0       label word
            db '*'
attr0       db 0
 
;----- обработчик прерывания -----
int_proc:
    pusha
    push es
    mov ax,0B800h
    mov es,ax
    xor di,di
    cld
    mov ax,char0
    mov cx,80
set_attr:
    and ah,0Fh
    stosw
    inc ah
    loop set_attr
    inc ah
    mov attr0,ah
    pop es
    popa
    jmp old_int 
 
;----- основная программа -----
entry:
;установка текстового видеорежима 80x25, 16 цветов
    mov ax,3
    int 10h
 
;получение адреса текущего обработчика прерывания   
    mov ax,3500h or INT_N
    int 21h
    mov old_int_o,bx
    mov old_int_s,es
;установка нового обработчика
    mov ax,2500h or INT_N
    lea dx,int_proc
    push cs
    pop ds
    int 21h
 
;ждать нажатия клавиши Escape
wait_esc:
    mov ah,8
    int 21h
    cmp al,27
    jne wait_esc
 
;восстановить стандартную обработку прерывания
    mov ax,2500h or INT_N
    lds dx,old_int
    int 21h
 
;выход из программы 
    mov ax,4C00h
    int 21h
 
end entry