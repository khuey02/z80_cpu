#include "instructions.h"
#include "cpu_em.h"

/*
	logic.c: AND, OR, XOR, and Compare Operations
*/

void AND(uint8_t operand, struct Z80* cpu) {
    uint8_t result = 0x00;
    uint8_t val = 0x00;

    switch (operand) {
        // AND B
        case 0xA0:
            result = cpu->A & cpu->B;
            update_flags_and(result, cpu);
            cpu->A = result;
            break;
        // AND C
        case 0xA1:
            result = cpu->A & cpu->C;
            update_flags_and(result, cpu);
            cpu->A = result;
            break;
        // AND D
        case 0xA2:
            result = cpu->A & cpu->D;
            update_flags_and(result, cpu);
            cpu->A = result;
            break;
        // AND E
        case 0xA3:
            result = cpu->A & cpu->E;
            update_flags_and(result, cpu);
            cpu->A = result;
            break;
        // AND H
        case 0xA4:
            result = cpu->A & cpu->H;
            update_flags_and(result, cpu);
            cpu->A = result;
            break;    
        // AND L
        case 0xA5:
            result = cpu->A & cpu->L;
            update_flags_and(result, cpu);
            cpu->A = result;
            break;
        // AND (HL)
        case 0xA6:
            val = z80_mem_read(cpu->HL);
            result = cpu->A & val;
            update_flags_and(result, cpu);
            cpu->A = result;
            break;
        // AND A
        case 0xA7:
            result = cpu->A & cpu->A;
            update_flags_and(result, cpu);
            cpu->A = result;
            break;
        // AND N
        case 0xE6:
            val = z80_mem_read(cpu->PC + 1);
            cpu->PC++;
            result = cpu->A & val;
            update_flags_and(result, cpu);
            cpu->A = result;
            break;
    }
}

void update_flags_and(uint8_t result, struct Z80* cpu) {
    uint8_t bit_count = 0x00;

    // Carry Flag: unset for and
    cpu->status_flags &= ~FLAG_C;

    // Add/Subtract flag: unset for and
    cpu->status_flags &= ~FLAG_N;

    // Parity/Overflow Flag: reset if overflow, reset otherwise
    // Parity/Overflow Flag: For each bit add 1 if it is set. If total is even, set flag
    bit_count += (result & 0x01) ? 1 : 0;
    bit_count += (result & 0x02) ? 1 : 0;
    bit_count += (result & 0x04) ? 1 : 0;
    bit_count += (result & 0x10) ? 1 : 0;
    bit_count += (result & 0x40) ? 1 : 0;
    bit_count += (result & 0x80) ? 1 : 0;

    if ((bit_count % 0x2) == 0x00)
        cpu->status_flags |= FLAG_PV;
    else
        cpu->status_flags &= ~FLAG_PV;

    // Half Carry Flag: set for and
    cpu->status_flags |= FLAG_H;

    // Zero Flag: set if the result is zero
    if (result == 0x00)
        cpu->status_flags |= FLAG_Z;
    else
        cpu->status_flags &= ~FLAG_Z;

    // Sign Flag:
    if (result & 0x80)
        cpu->status_flags |= FLAG_S;
    else
        cpu->status_flags &= ~FLAG_S;
}


