
def generar_input_phase4(expected: str) -> str:

    table = [
        0x65, 0x67, 0x6d, 0x63,
        0x66, 0x61, 0x69, 0x6a,
        0x6f, 0x70, 0x6e, 0x68,
        0x64, 0x62, 0x6b, 0x6c
    ]
    inv_map = {chr(b): i for i, b in enumerate(table)}

    result = ""
    for ch in expected:

        nibble = inv_map[ch]
        # Si nibble < 10 → dígito '0'+nibble; si ≥10 → letra '`'+nibble
        if nibble < 10:
            result += chr(ord('0') + nibble)
        else:
            result += chr(0x60 + nibble)
    return result

if __name__ == "__main__":
    # Cadena esperada obtenida con `x/s 0x4c709f` en GDB
    expected_output = "depila"
    input_phase4 = generar_input_phase4(expected_output)
    print(f"Input para phase_4: {input_phase4}")
