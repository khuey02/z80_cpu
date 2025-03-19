Keara Huey CMSC 411 Project README


1. Project: z80 CPU Emulator

    This is a simplified version of a z80 cpu emulator using C.


2. Project Description:

    The general structure of this project is divided into 3 components: fetch, decode, and execute.

    Makefile Structure:

        cpu_em.c (cpu_em.h, decoder.h)
	
	        decoder.c (decoder.h, instructions.h)
	
		        arithmetic.c (instructions.h, decoder.h)

                shifts.c (instructions.h, decoder.h)
		
		        logic.c (instructions.h, decoder.h)
		
		        load_store.c (instructions.h, decoder.h)
		
		        jumps_interrupts.c (instructions.h, decoder.h)
		

3. How to Run:

    How to compile:

        $ make clean
        $ make

    How to create test files: 
        Using zasm https://k1.spdns.de/Develop/Projects/zasm/Distributions/

        $ nano <name>.asm
        $ zasm --reqcolon -b input.asm -o input.bin

    How to run:
        $ cpu_em <test_file>


4. Work Process and Struggles

    1. I started off taking notes on the project instructions and outlining everything I needed to implement.
       I began researching using the sources given. 

    2. I started off with professor Sebald's reference code for the cpu_em.c setup functions and I referenced the "How to Write a Computer Emulator" with their fetch loop structure.

    3. I focused on creating the structure of the program, dividing components into separate files for better organization. 
       I drew out a diagram to visualize the dependencies of the separated files and the overall flow of operation before writing up the makefile.
       I decided to use a struct for holding cpu info, storing the cycle counts in cpu_em.c, and used different arrays and flags to differentiate the opcode tables.

       I ran into issues with Windows not recognizing rm for the Makefile, so I switched to using del to clean the generated executables.

    4. I then focused on creating a skeleton for the project. I filled out all of the cycle counts, finished the basic loop functionality in cpu_em.c, the basic z80 struct in cpu_em.h, and
        created a large switch case for decoder.c to identify opcodes.
    
    5. I researched different command-line assemblers to use to create test files that the program would run and went with using zasm.
       I struggled with zasm only generated .asm files, and couldn't get it to convert it to a .bin file unless I manually changed it, but this method seemed to work fine.

    6. I then focused on implementing the opcodes and this is when I came across clrhome's z80 opcode tables which were extremely helpful for finding 
       opcode cycles and names. I used this table as a secondary tool to make sure I wasn't missing anything. I used the z80 Family CPU User Manual for flag logic.

       I ran into issues with my F flag register not matching the test outputs. It seemed like the X unused registers were set in the expected output of the test code along with the
       flags you would expect with the operations executed. In class this question was answered - startup values can be whatever for some of the registers.

    7. I then incrementally went through each section of opcode implementations, researching when necessary.


5. Components that I know aren't working correctly/not implemented:
    Not implemented: IX IY loads/increments/etc, JR Unconditional Jumps
    Not working: provided call test failed

6. Resources:

    Professor Sebald's code for starting framework and test files

    Clrhome Z80 Opcode Table, https://clrhome.org/table/

    Z80 Family CPU Manual, https://www.zilog.com/docs/z80/z80cpu_um.pdf
    
    The Undocumented Z80 Documented, http://z80.info/zip/z80-documented.pdf

    How to Write a Computer Emulator, http://www.emulation.org/EMUL8/HOWTO.html

    Makefile UMBC CMSC 202 Lecture Slides by Professor Dixon

    z80 Heaven, "Flags and Bit Level Instructions",  http://z80-heaven.wikidot.com/flags-and-bit-level-instructions

    BTEC National for IT Practitioners, "Computer Hardware, Introduction to Bit Masking", http://www.folder101.com/Control/Notes/BitMasking/BitMasking.htm

    C/Macros, https://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)Macros.html

    GNU/onlinedocs ,"The C Preprocessor: Macros", https://gcc.gnu.org/onlinedocs/gcc-3.0.1/cpp_3.html

    CodeMentor, "C/C++ Macro and Bit Operations", Hamza Braham, https://www.codementor.io/@hbendali/c-c-macro-bit-operations-ztrat0et6

    A discussion/descripton of half-carry, https://www.robm.dev/articles/gameboy-half-carry/


