.text

// Inicializo X1 y X2 con valores usando MOVZ
movz X1, 5         // X1 = 5
movz X2, 10        // X2 = 10

// ADDS va a hacer X3 = X1 - X2 (resultado negativo)
subs X3, X1, X2    // X3 = 5 - 10 => -5, N=1, Z=0

// BLE debería saltar porque 5 <= 10
ble salto_ok

// Esto no debería ejecutarse si BLE funciona bien
movz X0, 999

// Salto correcto
salto_ok:
movz X0, 42        // Valor esperado si el salto fue correcto

hlt 0
