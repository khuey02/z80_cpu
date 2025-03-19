#include "instructions.h"
#include "cpu_em.h"

/* 
	jumps_interrupts.c: HALT, SLEEP, Interrupt Control, Unconditional Jumps, Calls, Returns, NOP
*/

void HALT(struct Z80* cpu) {
    FILE* mem_fp = NULL;
    cpu->halted = 0x1;
    cpu->PC++;  // Was one off, incrementing to match test code output 

    mem_fp = fopen("memory.bin", "wb");
    if (!mem_fp) {
        perror("Failed to open memory.bin");
        return;
    }

    // Printing register contents to stdout
    // %0#X means print at least # digits, prepends with 0 if not # digits
    printf("A:  %02X    \nF:  %02X\n", cpu->A, cpu->status_flags);
    printf("B:  %02X    \nC:  %02X\n", cpu->B, cpu->C);
    printf("D:  %02X    \nE:  %02X\n", cpu->D, cpu->E);
    printf("H:  %02X    \nL:  %02X\n", cpu->H, cpu->L);
    printf("I:  %02X    \nR:  %02X\n", cpu->I, cpu->R);
    printf("A':  %02X    \nF':  %02X\n", cpu->A2, cpu->F2);
    printf("B':  %02X    \nC':  %02X\n", cpu->B2, cpu->C2);
    printf("D':  %02X    \nE':  %02X\n", cpu->D2, cpu->E2);
    printf("AF: %04X   \nBC: %04X\n", cpu->AF, cpu->BC);
    printf("DE: %04X   \nHL: %04X\n", cpu->DE, cpu->HL);
    printf("IFF1: %X   \nIFF2: %X\n", cpu->IFF1, cpu->IFF2);
    printf("IM: %X    \nHidden 16-bit math register: %02X\n", cpu->IM, cpu->WZ);
    printf("IX: %04X    \nIY: %04X\n", cpu->IX, cpu->IY);
    printf("PC: %04X   \nSP: %04X\n", cpu->PC, cpu->SP);

    // Writing register contents to memory.bin
    fprintf(mem_fp, "A:  %02X    \nF : %02X\n", cpu->A, cpu->status_flags);
    fprintf(mem_fp, "B:  %02X    \nC:  %02X\n", cpu->B, cpu->C);
    fprintf(mem_fp, "D:  %02X    \nE:  %02X\n", cpu->D, cpu->E);
    fprintf(mem_fp, "H:  %02X    \nL:  %02X\n", cpu->H, cpu->L);
    fprintf(mem_fp, "I:  %02X    \nR:  %02X\n", cpu->I, cpu->R);
    fprintf(mem_fp, "A':  %02X    \nF':  %02X\n", cpu->A2, cpu->F2);
    fprintf(mem_fp, "B':  %02X    \nC':  %02X\n", cpu->B2, cpu->C2);
    fprintf(mem_fp, "D':  %02X    \nE':  %02X\n", cpu->D2, cpu->E2);
    fprintf(mem_fp, "AF: %04X   \nBC: %04X\n", cpu->AF, cpu->BC);
    fprintf(mem_fp, "DE: %04X   \nHL: %04X\n", cpu->DE, cpu->HL);
    fprintf(mem_fp, "IFF1: %X   \nIFF2: %X\n", cpu->IFF1, cpu->IFF2);
    fprintf(mem_fp, "IM: %X    \nHidden 16-bit math register: %02X\n", cpu->IM, cpu->WZ);
    fprintf(mem_fp, "IX: %04X    \nIY: %04X\n", cpu->IX, cpu->IY);
    fprintf(mem_fp, "PC: %04X   \nSP: %04X\n", cpu->PC, cpu->SP);

    fclose(mem_fp);
}


