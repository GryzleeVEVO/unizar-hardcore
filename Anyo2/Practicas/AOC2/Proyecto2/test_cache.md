
| @MD   | Valor inicial | Valor final (si cambia) | Uso                               |
| ----- | ------------- | ----------------------- | --------------------------------- |
| 0x0   | 0x1000080     |                         | Inicio pila (en scratch)          |
| 0x4   | 0x0100000     |                         | Registro interno MC               |
| 0x10  | 0x58          |                         |
| 0x20  | 0x10          |                         | Nº de registros a guardar en pila |
| 0x40  | 0x40          |                         |                                   |
| 0x44  | 0x44          |                         |                                   |
| 0x58  | 0x44          |                         |                                   |
| 0x88  | 0x0           | 0x40                    |                                   |
| 0x8C  | 0x0           | 0x44                    |                                   |
| 0x9c  | 0x0           | 0x58                    |                                   |
| 0xc8  | 0x0           | 0x01000000              |                                   |
| 0xcc  | 0x0           | 0x100000080             |                                   |
| 0x108 | 0x108         |                         |                                   |
| 0x10c | 0x10c         |                         |                                   |
| 0x138 | 0x138         |                         |                                   |
| 0x148 | 0x148         |                         |                                   |
| 0x14c | 0x14c         |                         |                                   |
| 0x160 | 0x4f          |                         | 'O'                               |
| 0x164 | 0x4b          |                         | 'K'                               |
| 0x180 | 0x0           | 0x108                   |                                   |
| 0x184 | 0x0           | 0x10c                   |                                   |
| 0x1c0 | 0x0           | 0x148                   |                                   |
| 0x1c4 | 0x0           | 0x14c                   |                                   |

---

| @MC | Valor final |
| --- | ----------- |

---

| @Scratch   | Valor final | Uso         |
| ---------- | ----------- | ----------- |
| 0x10000080 | -           | Inicio pila |


---

| Registro | Uso                                |
| -------- | ---------------------------------- |
| r0       | Const. 0, @base                    |
| r30      | @ Registro interno MC              |
| r31      | @ SP (usa @ de la memoria scratch) |


---

; Notación 
; R/W H/M: (Conj/via) <- Tag 

