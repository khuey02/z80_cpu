/*
    cpu_em.c: Sets up cpu, loops for number of cycles, for each loop it fetches the opcode, identifies number of cycles, and calls decode function
*/

#include "cpu_em.h"

#define NUM_CYCLES 1024
static uint8_t ram[65536];


void z80_mem_write(uint16_t addr, uint8_t value){
    ram[addr] = value;
}


uint8_t z80_mem_read(uint16_t addr){
    return ram[addr];
}


void z80_mem_write16(uint16_t addr, uint16_t value){
    ram[addr] = (uint8_t)(value & 0xff);
    ram[addr + 1] = (uint8_t)(value >> 8);
}


uint16_t z80_mem_read16(uint16_t addr){
    return ((uint16_t)ram[addr]) | (((uint16_t)ram[addr + 1]) << 8);
}


void z80_mem_dump(const char *fn){
    FILE *fp;

    if(!(fp = fopen(fn, "wb"))) {
        fprintf(stderr, "z80_mem_dump: Cannot open destination file %s: %s\n",
                fn, strerror(errno));
        return;
    }

    if(fwrite(ram, 1, 65536, fp) != 65536) {
        fprintf(stderr, "z80_mem_dump: Couldn't write full memory dump\n");
        fclose(fp);
        return;
    }

    fclose(fp);
}


void z80_mem_load(const char *fn){
    FILE *fp;

    memset(ram, 0, sizeof(ram)); // clear everything in memory before loading in new source file

    if(!(fp = fopen(fn, "rb"))) {
        fprintf(stderr, "z80_mem_load: Cannot open source file %s: %s\n",
                fn, strerror(errno));
        return;
    }  

    if(!fread(ram, 1, 65536, fp)) {
        fprintf(stderr, "z80_mem_read: Couldn't read program binary\n");
        fclose(fp);
        return;
    }

    fclose(fp);
}
    

int main(int argc, char *argv[]){
    int ncycles;
    struct Z80 cpu;    // Z80 struct for state info on cpu

    if(argc != 2) {
        fprintf(stderr, "Usage: %s program.bin\n", argv[0]);
        return -1;
    }

    z80_init(&cpu);
    z80_mem_load(argv[1]);
    ncycles = z80_execute(NUM_CYCLES, &cpu);

    printf("Ran %d cycles\n", ncycles);
    return 0;
}


void z80_init(struct Z80* cpu){
    printf("Inside z80_in\n");
    cpu->A = cpu->B = cpu->C = cpu->D = cpu->E = cpu->H = cpu->L = 0x00;
    cpu->AF = cpu->BC = cpu->DE = cpu->HL = 0x00;
    cpu->I = cpu->R = 0x00;
    cpu->IX = cpu->IY = 0xffff; // Set to this to match test code output
    cpu->SP = cpu->PC = 0x00;
    cpu->IFF1 = cpu->IFF2 = cpu->IM = 0x00;
    cpu->WZ = 0x00;
    cpu->halted = 0x00;
    cpu->main_opcode = cpu->ED_opcode = cpu->CB_opcode = 0x00;
    cpu->status_flags = 0x00;
}

