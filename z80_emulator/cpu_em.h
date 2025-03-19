#ifndef CPU_EM_H
#define CPU_EM_H

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

// Z80 status indicator flags
#define FLAG_C 0x01      // bit 0 (carry flag). (0x01 corresponds to bit 0)
#define FLAG_N 0x02      // bit 1 (add/subtract flag). (0x02 corresponds to bit 1)
#define FLAG_PV 0x04     // bit 2 (parity/overflow flag). (0x04 corresponds to bit 2)
#define FLAG_H 0x10      // bit 4 (half carry flag). (0x10 corresponds to bit 4)
#define FLAG_Z 0x40      // bit 6 (zero flag). (0x40 corresponds to bit 6)
#define FLAG_S 0x80      // bit 7 (sign flag). (0x80 corresponds to bit 7)
                         // Flag X not used for bits 3 and 5 so skipped

// Holds state info of CPU such as registers, flags, memory
typedef struct Z80 {
    uint8_t A, B, C, D, E, F, H, L;          // 8-bit registers
    uint8_t A2, B2, C2, D2, E2, F2;          // 8-bit alternate registers
    uint16_t AF, BC, DE, HL;                 // 16-bit register pairs
    uint8_t I, R;                            // Interrupt Vector Register and Memory Refresh Register
    uint16_t IY, IX;                         // Index Registers
    uint16_t SP, PC;                         // Stack Pointer and Program Counter
    uint8_t status_flags;
    uint8_t halted;                          // Halt flag (1 if halted)
    uint8_t IFF1;                            // Maskable interrupt 1
    uint8_t IFF2;                            // Maskable interrupt 2
    uint8_t IM;                              // Interrupt Mode: 0, 1, or 2
    uint8_t WZ;                              // Hidden math register
    uint8_t main_opcode, ED_opcode, CB_opcode;  // Flags to determine which switch case to use to call execution fn
} Z80;

void z80_mem_write(uint16_t addr, uint8_t value);
uint8_t z80_mem_read(uint16_t addr);
void z80_mem_write16(uint16_t addr, uint16_t value);
uint16_t z80_mem_read16(uint16_t addr);
void z80_mem_dump(const char* fn);
void z80_mem_load(const char* fn);
extern void z80_init(struct Z80* cpu);
extern int z80_execute(int cycles, struct Z80* cpu);
int decode(uint8_t opcode, struct Z80* cpu);

#endif // CPU_EM_H