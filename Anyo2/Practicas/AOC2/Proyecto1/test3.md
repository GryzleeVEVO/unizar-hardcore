## Test 3







; BEQ lw 1 ciclo 2 ciclos
lw r1, 0(r0)
lw r2, 4(r1)
add r3, r1 r2;
beq r1, r2, INI          1 ciclo no toma salto

lw r3, 8(r1)            2 ciclos toma salto
beq r3, r3, next


; WRO ARIT 1 ciclo 2 ciclos
add r3, r1, r2
wro r3                  2 ciclos

add r4, r1, r3
add r5, r4, r2
wro r4                  1 ciclo

; BEQ arit 1 ciclo 2 ciclos
add r6, r1, r2
add r7, r3, r4
beq r6, r1, INI         1 ciclo, no salta

add r8, r1, r2
beq r8, r8, end       2 ciclos, salta a fin


; WRO lw 1 ciclo 2 ciclos
lw r1, 0(r0)
wro r1          2 ciclos

lw r2, 4(r0)
lw r3, 8(r0)

wro r2          1 ciclo


beq r0, r0, end




