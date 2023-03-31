## Test 3: IRQ y RTE

Memoria datos:

| @     | Valor inicial | Valor final (si cambia) | Contenido              |
| ----- | ------------- | ----------------------- | ---------------------- |
| 0x0   | 0x100         |                         | sp_init  (inicio pila) |
| 0x4   | 0x0           |                         | Constante cero         |
| 0x8   | 0x1           |                         | Constante uno          |
| 0xC   | 0x8           |                         | Constante ocho       |
| 0x10  | 0x18          |                         | Constante veinticuatro |
| 0x14  | 0x0           | 0x179 (377)             | digito1                |
| 0x18  | 0x1           | 0x262 (610)             | digito2                |
| 0x1C  | 0xf           |                         | iter                   |
| 0x20  | 0x49          |                         | i_char                 |
| 0x24  | 0x21          |                         | bang                   |
| ...   | ...           | ...                     | ...                    |
| 0x100 | 0x0           |                         | Inicio pila            |
| ...   | ...           | ...                     | ...                    |


Memoria instrucciones:

| Etiqueta | @Memoria | Codigo operación | Instr. ensamblador    | Acción                           | Observaciones                          |
| -------- | -------- | ---------------- | --------------------- | -------------------------------- | -------------------------------------- |
| Reset    | 0x0      | 10210003         | beq r1, r1, INI       | Salta a INI (0x10)               | Al recibir reset                       |
| IRQ      | 0x4      | 1021003E         | beq r1, r1, RTI       | Salta a RTI (0x100)              | Al recibir IRQ                         |
| DAbort   | 0x8      | 1021005D         | beq r1, r1, RT_Abort  | Salta a RT_Abort  (0x180)        | AL recibir Data_Abort                  |
| UNDEF    | 0xc      | 1021006C         | beq r1, r1, RT_UNDEF  | Salta a UNDEF (0x1C0)            | Al recibir UNDEF                       |
| INI      | 0x10     | 081F0000         | lw  r31, 0(r0)        | r31 = SP                         | Será seguro interrumpir tras 4 ciclos  |
|          | 0x14     | 0806001C         | lw  r6,  1C(r0)       | r6 = iter                        |                                        |
|          | 0x18     | 08040008         | lw  r4,  8(r0)        | r4 = 1                           |                                        |
|          | 0x1C     | 08050004         | lw  r5,  4(r0)        | r5 = 0 (i)                       |                                        |
| Main     | 0x20     | 10A60008         | beq r5,  r6, end      | Salta a end si i = 15            | 2 ciclos de parada en la 1ª iteración  | <- IRQ en el 1er ciclo de parada, no se hace la parada y se vuelve a LW
|          | 0x24     | 08020018         | lw  r2,  18(r0)       | r2 = digito2                     |                                        |
|          | 0x28     | 80400000         | wro r2                | IO_output <- r2                  | 2 ciclos de parada                     | <- IRQ en el 2o ciclo de parada, no hay datos por la primera parada asi que se vuelve al WRO
|          | 0x2C     | 08010014         | lw  r1,  14(r0)       | r1 = digito1                     |                                        |
|          | 0x30     | 04411800         | add r3,  r2, r1       | r3 = digito1 + digito2           | 1 ciclo parada + anticipación WB rt    | <- IRQ en el 1er ciclo de parada, lo mismo que el primero
|          | 0x34     | 0C030018         | sw  r3,  18(r0)       | Mem(digito2) = digito1 + digito2 |                                        |
|          | 0x38     | 0C020014         | sw  r2,  14(r0)       | Mem(digito1) = digito2           |                                        | <- IRQ durante la instrucción, vuelve al ADD que está en EX (no se anticipa)
|          | 0x3C     | 04A42800         | add r5,  r5, r4       | i++                              |                                        |
|          | 0x40     | 1000FFF7         | beq r0,  r0, Main     | Salto a Main                     | Siempre genera riesgos control         | <- IRQ tras el salto, se vuelve al fetch
| end      | 0x44     | 1000FFFF         | beq r0,  r0, end      | Bucle infinito                   |                                        |
| ...      | ...      | ...              | ...                   | ...                              | ...                                    |
| RTI      | 0x100    | 0FE10000         | sw  r1,  0(r31)       | Guarda registros en pila         |                                        |
|          | 0x104    | 0FE20004         | sw  r2,  4(r31)       |                                  |                                        |
|          | 0x108    | 0FE30008         | sw  r3,  8(r31)       |                                  |                                        |
|          | 0x10C    | 0FE4000C         | sw  r4,  C(r31)       |                                  |                                        |
|          | 0x110    | 0FE50010         | sw  r5,  10(r31)      |                                  |                                        |
|          | 0x114    | 0FE60014         | sw  r6,  14(r31)      |                                  |                                        |
|          | 0x118    | 08010010         | lw  r1,  10(r0)       | r1 = 24                          |                                        |
|          | 0x11C    | 07E1F800         | add r31, r1, r31      | SP = SP + 24                     | 1 ciclo de parada + anticipación WB rs |
|          | 0x120    | 08020020         | lw  r2,  20(r0)       | r2 = 'I'                         |                                        |
|          | 0x124    | 80400000         | wro r2                | IO_output <- r2                  | 2 ciclos de parada                     |
|          | 0x128    | 08030008         | lw  r3,  8(r0)        | r3 = 1                           |                                        |
|          | 0x12C    | 0804000C         | lw  r4,  C(r0)        | r4 = 1                           |                                        |
|          | 0x130    | 04821000         | add r2,  r4, r2       | r2 = 'I' + 8 = 'Q'               | 1 ciclo parada + anticipación WB rs    |
|          | 0x134    | 04431000         | add r2,  r2, r3       | r2 = 'Q' + 1 = 'R'               | Anticipación MEM rs                    |
|          | 0x138    | 80400000         | wro r2                | IO_output <- r2                  | 2 ciclos de parada                     |
|          | 0x13C    | 04431001         | sub r2,  r2, r3       | r2 = 'Q' - 1 = 'Q'               |                                        |
|          | 0x140    | 08050024         | lw  r5,  24(r0)       | r5 = '!'                         |                                        |
|          | 0x144    | 80400000         | wro r2                | IO_output <- r2                  | 1 ciclo de parada                      |
|          | 0x148    | 80A00000         | wro r5                | IO_output <- r5                  | 1 ciclo de parada                      |
|          | 0x14C    | 07E1F801         | sub r31, r31, r1      | SP = SP - 20                     |                                        |
|          | 0x150    | 0BE10000         | lw  r1,  0(r31)       | Recupera registros de pila       |                                        |
|          | 0x154    | 0BE20004         | lw  r2,  4(r31)       |                                  |                                        |
|          | 0x158    | 0BE30008         | lw  r3,  8(r31)       |                                  |                                        |
|          | 0x15C    | 0BE4000C         | lw  r4,  C(r31)       |                                  |                                        |
|          | 0x160    | 0BE50010         | lw  r5,  10(r31)      |                                  |                                        |
|          | 0x164    | 0BE60014         | lw l6, 14(r31)        |
|          | 0x168    | 20000000         | rte                   | Sale de RTI                      |                                        |
| ...      | ...      | ...              | ...                   | ...                              | ...                                    |
| RT_Abort | 0x180    | 1000FFFF         | beq  r0, r0, RT_Abort | Bucle infinito                   |                                        |
| ...      | ...      | ...              | ...                   | ...                              | ...                                    |
| RT_UNDEF | 0x1C0    | 1000FFFF         | beq r0, r0, RT_UNDEF  |

