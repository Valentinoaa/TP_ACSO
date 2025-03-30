#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include "shell.h"


void update_flags(uint64_t result) {
    NEXT_STATE.FLAG_Z = (result == 0);
    NEXT_STATE.FLAG_N = ((int64_t)result < 0);
}

void handle_adds_register(uint32_t instruction) {
    uint32_t rd = instruction & 0x1F;
    uint32_t rn = (instruction >> 5) & 0x1F;
    uint32_t rm = (instruction >> 16) & 0x1F;

    uint64_t op1 = (rn == 31) ? 0 : CURRENT_STATE.REGS[rn];
    uint64_t op2 = (rm == 31) ? 0 : CURRENT_STATE.REGS[rm];
    uint64_t result = op1 + op2;

    NEXT_STATE.REGS[rd] = result;

    update_flags(result);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void handle_adds_immediate(uint32_t instruction) {
    uint32_t rd = instruction & 0x1F;
    uint32_t rn = (instruction >> 5) & 0x1F;
    uint32_t imm12 = (instruction >> 10) & 0xFFF;
    uint32_t sh = (instruction >> 22) & 0x1;

    uint64_t immediate = (sh == 1) ? (imm12 << 12) : imm12;
    uint64_t op1 = (rn == 31) ? 0 : CURRENT_STATE.REGS[rn];
    uint64_t result = op1 + immediate;

    NEXT_STATE.REGS[rd] = result;

    update_flags(result);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void handle_add_register(uint32_t instruction) {
    uint32_t rd = instruction & 0x1F;
    uint32_t rn = (instruction >> 5) & 0x1F;
    uint32_t rm = (instruction >> 16) & 0x1F;

    uint64_t op1 = (rn == 31) ? 0 : CURRENT_STATE.REGS[rn];
    uint64_t op2 = (rm == 31) ? 0 : CURRENT_STATE.REGS[rm];
    uint64_t result = op1 + op2;

    NEXT_STATE.REGS[rd] = result;

    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void handle_add_immediate(uint32_t instruction) {
    uint32_t rd = instruction & 0x1F;
    uint32_t rn = (instruction >> 5) & 0x1F;
    uint32_t imm12 = (instruction >> 10) & 0xFFF;
    uint32_t sh = (instruction >> 22) & 0x1;

    uint64_t immediate = (sh == 1) ? (imm12 << 12) : imm12;
    uint64_t op1 = (rn == 31) ? 0 : CURRENT_STATE.REGS[rn];
    uint64_t result = op1 + immediate;

    NEXT_STATE.REGS[rd] = result;

    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void handle_hlt(uint32_t instruction) {
    RUN_BIT = 0;
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}


void handle_subs_register(uint32_t instruction) {
    uint32_t rd = instruction & 0x1F;
    uint32_t rn = (instruction >> 5) & 0x1F;
    uint32_t rm = (instruction >> 16) & 0x1F;

    uint64_t op1 = (rn == 31) ? 0 : CURRENT_STATE.REGS[rn];
    uint64_t op2 = (rm == 31) ? 0 : CURRENT_STATE.REGS[rm];
    uint64_t result = op1 - op2;

    update_flags(result);

    NEXT_STATE.REGS[rd] = result;

    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}


void handle_subs_immediate(uint32_t instruction) {
    uint32_t rd = instruction & 0x1F;
    uint32_t rn = (instruction >> 5) & 0x1F;
    uint32_t imm12 = (instruction >> 10) & 0xFFF;
    uint32_t sh = (instruction >> 22) & 0x1;

    uint64_t immediate = (sh == 1) ? (imm12 << 12) : imm12;
    uint64_t op1 = (rn == 31) ? 0 : CURRENT_STATE.REGS[rn];
    uint64_t result = op1 - immediate;

    NEXT_STATE.REGS[rd] = result;

    update_flags(result);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void handle_ands_register(uint32_t instruction) {
    uint32_t rd = instruction & 0x1F;
    uint32_t rn = (instruction >> 5) & 0x1F;
    uint32_t rm = (instruction >> 16) & 0x1F;

    uint64_t op1 = (rn == 31) ? 0 : CURRENT_STATE.REGS[rn];
    uint64_t op2 = (rm == 31) ? 0 : CURRENT_STATE.REGS[rm];

    uint64_t result = op1 & op2;

    NEXT_STATE.REGS[rd] = result;

    update_flags(result);

    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void handle_eor_register(uint32_t instruction) {
    uint32_t rd = instruction & 0x1F;
    uint32_t rn = (instruction >> 5) & 0x1F;
    uint32_t rm = (instruction >> 16) & 0x1F;

    uint64_t op1 = (rn == 31) ? 0 : CURRENT_STATE.REGS[rn];
    uint64_t op2 = (rm == 31) ? 0 : CURRENT_STATE.REGS[rm];
    uint64_t result = op1 ^ op2;

    if (rd != 31) {
        NEXT_STATE.REGS[rd] = result;
    }

    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}


void handle_orr_register(uint32_t instruction) {
    uint32_t rd = instruction & 0x1F;
    uint32_t rn = (instruction >> 5) & 0x1F;
    uint32_t rm = (instruction >> 16) & 0x1F;

    uint64_t op1 = (rn == 31) ? 0 : CURRENT_STATE.REGS[rn];
    uint64_t op2 = (rm == 31) ? 0 : CURRENT_STATE.REGS[rm];
    uint64_t result = op1 | op2;

    if (rd != 31) {
        NEXT_STATE.REGS[rd] = result;
    }
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}


void handle_b(uint32_t instruction) {
    int32_t imm26 = instruction & 0x03FFFFFF;  // bits [25:0]
    // Sign-extend 26-bit value to 32 bits
    if (imm26 & 0x02000000) {
        imm26 |= 0xFC000000; // rellenar con unos si el bit 25 es 1
    }
    int64_t offset = ((int64_t)imm26) << 2; // desplazamiento a la izquierda 2 bits

    NEXT_STATE.PC = CURRENT_STATE.PC + offset;
}

void handle_b_cond(uint32_t instruction) {
    int32_t imm19 = (instruction >> 5) & 0x7FFFF; // bits [23:5]
    uint32_t cond = instruction & 0xF;            // bits [3:0]

    if (imm19 & 0x40000) {
        imm19 |= 0xFFF80000; // Sign-extend si bit 18 es 1
    }

    int64_t offset = ((int64_t)imm19) << 2;

    int branch = 0;
    switch (cond) {
        case 0x0: branch = (CURRENT_STATE.FLAG_Z == 1); break;                         // BEQ
        case 0x1: branch = (CURRENT_STATE.FLAG_Z == 0); break;                         // BNE
        case 0xC: branch = (CURRENT_STATE.FLAG_N == CURRENT_STATE.FLAG_Z); break;     // BGE
        case 0xD: branch = (CURRENT_STATE.FLAG_N != CURRENT_STATE.FLAG_Z); break;     // BLT
        case 0xE: branch = (CURRENT_STATE.FLAG_Z == 1 || CURRENT_STATE.FLAG_N != CURRENT_STATE.FLAG_Z); break; // BLE
        case 0xF: branch = (CURRENT_STATE.FLAG_Z == 0 && CURRENT_STATE.FLAG_N == CURRENT_STATE.FLAG_Z); break; // BGT
    }

    NEXT_STATE.PC = CURRENT_STATE.PC + (branch ? offset : 4);
}

void handle_lsl_immediate(uint32_t instruction) {
    uint32_t rd = instruction & 0x1F;           // bits [4:0]
    uint32_t rn = (instruction >> 5) & 0x1F;    // bits [9:5]
    uint32_t immr = (instruction >> 16) & 0x3F; // bits [21:16]
    uint32_t imms = (instruction >> 10) & 0x3F; // bits [15:10]

    // LSL: shamt = (64 - immr) & 0x3F
    uint32_t shamt = (64 - immr) & 0x3F;

    uint64_t value = (rn == 31) ? 0 : CURRENT_STATE.REGS[rn];

    uint64_t result = value << shamt;

    if (rd != 31) {
        NEXT_STATE.REGS[rd] = result;
    }

    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}


void handle_lsr_immediate(uint32_t instruction) {
    uint32_t rd = instruction & 0x1F;           // bits [4:0]
    uint32_t rn = (instruction >> 5) & 0x1F;    // bits [9:5]
    uint32_t immr = (instruction >> 16) & 0x3F; // bits [21:16]
    uint32_t imms = (instruction >> 10) & 0x3F; // bits [15:10]

    // Este caso es válido solo si imms == 63
    if (imms != 63) return;

    uint64_t value = (rn == 31) ? 0 : CURRENT_STATE.REGS[rn];
    uint64_t result = value >> immr;

    if (rd != 31) {
        NEXT_STATE.REGS[rd] = result;
    }

    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}


void handle_movz_immediate(uint32_t instruction) {
    uint32_t rd     =  instruction        & 0x1F;
    uint32_t imm16  = (instruction >> 5)  & 0xFFFF;
    uint32_t hw     = (instruction >> 21) & 0x3;

    uint64_t result = ((uint64_t)imm16) << (16 * hw);

    if (rd != 31) {
        NEXT_STATE.REGS[rd] = result;
    }

    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}


void handle_stur(uint32_t instruction) {
    uint32_t rt = instruction & 0x1F;             // destino
    uint32_t rn = (instruction >> 5) & 0x1F;      // base
    int64_t offset = (instruction >> 12) & 0x1FF; // offset de 9 bits

    // sign-extend si bit 8 es 1 (negativo)
    if (offset & 0x100) offset |= ~0x1FF;

    uint64_t base = (rn == 31) ? 0 : CURRENT_STATE.REGS[rn];
    uint64_t value = (rt == 31) ? 0 : CURRENT_STATE.REGS[rt];

    mem_write_32(base + offset, value & 0xFFFFFFFF);
    mem_write_32(base + offset + 4, (value >> 32) & 0xFFFFFFFF);

    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void handle_sturb(uint32_t instruction) {
    uint32_t rt = instruction & 0x1F;
    uint32_t rn = (instruction >> 5) & 0x1F;
    int64_t offset = (instruction >> 12) & 0x1FF;

    if (offset & 0x100) offset |= ~0x1FF;

    uint64_t base = (rn == 31) ? 0 : CURRENT_STATE.REGS[rn];
    uint64_t address = base + offset;

    uint8_t byte = CURRENT_STATE.REGS[rt] & 0xFF;

    // Alinear la dirección al word de 4 bytes
    uint64_t aligned_addr = address & ~0x3;
    uint32_t word = mem_read_32(aligned_addr);

    // Calcular en qué byte dentro del word escribir
    int byte_shift = (address & 0x3) * 8;

    // Reemplazar solo ese byte
    word = (word & ~(0xFF << byte_shift)) | (byte << byte_shift);
    mem_write_32(aligned_addr, word);

    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}


void handle_sturh(uint32_t instruction) {
    uint32_t rt = instruction & 0x1F;
    uint32_t rn = (instruction >> 5) & 0x1F;
    int64_t offset = (instruction >> 12) & 0x1FF;

    if (offset & 0x100) offset |= ~0x1FF;

    uint64_t base = (rn == 31) ? 0 : CURRENT_STATE.REGS[rn];
    uint16_t half = CURRENT_STATE.REGS[rt] & 0xFFFF;

    uint32_t current = mem_read_32(base + offset);
    current = (current & 0xFFFF0000) | half;
    mem_write_32(base + offset, current);

    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void handle_ldur(uint32_t instruction) {
    uint32_t rt = instruction & 0x1F;
    uint32_t rn = (instruction >> 5) & 0x1F;
    int64_t offset = (instruction >> 12) & 0x1FF;
    if (offset & 0x100) offset |= ~0x1FF;

    uint64_t base = (rn == 31) ? 0 : CURRENT_STATE.REGS[rn];
    uint64_t address = base + offset;

    uint32_t lower = mem_read_32(address);
    uint32_t upper = mem_read_32(address + 4);
    uint64_t value = ((uint64_t)upper << 32) | lower;

    if (rt != 31) NEXT_STATE.REGS[rt] = value;

    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void handle_ldurb(uint32_t instruction) {
    uint32_t rt = instruction & 0x1F;
    uint32_t rn = (instruction >> 5) & 0x1F;
    int64_t offset = (instruction >> 12) & 0x1FF;
    if (offset & 0x100) offset |= ~0x1FF;

    uint64_t base = (rn == 31) ? 0 : CURRENT_STATE.REGS[rn];
    uint64_t address = base + offset;

    uint32_t word = mem_read_32(address);
    uint8_t byte = word & 0xFF;

    if (rt != 31) NEXT_STATE.REGS[rt] = (uint64_t)byte;

    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void handle_ldurh(uint32_t instruction) {
    uint32_t rt = instruction & 0x1F;
    uint32_t rn = (instruction >> 5) & 0x1F;
    int64_t offset = (instruction >> 12) & 0x1FF;
    if (offset & 0x100) offset |= ~0x1FF;

    uint64_t base = (rn == 31) ? 0 : CURRENT_STATE.REGS[rn];
    uint64_t address = base + offset;

    uint32_t word = mem_read_32(address);
    uint16_t half = word & 0xFFFF;

    if (rt != 31) NEXT_STATE.REGS[rt] = (uint64_t)half;

    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void handle_br(uint32_t instruction) {
    uint32_t rn = (instruction >> 5) & 0x1F;
    uint64_t address = (rn == 31) ? 0 : CURRENT_STATE.REGS[rn];
    NEXT_STATE.PC = address;
}

void handle_mul(uint32_t instruction) {
    uint32_t rd = instruction & 0x1F;
    uint32_t rn = (instruction >> 5) & 0x1F;
    uint32_t rm = (instruction >> 16) & 0x1F;

    uint64_t op1 = (rn == 31) ? 0 : CURRENT_STATE.REGS[rn];
    uint64_t op2 = (rm == 31) ? 0 : CURRENT_STATE.REGS[rm];
    uint64_t result = op1 * op2;

    if (rd != 31) {
        NEXT_STATE.REGS[rd] = result;
    }

    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void handle_cbz(uint32_t instruction) {
    uint32_t rn = instruction & 0x1F;
    int32_t imm19 = (instruction >> 5) & 0x7FFFF;

    if (imm19 & (1 << 18)) {
        imm19 |= ~0x7FFFF;
    }

    int64_t offset = ((int64_t)imm19) << 2;

    uint64_t value = (rn == 31) ? 0 : CURRENT_STATE.REGS[rn];
    if (value == 0) {
        NEXT_STATE.PC = CURRENT_STATE.PC + offset;
    } else {
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    }
}

void handle_cbnz(uint32_t instruction) {
    uint32_t rn = instruction & 0x1F;
    int32_t imm19 = (instruction >> 5) & 0x7FFFF;

    if (imm19 & (1 << 18)) {
        imm19 |= ~0x7FFFF;
    }

    int64_t offset = ((int64_t)imm19) << 2;

    uint64_t value = (rn == 31) ? 0 : CURRENT_STATE.REGS[rn];
    if (value != 0) {
        NEXT_STATE.PC = CURRENT_STATE.PC + offset;
    } else {
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    }
}



void process_instruction() {
    uint32_t instruction = mem_read_32(CURRENT_STATE.PC);
    uint32_t opcode = (instruction >> 21);

    if (((instruction >> 26)) == 0x05) {
        handle_b(instruction);
        return;
    }

    if (((instruction >> 24) ) == 0x54) {
        handle_b_cond(instruction);
        return;
    }

    if (((instruction >> 24)) == 0xB4) {
        handle_cbz(instruction);
        return;
    }

    if (((instruction >> 24)) == 0xB5) {
        handle_cbnz(instruction);
        return;
    }

    if (((instruction >> 22) ) == 0x34d) {
        uint32_t imms = (instruction >> 10) & 0x3F;
        if (imms == 0x3F)
            handle_lsr_immediate(instruction);
        else
            handle_lsl_immediate(instruction);
        return;
    }

    switch(opcode) {
        case 0x458:
            handle_add_register(instruction);
            break;
        case 0x448:
        case 0x488:
        case 0x450:
            handle_add_immediate(instruction);
            break;
        case 0x694:
            handle_movz_immediate(instruction);
            break;
        case 0x550:
            handle_orr_register(instruction);
            break;
        case 0x558:
            handle_adds_register(instruction);
            break;
        case 0x588:
            handle_adds_immediate(instruction);
            break;
        case 0x6A8:
        case 0x758:
        case 0x688:
            handle_subs_register(instruction);
            break;
        case 0x650:
            handle_eor_register(instruction);
            break;

        case 0x788:
            handle_subs_immediate(instruction);
            break;

        case 0x6a2:
            handle_hlt(instruction);
            break;

        case 0x750:
            handle_ands_register(instruction);
            break;

        case 0x7C0:
            handle_stur(instruction);
            break;

        case 0x7C2:
            handle_ldur(instruction);
            break;

        case 0x1C0:
            handle_sturb(instruction);
            break;
        case 0x1C2:
            handle_ldurb(instruction);
            break;

        case 0x3C0:
            handle_sturh(instruction);
            break;
        case 0x3C2:
            handle_ldurh(instruction);
            break;

        case 0x6B0:
            handle_br(instruction);
            break;

        case 0x4d8:
            handle_mul(instruction);
            break;

        default:
            printf("Instrucción no implementada: opcode 0x%x\n", opcode);
            RUN_BIT = 0;
            break;
    }
}