int z80_execute(int cycles, struct Z80* cpu){
    printf("Inside z80_execute. Cycles: %d\n", cycles);

    int cycles_left = cycles;     // CPU cycles remaining
    //uint16_t PC = 0x0000;         // Program counter
    //int exitRequired = 0;       // Flag for exiting
    uint8_t opcode;               // Fetched opcode

    // Stores clock cycle count for each opcode in main opcode table
    int opcode_cycles[256] = {
        // NOP
        [0x00] = 4,
        // 8-bit Unsigned Arithmetic Addition
        [0x80] = 4,    // ADD A, B
        [0x81] = 4,    // ADD A, C
        [0x82] = 4,    // ADD A, D
        [0x83] = 4,    // ADD A, E
        [0x84] = 4,    // ADD A, H
        [0x85] = 4,    // ADD A, L
        [0x86] = 7,    // ADD A, (HL)
        [0x87] = 4,    // ADD A, A
        [0xC6] = 7,    // ADD A, N
        // 16-bit Unsigned Arithmetic Addition
        [0x09] = 11,   // ADD HL, BC
        [0x19] = 11,   // ADD HL, DE
        [0x29] = 11,   // ADD HL, HL
        [0x39] = 11,   // ADD HL, SP
        // 8-bit Arithmetic Unsigned Addition with Carry
        [0x88] = 4,    // ADC A, B
        [0x89] = 4,    // ADC A, C
        [0x8A] = 4,    // ADC A, D
        [0x8B] = 4,    // ADC A, E
        [0x8C] = 4,    // ADC A, H
        [0x8D] = 4,    // ADC A, L
        [0x8E] = 7,    // ADC A, (HL)
        [0x8F] = 4,    // ADC A, A
        [0xCE] = 7,    // ADC A, N
        // 8-bit Unsigned Arithmetic Subtraction
        [0x90] = 4,    // SUB B
        [0x91] = 4,    // SUB C
        [0x92] = 4,    // SUB D
        [0x93] = 4,    // SUB E
        [0x94] = 4,    // SUB H
        [0x95] = 4,    // SUB L
        [0x96] = 7,    // SUB (HL)
        [0x97] = 4,    // SUB A
        [0xD6] = 7,    // SUB N
        // 8-bit Unsigned Arithmetic Subtraction with Carry
        [0x98] = 4,    // SBC A, B
        [0x99] = 4,    // SBC A, C
        [0x9A] = 4,    // SBC A, D
        [0x9B] = 4,    // SBC A, E
        [0x9C] = 4,    // SBC A, H
        [0x9D] = 4,    // SBC A, L
        [0x9E] = 7,    // SBC A, (HL)
        [0x9F] = 4,    // SBC A, A
        [0xDE] = 7,    // SBC A, N
        // 8-Bit Increment
        [0x3C] = 4,    // INC A
        [0x04] = 4,    // INC B
        [0x0C] = 4,    // INC C
        [0x14] = 4,    // INC D
        [0x1C] = 4,    // INC E
        [0x24] = 4,    // INC H
        [0x2C] = 4,    // INC L
        [0x34] = 6,    // INC (HL)

        // 16-Bit Increment
        [0x03] = 6,    // INC BC
        [0x13] = 6,    // INC DE
        [0x23] = 6,    // INC HL
        [0x33] = 6,    // INC SP
        // 8-Bit Decrement
        [0x3D] = 4,    // DEC A
        [0x05] = 4,    // DEC B
        [0x0D] = 4,    // DEC C
        [0x15] = 4,    // DEC D
        [0x1D] = 4,    // DEC E
        [0x25] = 4,    // DEC H
        [0x2D] = 4,    // DEC L
        [0x35] = 11,   // DEC (HL)
        // 16-Bit Decrement
        [0x0B] = 6,    // DEC BC
        [0x1B] = 6,    // DEC DE
        [0x2B] = 6,    // DEC HL
        [0x3B] = 6,    // DEC SP
        // Other
        [0x27] = 4,    // DAA

        // 8-bit Bitwise AND
        [0xA0] = 4,    // AND B
        [0xA1] = 4,    // AND C
        [0xA2] = 4,    // AND D
        [0xA3] = 4,    // AND E
        [0xA4] = 4,    // AND H
        [0xA5] = 4,    // AND L
        [0xA6] = 7,    // AND (HL)
        [0xA7] = 4,    // AND A
        [0xE6] = 7,    // AND N
        // 8-bit Bitwise OR
        [0xB0] = 4,    // OR B
        [0xB1] = 4,    // OR C
        [0xB2] = 4,    // OR D
        [0xB3] = 4,    // OR E
        [0xB4] = 4,    // OR H
        [0xB5] = 4,    // OR L
        [0xB6] = 7,    // OR (HL)
        [0xB7] = 4,    // OR A
        [0xF6] = 7,    // OR N
        // 8-bit Bitwise XOR
        [0xA8] = 4,    // XOR B
        [0xA9] = 4,    // XOR C
        [0xAA] = 4,    // XOR D
        [0xAB] = 4,    // XOR E
        [0xAC] = 4,    // XOR H
        [0xAD] = 4,    // XOR L
        [0xAE] = 7,    // XOR (HL)
        [0xAF] = 4,    // XOR A
        [0xEE] = 7,    // XOR N
        // 8-Bit Compare Operations
        [0xB8] = 4,    // CP B
        [0xB9] = 4,    // CP C
        [0xBA] = 4,    // CP D
        [0xBB] = 4,    // CP E
        [0xBC] = 4,    // CP H
        [0xBD] = 4,    // CP L
        [0xBE] = 7,    // CP (HL)
        [0xBF] = 4,    // CP A
        [0xFE] = 7,    // CP N

        // 8-Bit Load/Store (Register to Register)
        [0x40] = 4,    // LD B, B
        [0x41] = 4,    // LD B, C
        [0x42] = 4,    // LD B, D
        [0x43] = 4,    // LD B, E
        [0x44] = 4,    // LD B, H
        [0x45] = 4,    // LD B, L
        [0x47] = 4,    // LD B, A
        [0x48] = 4,    // LD C, B
        [0x49] = 4,    // LD C, C
        [0x4A] = 4,    // LD C, D
        [0x4B] = 4,    // LD C, E
        [0x4C] = 4,    // LD C, H
        [0x4D] = 4,    // LD C, L
        [0x4F] = 4,    // LD C, A
        [0x50] = 4,    // LD D, B
        [0x51] = 4,    // LD D, C
        [0x52] = 4,    // LD D, D
        [0x53] = 4,    // LD D, E
        [0x54] = 4,    // LD D, H
        [0x55] = 4,    // LD D, L
        [0x57] = 4,    // LD D, A
        [0x58] = 4,    // LD E, B
        [0x59] = 4,    // LD E, C
        [0x5A] = 4,    // LD E, D
        [0x5B] = 4,    // LD E, E
        [0x5C] = 4,    // LD E, H
        [0x5D] = 4,    // LD E, L
        [0x5F] = 4,    // LD E, A
        [0x60] = 4,    // LD H, B
        [0x61] = 4,    // LD H, C
        [0x62] = 4,    // LD H, D
        [0x63] = 4,    // LD H, E
        [0x64] = 4,    // LD H, H
        [0x65] = 4,    // LD H, L
        [0x67] = 4,    // LD H, A
        [0x68] = 4,    // LD L, B
        [0x69] = 4,    // LD L, C
        [0x6A] = 4,    // LD L, D
        [0x6B] = 4,    // LD L, E
        [0x6C] = 4,    // LD L, H
        [0x6D] = 4,    // LD L, L
        [0x6F] = 4,    // LD L, A
        [0x78] = 4,    // LD A, B
        [0x79] = 4,    // LD A, C
        [0x7A] = 4,    // LD A, D
        [0x7B] = 4,    // LD A, E
        [0x7C] = 4,    // LD A, H
        [0x7D] = 4,    // LD A, L
        [0x7F] = 4,    // LD A, A
        // 8-Bit Load/Store (Memory to Register)
        [0x46] = 7,    // LD B, (HL)
        [0x4E] = 7,    // LD C, (HL)
        [0x56] = 7,    // LD D, (HL)
        [0x5E] = 7,    // LD E, (HL)
        [0x66] = 7,    // LD H, (HL)
        [0x6E] = 7,    // LD L, (HL)
        [0x7E] = 7,    // LD A, (HL)
        [0x3E] = 7,    // LD A, N
        [0x06] = 7,    // LD B, N
        [0x0E] = 7,    // LD C, N
        [0x16] = 7,    // LD D, N
        [0x1E] = 7,    // LD E, N
        [0x26] = 7,    // LD H, N
        [0x2E] = 7,    // LD L, N
        // 8-Bit Load/Store (Register to Memory)
        [0x70] = 7,    // LD (HL), B
        [0x71] = 7,    // LD (HL), C
        [0x72] = 7,    // LD (HL), D
        [0x73] = 7,    // LD (HL), E
        [0x74] = 7,    // LD (HL), H
        [0x75] = 7,    // LD (HL), L
        [0x77] = 7,    // LD (HL), A

        // Push
        [0xC5] = 11,   // PUSH BC
        [0xD5] = 11,   // PUSH DE
        [0xE5] = 11,   // PUSH HL
        [0xF5] = 11,   // PUSH AF
        // Pop
        [0xC1] = 10,   // POP BC
        [0xD1] = 10,   // POP DE
        [0xE1] = 10,   // POP HL
        [0xF1] = 10,   // POP AF
      
        // Halt
        [0x76] = 4, // HALT

        // Interrupt Control
        [0xF3] = 4, // DI
        [0xFB] = 4, // EI

        // Unconditional Jumps
        [0xC2] = 10,   // JP NZ, NN
        [0xC3] = 10,   // JP NN
        [0xCA] = 10,   // JP Z, NN
        [0xD2] = 10,   // JP NC, NN
        [0xDA] = 10,   // JP C, NN
        [0xE2] = 10,   // JP PO, NN
        [0xE9] = 4,    // JP (HL)
        [0xEA] = 10,   // JP PE, NN
        [0xF2] = 10,   // JP P, NN
        [0xFA] = 10,   // JP M, NN

        // Calls
        [0xC4] = 10,   // CALL NZ, NN (17/10 cycles)
        [0xCC] = 10,   // CALL Z, NN (17/10 cycles)
        [0xCD] = 10,   // CALL NN
        [0xD4] = 10,   // CALL NC, NN (17/10 cycles)
        [0xDC] = 10,   // CALL C, NN (17/10 cycles)
        [0xE4] = 10,   // CALL PO, NN (17/10 cycles)
        [0xEC] = 10,   // CALL PE, NN (17/10 cycles)
        [0xF4] = 10,   // CALL P, NN (17/10 cycles)
        [0xFC] = 10,   // CALL M, NN (17/10 cycles)

        // 8-Bit Return
        [0xC0] = 5,    // RET NZ (5/11 cycles)
        [0xC8] = 5,    // RET Z  (5/11 cycles)
        [0xC9] = 10,   // RET
        [0xD0] = 5,    // RET NC (5/11 cycles)
        [0xD8] = 5,    // RET C  (5/11 cycles)
        [0xE0] = 5,    // RET PO (5/11 cycles)
        [0xE8] = 5,    // RET PE (5/11 cycles)
        [0xF0] = 5,    // RET P (5/11 cycles)
        [0xF8] = 5,    // RET M (5/11 cycles)
    };

    // Stores cycle count for each opcode in ED opcode table
    int opcode_cycles_ED[256] = {
        // Sleep
        [0x76] = 8, // SLP (alias for HALT, Treat as HALT)
        // 24-Bit ED Pre-fix Instructions
        [0x00] = 12,  // IN0 B, (N) 
        [0x01] = 13,  // OUT0 (N), B
        // 16-Bit Return
        [0x45] = 14, // RETN (Treat as RET)
        [0x4D] = 14, // RETI (Treat as RET)
        // 16-Bit Interrupts
        [0x46] = 8, // IM 0
        [0x56] = 8, // IM 1
        [0x5E] = 8, // IM 2
    };

    // Stores cycle count for each opcode in CB opcode table
    int opcode_cycles_CB[256] = {
        // 16-Bit Shift Left Unsigned
        [0x20] = 8, // SLA B
        [0x21] = 8, // SLA C
        [0x22] = 8, // SLA D
        [0x23] = 8, // SLA E
        [0x24] = 8, // SLA H
        [0x25] = 8, // SLA L
        [0x26] = 15,// SLA (HL)
        [0x27] = 8, // SLA A
        // Shift Left Logical
        [0x30] = 8, // SLL B
        [0x31] = 8, // SLL C
        [0x32] = 8, // SLL D
        [0x33] = 8, // SLL E
        [0x34] = 8, // SLL H
        [0x35] = 8, // SLL L
        [0x36] = 8, // SLL (HL)
        [0x37] = 8, // SLL A
        // 16-Bit Shift Right Signed
        [0x28] = 8, // SRA B
        [0x29] = 8, // SRA C
        [0x2A] = 8, // SRA D
        [0x2B] = 8, // SRA E
        [0x2C] = 8, // SRA H
        [0x2D] = 8, // SRA L
        [0x2E] = 15,// SRA (HL)
        [0x2F] = 8, // SRA A
        // 16-Bit Shift Right Unsigned
        [0x38] = 8, // SRL B
        [0x39] = 8, // SRL C
        [0x3A] = 8, // SRL D    
        [0x3B] = 8, // SRL E
        [0x3C] = 8, // SRL H
        [0x3D] = 8, // SRL L
        [0x3E] = 15,// SRL (HL)
        [0x3F] = 8, // SRL A
    };

    // Each loop handles one instruction
    while(cycles_left > 0){
        opcode = z80_mem_read(cpu->PC);

        //printf("pre main fn cpu->PC: %X\n", z80_mem_read(cpu->PC));
        printf("current cycles: %d\n", cycles - cycles_left);

        if (opcode_cycles[opcode]) {
            cycles_left -= opcode_cycles[opcode];
            cpu->main_opcode = 1;
        }
        else if (opcode == 0xED) {
            cpu->R++;
            cpu->PC++;
            opcode = z80_mem_read(cpu->PC);
            cycles_left -= opcode_cycles_ED[opcode];
            cpu->ED_opcode = 1;
        }
        else if (opcode == 0xCB) {
            cpu->R++;
            cpu->PC++;
            opcode = z80_mem_read(cpu->PC);
            cycles_left -= opcode_cycles_CB[opcode];
            cpu->CB_opcode = 1;
        }
        else {
            printf("opcode: %02X\n", opcode);
            // If opcode isn't identified, treat as NOP
            opcode = 0x00;
            cycles_left -= 4;
            cpu->main_opcode = 1;
        }

        cpu->R += 1;    // Increments R register with each instruction
        cycles_left -= decode(opcode, cpu);    // Decodes and executes, decrements any conditional cycles the opcode needed
        cpu->PC++;      // Increment for next opcode

        // Reset opcode table identity flags
        cpu->main_opcode = 0;
        cpu->ED_opcode = 0;
        cpu->CB_opcode = 0;
        
        // <Interrupt check, perform any tasks that need to be synchronized with system clock>
        if (cpu->halted) {
            printf("Cycles left: %d\n", cycles_left);
            return cycles - cycles_left;
        }
    }

    return cycles - cycles_left;    // Returns the number of cycles ran
}