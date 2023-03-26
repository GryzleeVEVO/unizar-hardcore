; Nota: El diseño tampoco tiene carga en BR en medio ciclo.
; Por tanto, los riesgos de datos son 3-retardados
; Los riesgos de control son 1-retardados

	mov     #0, r0      ; r0 = suma
        mov     #71, r1     ; r1 = base
        mov     #6, r4      ; r4 - 6
        mov     #1, r5     ; r5 = 1
	nop
	nop
        add     r1, r4, r2  ; r2 = base + 6
	nop
	nop
	nop

loop:   beq     r1, r2, #20 ; if base=i = 77 -> exit
	nop
        ld      r1, r3      ; r3 = @base
	nop
	nop
	nop
        add     r0, r3, r0  ; suma += @base
        add     r1, r5, r1  ; base++
        beq     r5, r5, #10 ; -> loop
	nop

exit:   mov     #ffff, r6   ; r6 = tty
        mov     #0, r7      ; r7 = 0
	nop
	nop
	nop
        ld      r7, r7      ; r7 = 817570 (@0)      
	nop
	nop
	nop
        beq     r0, r7, #46 ; if suma = 817570 -> ok
	nop

        mov     #78, r8     ; r8 = 'N'
	nop
	nop
	nop
        st      r6, r8      ; tty = char

        mov     #79, r8     ; r8 = '0'
	nop
	nop
	nop
        st      r6, r8      ; tty = char

        mov     #32, r8     ; r8 = ' '
	nop
	nop
	nop
        st      r6, r8      ; tty = char

ok:     mov     #79, r8     ; r8 = 'O'
	nop
	nop
	nop
        st      r6, r8      ; tty = char

        mov     #75, r8     ; r8 = 'K'
	nop
	nop
	nop
        st      r6, r8      ; tty = char

end:    beq     r5, r5, #56 ; -> end