void IN(struct Z80* cpu) {
    // Printing register contents to stdout
    // %0#X means print at least # digits, prepends with 0 if not # digits
    printf("A:  %02X    \nF:  %02X\n", cpu->A, cpu->status_flags);
    printf("B:  %02X    \nC:  %02X\n", cpu->B, cpu->C);
    printf("D:  %02X    \nE:  %02X\n", cpu->D, cpu->E);
    printf("H:  %02X    \nL:  %02X\n", cpu->H, cpu->L);
    printf("H:  %02X    \nL:  %02X\n", cpu->H, cpu->L);
    printf("I:  %02X    \nR:  %02X\n", cpu->I, cpu->R);
    printf("A':  %02X    \nF':  %02X\n", cpu->A2, cpu->F2);
    printf("B':  %02X    \nC':  %02X\n", cpu->B2, cpu->C2);
    printf("D':  %02X    \nE':  %02X\n", cpu->D2, cpu->E2);
    printf("AF: %04X   \nBC: %04X\n", cpu->AF, cpu->BC);
    printf("DE: %04X   \nHL: %04X\n", cpu->DE, cpu->HL);
    printf("IFF1: %X   \nIFF2: %X\n", cpu->IFF1, cpu->IFF2);
    printf("IM: %X    \nHidden 16-bit math register: %02X\n", cpu->IM, cpu->WZ);
    printf("IX: %04X    \nIY: %04X\n", cpu->IX, cpu->IY);
    printf("PC: %04X   \nSP: %04X\n", cpu->PC, cpu->SP);
}


void OUT(struct Z80* cpu) {
    FILE* mem_fp = NULL;

    mem_fp = fopen("memory.bin", "wb");
    if (!mem_fp) {
        perror("Failed to open memory.bin");
        return;
    }

    // Writing register contents to memory.bin
    fprintf(mem_fp, "A:  %02X    \nF : %02X\n", cpu->A, cpu->status_flags);
    fprintf(mem_fp, "B:  %02X    \nC:  %02X\n", cpu->B, cpu->C);
    fprintf(mem_fp, "D:  %02X    \nE:  %02X\n", cpu->D, cpu->E);
    fprintf(mem_fp, "H:  %02X    \nL:  %02X\n", cpu->H, cpu->L);
    fprintf(mem_fp, "I:  %02X    \nR:  %02X\n", cpu->I, cpu->R);
    fprintf(mem_fp, "A':  %02X    \nF' : %02X\n", cpu->A2, cpu->F2);
    fprintf(mem_fp, "B':  %02X    \nC':  %02X\n", cpu->B2, cpu->C2);
    fprintf(mem_fp, "D':  %02X    \nE':  %02X\n", cpu->D2, cpu->E2);
    fprintf(mem_fp, "AF: %04X   \nBC: %04X\n", cpu->AF, cpu->BC);
    fprintf(mem_fp, "DE: %04X   \nHL: %04X\n", cpu->DE, cpu->HL);
    fprintf(mem_fp, "IFF1: %X   \nIFF2: %X\n", cpu->IFF1, cpu->IFF2);
    fprintf(mem_fp, "IM: %X    \nHidden 16-bit math register: %02X\n", cpu->IM, cpu->WZ);
    fprintf(mem_fp, "IX: %04x    \nIY: %04x\n", cpu->IX, cpu->IY);
    fprintf(mem_fp, "PC: %04X   \nSP: %04X\n", cpu->PC, cpu->SP);

    fclose(mem_fp);
}


void DI(struct Z80* cpu) {
    cpu->IFF1 = 0x00;
    cpu->IFF2 = 0x00;
}


void EI(struct Z80* cpu) {
    cpu->IFF1 = 0x01;
    cpu->IFF2 = 0x01;
}


void IM0(struct Z80* cpu) {
    cpu->IM = 0;
}


void IM1(struct Z80* cpu) {
    cpu->IM = 1;
}


void IM2(struct Z80* cpu) {
    cpu->IM = 2;
}


