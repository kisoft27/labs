;INT_N  equ 8   ;���������� ������� (IRQ0)
;INT_N  equ 9   ;���������� ���������� (IRQ1)
INT_N   equ 1Ch ;���������� ������� BIOS - ����������� ������ int 8
 
.186
.model small
.stack 2048
 
.code
 
;----- ������ ----
;(����������� � ������ ���� ��� �������� ������� �� �����������)
 
;����� ����������� ����������� ����������
old_int     label dword
old_int_o   dw ?
old_int_s   dw ?
 
;������ � �������� ��� ������ �� �����
char0       label word
            db '*'
attr0       db 0
 
;----- ���������� ���������� -----
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
 
;----- �������� ��������� -----
entry:
;��������� ���������� ����������� 80x25, 16 ������
    mov ax,3
    int 10h
 
;��������� ������ �������� ����������� ����������   
    mov ax,3500h or INT_N
    int 21h
    mov old_int_o,bx
    mov old_int_s,es
;��������� ������ �����������
    mov ax,2500h or INT_N
    lea dx,int_proc
    push cs
    pop ds
    int 21h
 
;����� ������� ������� Escape
wait_esc:
    mov ah,8
    int 21h
    cmp al,27
    jne wait_esc
 
;������������ ����������� ��������� ����������
    mov ax,2500h or INT_N
    lds dx,old_int
    int 21h
 
;����� �� ��������� 
    mov ax,4C00h
    int 21h
 
end entry