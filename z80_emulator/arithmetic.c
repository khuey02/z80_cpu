#include "instructions.h"
#include "cpu_em.h"


// Need to do DAA (given)

/*
	arithmetic.c: Addition, Subtraction, Increment, Decrement, NOP
*/

void ADD_or_ADC(uint8_t operand, struct Z80* cpu){
    uint8_t result_8bit = 0x00;
    uint16_t result_16bit = 0x00;
    uint8_t val = 0x00;
    uint8_t carry = 0x00;

    // ADC opcode carry logic
    if ((operand == 0x88) || (operand == 0x89) || (operand == 0x8A) || (operand == 0x8B) \
        || (operand == 0x8C) || (operand == 0x8D) || (operand == 0x8E) || (operand == 0x8F) \
        || (operand == 0xCE))
    {
        carry = (cpu->status_flags & FLAG_C) ? 1 : 0;    // Adding carry bit if prev operation's carry flag was set
    }

    /* 
        Identify ADD/ADC, get result of sum, update flags, and modify output register 
    */
    // (ADD A, B) or (ADC A, B)
    if ((operand == 0x80) || (operand == 0x88)) {
        result_8bit = cpu->A + cpu->B + carry;
        update_flags_add_8bit(cpu->A, cpu->B, result_8bit, cpu);
        cpu->A = result_8bit;
    }
    // (ADD A, C) or (ADC A, C)
    else if ((operand == 0x81) || (operand == 0x89)) {
        result_8bit = cpu->A + cpu->C + carry;
        update_flags_add_8bit(cpu->A, cpu->C, result_8bit, cpu);
        cpu->A = result_8bit;
    }
    // (ADD A, D) or (ADC A, D)
    else if ((operand == 0x82) || (operand == 0x8A)) {
        result_8bit = cpu->A + cpu->D + carry;
        update_flags_add_8bit(cpu->A, cpu->D, result_8bit, cpu);
        cpu->A = result_8bit;
    }
    // (ADD A, E) or (ADC A, E)
    else if ((operand == 0x83) || (operand == 0x8B)) {
        result_8bit = cpu->A + cpu->E + carry;
        update_flags_add_8bit(cpu->A, cpu->E, result_8bit, cpu);
        cpu->A = result_8bit;
    }
    // (ADD A, H) or (ADC A, H)
    else if ((operand == 0x84) || (operand == 0x8C)) {
        result_8bit = cpu->A + cpu->H + carry;
        update_flags_add_8bit(cpu->A, cpu->H, result_8bit, cpu);
        cpu->A = result_8bit;
    }
    // (ADD A, L) or (ADC A, L)
    else if ((operand == 0x85) || (operand == 0x8D)) {
        result_8bit = cpu->A + cpu->L + carry;
        update_flags_add_8bit(cpu->A, cpu->L, result_8bit, cpu);
        cpu->A = result_8bit;
    }
    // (ADD A, (HL)) or (ADC A, (HL))
    else if ((operand == 0x86) || (operand == 0x8E)) {
        val = z80_mem_read(cpu->HL);
        result_8bit = cpu->A + val + carry;
        update_flags_add_8bit(cpu->A, val, result_8bit, cpu);
        cpu->A = result_8bit;
    }
    // (ADD A, A) or (ADC A, A)
    else if ((operand == 0x87) || (operand == 0x8F)) {
        result_8bit = cpu->A + cpu->A + carry;
        update_flags_add_8bit(cpu->A, cpu->A, result_8bit, cpu);
        cpu->A = result_8bit;
    }
    // (ADD A, N) or (ADC A, N)
    else if ((operand == 0xC6) || (operand == 0xCE)) {
        val = z80_mem_read(cpu->PC + 1);
        cpu->PC++;
        result_8bit = cpu->A + val + carry;
        update_flags_add_16bit(cpu->A, val, result_8bit, cpu);
        cpu->A = result_8bit;
    }
    // ADD HL, BC
    else if (operand == 0x09) {
        result_16bit = cpu->HL + cpu->BC + carry;
        update_flags_add_16bit(cpu->HL, cpu->BC, result_16bit, cpu);
        cpu->HL = result_16bit;
    }
    // ADD HL, DE
    else if (operand == 0x19) {
        result_16bit = cpu->HL + cpu->DE + carry;
        update_flags_add_16bit(cpu->HL, cpu->DE, result_16bit, cpu);
        cpu->HL = result_16bit;
    }
    // ADD HL, HL
    else if (operand == 0x29) {
        result_16bit = cpu->HL + cpu->HL + carry;
        update_flags_add_16bit(cpu->HL, cpu->HL, result_16bit, cpu);
        cpu->HL = result_16bit;
    }
    // ADD HL, SP
    else if (operand == 0x39) {
        result_16bit = cpu->HL + cpu->SP + carry;
        update_flags_add_16bit(cpu->HL, cpu->SP, result_16bit, cpu);
        cpu->HL = result_16bit;
    }
}