void JP(uint8_t opcode, struct Z80* cpu) {
    cpu->PC++;
    uint8_t low_byte = z80_mem_read(cpu->PC);
    cpu->PC++;
    uint8_t high_byte = z80_mem_read(cpu->PC);
    uint16_t address = (high_byte << 8) | low_byte;

    switch (opcode) {
        // JP NZ, NN
        case 0xC2:
            if (!(cpu->status_flags & FLAG_Z))
                cpu->PC = address;
            else
                cpu->PC = cpu->PC + 3;
            break;
        // JP NN
        case 0xC3:
            cpu->PC = address;
            cpu->PC--;
            break;
        // JP Z, NN
        case 0xCA:
            if (cpu->status_flags & FLAG_Z)
                cpu->PC = address;
            else
                cpu->PC = cpu->PC + 3;
            break;
        // JP NC, NN
        case 0xD2:
            if (!(cpu->status_flags & FLAG_C))
                cpu->PC = address;
            else
                cpu->PC = cpu->PC + 3;
            break;
        // JP C, NN
        case 0xDA:
            if (cpu->status_flags & FLAG_C)
                cpu->PC = address;
            else
                cpu->PC = cpu->PC + 3;
            break;
        // JP PO, NN
        case 0xE2:
            if (!(cpu->status_flags & FLAG_PV))
                cpu->PC = address;
            else
                cpu->PC = cpu->PC + 3;
            break;
        // JP (HL)
        case 0xE9:
            cpu->PC = z80_mem_read(cpu->HL);
            break;
        // JP PE, NN
        case 0xEA:
            if (cpu->status_flags & FLAG_PV)
                cpu->PC = address;
            else
                cpu->PC = cpu->PC + 3;
            break;
        // JP P, NN
        case 0xF2:
            if (!(cpu->status_flags & FLAG_S))
                cpu->PC = address;
            else
                cpu->PC = cpu->PC + 3;
            break;
        // JP M, NN
        case 0xFA:
            if (cpu->status_flags & FLAG_PV)
                cpu->PC = address;
            else
                cpu->PC = cpu->PC + 3;
            break;
    }
}


int CALL(uint8_t opcode, struct Z80* cpu) {
    uint16_t address = z80_mem_read(cpu->PC) | (z80_mem_read(cpu->PC + 1) << 8);
    uint16_t return_address = cpu->PC + 3;
    uint8_t cycles = 0x00;

    switch (opcode) {
        // CALL NZ, NN (17/10 cycles)
    case 0xC4:
        if (!(cpu->status_flags & FLAG_Z)) {
            cpu->SP -= 2;
            z80_mem_write(cpu->SP, return_address & 0xFF);   // low byte
            z80_mem_write(cpu->SP + 1, return_address >> 8); // high byte
            cpu->PC = address;
            cycles = 0x07;
        }
        else
            cpu->PC += 3;
        break;
        // CALL Z, NN (17/10 cycles)
    case 0xCC:
        if (cpu->status_flags & FLAG_Z) {
            cpu->SP -= 2;
            z80_mem_write(cpu->SP, return_address & 0xFF);   // low byte
            z80_mem_write(cpu->SP + 1, return_address >> 8); // high byte
            cpu->PC = address;
            cycles = 0x07;
        }
        else
            cpu->PC += 3;
        break;
        // CALL NN
    case 0xCD:
        cpu->SP -= 2;
        z80_mem_write(cpu->SP, return_address & 0xFF);   // low byte
        z80_mem_write(cpu->SP + 1, return_address >> 8); // high byte
        cpu->PC = address;
        cycles = 0x07;
        break;
        // CALL NC, NN (17/10 cycles)
    case 0xD4:
        if (!(cpu->status_flags & FLAG_C)) {
            cpu->SP -= 2;
            z80_mem_write(cpu->SP, return_address & 0xFF);   // low byte
            z80_mem_write(cpu->SP + 1, return_address >> 8); // high byte
            cpu->PC = address;
            cycles = 0x07;
        }
        else
            cpu->PC += 3;
        break;
        // CALL C, NN (17/10 cycles)
    case 0xDC:
        if (cpu->status_flags & FLAG_C) {
            cpu->SP -= 2;
            z80_mem_write(cpu->SP, return_address & 0xFF);   // low byte
            z80_mem_write(cpu->SP + 1, return_address >> 8); // high byte
            cpu->PC = address;
            cycles = 0x07;
        }
        else
            cpu->PC += 3;
        break;
        // CALL PO, NN (17/10 cycles)
    case 0xE4:
        if (!(cpu->status_flags & FLAG_PV)) {
            cpu->SP -= 2;
            z80_mem_write(cpu->SP, return_address & 0xFF);   // low byte
            z80_mem_write(cpu->SP + 1, return_address >> 8); // high byte
            cpu->PC = address;
            cycles = 0x07;
        }
        else
            cpu->PC += 3;
        break;
        // CALL PE, NN (17/10 cycles)
    case 0xEC:
        if (cpu->status_flags & FLAG_PV) {
            cpu->SP -= 2;
            z80_mem_write(cpu->SP, return_address & 0xFF);   // low byte
            z80_mem_write(cpu->SP + 1, return_address >> 8); // high byte
            cpu->PC = address;
            cycles = 0x07;
        }
        else
            cpu->PC += 3;
        break;
        // CALL P, NN (17/10 cycles)
    case 0xF4:
        if (!(cpu->status_flags & FLAG_S)) {
            cpu->SP -= 2;
            z80_mem_write(cpu->SP, return_address & 0xFF);   // low byte
            z80_mem_write(cpu->SP + 1, return_address >> 8); // high byte
            cpu->PC = address;
            cycles = 0x07;
        }
        else
            cpu->PC += 3;
        break;
        // CALL M, NN (17/10 cycles)
    case 0xFC:
        if (cpu->status_flags & FLAG_S) {
            cpu->SP -= 2;
            z80_mem_write(cpu->SP, return_address & 0xFF);   // low byte
            z80_mem_write(cpu->SP + 1, return_address >> 8); // high byte
            cpu->PC = address;
            cycles = 0x07;
        }
        else
            cpu->PC += 3;
        break;
    }

    return cycles;
}


