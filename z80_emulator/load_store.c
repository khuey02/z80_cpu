#include "instructions.h"
#include "cpu_em.h"

/*
    load_store.c: Load and Store Operations for (Register to Register), (Memory to Register), (Register to Memory), push and pop
*/

void reg_to_reg_load(uint8_t operand, struct Z80* cpu) {
    switch (operand) {
        // LD B, B
        case 0x40:
            cpu->B = cpu->B;
            break;
        // LD B, C
        case 0x41:
            cpu->B = cpu->C;
            break;
        // LD B, D
        case 0x42:
            cpu->B = cpu->D;
            break;
        // LD B, E
        case 0x43:
            cpu->B = cpu->E;
            break;
        // LD B, H
        case 0x44:
            cpu->B = cpu->H;
            break;
        // LD B, L
        case 0x45:
            cpu->B = cpu->L;
            break;
        // LD B, A
        case 0x47:
            cpu->B = cpu->A;
            break;
        // LD C, B
        case 0x48:
            cpu->C = cpu->B;
            break;
        // LD C, C
        case 0x49:
            cpu->C = cpu->C;
            break;
        // LD C, D
        case 0x4A:
            cpu->C = cpu->D;
            break;
        // LD C, E
        case 0x4B:
            cpu->C = cpu->E;
            break;
        // LD C, H
        case 0x4C:
            cpu->C = cpu->H;
            break;
        // LD C, L
        case 0x4D:
            cpu->C = cpu->L;
            break;
        // LD C, A
        case 0x4F:
            cpu->C = cpu->A;
            break;
        // LD D, B
        case 0x50:
            cpu->D = cpu->B;
            break;
        // LD D, C
        case 0x51:
            cpu->D = cpu->C;
            break;
        // LD D, D
        case 0x52:
            cpu->D = cpu->D;
            break;
        // LD D, E
        case 0x53:
            cpu->D = cpu->E;
            break;
        // LD D, H
        case 0x54:
            cpu->D = cpu->H;
            break;
        // LD D, L
        case 0x55:
            cpu->D = cpu->L;
            break;
        // LD D, A
        case 0x57:
            cpu->D = cpu->A;
            break;
        // LD E, B
        case 0x58:
            cpu->E = cpu->B;
            break;
        // LD E, C
        case 0x59:
            cpu->E = cpu->C;
            break;
        // LD E, D
        case 0x5A:
            cpu->E = cpu->D;
            break;
        // LD E, E
        case 0x5B:
            cpu->E = cpu->E;
            break;
        // LD E, H
        case 0x5C:
            cpu->E = cpu->H;
            break;
        // LD E, L
        case 0x5D:
            cpu->E = cpu->L;
            break;
        // LD E, A
        case 0x5F:
            cpu->E = cpu->A;
            break;
        // LD H, B
        case 0x60:
            cpu->H = cpu->B;
            break;
        // LD H, C
        case 0x61:
            cpu->H = cpu->C;
            break;
        // LD H, D
        case 0x62:
            cpu->H = cpu->D;
            break;
        // LD H, E
        case 0x63:
            cpu->H = cpu->E;
            break;
        // LD H, H
        case 0x64:
            cpu->H = cpu->H;
            break;
        // LD H, L
        case 0x65:
            cpu->H = cpu->L;
            break;
        // LD H, A
        case 0x67:
            cpu->H = cpu->A;
            break;
        // LD L, B
        case 0x68:
            cpu->L = cpu->B;
            break;
        // LD L, C
        case 0x69:
            cpu->L = cpu->C;
            break;
        // LD L, D
        case 0x6A:
            cpu->L = cpu->D;
            break;
        // LD L, E
        case 0x6B:
            cpu->L = cpu->E;
            break;
        // LD L, H
        case 0x6C:
            cpu->L = cpu->H;
            break;
        // LD L, L
        case 0x6D:
            cpu->L = cpu->L;
            break;
        // LD L, A
        case 0x6F:
            cpu->L = cpu->A;
            break;
        // LD A, B
        case 0x78:
            cpu->A = cpu->B;
            break;
        // LD A, C
        case 0x79:
            cpu->A = cpu->C;
            break;
        // LD A, D
        case 0x7A:
            cpu->A = cpu->D;
            break;
        // LD A, E
        case 0x7B:
            cpu->A = cpu->E;
            break;
        // LD A, H
        case 0x7C:
            cpu->A = cpu->H;
            break;
        // LD A, L
        case 0x7D:
            cpu->A = cpu->L;
            break;
        // LD A, A
        case 0x7F:
            cpu->A = cpu->A;
            break;
        default:
            break;
    }
}