void OR(uint8_t operand, struct Z80* cpu) {
    uint8_t result = 0x00;
    uint8_t val = 0x00;

    switch (operand) {
        // OR B
        case 0xB0:
            result = cpu->A | cpu->B;
            update_flags_or(cpu->A, cpu->B, result, cpu);
            cpu->A = result;
            break;
        // OR C
        case 0xB1:
            result = cpu->A | cpu->C;
            update_flags_or(cpu->A, cpu->C, result, cpu);
            cpu->A = result;
            break;
        // OR D
        case 0xB2:
            result = cpu->A | cpu->D;
            update_flags_or(cpu->A, cpu->D, result, cpu);
            cpu->A = result;
            break;
        // OR E
        case 0xB3:
            result = cpu->A | cpu->E;
            update_flags_or(cpu->A, cpu->E, result, cpu);
            cpu->A = result;
            break;
        // OR H
        case 0xB4:
            result = cpu->A | cpu->H;
            update_flags_or(cpu->A, cpu->H, result, cpu);
            cpu->A = result;
            break;
        // OR L
        case 0xB5:
            result = cpu->A | cpu->L;
            update_flags_or(cpu->A, cpu->L, result, cpu);
            cpu->A = result;
            break;
        // OR (HL)
        case 0xB6:
            val = z80_mem_read(cpu->HL);
            result = cpu->A | val;
            update_flags_or(cpu->A, val, result, cpu);
            cpu->A = result;
            break;
        // OR A
        case 0xB7:
            result = cpu->A | cpu->A;
            update_flags_or(cpu->A, cpu->A, result, cpu);
            cpu->A = result;
            break;
        // OR N
        case 0xF6:
            val = z80_mem_read(cpu->PC + 1);
            cpu->PC++;
            result = cpu->A | val;
            update_flags_or(cpu->A, val, result, cpu);
            cpu->A = result;
            break;
    }
}


void update_flags_or(uint8_t A, uint8_t B, uint8_t result, struct Z80* cpu) {
    // Carry Flag: unset for or
    cpu->status_flags &= ~FLAG_C;

    // Add/Subtract flag: unset for or
    cpu->status_flags &= ~FLAG_N;

    // Parity/Overflow Flag: set if overflow, reset otherwise
    if ((result < A) || (result < B))
        cpu->status_flags |= FLAG_PV;
    else
        cpu->status_flags &= ~FLAG_PV;

    // Half Carry Flag: unset for or
    cpu->status_flags &= ~FLAG_H;

    // Zero Flag: set if the result is zero
    if (result == 0x00)
        cpu->status_flags |= FLAG_Z;
    else
        cpu->status_flags &= ~FLAG_Z;

    // Sign Flag:
    if (result & 0x80)
        cpu->status_flags |= FLAG_S;
    else
        cpu->status_flags &= ~FLAG_S;
}


void XOR(uint8_t operand, struct Z80* cpu) {
    uint8_t result = 0x00;
    uint8_t val = 0x00;

    switch (operand) {
        // XOR B
        case 0xA8:
            result = cpu->A ^ cpu->B;
            update_flags_xor(result, cpu);
            cpu->A = result;
            break;
        // XOR C
        case 0xA9:
            result = cpu->A ^ cpu->C;
            update_flags_xor(result, cpu);
            cpu->A = result;
            break;
        // XOR D
        case 0xAA:
            result = cpu->A ^ cpu->D;
            update_flags_xor(result, cpu);
            cpu->A = result;
            break;
        // XOR E
        case 0xAB:
            result = cpu->A ^ cpu->E;
            update_flags_xor(result, cpu);
            cpu->A = result;
            break;
        // XOR H
        case 0xAC:
            result = cpu->A ^ cpu->H;
            update_flags_xor(result, cpu);
            cpu->A = result;
            break;
        // XOR L
        case 0xAD:
            result = cpu->A ^ cpu->L;
            update_flags_xor(result, cpu);
            cpu->A = result;
            break;
        // XOR (HL)
        case 0xAE:
            val = z80_mem_read(cpu->HL);
            result = cpu->A ^ val;
            update_flags_xor(result, cpu);
            cpu->A = result;
            break;
        // XOR A
        case 0xAF:
            result = cpu->A ^ cpu->A;
            update_flags_xor(result, cpu);
            cpu->A = result;
            break;
        // XOR N
        case 0xEE:
            val = z80_mem_read(cpu->PC + 1);
            cpu->PC++;
            result = cpu->A ^ val;
            update_flags_xor(result, cpu);
            cpu->A = result;
            break;
    }
}


