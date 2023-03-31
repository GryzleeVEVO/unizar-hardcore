        mov     #0, r0      ; r0 = suma
        mov     #71, r1     ; r1 = base
        mov     #6, r4      ; r4 - 6
        mov     #1., r5     ; r5 = 1
        add     r1, r4, r2  ; r2 = base + 6

loop:   beq     r1, r2, #10 ; if base=i = 77 -> exit
        ld      r1, r3      ; r3 = @base
        add     r0, r3, r0  ; suma += @base
        add     r1, r5, r1  ; base++
        beq     r5, r5, #5  ; -> loop

exit:   mov     #ffff, r6   ; r6 = tty
        mov     #0, r7      ; r7 = 0
        ld      r7, r7      ; r7 = 817570 (@0)      
        beq     r0, r7, #20 ; if suma = 817570 -> ok

        mov     #78, r8     ; r8 = 'N'
        st      r6, r8      ; tty = char

        mov     #79, r8     ; r8 = '0'
        st      r6, r8      ; tty = char

        mov     #32, r8     ; r8 = ' '
        st      r6, r8      ; tty = char

ok:     mov     #79, r8     ; r8 = 'O'
        st      r6, r8      ; tty = char

        mov     #75, r8     ; r8 = 'K'
        st      r6, r8      ; tty = char

end:    beq     r5, r5, #24 ; -> end