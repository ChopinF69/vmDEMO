#include "birchvm_test.h"
#include "../include/birchvm.h"
#include <assert.h>
#include <stdio.h>

Program exProgram() {
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

void testMovAddSub() {
  printf("%s\n", "Test with mov , add , sub");
  Program program = exProgram();
  VM *vm = virtualMachine(program, 3);
  executeVM(vm);
  assert(vm->cpu.r.ax == 6);

  free(vm->workingProgram);
  free(vm);
}

void testMulDiv() {
  printf("%s\n", "Test with mul , div");
  Program program = exProgram();

  Instruction i1, i2, i3, i4;
  i1 = (Instruction)malloc(sizeof(struct s_instruction) + 2 * sizeof(Args));
  i1->o = mov;
  i1->a[0] = 0x00; // ax register
  i1->a[1] = 0x04; // value of 4

  i2 = (Instruction)malloc(sizeof(struct s_instruction) + 2 * sizeof(Args));
  i2->o = mul;
  i2->a[0] = 0x00; // ax register
  i2->a[1] = 0x03; // we multiply by 3 : ax = 4 * 3 -> 12

  i3 = (Instruction)malloc(sizeof(struct s_instruction) + 2 * sizeof(Args));
  i3->o = divv;
  i3->a[0] = 0x00; // ax register
  i3->a[1] = 0x04; // we divide by 4 : ax = 12 / 4 -> 3

  i4 = (Instruction)malloc(sizeof(struct s_instruction) + 0 * sizeof(Args));
  i4->o = hlt;

  program[0] = i1;
  program[1] = i2;
  program[2] = i3;
  program[3] = i4;

  VM *vm = virtualMachine(program, 5);
  vm->workingProgram = program;
  executeVM(vm);

  assert(vm->cpu.r.ax == 3);

  free(vm->workingProgram);
  free(vm);
  for (int i = 0; i < 3; ++i) {
    free(program[i]);
  }
}

void testAll() {
  printf("Doing tests...");
  testMovAddSub();
  printf("%s\n", "Tests for mov, add, sub passed!\n");
  testMulDiv();
  printf("%s\n", "Tests for mul , div passed !\n");
}
