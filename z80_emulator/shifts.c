#include "instructions.h"
#include "cpu_em.h"

/*
    shifts.c: Shifts Left Unsigned, Right Signed, Right Unsigned
*/

void SLA(uint8_t operand, struct Z80* cpu) {
    uint8_t result = 0x00;
    uint8_t val = 0x00;

    switch (operand) {
        // SLA B
        case 0x20:
            result = cpu->B << 1;
            update_flags_sla(cpu->B, result, cpu);
            cpu->B = result;
            break;
        // SLA C
        case 0x21:
            result = cpu->C << 1;
            update_flags_sla(cpu->C, result, cpu);
            cpu->C = result;
            break;
        // SLA D
        case 0x22:
            result = cpu->D << 1;
            update_flags_sla(cpu->D, result, cpu);
            cpu->D = result;
            break;
        // SLA E
        case 0x23:
            result = cpu->E << 1;
            update_flags_sla(cpu->E, result, cpu);
            cpu->E = result;
            break;
        // SLA H
        case 0x24:
            result = cpu->H << 1;
            update_flags_sla(cpu->H, result, cpu);
            cpu->H = result;
            break;
        // SLA L
        case 0x25:
            result = cpu->L << 1;
            update_flags_sla(cpu->L, result, cpu);
            cpu->L = result;
            break;
        // SLA (HL)
        case 0x26:
            val = z80_mem_read(cpu->HL);
            result = val << 1;
            update_flags_sla(val, result, cpu);
            z80_mem_write(cpu->HL, result);
            break;
        // SLA A
        case 0x27:
            result = cpu->A << 1;
            update_flags_sla(cpu->A, result, cpu);
            cpu->A = result;
            break;
    }
}


