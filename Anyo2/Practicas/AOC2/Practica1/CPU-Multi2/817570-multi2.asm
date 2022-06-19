; Dorian Bolesla Wozniak        817570
; AOC 2
; Programa que suma mediante un bucle el contenido de un vector
; de 6 posiciones.

; Carga los datos necesarios
        mov 70, r0          ; r0 = @base
        mov 0, r1           ; r1 = SUM(Mem(@base + i))
        mov 0, r2           ; r2 = Mem(@base + i)
        mov 6, r3           ; r3 = Iteraciones
        mov 0, r4           ; r4 = i
        mov 1, r5           ; r5 = 1
        mov 0, r6           ; r6 = 0
        mov 0xffff, r8      ; r8 = 0xff
        mov r9, 10          ; r9 = 10
        ld (r9), r7         ; r7 = Mem(@10) (817570)

; Bucle
loop:   add r4, r5, r4      ; i = i + 1
        add r0, r5, r0      ; @base + i = @base + i + 1
        ld (r0), r2         ; r2 = Mem(@base + i + 1)
        add r1, r2, r1      ; r1 = r1 + Mem(@base + i)
        beq r3, r4, out     ; if Iter - i = 0 then PC = loop else PC = PC + 1
        beq r5, r5, loop    ; PC = loop

; Guarda resultado en @0
out:    st r1, (r6)         ; @0 = SUM(Mem(@base + i))

; Comprueba que el número sea 817570
        beq r1, r7, ok      ; if SUM(Mem(@base + i)) - 817570 = 0 then PC = ok else PC= PC + 1

; Pone "NO " si no es así en el teletipo
        mov r9, 0x4E        ; r9 = 'N'
        st  r9, (r8)        ; @0xff = 'N'

        mov r9, 0x4F        ; r9 = 'O'
        st  r9, (r8)        ; @0xff = 'O'

        mov r9, 0x20        ; r9 = ' '
        st  r9, (r8)        ; @0xff = ' '

; Pone "OK" en el teletipo
ok:     mov r9, 0x4F        ; r9 = 'O'
        st  r9, (r8)        ; @0xff = 'O'

        mov r9, 0x4B        ; r9 = 'K'
        st  r9, (r8)        ; @0xff = 'K'

; Fin de programa
fin:    beq r5, r5, fin     ; PC = fin