| @     | Codif      | Etiq     | Código               | Comentario                                                    |
| ----- | ---------- | -------- | -------------------- | ------------------------------------------------------------- |
| 0x0   | 0x10210003 | Reset    | beq r1, r1, INI      | Incondicional a INI                                           |
| 0x4   | 0x1021003E | IRQ      | beq r1, r1, RTI      | Incondicional a RTI                                           |
| 0x8   | 0x1021005D | DABort   | beq r1, r1, RT_Abort | Incondicional a rutina Abort                                  |
| 0xc   | 0x1021006C | UNDEF    | beq r1, r1, RT_UNDEF | Incondicional a rutina UNDEF                                  |
| 0x10  | 0x081F0000 | INI      | lw r31, 0(r0)        | r31 = @SP          [RM : (0,0) <- 0]                          |
| 0x14  | 0x081E0004 |          | lw r30, 4(r0)        | r30 = @internal    [rh : (0,0)]                               |
| 0x18  | 0x08010040 |          | lw r1, 40(r0)        | r1 = 0x40          [RM : (0, 1) <- 1]                         |
| 0x1c  | 0x08020044 |          | lw r2, 44(r0)        | r2 = 0x44          [rh : (0,1)]                               |
| 0x20  | 0x0C010088 |          | sw r1, 88(r0)        | @0x88 = 0x40       [WM: (0,0) <- 2]                           |
| 0x24  | 0x0C02008C |          | sw r2, 8c(r0)        | @0x8c = 0x44       [wh: (0,0)]                                |
| 0x28  | 0x0C1F00C8 |          | sw r30, c8(r0)       | @0xc8 = @internal  [WM : (0, 1) <- 3]                         |
| 0x2c  | 0x0C1E00CC |          | sw r31, cc(r0)       | @0xcc = @SP        [wh : (0,1)]                               |
| 0x30  | 0x08010108 |          | lw r1, 108(r0)       | r1 = 0x108         [RM : (0,0) <- 4]                          |
| 0x34  | 0x0802010C |          | lw r2, 10c(r0)       | r2 = 0x10c         [rh : (0,0)]                               |
| 0x38  | 0x08030148 |          | lw r3, 148(r0)       | r3 = 0x148         [RM : (0,1) <- 5]                          |
| 0x3c  | 0x0804014C |          | lw r4, 14c(r0)       | r4 = 0x14c         [rh : (0,1)]                               |
| 0x40  | 0x0C010180 |          | sw r1, 180(r0)       | @0x180 = 0x108     [WM: (0,0) <- 6]                           |
| 0x44  | 0x0C020184 |          | sw r2, 184(r0)       | @0x184 = 0x10c     [wh: (0,0)]                                |
| 0x48  | 0x0C0301c0 |          | sw r3, 1c0(r0)       | @0x1c0 = 0x148     [WM : (0, 1) <- 7]                         |
| 0x4c  | 0x0C0401c4 |          | sw r4, 1c4(r0)       | @0x1c4 = 0x14c     [wh : (0,1)]                               |
| ----- | ---------- | -------- | -------------------- | ------------------------------------------------------------  |
| 0x50  | 0x08010010 | TEST2    | lw r1, 10(r0)        | r1 = 0x58          [RM : (1, 0) <- 0]                         |
| 0x54  | 0x08220000 |          | lw r2, 0(r1)         | r2 = 0x44          [RM : (1, 1) <- 1] 1c * datos + Ant. rs WB |
| 0x58  | 0x04221800 |          | add r3, r1, r2       | r3 = 0x58 + 0x44   1c * datos + Ant rt WB                     |
| 0x5c  | 0x04032000 |          | add r4, r0, r3       | r4 = 0x0 + 0x9c    Ant. rt Mem                                |
| 0x60  | 0x0C810000 |          | sw r1, 0(r4)         | @0x9c = 0x58       [WM : (1, 0) <- 3] rs Mem                  | INT -> Deben realizarse el segundo load y no el add |
| 0x64  | 0x04632800 |          | add r5, r3, r3       | r5 = 2*0x9c =0x138                                            |
| 0x68  | 0x10A50006 |          | beq r5, r5, #5       | Incondicional a TEST4  2c * datos + control                   |
| ----- | ---------- | -------- | -------------------- | ------------------------------------------------------------  |
| 0x80  | 0x08A10000 | TEST4    | lw r1, 0(r5)         | r1 = 0x138                                                    |
| 0x84  | 0x08020131 |          | lw r2, 131(r0)       | DESALINEADO, ABORT  -> IO_output escribe fallo en bloque @130 |
| 0x88  | 0x0C030200 |          | sw r3, 200(r0)       | FUERA DE RANGO, ABORT -> " en bloque @512                     |
| 0x8c  | 0x00000000 |          | NOP                  |                                                               |
| 0x90  | 0x00000000 |          | NOP                  |                                                               |
| 0x94  | 0x00000000 |          | NOP                  |                                                               |
| 0x98  | 0xffffffff |          | ???                  | Excepción UNDEF, bucle infinito                               |
| ----- | ---------- | -------- | -------------------- | ------------------------------------------------------------  |
| 0x100 | 0x0FE10000 | RTI      | sw r1, 0(r31)        | Guarda registros en Scratch                                   |
| 0x104 | 0x0FE20004 |          | sw r2, 4(r31)        |                                                               |
| 0x108 | 0x0FE30008 |          | sw r3, 8(r31)        |                                                               |
| 0x10c | 0x0FE4000c |          | sw r4, c(r31)        |                                                               |
| 0x110 | 0x08010020 |          | lw r1, 20(r0)        | r1 = 0x10 [RM : (2, 0) <- 0]                                  |
| 0x114 | 0x07E1F800 |          | add r31, r31, r1     | SP += 16                                                      |
| 0x118 | 0x08020160 |          | lw r2, 160(r0)       | r2 = 'O'           [RM : (2, 1) <- 5]                         |
| 0x11c | 0x80400000 |          | wro r2               | IO_output = 'O'    2 ciclos det                               |
| 0x120 | 0x08030164 |          | lw r3, 164(r0)       | r3 = 'K'           [rh : (2, 1)]                              |
| 0x124 | 0x80600000 |          | wro r3               | IO_output = 'K'    2 ciclos det                               |
| 0x128 | 0x07E1F801 |          | sub r31, r31, r1     | SP -= 16                                                      |
| 0x12c | 0x0BE10000 |          | lw r1, 0(r31)        | Recupera registros en Scratch                                 |
| 0x130 | 0x0BE20004 |          | lw r2, 4(r31)        |                                                               |
| 0x134 | 0x0BE30008 |          | lw r3, 8(r31)        |                                                               |
| 0x138 | 0x0BE4000c |          | lw r4, c(r31)        |                                                               |
| 0x13c | 0x20000000 |          | rte                  |                                                               |
| ----- | ---------- | -------- | -------------------- | ------------------------------------------------------------  |
| 0x180 | 0x0BC10000 | RT_Abort | lw r1, 0(r30)        | Lee registro interno MC                                       |
|       | 0x80200000 |          | wro r1               | Escribe @ error en registro salida         [2 ciclos parada]  |
|       | 0x20000000 |          | rte                  | Continúa con la ejecución del programa                        |
| ----- | ---------- | -------- | -------------------- | ------------------------------------------------------------  |
| 0x1C0 | 0x1000FFFF | RT_UNDEF | beq r0, r0, RT_UNDEF | Bucle infinto                                                 |


```
; Probar conjunto 2
; Probar IRQ en medio de paradas
; Probar conjunto 3
; Aborts (salta abort -> proc lee @error -> continúa)
```

