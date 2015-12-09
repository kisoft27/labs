;INT_N  equ 8   ;прерывание таймера (IRQ0)
;INT_N  equ 9   ;прерывание клавиатуры (IRQ1)
INT_N   equ 1Ch         ;прерывание таймера BIOS - программный аналог int 8
Vector_1Ch_ofs  Equ 4*1Ch   ;
Vector_1Ch_seg  Equ 4*1Ch+2 ;
 
.model  small
.386
.stack  2048
 
.code
 
;----- данные ----
;(расположены в секции кода для удобного доступа из обработчика)
 
;адрес предыдущего обработчика прерывани
;old_int     label dword
;old_int_o   dw ?
;old_int_s   dw ?
    Old_isr1C   dd ?
;символ и аттрибут для вывода на экран
    char0   label word
    db  '*'
    attr0   db 0
 
;----- обработчик прерывания -----
int_proc:
    pushf
    call    dword ptr cs:Old_isr1C
    pusha
    push    es
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
    loop    set_attr
    inc ah
    mov attr0,ah
    pop es
    popa
    retf    2
 
;----- основная программа -----
entry:
;установка текстового видеорежима 80x25, 16 цветов
    mov ax,3
    int 10h
 
;получение адреса текущего обработчика прерывани
    push    es
    push    0
    pop es
    push    word ptr es:Vector_1Ch_seg;
    push    word ptr es:Vector_1Ch_ofs
    pop word ptr cs:Old_isr1C   ;
    pop word ptr cs:Old_isr1C+2
;установка нового обработчика
    cli
    push    offset int_proc
    push    cs
    pop word ptr es:Vector_1Ch_seg
    pop word ptr es:Vector_1Ch_ofs
    sti
    pop es
;ждать нажатия клавиши Escape
wait_esc:
    mov ah,8
    int 21h
    cmp al,27
    jne wait_esc
 
;восстановить стандартную обработку прерывани
    mov ax,2500h or INT_N
    lds dx, Old_isr1C
    int 21h
 
;выход из программы
    mov ax,4C00h
    int 21h
 
    end entry