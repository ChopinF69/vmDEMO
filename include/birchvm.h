#ifndef BIRCHVM_H
#define BIRCHVM_H

#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ErrMem 0x01
#define NoArgs {0x00, 0x00}
#define ErrSegv 0x02
#define VM_MEMORY_SIZE 0x10000 // 64 KB memory
#define segFault(x) error((x), ErrSegv)

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;
typedef unsigned long long int64;
typedef unsigned short int Reg;

struct s_registers {
  union {
    struct {
      union {
        struct {
          uint8_t al; // Low 8 bits of AX
          uint8_t ah; // High 8 bits of AX
        };
        uint16_t ax; // 16-bit AX register (composed of al and ah)
      };
      union {
        struct {
          uint8_t bl; // Low 8 bits of BX
          uint8_t bh; // High 8 bits of BX
        };
        uint16_t bx; // 16-bit BX register (composed of bl and bh)
      };
      union {
        struct {
          uint8_t cl; // Low 8 bits of CX
          uint8_t ch; // High 8 bits of CX
        };
        uint16_t cx; // 16-bit CX register (composed of cl and ch)
      };
      union {
        struct {
          uint8_t dl; // Low 8 bits of DX
          uint8_t dh; // High 8 bits of DX
        };
        uint16_t dx; // 16-bit DX register (composed of dl and dh)
      };
    };
    struct {
      uint32_t eax; // 32-bit extended AX register
      uint32_t ebx; // 32-bit extended BX register
      uint32_t ecx; // 32-bit extended CX register
      uint32_t edx; // 32-bit extended DX register
    };
  };
  uint16_t sp; // Stack Pointer (16-bit)
  uint16_t ip; // Instruction Pointer (16-bit)
};

typedef struct s_registers Registers;

struct s_cpu {
  Registers r;
  int8 interruptFlag; // 1 for enabled, 0 for disabled
};
typedef struct s_cpu CPU;

// Opcode definitions
// enum e_upcode { mov = 0x01, nop = 0x02, hlt = 0x03, add = 0x04, sub = 0x05 };
enum e_upcode {
  mov = 0x01,
  nop = 0x02,
  hlt = 0x03,
  add = 0x04,
  sub = 0x05,
  cli = 0x06,
  sti = 0x07,
  mul = 0x08,
  divv = 0x09
};
typedef enum e_upcode Opcode;

// Instruction mapping structure
struct s_instrmap {
  Opcode o;
  int8 size;
};
typedef struct s_instrmap IM;

typedef int8 Args;

// Instruction structure
struct s_instruction {
  Opcode o;
  Args a[]; // Variable-length array for arguments
};
typedef struct s_instruction *Instruction;

typedef unsigned char ErrorCode;
typedef Instruction *Program; // a program is an array of instructions

struct s_vm {
  CPU cpu;
  int8 memory[VM_MEMORY_SIZE];
  int16 breakPoint;
  Program workingProgram;
};

typedef struct s_vm VM;

void executeVM(VM *);
Program exampleProgram(void);
int8 map(Opcode);
VM *virtualMachine(Program currentProgram, int16 programSize);
void error(VM *, ErrorCode);
void printOutRegisters(VM *);

#endif // BIRCHVM_H
