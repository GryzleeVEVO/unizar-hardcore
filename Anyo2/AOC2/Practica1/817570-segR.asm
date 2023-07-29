; Nota: El diseño tampoco tiene carga en BR en medio ciclo.
; Por tanto, los riesgos de datos son 3-retardados
; Los riesgos de control son 1-retardados

        mov     #71, r1     ; r1 = base	
        mov     #6, r4      ; r4 - 6
        mov     #0, r7      ; r7 = 0
	mov     #0, r0      ; r0 = suma
        mov     #1, r5      ; r5 = 1
        add     r1, r4, r2  ; r2 = base + 6
	mov     #ffff, r6   ; r6 = tty
        ld      r7, r7      ; r7 = 817570 (@0)      
	nop

loop:   beq     r1, r2, #15 ; if base=i = 77 -> exit
        ld      r1, r3      ; r3 = @base
        add     r1, r5, r1  ; base++
	nop
        beq     r5, r5, #9  ; -> loop
        add     r0, r3, r0  ; suma += @base

exit:   nop 
        beq     r0, r7, #25 ; if suma = 817570 -> ok
	nop

        mov     #78, r8     ; r8 = 'N'
        mov     #79, r8     ; r8 = 'O'
        mov     #32, r8     ; r8 = ' '
	nop
        st      r6, r8      ; tty = char
        st      r6, r8      ; tty = char
        st      r6, r8      ; tty = char

ok:     mov     #79, r8     ; r8 = 'O'
        mov     #75, r8     ; r8 = 'K'
	nop
	nop
        st      r6, r8      ; tty = char
        st      r6, r8      ; tty = char

end:    beq     r5, r5, #32 ; -> end
