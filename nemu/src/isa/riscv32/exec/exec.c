#include <cpu/exec.h>
#include "../local-include/decode.h"
#include "all-instr.h"

static inline void set_width(DecodeExecState *s, int width) {
  if (width != 0) s->width = width;
}

static inline def_EHelper(load) {
  switch (s->isa.instr.i.funct3) {
    EXW  (5, ld, 2) //lhu
    EXW  (4, ld, 1) //lbu
    EXW  (2, ld, 4) //lw
    default: exec_inv(s);
  }
}

static inline def_EHelper(store) {
  switch (s->isa.instr.s.funct3) {
    EXW  (0, st, 1) //sb
    EXW  (1, st, 2) //sh
    EXW  (2, st, 4) //sw
    default: exec_inv(s);
  }
}

static inline def_EHelper(op_imm) {
  switch (s->isa.instr.i.funct3) {
    EX   (0, addi)
    EX   (3, sltiu)
  default:
    exec_inv(s);
  }
}

static inline def_EHelper(add_sub) {
  switch (s->isa.instr.r.funct7) {
    EX   (0b0000000, add)
    EX   (0b0100000, sub) 
    default: exec_inv(s);
  }
}

static inline def_EHelper(op) {
  switch (s->isa.instr.r.funct3) {
    EX   (0, add_sub) 
    default: exec_inv(s);
  }
}


static inline void fetch_decode_exec(DecodeExecState *s) {
  Log("executing %#x", s->seq_pc);
  s->isa.instr.val = instr_fetch(&s->seq_pc, 4);
  Assert(s->isa.instr.i.opcode1_0 == 0x3, "Invalid instruction");
  switch (s->isa.instr.i.opcode6_2) {
    IDEX (0b00000, I, load)
    IDEX (0b01000, S, store)
    IDEX (0b00100, I, op_imm) //包括一系列指令，如addi，通过func3确定细分
    IDEX (0b01100, R, op) // 也是一系列，add included
    IDEX (0b00101, U, auipc)
    IDEX (0b01101, U, lui)
    IDEX (0b11011, J, jal)
    IDEX (0b11001, I, jalr)
    // IDEX (0b11000, B, branch)
    EX   (0b11010, nemu_trap)
    default: exec_inv(s);
  }
}

static inline void reset_zero() {
  reg_l(0) = 0;
}

vaddr_t isa_exec_once() {
  DecodeExecState s;
  s.is_jmp = 0;
  s.seq_pc = cpu.pc;

  fetch_decode_exec(&s);
  update_pc(&s);

  reset_zero();

  return s.seq_pc;
}
