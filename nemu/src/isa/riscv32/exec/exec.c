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
    EXW  (1, lds, 2) //lh
    EXW  (0, lds, 1) //lb
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

static inline def_EHelper(srli_srai){
  switch(s->isa.instr.r.funct7){
    EX   (0b0100000, srai)
    EX   (0b0000000, srli)
    default: exec_inv(s);
  }
}

static inline def_EHelper(op_imm) {
  switch (s->isa.instr.i.funct3) {
    EX   (0, addi)
    EX   (1, slli)
    EX   (2, slti)
    EX   (3, sltiu)
    EX   (4, xori)
    EX   (5, srli_srai)
    EX   (6, ori)
    EX   (7, andi)
  default: exec_inv(s);
  }
}

static inline def_EHelper(add_sub) {
  switch (s->isa.instr.r.funct7) {
    EX   (0b0000000, add)
    EX   (0b0100000, sub) 
    default: exec_inv(s);
  }
}

static inline def_EHelper(srl_sra){
  switch(s->isa.instr.r.funct7){
    EX   (0b0100000, sra)
    EX   (0b0000000, srl)
    default: exec_inv(s);
  }
}

static inline def_EHelper(op_base) {
  switch (s->isa.instr.r.funct3) {
    EX   (0, add_sub) 
    EX   (1, sll)
    EX   (2, slt)
    EX   (3, sltu)
    EX   (4, xor)
    EX   (5, srl_sra)
    EX   (6, or)
    EX   (7, and)
    default: exec_inv(s);
  }
}

static inline def_EHelper(op_muldiv) {
  switch (s->isa.instr.r.funct3) {
    EX   (0, mul)
    // EX   (1, mulh)
    // EX   (2, mulhsu)
    // EX   (3, mulhu)
    // EX   (4, div)
    // EX   (5, divu)
    // EX   (6, rem)
    // EX   (7, remu)
    default: exec_inv(s);
  }
}

static inline def_EHelper(op) {
  switch (s->isa.instr.r.funct7) {
    EX   (0b0000000, op_base)
    EX   (0b0100000, op_base)
    EX   (0b0000001, op_muldiv)
    default: exec_inv(s);
  }
}

static inline def_EHelper(branch){
  switch(s->isa.instr.b.funct3){
    EX   (0, beq)
    EX   (1, bne)
    EX   (4, blt)
    EX   (5, bge)
    EX   (6, bltu)
    EX   (7, bgeu)
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
    IDEX (0b11000, B, branch)
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
