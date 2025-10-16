swap:
    ; setup
    ; qui si prepara lo stack frame standard di una funzione C
    push ebp            ; salva il valore del base pointer del chiamante
    mov ebp, esp        ; imposta ebp come riferimento al frame corrente
    push ebx            ; salva ebx perchè verrà modificato e poi ripristinato prima del ret finale

    ; body
    mov ecx, [ebp+12]   ; ebp+12 è il secondo parametro, ecx = yp
    mov edx, [ebp+8]    ; ecx+8 è il primo parametro, edx = xp
    ; edx contiene l'indirizzo di xp
    ; ecx contiene l'indirizzo di yp
    mov eax, [ecx]      ; eax = *yp (t1)
    mov ebx, [edx]      ; ebx = *xp (t0)
    ; ho caricato in eax il valore puntato da yp
    ; ho caricato in ebx il valore puntato da xp
    mov [edx], eax      ; *xp = eax
    mov [ecx], ebx      ; *yp = ebx
    ; scrive in *xp il contenuto di eax (l'ex *yp)
    ; scrive in *yp il contenuto di ebx (l'ex *xp)

    ; finish
    pop ebx             ; ripristino ebx
    mov esp, ebp        ; ripristina il puntatore dello stack (esp) alla posizione che aveva prima
    pop ebp             ; ripristina il base pointer del chiamante
    ret                 ; ritorna

; swap:
;    pushl %ebp
;    movl %esp,%ebp
;    pushl %ebx
;    movl 12(%ebp),%ecx
;    movl 8(%ebp),%edx
;    movl (%ecx),%eax
;    movl (%edx),%ebx
;    movl %eax,(%edx)
;    movl %ebx,(%ecx)
;    movl -4(%ebp),%ebx
;    movl %ebp,%esp
;    popl %ebp
;    ret