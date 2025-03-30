movz x1, #1
cbnz x1, label
movz x2, #42
hlt 0

label:
    movz x3, #99
    hlt 0