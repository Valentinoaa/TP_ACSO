.text

// Cargar valores iniciales para probar
movz X1, 10
movz X2, 20
movz X3, 30
movz X4, 40
movz X5, 50
movz X6, 60
movz X7, 70
movz X8, 80
movz X9, 90
movz X10, 100
movz X11, 110
movz X12, 120

// ADD y ADDS
adds X13, X1, X2       // X13 = 10 + 20
adds X14, X3, 3        // X14 = 30 + 3
// adds X15, X3, 3, LSL #12 // shift == 01: 3 << 12 = 12288, X15 = 30 + 12288

// SUB y SUBS
subs X16, X5, X1       // X16 = 50 - 10
subs X17, X6, 5        // X17 = 60 - 5
// subs X18, X6, 5, LSL #12 // shift == 01: 5 << 12 = 20480

// CMP
cmp X1, X2             // 10 - 20
cmp X2, 10             // 20 - 10

// ANDS, EOR, ORR
ands X19, X1, X2       // 10 & 20
eor X20, X1, X2        // 10 ^ 20
orr X21, X1, X2        // 10 | 20

// Shift LSL y LSR
lsl X22, X1, 2         // 10 << 2 = 40
lsr X23, X4, 2         // 40 >> 2 = 10

// STUR y LDUR
stur X1, [X10, #0x10]   // M[100 + 0x10] = X1
ldur X24, [X10, #0x10]  // X24 = M[100 + 0x10]

// STURB y LDURB (¡OJO con los registros W!)
sturb W1, [X10, #0x20]   // guarda W1(7:0)
ldurb W25, [X10, #0x20]  // carga en W25

// STURH y LDURH
sturh W1, [X10, #0x28]   // guarda W1(15:0)
ldurh W26, [X10, #0x28]  // carga 16 bits en W26


// MUL
mul X27, X1, X2         // 10 * 20

// B (salto incondicional)
b salto1
movz X0, 999           // no se debería ejecutar

salto1:
add X28, X1, X2         // debería ejecutarse

// BR
movz X29, salto2_offset
b salto2_real
movz X0, 888
salto2_real:
add X30, X1, X1         // X30 = 10 + 10

// CBZ y CBNZ
cbz X0, etiqueta_cbz    // X0 es 0 => salta
movz X0, 111            // no se ejecuta
etiqueta_cbz:
cbnz X1, etiqueta_cbnz  // X1 != 0 => salta
movz X0, 222            // no se ejecuta
etiqueta_cbnz:

// B.Cond (BEQ, BNE, etc)
cmp X1, X1              // Z = 1
beq igual
movz X0, 333            // no se ejecuta
igual:

cmp X1, X2              // Z = 0
bne distinto
movz X0, 444            // no se ejecuta
distinto:

cmp X2, X1              // 20 > 10
bgt mayor
movz X0, 555            // no se ejecuta
mayor:

cmp X1, X2              // 10 < 20
blt menor
movz X0, 666            // no se ejecuta
menor:

cmp X1, X1              // 10 == 10
bge mayor_igual
movz X0, 777            // no se ejecuta
mayor_igual:

cmp X1, X2              // 10 <= 20
ble menor_igual
movz X0, 888            // no se ejecuta
menor_igual:

salto2_offset:
add X30, X1, X1

// HLT
hlt 0