void update_flags_add_8bit(uint8_t A, uint8_t B, uint8_t result, struct Z80* cpu){
    // Carry Flag: set if carry from bit 7. If A + B is greater than max 8 bits can represent (255), carry flag set
    if ((A + B) > 0xFF)
        cpu->status_flags |= FLAG_C;     // bitwise or to set bit
    else
        cpu->status_flags &= ~FLAG_C;    // bitwise and not operator to clear bit

    // Add/Subtract flag: unset for addition
    cpu->status_flags &= ~FLAG_N;

    // Parity/Overflow Flag:
    if ((result < A) || (result < B))
        cpu->status_flags |= FLAG_PV;
    else
        cpu->status_flags &= ~FLAG_PV;

    // Half Carry Flag: set if the addition causes a carry from bit 3 to bit 4
    // Isolate the lower 4 bits of A and B then sum and compare to 15 (4 bits)
    if (((A & 0xF) + (B & 0xF)) > 0xF)
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


void update_flags_add_16bit(uint16_t A, uint16_t B, uint16_t result, struct Z80* cpu) {
    // Carry Flag: set if carry from bit 7. If A + B is greater than max 16 bits can represent (65535), carry flag set
    if ((A + B) > 0xFFFF)
        cpu->status_flags |= FLAG_C;     // bitwise or to set bit
    else
        cpu->status_flags &= ~FLAG_C;    // bitwise and not operator to clear bit

    // Add/Subtract flag: unset for addition
    cpu->status_flags &= ~FLAG_N;

    // Parity/Overflow Flag:
    if ((result < A) || (result < B))
        cpu->status_flags |= FLAG_PV;
    else
        cpu->status_flags &= ~FLAG_PV;

    // Half Carry Flag: set if the addition causes a carry from bit 3 to bit 4
    // Isolate the lower 8 bits of A and B then sum and compare to 0xFF (8 bits)
    if (((A & 0xFF) + (B & 0xFF)) > 0xFF)
        cpu->status_flags |= FLAG_H;
    else
        cpu->status_flags &= ~FLAG_H;

    // Zero Flag: set if the result is zero
    if (result == 0x00)
        cpu->status_flags |= FLAG_Z;
    else
        cpu->status_flags &= ~FLAG_Z;

    // Sign Flag:
    if (result & 0x8000)
        cpu->status_flags |= FLAG_S;
    else
        cpu->status_flags &= ~FLAG_S;
}


void SUB_or_SBC(uint8_t operand, struct Z80* cpu) {
    uint8_t result_8bit = 0x00;
    uint8_t val = 0x00;
    uint8_t carry = 0x00;

    // SBC carry logic
    if ((operand == 0x98) || (operand == 0x99) || (operand == 0x9A) || (operand == 0x9B) \
        || (operand == 0x9C) || (operand == 0x9D) || (operand == 0x9E) || (operand == 0x9F) \
        || (operand == 0xDE))
    {
        carry = (cpu->status_flags & FLAG_C) ? 1 : 0;    // If carry bit is set, an extra 1 is not subtracted, else an extra 1 is subtracted
    }

    /*
      Identify SUB/SBC, get result of subtraction, update flags, and modify output register
    */
    // (SUB B) or (SBC A, B)
    if ((operand == 0x90) || (operand == 0x98)) {
        result_8bit = cpu->A - (cpu->B + carry);
        update_flags_sub_8bit(cpu->A, cpu->B, result_8bit, cpu);
        cpu->A = result_8bit;
    }
    // (SUB C) or (SBC A, C)
    else if ((operand == 0x91) || (operand == 0x99)) {
        result_8bit = cpu->A - (cpu->C + carry);
        update_flags_sub_8bit(cpu->A, cpu->C, result_8bit, cpu);
        cpu->A = result_8bit;
    }
    // (SUB D) or (SBC A, D)
    else if ((operand == 0x92) || (operand == 0x9A)) {
        result_8bit = cpu->A - (cpu->D + carry);
        update_flags_sub_8bit(cpu->A, cpu->D, result_8bit, cpu);
        cpu->A = result_8bit;
    }
    // (SUB E) or (SBC A, E)
    else if ((operand == 0x93) || (operand == 0x9B)) {
        result_8bit = cpu->A - (cpu->E + carry);
        update_flags_sub_8bit(cpu->A, cpu->E, result_8bit, cpu);
        cpu->A = result_8bit;
    }
    // (SUB H) or (SBC A, H)
    else if ((operand == 0x94) || (operand == 0x9C)) {
        result_8bit = cpu->A - (cpu->H + carry);
        update_flags_sub_8bit(cpu->A, cpu->H, result_8bit, cpu);
        cpu->A = result_8bit;
    }
    // (SUB L) or (SBC A, L)
    else if ((operand == 0x95) || (operand == 0x9D)) {
        result_8bit = cpu->A - (cpu->L + carry);
        update_flags_sub_8bit(cpu->A, cpu->L, result_8bit, cpu);
        cpu->A = result_8bit;
    }
    // (SUB (HL)) or (SBC A, (HL))
    else if ((operand == 0x96) || (operand == 0x9E)) {
        val = z80_mem_read(cpu->HL);
        result_8bit = cpu->A - (val + carry);
        update_flags_sub_8bit(cpu->A, val, result_8bit, cpu);
        cpu->A = result_8bit;
    }
    // (SUB A) or (SBC A, A)
    else if ((operand == 0x97) || (operand == 0x9F)) {
        result_8bit = cpu->A - (cpu->A + carry);
        update_flags_sub_8bit(cpu->A, cpu->A, result_8bit, cpu);
        cpu->A = result_8bit;
    }
    // (SUB N) or (SBC A, N)
    else if ((operand == 0xD6) || (operand == 0xDE)) {
        val = z80_mem_read(cpu->PC + 1);
        cpu->PC++;
        result_8bit = cpu->A - (val + carry);
        update_flags_sub_8bit(cpu->A, val, result_8bit, cpu);
        cpu->A = result_8bit;
    }
}


void update_flags_sub_8bit(uint8_t A, uint8_t B, uint8_t result, struct Z80* cpu) {
    // Carry Flag: set if A is smaller than B
    if (A < B)
        cpu->status_flags |= FLAG_C;     // bitwise or to set bit
    else
        cpu->status_flags &= ~FLAG_C;    // bitwise and not operator to clear bit

    // Add/Subtract flag: set for subtraction
    cpu->status_flags |= FLAG_N;

    // Parity/Overflow Flag:  set if subtraction causes illogical result
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


void INC(uint8_t operand, struct Z80* cpu) {
    uint8_t result_8bit = 0x00;
    uint16_t result_16bit = 0x00;
    uint8_t val = 0x00;

    /*
        Identify INC, get result of increment, update flags, and modify output register
    */
    // INC A
    if (operand == 0x3C) {
        result_8bit = cpu->A + 1;
        update_flags_inc_8bit(cpu->A, result_8bit, cpu);
        cpu->A += 1;
    }
    // INC B
    else if (operand == 0x04) {
        result_8bit = cpu->B + 1;
        update_flags_inc_8bit(cpu->B, result_8bit, cpu);
        cpu->B += 1;
    }
    // INC C
    else if (operand == 0x0C) {
        result_8bit = cpu->C + 1;
        update_flags_inc_8bit(cpu->C, result_8bit, cpu);
        cpu->C += 1;
    }
    // INC D
    else if (operand == 0x14) {
        result_8bit = cpu->D + 1;
        update_flags_inc_8bit(cpu->D, result_8bit, cpu);
        cpu->D += 1;
    }
    // INC E
    else if (operand == 0x1C) {
        result_8bit = cpu->E + 1;
        update_flags_inc_8bit(cpu->E, result_8bit, cpu);
        cpu->E += 1;
    }
    // INC H
    else if (operand == 0x24) {
        result_8bit = cpu->H + 1;
        update_flags_inc_8bit(cpu->H, result_8bit, cpu);
        cpu->H += 1;
    }
    // INC L
    else if (operand == 0x2C) {
        result_8bit = cpu->L + 1;
        update_flags_inc_8bit(cpu->L, result_8bit, cpu);
        cpu->L += 1;
    }
    // INC (HL)
    else if (operand == 0x34) {
        val = z80_mem_read(cpu->HL) + 1;
        update_flags_inc_8bit(z80_mem_read(cpu->HL), val, cpu);
        z80_mem_write(cpu->HL, val);
    }
    // INC BC
    else if (operand == 0x03) {
        result_16bit = cpu->BC + 1;
        update_flags_inc_16bit(cpu->BC, result_16bit, cpu);
        cpu->BC += 1;
    }
    // INC DE
    else if (operand == 0x13) {
        result_16bit = cpu->DE + 1;
        update_flags_inc_16bit(cpu->DE, result_16bit, cpu);
        cpu->DE += 1;
    }
    // INC HL
    else if (operand == 0x23) {
        result_16bit = cpu->HL + 1;
        update_flags_inc_16bit(cpu->HL, result_16bit, cpu);
        cpu->HL += 1;
    }
    // INC SP
    else if (operand == 0x33) {
        result_16bit = cpu->SP + 1;
        update_flags_inc_16bit(cpu->SP, result_16bit, cpu);
        cpu->SP += 1;
    }
}


void update_flags_inc_8bit(uint8_t A, uint8_t result, struct Z80* cpu) {
    // Carry flag isn't affected

    // Add/Subtract flag: unset for increments
    cpu->status_flags &= ~FLAG_N;

    // Parity/Overflow Flag:
    // Set if register was 127 before operation
    if (A == 0x7F)
        cpu->status_flags |= FLAG_PV;
    else
        cpu->status_flags &= ~FLAG_PV;

    // Half Carry Flag: set if the increment causes a carry from bit 3 to bit 4
    // Isolate lower 4 bits and check if +1 makes it greater than 15 (4 bits)
    if (((A & 0xF) + 0x01) > 0xF)
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


void update_flags_inc_16bit(uint16_t A, uint16_t result, struct Z80* cpu) {
    // Carry flag isn't affected

    // Add/Subtract flag: unset for increments
    cpu->status_flags &= ~FLAG_N;

    // Parity/Overflow Flag:
    // Set if register was 32,767 ((2 to power 15) - 1) before operation
    if (A == 0x7FFF)
        cpu->status_flags |= FLAG_PV;
    else
        cpu->status_flags &= ~FLAG_PV;

    // Half Carry Flag: set if the increment causes a carry from bit 3 to bit 4
    // Isolate lower 4 bits and check if +1 is greater than 256 (8 bits)
    if (((A & 0x100) + 0x01) > 0x100)
        cpu->status_flags |= FLAG_H;
    else
        cpu->status_flags &= ~FLAG_H;

    // Zero Flag: set if the result is zero
    if (result == 0x00)
        cpu->status_flags |= FLAG_Z;
    else
        cpu->status_flags &= ~FLAG_Z;

    // Sign Flag:
    if (result & 0x8000)
        cpu->status_flags |= FLAG_S;
    else
        cpu->status_flags &= ~FLAG_S;
}


void DEC(uint8_t operand, struct Z80* cpu) {
    uint8_t result_8bit = 0x00;
    uint16_t result_16bit = 0x00;
    uint8_t val = 0x00;

    /*
        Identify DEC, get result of decrement, update flags, and modify output register
    */
    // DEC A
    if (operand == 0x3D) {
        result_8bit = cpu->A - 1;
        update_flags_dec_8bit(cpu->A, result_8bit, cpu);
        cpu->A -= 1;
    }
    // DEC B
    else if (operand == 0x05) {
        result_8bit = cpu->B - 1;
        update_flags_dec_8bit(cpu->B, result_8bit, cpu);
        cpu->B -= 1;
    }
    // DEC C
    else if (operand == 0x0D) {
        result_8bit = cpu->C - 1;
        update_flags_dec_8bit(cpu->C, result_8bit, cpu);
        cpu->C -= 1;
    }
    // DEC D
    else if (operand == 0x15) {
        result_8bit = cpu->D - 1;
        update_flags_dec_8bit(cpu->D, result_8bit, cpu);
        cpu->D -= 1;
    }
    // DEC E
    else if (operand == 0x1D) {
        result_8bit = cpu->E - 1;
        update_flags_dec_8bit(cpu->E, result_8bit, cpu);
        cpu->E -= 1;
    }
    // DEC H
    else if (operand == 0x25) {
        result_8bit = cpu->H - 1;
        update_flags_dec_8bit(cpu->H, result_8bit, cpu);
        cpu->H -= 1;
    }
    else if (operand == 0x2D) {
        result_8bit = cpu->L - 1;
        update_flags_dec_8bit(cpu->L, result_8bit, cpu);
        cpu->L -= 1;
    }
    // DEC (HL)
    else if (operand == 0x35) {
        val = z80_mem_read(cpu->HL) - 1;
        update_flags_dec_8bit(z80_mem_read(cpu->HL), val, cpu);
        z80_mem_write(cpu->HL, val);
    }
    // DEC BC
    else if (operand == 0x0B) {
        result_16bit = cpu->BC - 1;
        update_flags_dec_8bit(cpu->BC, result_16bit, cpu);
        cpu->BC -= 1;
    }
    // DEC DE
    else if (operand == 0x1B) {
        result_16bit = cpu->DE - 1;
        update_flags_dec_8bit(cpu->DE, result_16bit, cpu);
        cpu->DE -= 1;
    }
    // DEC HL
    else if (operand == 0x2B) {
        result_16bit = cpu->HL - 1;
        update_flags_dec_8bit(cpu->HL, result_16bit, cpu);
        cpu->HL -= 1;
    }
    // DEC SP
    else if (operand == 0x3B) {
        result_16bit = cpu->SP - 1;
        update_flags_dec_8bit(cpu->SP, result_16bit, cpu);
        cpu->SP -= 1;
    }
}


void update_flags_dec_8bit(uint8_t A, uint8_t result, struct Z80* cpu) {
    // Carry flag isn't affected

    // Add/Subtract flag: set for decrements
    cpu->status_flags |= FLAG_N;

    // Parity/Overflow Flag:
    if (A == 0x00)
        cpu->status_flags |= FLAG_PV;
    else
        cpu->status_flags &= ~FLAG_PV;

    // Half Carry Flag: set if the decrement causes a borrow from bit 4 to 3
    // Isolate the lower 4 bits of A, if A can't decrement 1 without borrowing, 
    // to borrow from the 4th bit the lower 4 bits of A must all be 0
    if ((A & 0xF) == 0x00)
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


void DAA(struct Z80* cpu) {
    uint8_t adjust = 0x00;

    // last operation was addition or lower nibble > 0x09 (carry), adjust
    if ((cpu->status_flags & FLAG_H) || ((cpu->A & 0x0F) > 0x09)) {
        adjust = 0x06;
    }
    // last operation was addition or upper nibble > 0x99 (carry), adjust
    if ((cpu->status_flags & FLAG_C) || (cpu->A > 0x99)) {
        adjust += 0x60;
        cpu->status_flags |= FLAG_C;
    }

    cpu->A += adjust;

    // Zero flag: Set if result is zero
    if (cpu->A == 0x00)
        cpu->status_flags |= FLAG_Z;
    else
        cpu->status_flags &= ~FLAG_Z;

    // Sign flag: Set if result is negative (bit 7 of A)
    if (cpu->A & 0x80)
        cpu->status_flags |= FLAG_S;
    else
        cpu->status_flags &= ~FLAG_S;
}


void NOP(uint8_t operand, struct Z80* cpu) {
    operand += 0x00;
    cpu->halted = 0x00;
    return;
}