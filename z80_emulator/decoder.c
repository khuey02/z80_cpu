/*
    decoder.c: Identifies the opcode and call the corresponding function from arithmetic.c, logic.c, jumps_interrupts.c, shifts.c, or load_store.c
*/

#include "cpu_em.h"
#include "instructions.h"


int decode(uint8_t opcode, struct Z80* cpu){
    int cycles = 0;

    if (cpu->main_opcode) {

        switch (opcode) {
            // 8-bit Unsigned Arithmetic Addition
            case 0x80: // ADD A, B
            case 0x81: // ADD A, C
            case 0x82: // ADD A, D
            case 0x83: // ADD A, E
            case 0x84: // ADD A, H
            case 0x85: // ADD A, L
            case 0x86: // ADD A, (HL)
            case 0x87: // ADD A, A
            case 0xC6: // ADD A, N
            // 16-Bit Unsigned Arithmetic Addition
            case 0x09: // ADD HL, BC
            case 0x19: // ADD HL, DE
            case 0x29: // ADD HL, HL
            case 0x39: // ADD HL, SP
            // 8-bit Arithmetic Addition with Carry
            case 0x88: // ADC A, B
            case 0x89: // ADC A, C
            case 0x8A: // ADC A, D
            case 0x8B: // ADC A, E
            case 0x8C: // ADC A, H
            case 0x8D: // ADC A, L
            case 0x8E: // ADC A, (HL)
            case 0x8F: // ADC A, A
            case 0xCE: // ADC A, N
                ADD_or_ADC(opcode, cpu);
                break;

            // 8-bit Arithmetic Subtraction
            case 0x90: // SUB B
            case 0x91: // SUB C
            case 0x92: // SUB D
            case 0x93: // SUB E
            case 0x94: // SUB H
            case 0x95: // SUB L
            case 0x96: // SUB (HL)
            case 0x97: // SUB A
            case 0xD6: // SUB N
            // 8-bit Arithmetic Subtraction with Carry
            case 0x98: // SBC A, B
            case 0x99: // SBC A, C
            case 0x9A: // SBC A, D
            case 0x9B: // SBC A, E
            case 0x9C: // SBC A, H
            case 0x9D: // SBC A, L
            case 0x9E: // SBC A, (HL)
            case 0x9F: // SBC A, A
            case 0xDE: // SBC A, N
                SUB_or_SBC(opcode, cpu);
                break;

            // 8-Bit Increment
            case 0x3C: // INC A
            case 0x04: // INC B
            case 0x0C: // INC C
            case 0x14: // INC D
            case 0x1C: // INC E
            case 0x23: // INC HL
            case 0x24: // INC H
            case 0x2C: // INC L
            case 0x33: // INC SP
            case 0x34: // INC (HL)
            case 0x03: // INC BC
            case 0x13: // INC DE
                INC(opcode, cpu);
                break;

            // 8-Bit Decrement
            case 0x3B: // DEC SP
            case 0x3D: // DEC A
            case 0x05: // DEC B
            case 0x0B: // DEC BC
            case 0x0D: // DEC C
            case 0x15: // DEC D
            case 0x1B: // DEC DE
            case 0x1D: // DEC E
            case 0x25: // DEC H
            case 0x2B: // DEC HL
            case 0x2D: // DEC L
            case 0x35: // DEC (HL)
                DEC(opcode, cpu);
                break;

            case 0x27: // DAA
                DAA(cpu);
                break;

            case 0x00: // NOP
                NOP(opcode, cpu);
                break;

            // 8-bit Bitwise AND
            case 0xA0: // AND B
            case 0xA1: // AND C
            case 0xA2: // AND D
            case 0xA3: // AND E
            case 0xA4: // AND H
            case 0xA5: // AND L
            case 0xA6: // AND (HL)
            case 0xA7: // AND A
            case 0xE6: // AND N
                AND(opcode, cpu);
                break;

            // 8-bit Bitwise OR
            case 0xB0: // OR B
            case 0xB1: // OR C
            case 0xB2: // OR D
            case 0xB3: // OR E
            case 0xB4: // OR H
            case 0xB5: // OR L
            case 0xB6: // OR (HL)
            case 0xB7: // OR A
            case 0xF6: // OR N
                OR(opcode, cpu);
                break;

            // 8-bit Bitwise XOR
            case 0xA8: // XOR B
            case 0xA9: // XOR C
            case 0xAA: // XOR D
            case 0xAB: // XOR E
            case 0xAC: // XOR H
            case 0xAD: // XOR L
            case 0xAE: // XOR (HL)
            case 0xAF: // XOR A
            case 0xEE: // XOR N
                XOR(opcode, cpu);
                break;

            // 8-Bit Compare Operations
            case 0xB8: // CP B
            case 0xB9: // CP C
            case 0xBA: // CP D
            case 0xBB: // CP E
            case 0xBC: // CP H
            case 0xBD: // CP L
            case 0xBE: // CP (HL)
            case 0xBF: // CP A
            case 0xFE: // CP N
                CP(opcode, cpu);
                break;

            // 8-bit Load / Store (Register to Register)
            case 0x40: // LD B, B
            case 0x41: // LD B, C
            case 0x42: // LD B, D
            case 0x43: // LD B, E
            case 0x44: // LD B, H
            case 0x45: // LD B, L
            case 0x47: // LD B, A
            case 0x48: // LD C, B
            case 0x49: // LD C, C
            case 0x4A: // LD C, D
            case 0x4B: // LD C, E
            case 0x4C: // LD C, H
            case 0x4D: // LD C, L
            case 0x4F: // LD C, A
            case 0x50: // LD D, B
            case 0x51: // LD D, C
            case 0x52: // LD D, D
            case 0x53: // LD D, E
            case 0x54: // LD D, H
            case 0x55: // LD D, L
            case 0x57: // LD D, A
            case 0x58: // LD E, B
            case 0x59: // LD E, C
            case 0x5A: // LD E, D
            case 0x5B: // LD E, E
            case 0x5C: // LD E, H
            case 0x5D: // LD E, L
            case 0x5F: // LD E, A
            case 0x60: // LD H, B
            case 0x61: // LD H, C
            case 0x62: // LD H, D
            case 0x63: // LD H, E
            case 0x64: // LD H, H
            case 0x65: // LD H, L
            case 0x67: // LD H, A
            case 0x68: // LD L, B
            case 0x69: // LD L, C
            case 0x6A: // LD L, D
            case 0x6B: // LD L, E
            case 0x6C: // LD L, H
            case 0x6D: // LD L, L
            case 0x6F: // LD L, A
            case 0x78: // LD A, B
            case 0x79: // LD A, C
            case 0x7A: // LD A, D
            case 0x7B: // LD A, E
            case 0x7C: // LD A, H
            case 0x7D: // LD A, L
            case 0x7F: // LD A, A
                reg_to_reg_load(opcode, cpu);
                break;

            // 8-bit Load / Store (Memory to Register)
            case 0x46: // LD B, (HL)
            case 0x4E: // LD C, (HL)
            case 0x56: // LD D, (HL)
            case 0x5E: // LD E, (HL)
            case 0x66: // LD H, (HL)
            case 0x6E: // LD L, (HL)
            case 0x7E: // LD A, (HL)
            case 0x3E: // LD A, N
            case 0x06: // LD B, N
            case 0x0E: // LD C, N
            case 0x16: // LD D, N
            case 0x1E: // LD E, N
            case 0x26: // LD H, N
            case 0x2E: // LD L, N
                mem_to_reg_load(opcode, cpu);
                break;

            // 8-bit Load / Store (Register to Memory)
            case 0x70: // LD (HL), B
            case 0x71: // LD (HL), C
            case 0x72: // LD (HL), D
            case 0x73: // LD (HL), E
            case 0x74: // LD (HL), H
            case 0x75: // LD (HL), L
            case 0x77: // LD (HL), A
                reg_to_mem_load(opcode, cpu);
                break;

            // Push
            case 0xC5: // PUSH BC
            case 0xD5: // PUSH DE
            case 0xE5: // PUSH HL
            case 0xF5: // PUSH AF
                PUSH(opcode, cpu);
                break;

            // Pop
            case 0xC1: // POP BC
            case 0xD1: // POP DE
            case 0xE1: // POP HL
            case 0xF1: // POP AF
                POP(opcode, cpu);
                break;

            case 0x76: // HALT
                HALT(cpu);
                break;

            // Unconditional Jump
            case 0xC2: // JP NZ, NN
            case 0xC3: // JP NN
            case 0xCA: // JP Z, NN
            case 0xD2: // JP NC, NN
            case 0xDA: // JP C, NN
            case 0xE2: // JP PO, NN
            case 0xE9: // JP (HL)
            case 0xEA: // JP PE, NN
            case 0xF2: // JP P, NN
            case 0xFA: // JP M, NN
                JP(opcode, cpu);
                break;

            case 0xC4: // CALL NZ, NN
            case 0xCC: // CALL Z, NN
            case 0xCD: // CALL NN
            case 0xD4: // CALL NC, NN
            case 0xDC: // CALL C, NN
            case 0xE4: // CALL PO, NN
            case 0xEC: // CALL PE, NN
            case 0xF4: // CALL P, NN
            case 0xFC: // CALL M, NN
                cycles = CALL(opcode, cpu);
                break;

            // 8-Bit Return
            case 0xC0: // RET NZ
            case 0xC8: // RET Z
            case 0xC9: // RET
            case 0xD0: // RET NC
            case 0xD8: // RET C
            case 0xE0: // RET PO
            case 0xE8: // RET PE
            case 0xF0: // RET P
            case 0xF8: // RET M
                cycles = RET(opcode, cpu);
                break;

            // Treat anything else as NOP
            default:
                NOP(opcode, cpu);
                break;
        }
    }

    else if (cpu->ED_opcode) {
        switch (opcode) {
            case 0x00: // IN0 B, (N)
                IN(cpu);
                break;

            case 0x01: // OUT0 (N), B
                OUT(cpu);
                break;

            case 0x45: // RETN (Treat as RET (0xC9))
            case 0x4D: // RETI (Treat as RET (0xC9))
                RET(0xC9, cpu);
                break;

            case 0x46: // IM 0
                IM0(cpu);
                break;
                    ;
            case 0x56: // IM 1
                IM1(cpu);
                break;

            case 0x5E: // IM 2
                IM2(cpu);
                break;

            case 0x76: // SLP
                HALT(cpu);
                break;

            // Treat anything else as NOP
            default:
                NOP(opcode, cpu);
                break;
        }
    }

    else if (cpu->CB_opcode) {
        switch (opcode) {
            // Shift Left Unsigned
            case 0x20: // SLA B
            case 0x21: // SLA C
            case 0x22: // SLA D
            case 0x23: // SLA E
            case 0x24: // SLA H
            case 0x25: // SLA L
            case 0x26: // SLA (HL)
            case 0x27: // SLA A
                SLA(opcode, cpu);
                break;

            // Shift Right Signed
            case 0x28: // SRA B
            case 0x29: // SRA C
            case 0x2A: // SRA D
            case 0x2B: // SRA E
            case 0x2C: // SRA H
            case 0x2D: // SRA L
            case 0x2E: // SRA (HL)
            case 0x2F: // SRA A
                SRA(opcode, cpu);
                break;

            // Shift Right Unsigned
            case 0x38: // SRL B
            case 0x39: // SRL C
            case 0x3A: // SRL D
            case 0x3B: // SRL E
            case 0x3C: // SRL H
            case 0x3D: // SRL L
            case 0x3E: // SRL (HL)
            case 0x3F: // SRL A
                SRL(opcode, cpu);
                break;

            // Shift Left Logical        
            case 0x30: // SLL B
            case 0x31: // SLL C
            case 0x32: // SLL D
            case 0x33: // SLL E
            case 0x34: // SLL H
            case 0x35: // SLL L
            case 0x36: // SLL (HL)
            case 0x37: // SLL A
                SLL(opcode, cpu);
                break;

            // Treat anything else as NOP
            default:
                NOP(opcode, cpu);
                break;
        }
    }

    else
        NOP(opcode, cpu);

    return cycles;
}