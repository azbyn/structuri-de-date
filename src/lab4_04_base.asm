%macro printCharIntrusive 1
  mov ah, 2h
  mov dl, %1
  int 21h
%endmacro
  
%macro printChar 1
  push ax
  push dx
  mov ah, 2h
  mov dl, %1
  int 21h
  pop dx
  pop ax
%endmacro


%macro stackRestore 0
  mov sp, bp
  pop bp
%endmacro

%macro stackStore 0
  push bp
  mov bp, sp
%endmacro

%macro printNewl 0
  printChar 10
%endmacro
%macro printStr 1
  mov dx, %1
  mov ah, 9
  int 21h
%endmacro


section .data
  msgBase db "base: $"
  msgNum db "n: ", '$'
  msgRes db "res: ", '$'
  msgTrue db "true", '$'
  msgFalse db "false", '$'

;i'll denote register names like so:
; ax 'base' = 0

section .text
;this is very important for dos .com files
org 100h
start:
_start_getBase:
  printStr msgBase
  call getInt
  cmp cx, 2
  jl _start_getBase
  cmp cx, 16
  jg _start_getBase
  push cx
_start_getNum:
  printStr msgNum
  call getInt
  cmp cx, 0xfff
  je _start_getNum
  push cx

  printStr msgRes
  pop ax
  pop bx
  call printInt

  printNewl

  ;return 0
  mov al, 00h
  mov ah, 4ch
  int 21h

; Get a number in base 10 from the keyboard
; Reads all chars until enter is pressed
; if a valid number is inserted, the result is put in cx, and dx is set to 1
; else dx is set to 0
; clobbers ax, bx, cx, dx
getInt:
  stackStore
  mov cx, 0    ; cx 'res' = 0
  ;mov bx, 10   ; bx 'base' = 10
_getInt_begin:
  ;;printChar 'l'
  mov ah, 01h
  int 21h      ; al 'char' = getch()

  mov ah, 0 ;probably useless

  ;printChar 'j'
  cmp al, 0xD   ; if (al == '\r')
  je _getInt_end ;     goto end

  cmp al, 0xA   ; if (al == '\n')
  je _getInt_end ;     goto end
  ;printChar 'A'


  ;cmp ax, '0'
  ;jl _getInt_invalid

  ;cmp ax, '9'
  ;jg _getInt_invalid

  sub ax, '0'        ;
  cmp ax, 9          ; if (unsigned(al 'char' - '0') >= 9)
  ja _getInt_invalid ;     goto invalid

  mov bh, 0
  mov bl, al   ; bl 'lastChar' = al

  mov ax, cx

  mov dx, 10
  ;might clobber dx if result overflows
  mul dx       ; dx::ax = ax * 10

  add ax, bx   ;ax += bx
  mov cx, ax   ; update res

  jmp _getInt_begin

_getInt_invalid:
  printChar 10
  mov cx, 0xffff
  ;ret stackRestore
_getInt_end:
  ;;printChar 'e'
  ;mov dx, 1
  stackRestore
  ret

%ifdef COMMENT
____start:
  printStr msgA

  ;mov ax, 0xfff0                ;125
  mov ax, 1000
  mov bx, 10
  push ax
  call printInt

  ;printNewl

  printStr msgB
  mov ax, 71   ; 125
  push ax
  ;mov bx, 10
  call printInt


  printStr msgRes
  ; printInt(a+b)
  pop ax
  pop cx
  mov bx, 10
  add ax, cx
  call printInt

;return 0
  mov al, 00h
  mov ah, 4ch
  int 21h

%endif

printInt: ;(ax = number, bx = base)
  ;stack store
  push bp
  mov bp, sp

  mov cx, 0

  ; push digits to stack
_divLoop:
  mov dx, 0    ; this is required for div to work properly
  div bx       ; ax /= bx, dx = ax % bx
  add dx, '0'  ; dx += '0'
  push dx      ; stack.push_back(dx)

  ;mov ah, 2h
  ;int 21h
  ;mov ax, cx
  inc cx
  cmp ax, 0
  jnz _divLoop

  ; prepare for writing bytes to stdout
  mov ah, 2h

  
  ;mov dx, sp
  ;sub dx, bp
  ;add dx, 'A'
  ;int 21h
  cmp bx, 10
  jge _printLoopGreater10

_printLoop:
  pop dx
  int 21h

  dec cx
  cmp cx, 0
  jnz _printLoop


  ;stack restore
  mov sp, bp
  pop bp

  ret

_printLoopGreater10:
  pop dx
  cmp dx, '9'
  jle _dontAdd
  add dx, 'A'-10-'0' ; dx += '0'
_dontAdd:
  int 21h

  dec cx
  cmp cx, 0
  jnz _printLoopGreater10



  ;stack restore
  mov sp, bp
  pop bp

  ret