void mem_to_reg_load(uint8_t operand, struct Z80* cpu) {
    
    switch (operand) {
        // LD B, (HL)
        case 0x46:
            cpu->B = z80_mem_read(cpu->HL);
            break;
        // LD C, (HL)
        case 0x4E:
            cpu->C = z80_mem_read(cpu->HL);
            break;
        // LD D, (HL)
        case 0x56:
            cpu->D = z80_mem_read(cpu->HL);
            break;
        // LD E, (HL)
        case 0x5E:
            cpu->E = z80_mem_read(cpu->HL);
            break;
        // LD H, (HL)
        case 0x66:
            cpu->H = z80_mem_read(cpu->HL);
            break;
        // LD L, (HL)
        case 0x6E:
            cpu->L = z80_mem_read(cpu->HL);
            break;
        // LD A, (HL)
        case 0x7E:
            cpu->A = z80_mem_read(cpu->HL);
            break;
        case 0x3E: // LD A, N
            cpu->PC++;
            cpu->A = z80_mem_read(cpu->PC);
            break;
        case 0x06: // LD B, N
            cpu->PC++;
            cpu->B = z80_mem_read(cpu->PC);
            break;
        case 0x0E: // LD C, N
            cpu->PC++;
            cpu->C = z80_mem_read(cpu->PC);
            break;
        case 0x16: // LD D, N
            cpu->PC++;
            cpu->D = z80_mem_read(cpu->PC);
            break;
        case 0x1E: // LD E, N
            cpu->PC++;
            cpu->E = z80_mem_read(cpu->PC);
            break;
        case 0x26: // LD H, N
            cpu->PC++;
            cpu->H = z80_mem_read(cpu->PC);
            break;
        case 0x2E: // LD L, N
            cpu->PC++;
            cpu->L = z80_mem_read(cpu->PC);
            break;
        default:
            break;
    }
}


void reg_to_mem_load(uint8_t operand, struct Z80* cpu) {
    switch (operand) {
        // LD (HL), B
        case 0x70:
            z80_mem_write(cpu->HL, cpu->B);
            break;
        // LD (HL), C
        case 0x71:
            z80_mem_write(cpu->HL, cpu->C);
            break;
        // LD (HL), D
        case 0x72:
            z80_mem_write(cpu->HL, cpu->D);
            break;
        // LD (HL), E
        case 0x73:
            z80_mem_write(cpu->HL, cpu->E);
            break;
        // LD (HL), H
        case 0x74:
            z80_mem_write(cpu->HL, cpu->H);
            break;
        // LD (HL), L
        case 0x75:
            z80_mem_write(cpu->HL, cpu->L);
            break;
        // LD (HL), A
        case 0x77:
            z80_mem_write(cpu->HL, cpu->A);
            break;
        default:
            break;
    }
}


void PUSH(uint8_t operand, struct Z80* cpu) {
    switch (operand) {
        // PUSH BC
        case 0xC5:
            cpu->SP -= 1;
            z80_mem_write(cpu->SP, cpu->B);
            cpu->SP -= 1;
            z80_mem_write(cpu->SP, cpu->C);
            break;
        // PUSH DE
        case 0xD5:
            cpu->SP -= 1;
            z80_mem_write(cpu->SP, cpu->D);
            cpu->SP -= 1;
            z80_mem_write(cpu->SP, cpu->E);
            break;
        // PUSH HL
        case 0xE5:
            cpu->SP -= 1;
            z80_mem_write(cpu->SP, cpu->H);
            cpu->SP -= 1;
            z80_mem_write(cpu->SP, cpu->L);
            break;
        // PUSH AF
        case 0xF5:
            cpu->SP -= 1;
            z80_mem_write(cpu->SP, cpu->A);
            cpu->SP -= 1;
            z80_mem_write(cpu->SP, cpu->F);
            break;
        default:
            break;
    }
}


void POP(uint8_t operand, struct Z80* cpu) {
    switch (operand) {
        // POP BC
        case 0xC1:
            cpu->C = z80_mem_read(cpu->SP);
            cpu->SP += 1;
            cpu->B = z80_mem_read(cpu->SP);
            cpu->SP += 1;
            break;
        // POP DE
        case 0xD1:
            cpu->E = z80_mem_read(cpu->SP);
            cpu->SP += 1;
            cpu->D = z80_mem_read(cpu->SP);
            cpu->SP += 1;
            break;
        // POP HL
        case 0xE1:
            cpu->L = z80_mem_read(cpu->SP);
            cpu->SP += 1;
            cpu->H = z80_mem_read(cpu->SP);
            cpu->SP += 1;
            break;
        // POP AF
        case 0xF1:
            cpu->F = z80_mem_read(cpu->SP);
            cpu->SP += 1;
            cpu->A = z80_mem_read(cpu->SP);
            cpu->SP += 1;
            break;
        default:
            break;
    }
}