void update_flags_sla(uint8_t A, uint8_t result, struct Z80* cpu) {
    uint8_t bit_count = 0x00;

    // Carry Flag: set carry flag to whatever the 7th bit was
    if(A & 0x80)
        cpu->status_flags |= FLAG_C;     // bitwise or to set bit
    else
        cpu->status_flags &= ~FLAG_C;    // bitwise and not operator to clear bit

    // Add/Subtract flag: unset for sla
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

    // Half Carry Flag: unset for sla
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


void SLL(uint8_t operand, struct Z80* cpu) {
    uint8_t result = 0x00;
    uint8_t val = 0x00;

    switch (operand) {
    // SLL B
    case 0x30:
        result = (cpu->B << 1) | 0x01;
        update_flags_sll(cpu->B, result, cpu);
        cpu->B = result;
        break;
     // SLL C
    case 0x31:
        result = (cpu->C << 1) | 0x01;
        update_flags_sll(cpu->C, result, cpu);
        cpu->C = result;
        break;
    // SLL D
    case 0x32:
        result = (cpu->D << 1) | 0x01;
        update_flags_sll(cpu->D, result, cpu);
        cpu->D = result;
        break;
    // SLL E
    case 0x33:
        result = (cpu->E << 1) | 0x01;
        update_flags_sll(cpu->E, result, cpu);
        cpu->E = result;
        break;
    // SLL H
    case 0x34:
        result = (cpu->H << 1) | 0x01;
        update_flags_sll(cpu->H, result, cpu);
        cpu->H = result;
        break;
    // SLL L
    case 0x35:
        result = (cpu->L << 1) | 0x01;
        update_flags_sll(cpu->L, result, cpu);
        cpu->L = result;
        break;
    // SLL (HL)
    case 0x36:
        val = z80_mem_read(cpu->HL);
        result = (val << 1) | 0x01;
        update_flags_sll(val, result, cpu);
        z80_mem_write(cpu->HL, result);
        break;
    // SLL A
    case 0x37:
        result = (cpu->A << 1) | 0x01;
        update_flags_sll(cpu->A, result, cpu);
        cpu->A = result;
        break;
    }
}


void update_flags_sll(uint8_t A, uint8_t result, struct Z80* cpu) {
    uint8_t bit_count = 0x00;

    // Carry Flag: set carry flag to whatever the 7th bit was
    if (A & 0x80)
        cpu->status_flags |= FLAG_C;     // bitwise or to set bit
    else
        cpu->status_flags &= ~FLAG_C;    // bitwise and not operator to clear bit

    // Add/Subtract flag: unset for sla
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

    // Half Carry Flag: unset for sla
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


void SRA(uint8_t operand, struct Z80* cpu) {
    uint8_t result = 0x00;
    uint8_t val = 0x00;

    switch (operand) {
        // SRA B
        case 0x28:
            result = (cpu->B >> 1) | (cpu->B & 0x80);
            update_flags_sra(cpu->B, result, cpu);
            cpu->B = result;
            break;
        // SRA C
        case 0x29:
            result = (cpu->C >> 1) | (cpu->C & 0x80);
            update_flags_sra(cpu->C, result, cpu);
            cpu->C = result;
            break;
        // SRA D
        case 0x2A:
            result = (cpu->D >> 1) | (cpu->D & 0x80);
            update_flags_sra(cpu->D, result, cpu);
            cpu->D = result;
            break;
        // SRA E
        case 0x2B:
            result = (cpu->E >> 1) | (cpu->E & 0x80);
            update_flags_sra(cpu->E, result, cpu);
            cpu->E = result;
            break;
        // SRA H
        case 0x2C:
            result = (cpu->H >> 1) | (cpu->H & 0x80);
            update_flags_sra(cpu->H, result, cpu);
            cpu->H = result;
            break;
        // SRA L
        case 0x2D:
            result = (cpu->L >> 1) | (cpu->L & 0x80);
            update_flags_sra(cpu->L, result, cpu);
            cpu->L = result;
            break;
        // SRA (HL)
        case 0x2E:
            val = z80_mem_read(cpu->HL);
            result = (val >> 1) | (val & 0x80);
            update_flags_sra(val, result, cpu);
            z80_mem_write(cpu->HL, result);
            break;
        // SRA A
        case 0x2F:
            result = (cpu->A >> 1) | (cpu->A & 0x80);
            update_flags_sra(cpu->A, result, cpu);
            cpu->A = result;
            break;
    }
}


void update_flags_sra(uint8_t A, uint8_t result, struct Z80* cpu) {
    uint8_t bit_count = 0x00;

    // Carry Flag: data from bit 0 of source register
    if (A & 0x01)
        cpu->status_flags |= FLAG_C;     // bitwise or to set bit
    else
        cpu->status_flags &= ~FLAG_C;    // bitwise and not operator to clear bit

    // Add/Subtract flag: unset for sra
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

    // Half Carry Flag: unset for sra
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


void SRL(uint8_t operand, struct Z80* cpu) {
    uint8_t result = 0x00;
    uint8_t val = 0x00;

    switch (operand) {
        // SRL B
        case 0x38:
            result = cpu->B >> 1;
            update_flags_srl(cpu->B, result, cpu);
            cpu->B = result;
            break;
        // SRL C
        case 0x39:
            result = cpu->C >> 1;
            update_flags_srl(cpu->C, result, cpu);
            cpu->C = result;
            break;
        // SRL D
        case 0x3A:
            result = cpu->D >> 1;
            update_flags_srl(cpu->D, result, cpu);
            cpu->D = result;
            break;
        // SRL E
        case 0x3B:
            result = cpu->E >> 1;
            update_flags_srl(cpu->E, result, cpu);
            cpu->E = result;
            break;
        // SRL H
        case 0x3C:
            result = cpu->H >> 1;
            update_flags_srl(cpu->H, result, cpu);
            cpu->H = result;
            break;
        // SRL L
        case 0x3D:
            result = cpu->L >> 1;
            update_flags_srl(cpu->L, result, cpu);
            cpu->L = result;
            break;
        // SRL (HL)
        case 0x3E:
            val = z80_mem_read(cpu->HL);
            result = val >> 1;
            update_flags_srl(val, result, cpu);
            z80_mem_write(cpu->HL, result);
            break;
        // SRL A
        case 0x3F:
            result = cpu->A >> 1;
            update_flags_srl(cpu->A, result, cpu);
            cpu->A = result;
            break;
    }
}


void update_flags_srl(uint8_t A, uint8_t result, struct Z80* cpu) {
    uint8_t bit_count = 0x00;

    // Carry Flag: data from bit 0 of source register
    if (A & 0x01)
        cpu->status_flags |= FLAG_C;     // bitwise or to set bit
    else
        cpu->status_flags &= ~FLAG_C;    // bitwise and not operator to clear bit

    // Add/Subtract flag: unset for srl
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

    // Half Carry Flag: unset for srl
    cpu->status_flags &= ~FLAG_H;

    // Zero Flag: set if the result is zero
    if (result == 0x00)
        cpu->status_flags |= FLAG_Z;
    else
        cpu->status_flags &= ~FLAG_Z;

    // Sign Flag: unset for srl
    cpu->status_flags &= ~FLAG_S;
}