#include "../include/birchvm.h"
// #include "test/birchvm_test.h"
#include "../include/utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static IM instrMap[] = {
    {mov, 0x03}, // mov has 2 arguments
    {nop, 0x01}, // nop has no arguments
    {hlt, 0x03}, // hlt has no arguments but still uses opcode
    {add, 0x03}, // add has 2 arguments: register and value
    {sub, 0x03}  // sub has 2 arguments: register and value
};

#define IMsize ((sizeof(instrMap)) / (sizeof(struct s_instrmap)))

int8 map(Opcode currentOpcode) {
  for (int8 n = IMsize; n > 0; n--) {
    if (instrMap[IMsize - n].o == currentOpcode) {
      return instrMap[IMsize - n].size;
    }
  }
  return 0;
}

Program exampleProgram() {
  static Instruction prog[7];
  Instruction i1, i2, i3, i4, i5, i6, i7;

  i1 = (Instruction)malloc(sizeof(struct s_instruction) + 2 * sizeof(Args));
  i1->o = mov;
  i1->a[0] = 0x00; // ax
  i1->a[1] = 0x05; // set ax to 5

  i2 = (Instruction)malloc(sizeof(struct s_instruction) + 0 * sizeof(Args));
  i2->o = cli; // Disable interrupts

  i3 = (Instruction)malloc(sizeof(struct s_instruction) + 2 * sizeof(Args));
  i3->o = add;
  i3->a[0] = 0x00; // ax
  i3->a[1] = 0x03; // add 3 to ax (ax becomes 8)

  i4 = (Instruction)malloc(sizeof(struct s_instruction) + 0 * sizeof(Args));
  i4->o = sti; // Enable interrupts

  i5 = (Instruction)malloc(sizeof(struct s_instruction) + 2 * sizeof(Args));
  i5->o = sub;
  i5->a[0] = 0x00; // ax
  i5->a[1] = 0x02; // subtract 2 from ax (ax becomes 6)

  i6 = (Instruction)malloc(sizeof(struct s_instruction) + 0 * sizeof(Args));
  i6->o = nop; // No operation

  i7 = (Instruction)malloc(sizeof(struct s_instruction) + 0 * sizeof(Args));
  i7->o = hlt; // Halting

  prog[0] = i1;
  prog[1] = i2;
  prog[2] = i3;
  prog[3] = i4;
  prog[4] = i5;
  prog[5] = i6;
  prog[6] = i7;

  return prog;
}

VM *virtualMachine(Program currentProgram, int16 programSize) {
  VM *currentVM = (VM *)malloc(sizeof(struct s_vm));
  if (!currentVM) {
    errno = ErrMem;
    return NULL;
  }

  memset(currentVM, 0, sizeof(struct s_vm));
  currentVM->workingProgram =
      (Program)malloc(programSize * sizeof(Instruction));

  if (!currentVM->workingProgram) {
    free(currentVM);
    errno = ErrMem;
    return NULL;
  }

  // Copy the instructions to the virtual machine's program
  int memoryAddress = 0; // Starting memory address
  for (int16 i = 0; i < programSize; i++) {
    currentVM->workingProgram[i] = currentProgram[i];
    printf("Executing = %d\n", currentProgram[i]->o);

    // We will store the upcode too
    if (memoryAddress < VM_MEMORY_SIZE) {
      currentVM->memory[memoryAddress++] = currentProgram[i]->o;
    } else {
      fprintf(stderr, "Memory overflow error.\n");
      free(currentVM->workingProgram);
      free(currentVM);
      return NULL;
    }

    // Write instruction arguments to VM memory, sequentially
    // We store each argument of the upcode in the VM memory
    for (int8 j = 0; j < map(currentProgram[i]->o) - 1; j++) {
      if (memoryAddress < VM_MEMORY_SIZE) {
        currentVM->memory[memoryAddress++] = currentProgram[i]->a[j];
      } else {
        fprintf(stderr, "Memory overflow error.\n");
        free(currentVM->workingProgram);
        free(currentVM);
        return NULL;
      }
    }
  }

  // Here we will but the break point
  currentVM->breakPoint = (int16)memoryAddress;
  return currentVM;
}