int RET(uint8_t opcode, struct Z80* cpu) {
    uint16_t address = z80_mem_read(cpu->SP) | (z80_mem_read(cpu->SP + 1) << 8);
    uint8_t cycles = 0x00;

    switch (opcode) {
        // RET NZ
        case 0xC0:
            if (!(cpu->status_flags & FLAG_Z)) {
                cpu->PC = address;
                cpu->SP += 2;
                cycles = 0x06;
            }
            else
                cpu->SP += 3;
            break;
        // RET Z
        case 0xC8:
            if (cpu->status_flags & FLAG_Z) {
                cpu->PC = address;
                cpu->SP += 2;
                cycles = 0x06;
            }
            else
                cpu->SP += 3;
            break;
        // RET
        case 0xC9:
            cpu->PC = address;
            cpu->SP += 2;
            break;
        // RET NC
        case 0xD0:
            if (!(cpu->status_flags & FLAG_C)) {
                cpu->PC = address;
                cpu->SP += 2;
                cycles = 0x06;
            }
            else
                cpu->SP += 3;
            break;
        // RET C
        case 0xD8:
            if (cpu->status_flags & FLAG_C) {
                cpu->PC = address;
                cpu->SP += 2;
                cycles = 0x06;
            }
            else
                cpu->SP += 3;
            break;
        // RET PO
        case 0xE0:
            if (!(cpu->status_flags & FLAG_PV)) {
                cpu->PC = address;
                cpu->SP += 2;
                cycles = 0x06;
            }
            else
                cpu->SP += 3;
            break;
        // RET PE
        case 0xE8:
            if (cpu->status_flags & FLAG_PV) {
                cpu->PC = address;
                cpu->SP += 2;
                cycles = 0x06;
            }
            else
                cpu->SP += 3;
            break;
        // RET P
        case 0xF0:
            if (!(cpu->status_flags & FLAG_S)) {
                cpu->PC = address;
                cpu->SP += 2;
                cycles = 0x06;
            }
            else
                cpu->SP += 3;
            break;
        // RET M
        case 0xF8:
            if (cpu->status_flags & FLAG_S) {
                cpu->PC = address;
                cpu->SP += 2;
                cycles = 0x06;
            }
            else
                cpu->SP += 3;
            break;
        }
    return cycles;
}
