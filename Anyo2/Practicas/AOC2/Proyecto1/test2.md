| @    | Valor inicial | Valor final (si cambia) | Contenido           |
| ---- | ------------- | ----------------------- | ------------------- |
| 0x0  | 0x0           |                         | Constante cero      |
| 0x4  | 0x1           |                         | Constante uno       |
| 0x8  | 0x2           |                         | Constante dos       |
| 0xC  | 0x4           |                         | Constante cuatro    |
| 0x10 | 0x8           |                         | Constante ocho      |
| 0x14 | 0x10          |                         | Constante dieciseis |
| ...  | ...           | ...                     | ...                 |
| 0x20 | 0             | 10                      | Contenidos r7       |
| 0x24 | 0             | 15                      | Contenidos r12      |


Banco de registros

| Registro | Resultado |
| -------- | --------- |
| 1        | 1         |
| 2        | 2         |
| 3        | 4         |
| 4        | 8         |
| 5        | 3         |
| 6        | 7         |
| 7        | 10        |
| 8        | 8         |
| 9        | 8         |
| 10       | 16        |
| 11       | 11        |
| 12       | 15        |
| 13       | 26        |
| 14       | 24        |


; Prueba anticipación entre operaciones aritméticas
08010004    lw r1, 4(r0)    ; r1 = 1                        
08020008    lw r2, 8(r0)    ; r2 = 2
08030008    lw r3, c(r0)    ; r3 = 4
08040008    lw r4, 10(r0)   ; r4 = 8

04222800    add r5, r1, r2  ; r5 = 1 + 2. Sin anticipación
04A33000    add r6, r5, r3  ; r6 = 3 + 4. Anticipa rs MEM
04A63800    add r7, r5, r6  ; r7 = 3 + 7. Anticipa rs WB, rt MEM
04264000    add r8, r1, r6  ; r8 = 1 + 7. Anticipa rt WB.
0C070020    sw r7, 20(r0)   ; Anticipa rt WB

; Prueba anticipación para LW + consumidor
08090010    lw r9, 10(r0)   ; r1 = 8;
04A95800    add r11, r9, r5 ; r11 = 8 + 3. Parada 1 ciclo + anticipación rs WB
05266000    add r12, r9, r6 ; r12 = 8 + 7. Anticipación rs WB 
0C0C0024    sw r12, 24(r0)  ; Anticipa rt MEM

080A0014    lw r10, 14(r0)   ; r10 = 16;
04EA6800    add r13, r7, r10 ; r13 = 10 + 16. Parada 1 ciclo + anticipación rt WB
050A7000    add r14, r8, r10 ; r14 = 8 + 16. Anticipación rt WB

1000FFFF
```c

void main() {
    int a = 1, b = 2, c = 3, d = 4;

    int i = a + b; // = 3
    int j = i + c; // = 7
    int k = i + j; // = 10
    int l = a + j; // = 8

    int e = 8;
    int m = e + i; // = 11
    int n = e + j; // = 15

    int f = 16;
    int o = k + f; // = 26
    int p = l + f; // = 24

    while(1);
}

```