void update_flags_xor(uint8_t result, struct Z80* cpu) {
    uint8_t bit_count = 0x00;

    // Carry Flag: unset for xor
    cpu->status_flags &= ~FLAG_C;

    // Add/Subtract flag: unset for xor
    cpu->status_flags &= ~FLAG_N;

    // Parity/Overflow Flag: For each bit add 1 if it is set. If total is even, set flag
    bit_count += (result & 0x01) ? 1 : 0;
    bit_count += (result & 0x02) ? 1 : 0;
    bit_count += (result & 0x04) ? 1 : 0;
    bit_count += (result & 0x10) ? 1 : 0;
    bit_count += (result & 0x40) ? 1 : 0;
    bit_count += (result & 0x80) ? 1 : 0;

    if ((bit_count % 0x2) == 0x00)
        cpu->status_flags |= FLAG_PV;
    else
        cpu->status_flags &= ~FLAG_PV;

    // Half Carry Flag: unset for xor
    cpu->status_flags &= ~FLAG_H;

    // Zero Flag: set if the result is zero
    if (result == 0x00)
        cpu->status_flags |= FLAG_Z;
    else
        cpu->status_flags &= ~FLAG_Z;

    // Sign Flag:
    if (result & 0x80)
        cpu->status_flags |= FLAG_S;
    else
        cpu->status_flags &= ~FLAG_S;
}


void CP(uint8_t operand, struct Z80* cpu) {
    uint8_t result = 0x00;
    uint8_t val = 0x00;

    switch (operand) {
        // CP B
        case 0xB8:
            result = cpu->A - cpu->B;
            update_flags_cp(cpu->A, cpu->B, result, cpu);
            break;
        // CP C
        case 0xB9:
            result = cpu->A - cpu->C;
            update_flags_cp(cpu->A, cpu->C, result, cpu);
            break;
        // CP D
        case 0xBA:
            result = cpu->A - cpu->D;
            update_flags_cp(cpu->A, cpu->D, result, cpu);
            break;
        // CP E
        case 0xBB:
            result = cpu->A - cpu->E;
            update_flags_cp(cpu->A, cpu->E, result, cpu);
            break;
        // CP H
        case 0xBC:
            result = cpu->A - cpu->H;
            update_flags_cp(cpu->A, cpu->H, result, cpu);
            break;
        // CP L
        case 0xBD:
            result = cpu->A - cpu->L;
            update_flags_cp(cpu->A, cpu->L, result, cpu);
            break;
        // CP (HL)
        case 0xBE:
            val = z80_mem_read(cpu->HL);
            result = cpu->A - val;
            update_flags_cp(cpu->A, val, result, cpu);
            break;
        // CP A
        case 0xBF:
            result = cpu->A - cpu->A;
            update_flags_cp(cpu->A, cpu->A, result, cpu);
            break;
        // CP N
        case 0xFE:
            val = z80_mem_read(cpu->PC + 1);
            cpu->PC++;
            result = cpu->A - val;
            update_flags_cp(cpu->A, val, result, cpu);
            break;
    }
}


void update_flags_cp(uint8_t A, uint8_t B, uint8_t result, struct Z80* cpu) {
    // Carry Flag: set when carry is needed if A is less than B
    if (A < B)
        cpu->status_flags |= FLAG_C;     // bitwise or to set bit
    else
        cpu->status_flags &= ~FLAG_C;    // bitwise and not operator to clear bit

    // Add/Subtract flag: set for cp
    cpu->status_flags |= FLAG_N;

    // Parity/Overflow Flag: set if subtraction causes illogical result
    if (((A ^ result) & (B ^ result) & 0x80) != 0)
        cpu->status_flags |= FLAG_PV;
    else
        cpu->status_flags &= ~FLAG_PV;

    // Half Carry Flag: set if the subtraction causes a borrow from bit 4 to bit 3
    // Isolate the lower 4 bits then does A - B, if A is less than B a borrow occurred
    if ((A & 0xF) < (B & 0xF))
        cpu->status_flags |= FLAG_H;
    else
        cpu->status_flags &= ~FLAG_H;

    // Zero Flag: set if the result is zero
    if (result == 0x00)
        cpu->status_flags |= FLAG_Z;
    else
        cpu->status_flags &= ~FLAG_Z;

    // Sign Flag:
    if (result & 0x80)
        cpu->status_flags |= FLAG_S;
    else
        cpu->status_flags &= ~FLAG_S;
}