Código alto nivel:

```c        
// Constantes
const int sp_init = 256;
const int zero = 0;
const int one = 1;
const int eight = 8;
const int twentyfour = 24;

// Para calcular fibonacci
int digito1 = 0;
int digito2 = 1;
int iter = 1;

// Para IRQ
const int i_char = 73;  // 0x49 = 'I'
const int bang = 33     // 0x21 = '!'

// Rutina servicio IRQ: Calcula y escribe "IRQ!" como salida
// IRQ! = x049 0x52 0x51 0x21
void RTI(void) __irq 
{
    int c = i_char;
    Print(c);           // 'I'
    c += 4 - 1;
    Print(c);           // 'R'
    c -= 1;
    int c2 = bang;
    Print(c);           // 'Q'
    Print(c2);          // '!'
}

// Cualquier acceso mal alineado es fin de ejecución
void Abort(void) __abort { while(1); }

// Cualquier instrucción indefinida es fin de ejecución
void UNDEF(void) __undef { while(1); }

void main() 
{
    int i = 0;

    while (i < iter) 
    {
        int aux = digito1 + digito2;    // Obtiene el siguiente número de fibonacci
        Print(digito2);                 // Imprime el digito 2
        digito1 = digito2;              // Actualiza variables
        digito2 = aux;              
        i++;                    
    }

    while(1);
}

```

```
Ciclos totales: 
Ciclos (búcle principal, sin interrupciones): 205
Ciclos (cada interrupción):
Riesgos datos generados (búcle principal): 47
Riesgos control (búcle principal): 17
```