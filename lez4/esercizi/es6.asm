    .global main
    .type main, @function
main:
    pushq   %rbp
    movq    %rsp, %rbp

    movl    $3, %eax    ; imposta valore di ritorno a 3

    popq    %rbp
    ret

; gcc ret3.s -o ret3
; ./ret3
; /bin/echo $?