6. Supported Operations:

    8-bit Unsigned Arithmetic Addition
        [0x80]: ADD A, B    
        [0x81]: ADD A, C    
        [0x82]: ADD A, D    
        [0x83]: ADD A, E    
        [0x84]: ADD A, H
        [0x85]: ADD A, L     
        [0x86]: ADD A, (HL)
        [0x87]: ADD A, A
        [0xC6]: ADD A, N
    16-bit Unsigned Arithmetic Addition
        [0x09]: ADD HL, BC
        [0x19]: ADD HL, DE
        [0x29]: ADD HL, HL
        [0x39]: ADD HL, SP
    8-bit Arithmetic Addition with Carry
        [0x88]: ADC A, B
        [0x89]: ADC A, C
        [0x8A]: ADC A, D
        [0x8B]: ADC A, E
        [0x8C]: ADC A, H
        [0x8D]: ADC A, L
        [0x8E]: ADC A, (HL)
        [0x8F]: ADC A, A
        [0xCE]: ADC A, N
    8-bit Arithmetic Subtraction
        [0x90]: SUB B
        [0x91]: SUB C
        [0x92]: SUB D
        [0x93]: SUB E
        [0x94]: SUB H
        [0x95]: SUB L
        [0x96]: SUB (HL)
        [0x97]: SUB A
        [0xD6]: SUB N
    8-bit Arithmetic Subtraction with Carry
        [0x98]: SBC A, B
        [0x99]: SBC A, C
        [0x9A]: SBC A, D
        [0x9B]: SBC A, E
        [0x9C]: SBC A, H
        [0x9D]: SBC A, L
        [0x9E]: SBC A, (HL)
        [0x9F]: SBC A, A
        [0xDE]: SBC A, N
    8-Bit Increment
        [0x3C]: INC A
        [0x04]: INC B
        [0x0C]: INC C
        [0x14]: INC D
        [0x1C]: INC E
        [0x24]: INC H
        [0x2C]: INC L
        [0x34]: INC (HL)
    16-Bit Increment
        [0x03]: INC BC
        [0x13]: INC DE
        [0x23]: INC HL
        [0x33]: INC SP
    8-Bit Decrement
        [0x3D]: DEC A
        [0x05]: DEC B
        [0x0D]: DEC C
        [0x15]: DEC D
        [0x1D]: DEC E
        [0x25]: DEC H
        [0x2D]: DEC L
        [0x35]: DEC (HL)
    16-Bit Decrement
        [0x0B]: DEC BC
        [0x1B]: DEC DE
        [0x2B]: DEC HL
        [0x3B]: DEC SP
    NOP
        [0x00]: NOP
    16-Bit Shift Left Unsigned
        [0x20]: SLA B
        [0x21]: SLA C
        [0x22]: SLA D
        [0x23]: SLA E
        [0x24]: SLA H
        [0x25]: SLA L
        [0x26]: SLA (HL)
        [0x27]: SLA A
    16-Bit Shift Right Signed
        [0x28]: SRA B
        [0x29]: SRA C
        [0x2A]: SRA D
        [0x2B]: SRA E
        [0x2C]: SRA H 
        [0x2D]: SRA L
        [0x2E]: SRA (HL)
        [0x2F]: SRA A
    16-Bit Shift Left Logical
        [0x30] = 8, // SLL B
        [0x31] = 8, // SLL C
        [0x32] = 8, // SLL D
        [0x33] = 8, // SLL E
        [0x34] = 8, // SLL H
        [0x35] = 8, // SLL L
        [0x36] = 8, // SLL (HL)
        [0x37] = 8, // SLL A
    16-Bit Shift Right Unsigned
        [0x38]: SRL B
        [0x39]: SRL C
        [0x3A]: SRL D
        [0x3B]: SRL E
        [0x3C]: SRL H
        [0x3D]: SRL L
        [0x3E]: SRL (HL)
        [0x3F]: SRL A
    8-bit Bitwise AND
        [0xA0]: AND B
        [0xA1]: AND C
        [0xA2]: AND D
        [0xA3]: AND E
        [0xA4]: AND H
        [0xA5]: AND L
        [0xA6]: AND (HL)
        [0xA7]: AND A
        [0xE6]: AND N
    8-bit Bitwise OR
        [0xB0]: OR B
        [0xB1]: OR C
        [0xB2]: OR D
        [0xB3]: OR E
        [0xB4]: OR H
        [0xB5]: OR L
        [0xB6]: OR (HL)
        [0xB7]: OR A
        [0xF6]: OR N
   8-bit Bitwise XOR
        [0xA8]: XOR B
        [0xA9]: XOR C
        [0xAA]: XOR D
        [0xAB]: XOR E
        [0xAC]: XOR H
        [0xAD]: XOR L
        [0xAE]: XOR (HL)
        [0xAF]: XOR A
        [0xEE]: XOR N
   8-Bit Compare Operations
        [0xB8]: CP B
        [0xB9]: CP C
        [0xBA]: CP D
        [0xBB]: CP E
        [0xBC]: CP H
        [0xBD]: CP L
        [0xBE]: CP (HL)
        [0xBF]: CP A
        [0xFE]: CP N
    8-Bit Load/Store (Register to Register)
        [0x40]: LD B, B
        [0x41]: LD B, C
        [0x42]: LD B, D
        [0x43]: LD B, E
        [0x44]: LD B, H
        [0x45]: LD B, L
        [0x47]: LD B, A
        [0x48]: LD C, B
        [0x49]: LD C, C
        [0x4A]: LD C, D
        [0x4B]: LD C, E
        [0x4C]: LD C, H
        [0x4D]: LD C, L
        [0x4E]: LD c, (HL)
        [0x4F]: LD C, A
        [0x50]: LD D, B
        [0x51]: LD D, C
        [0x52]: LD D, D
        [0x53]: LD D, E
        [0x54]: LD D, H
        [0x55]: LD D, L
        [0x57]: LD D, A
        [0x58]: LD E, B
        [0x59]: LD E, C
        [0x5A]: LD E, D
        [0x5B]: LD E, E
        [0x5C]: LD E, H
        [0x5D]: LD E, L
        [0x5F]: LD E, A
        [0x60]: LD H, B
        [0x61]: LD H, C
        [0x62]: LD H, D
        [0x63]: LD H, E
        [0x64]: LD H, H
        [0x65]: LD H, L
        [0x67]: LD H, A
        [0x68]: LD L, B
        [0x69]: LD L, C
        [0x6A]: LD L, D
        [0x6B]: LD L, E
        [0x6C]: LD L, H
        [0x6D]: LD L, L
        [0x6F]: LD L, A
        [0x78]: LD A, B
        [0x79]: LD A, C
        [0x7A]: LD A, D
        [0x7B]: LD A, E
        [0x7C]: LD A, H
        [0x7D]: LD A, L
        [0x7F]: LD A, A
    8 bit Load/Store (Memory to Register)
        [0x46]: LD B, (HL)
        [0x4E]: LD C, (HL)
        [0x56]: LD D, (HL)
        [0x5E]: LD E, (HL)
        [0x66]: LD H, (HL)
        [0x6E]: LD L, (HL)
        [0x7E]: LD A, (HL)
    8-Bit Load/Store (Register to Memory)
        [0x70]: LD (HL), B
        [0x71]: LD (HL), C
        [0x72]: LD (HL), D
        [0x73]: LD (HL), E
        [0x74]: LD (HL), H
        [0x75]: LD (HL), L
        [0x77]: LD (HL), A
    Push
        [0xC5]: PUSH BC
        [0xD5]: PUSH DE
        [0xE5]: PUSH HL
        [0xF5]: PUSH AF
    Pop
        [0xC1]: POP BC
        [0xD1]: POP DE
        [0xE1]: POP HL
        [0xF1]: POP AF
    Halt
        [0x76]: HALT
    Sleep
        [0xED77]: SLP treat like HALT
    Print and Dump
        [0xED00]: IN0 B, (N)
        [0xED01]: OUT0 (N), B
    Interrupt Control
        [0xF3]: DI
        [0xFB]: EI
    16-Bit Interrupts
        [0x46]: IM 0
        [0x56]: IM 1
        [0x5E]: IM 2
    Unconditional Jumps
        [0xC2]: JP NZ, NN
        [0xC3]: JP NN
        [0xCA]: JP Z, NN
        [0xD2]: JP NC, NN
        [0xDA]: JP C, NN
        [0xE2]: JP PO, NN
        [0xE9]: JP (HL)
        [0xEA]: JP PE, NN
        [0xF2]: JP P, NN
        [0xFA]: JP M, NN
    Calls
        [0xC4]: CALL NZ, NN (17/10 cycles)
        [0xCC]: CALL Z, NN (17/10 cycles)
        [0xCD]: CALL NN
        [0xD4]: CALL NC, NN (17/10 cycles)
        [0xDC]: CALL C, NN (17/10 cycles)
        [0xE4]: CALL PO, NN (17/10 cycles)
        [0xEC]: CALL PE, NN (17/10 cycles)
        [0xF4]: CALL P, NN (17/10 cycles)
        [0xFC]: CALL M, NN (17/10 cycles)
    8-Bit Return
        [0xC0]: RET NZ
        [0xC8]: RET Z
        [0xC9]: RET
        [0xD0]: RET NC
        [0xD8]: RET C
        [0xE0]: RET PO
        [0xE8]: RET PE
        [0xF0]: RET P
        [0xF8]: RET M
    16-Bit Return
        [0x45]: RETN (Treat as RET)
        [0x4D]: RETI (Treat as RET)