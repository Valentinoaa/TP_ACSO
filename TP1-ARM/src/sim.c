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

    if (rd != 31) NEXT_STATE.REGS[rd] = result;

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

    if (rd != 31) NEXT_STATE.REGS[rd] = result;

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

    if (rd != 31) NEXT_STATE.REGS[rd] = result;

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

    if (rd != 31) NEXT_STATE.REGS[rd] = result;

    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void handle_cmp_register(uint32_t instruction) {
    uint32_t rn = (instruction >> 5) & 0x1F;
    uint32_t rm = (instruction >> 16) & 0x1F;

    uint64_t op1 = (rn == 31) ? 0 : CURRENT_STATE.REGS[rn];
    uint64_t op2 = (rm == 31) ? 0 : CURRENT_STATE.REGS[rm];

    uint64_t result = op1 - op2;

    update_flags(result);

    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void handle_hlt(uint32_t instruction) {
    RUN_BIT = 0;
}

void handle_subs_register(uint32_t instruction) {
    uint32_t rd = instruction & 0x1F;
    uint32_t rn = (instruction >> 5) & 0x1F;
    uint32_t rm = (instruction >> 16) & 0x1F;

    uint64_t op1 = (rn == 31) ? 0 : CURRENT_STATE.REGS[rn];
    uint64_t op2 = (rm == 31) ? 0 : CURRENT_STATE.REGS[rm];
    uint64_t result = op1 - op2;

    if (rd != 31) NEXT_STATE.REGS[rd] = result;

    update_flags(result);
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

    if (rd != 31) NEXT_STATE.REGS[rd] = result;

    update_flags(result);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void process_instruction() {
    uint32_t instruction = mem_read_32(CURRENT_STATE.PC);
    uint32_t opcode = (instruction >> 21) & 0x7FF;

    printf("DEBUG: PC=0x%lx, instr=0x%08x, opcode=0x%x\n", CURRENT_STATE.PC, instruction, opcode);

    switch(opcode) {
        // ADD
        case 0x458:
            handle_add_register(instruction);
            break;
        case 0x448:
        case 0x450:
            handle_add_immediate(instruction);
            break;

        case 0x588:
            handle_adds_immediate(instruction);
            break;
        case 0x558:
            handle_adds_register(instruction);
            break;

        case 0x6A8:
        case 0x688:
            handle_subs_register(instruction);
            break;
        case 0x650:
        case 0x648:
            handle_subs_immediate(instruction);
            break;
        case 0x6a2:
            handle_hlt(instruction);
            break;

        default:
            printf("Instrucción no implementada: opcode 0x%x\n", opcode);
            RUN_BIT = 0;
            break;
    }
}