void executeVM(VM *vm) {
  assert(vm && vm->workingProgram);

  int16 instructionPointer = 0;
  while (instructionPointer < vm->breakPoint) {
    Opcode opcode = (Opcode)vm->memory[instructionPointer++];

    switch (opcode) {
    case mov: {
      int8 reg = vm->memory[instructionPointer++];   // The actual register
      int8 value = vm->memory[instructionPointer++]; // The value to be put in
                                                     // the register

      if (reg == 0)
        vm->cpu.r.ax = value;
      else if (reg == 1)
        vm->cpu.r.bx = value;
      else if (reg == 2)
        vm->cpu.r.cx = value;
      else if (reg == 3)
        vm->cpu.r.dx = value;
      else {
        fprintf(stderr, "Invalid register specified in mov operation\n");
        segFault(vm);
      }
      break;
    }
    case add: {
      int8 reg = vm->memory[instructionPointer++];
      int8 value = vm->memory[instructionPointer++];

      if (reg == 0)
        vm->cpu.r.ax += value;
      else if (reg == 1)
        vm->cpu.r.bx += value;
      else if (reg == 2)
        vm->cpu.r.cx += value;
      else if (reg == 3)
        vm->cpu.r.dx += value;
      else {
        fprintf(stderr, "Invalid register specified\n");
        segFault(vm);
        return;
      }
      break;
    }
    case sub: {
      int8 reg = vm->memory[instructionPointer++];
      int8 value = vm->memory[instructionPointer++];

      if (reg == 0)
        vm->cpu.r.ax -= value;
      else if (reg == 1)
        vm->cpu.r.bx -= value;
      else if (reg == 2)
        vm->cpu.r.cx -= value;
      else if (reg == 3)
        vm->cpu.r.dx -= value;
      else {
        fprintf(stderr, "Invalid register specified\n");
        segFault(vm);
        return;
      }
      break;
    }
    case mul: {
      int8 reg = vm->memory[instructionPointer++];
      int8 value = vm->memory[instructionPointer++];
      if (reg == 0)
        vm->cpu.r.ax *= value;
      else if (reg == 1)
        vm->cpu.r.bx *= value;
      else if (reg == 2)
        vm->cpu.r.cx *= value;
      else if (reg == 3)
        vm->cpu.r.dx *= value;
      else {
        fprintf(stderr, "Invalid register specified\n");
        segFault(vm);
        return;
      }
      break;
    }
    case divv: {
      int8 reg = vm->memory[instructionPointer++];
      int8 value = vm->memory[instructionPointer++];
      if (reg == 0)
        vm->cpu.r.ax /= value;
      else if (reg == 1)
        vm->cpu.r.bx /= value;
      else if (reg == 2)
        vm->cpu.r.cx /= value;
      else if (reg == 3)
        vm->cpu.r.dx /= value;
      else {
        fprintf(stderr, "Invalid register specified\n");
        segFault(vm);
        return;
      }
      break;
    }
    case cli:
      vm->cpu.interruptFlag = 0;
      printf("Interrupt flag diabled");
      break;
    case sti:
      vm->cpu.interruptFlag = 1;
      printf("Interrupt flag enabled");
      break;
    case nop:
      // Simply a no-op; continues to the next instruction
      break;
    case hlt:
      if (vm->cpu.interruptFlag) {
        printf("Halting execution\n");
        return;
      } else {
        printf("Cannot halt ; Interrupts are enabled");
      }
      break;
    default:
      fprintf(stderr, "Unknown opcode: 0x%x\n", opcode);
      segFault(vm);
      return;
    }
  }
  if (instructionPointer == vm->breakPoint) {
    printf("Did execute the command properly!\n");
  }
  if (instructionPointer > vm->breakPoint)
    segFault(vm);
}

void error(VM *vm, ErrorCode e) {
  if (vm) {
    if (vm->workingProgram)
      free(vm->workingProgram);
    free(vm);
  }
  switch (e) {
  case ErrSegv:
    fprintf(stderr, "%s\n", "VM segmentation fault");
    break;
  default:
    break;
  }
  exit(-1);
}

void printOutRegisters(VM *vm) {
  printf("Value of register AX : %d\n", vm->cpu.r.ax);
  printf("Value of register BX : %d\n", vm->cpu.r.bx);
  printf("Value of register CX : %d\n", vm->cpu.r.cx);
  printf("Value of register DX : %d\n", vm->cpu.r.dx);
  printf("Value of register SP : %d\n", vm->cpu.r.sp);
  printf("Value of register IP : %d\n", vm->cpu.r.ip);
}

int main(int argc, char *argv[]) {
  Program program = exampleProgram();
  VM *vm;

  if (!program) {
    fprintf(stderr, "Failed to create example program.\n");
    return EXIT_FAILURE;
  }

  printf("Pointer at program = %p\n", (void *)program);

  int8 size2Instructions = map(mov) + map(nop) + map(hlt);
  vm = virtualMachine(program, 3); // Pass number of instructions

  if (vm) {
    for (int i = 0; i < size2Instructions; i++) {
      printf("memory[%d] = 0x%x\n", i, vm->memory[i]);
    }

    executeVM(vm);

    printOutRegisters(vm);

    free(vm->workingProgram);
    free(vm);
  } else {
    fprintf(stderr, "Failed to create virtual machine.\n");
  }

  for (int i = 0; i < 2; i++) {
    free(program[i]);
  }

  return EXIT_SUCCESS;
}
