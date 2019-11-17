; made for nasm
section .text
;this is very important for dos .com files
org 100h
start:
  mov ah, 7h   ;read without echo
  int 21h
  mov ah, 0

  mov dl, al
  mov cl, al
  shr dl, 4

  mov ah, 2

  call printHexDigit

  mov dl, cl
  and dl, 0xF

  call printHexDigit

  ;return 0
  mov al, 00h
  mov ah, 4ch
  int 21h

printHexDigit:    ; dl - digit for printing
  cmp dl, 10
  jge .greater

  add dl, '0'
  int 21h
  ret

.greater:
  add dl, 'A'-10
  int 21h
  ret
