#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdio.h>
#include <stdint.h>
#include "cpu_em.h"

// arithmetic.c
void ADD_or_ADC(uint8_t operand, struct Z80* cpu);
void update_flags_add_8bit(uint8_t A, uint8_t B, uint8_t result, struct Z80* cpu);
void update_flags_add_16bit(uint16_t A, uint16_t B, uint16_t result, struct Z80* cpu);
void SUB_or_SBC(uint8_t operand, struct Z80* cpu);
void update_flags_sub_8bit(uint8_t A, uint8_t B, uint8_t result, struct Z80* cpu);
void INC(uint8_t operand, struct Z80* cpu);
void update_flags_inc_8bit(uint8_t A, uint8_t result, struct Z80* cpu);
void update_flags_inc_16bit(uint16_t A, uint16_t result, struct Z80* cpu);
void DEC(uint8_t operand, struct Z80* cpu);
void update_flags_dec_8bit(uint8_t A, uint8_t result, struct Z80* cpu);
void DAA(struct Z80* cpu);
void NOP(uint8_t operand, struct Z80* cpu);

// shifts.c
void SLA(uint8_t operand, struct Z80* cpu);
void update_flags_sla(uint8_t A, uint8_t result, struct Z80* cpu);
void SLL(uint8_t operand, struct Z80* cpu);
void update_flags_sll(uint8_t A, uint8_t result, struct Z80* cpu);
void SRA(uint8_t operand, struct Z80* cpu);
void update_flags_sra(uint8_t A, uint8_t result, struct Z80* cpu);
void SRL(uint8_t operand, struct Z80* cpu);
void update_flags_srl(uint8_t A, uint8_t result, struct Z80* cpu);

// logic.c
void AND(uint8_t operand, struct Z80* cpu);
void update_flags_and(uint8_t result, struct Z80* cpu);
void OR(uint8_t operand, struct Z80* cpu);
void update_flags_or(uint8_t A, uint8_t B, uint8_t result, struct Z80* cpu);
void XOR(uint8_t operand, struct Z80* cpu);
void update_flags_xor(uint8_t result, struct Z80* cpu);
void CP(uint8_t operand, struct Z80* cpu);
void update_flags_cp(uint8_t A, uint8_t B, uint8_t result, struct Z80* cpu);

// load_store.c
void reg_to_reg_load(uint8_t operand, struct Z80* cpu);
void mem_to_reg_load(uint8_t operand, struct Z80* cpu);
void reg_to_mem_load(uint8_t operand, struct Z80* cpu);
void PUSH(uint8_t operand, struct Z80* cpu);
void POP(uint8_t operand, struct Z80* cpu);

// jumps_interrupts.c
void HALT(struct Z80* cpu);
void DI(struct Z80* cpu);
void EI(struct Z80* cpu);
void IM0(struct Z80* cpu);
void IM1(struct Z80* cpu);
void IM2(struct Z80* cpu);
void JP(uint8_t opcode, struct Z80* cpu);
int CALL(uint8_t opcode, struct Z80* cpu);
int RET(uint8_t opcode, struct Z80* cpu);
void IN(struct Z80* cpu);
void OUT(struct Z80* cpu);

#endif // INSTRUCTIONS_H
