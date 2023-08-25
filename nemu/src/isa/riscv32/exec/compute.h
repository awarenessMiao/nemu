static inline def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
  print_asm_template2(lui);
}

static inline def_EHelper(addi){
  rtl_addi(s, ddest, dsrc1, id_src2->simm);
  print_asm_template3(addi);
}

static inline def_EHelper(xori){
  rtl_xori(s, ddest, dsrc1, id_src2->simm);
  print_asm_template3(xori);
}

static inline def_EHelper(ori){
  rtl_ori(s, ddest, dsrc1, id_src2->simm);
  print_asm_template3(ori);
}

static inline def_EHelper(andi){
  rtl_andi(s, ddest, dsrc1, id_src2->simm);
  print_asm_template3(andi);
}

static inline def_EHelper(slli){
  // word_t tmp = id_src2->simm & 0b11111;
  rtl_li(s, s0, id_src2->simm);
  rtl_andi(s, s1, s0, 0b11111);
  rtl_shl(s, ddest, dsrc1, s1);
  print_asm_template3(slli);
}

static inline def_EHelper(sltiu){
  rtl_setrelopi(s, RELOP_LTU, ddest, dsrc1, id_src2->imm);
  print_asm_template3(sltiu);
}

static inline def_EHelper(srai){
  rtl_li(s, s0, id_src2->simm);
  rtl_andi(s, s1, s0, 0b11111);
  rtl_sar(s, ddest, dsrc1, s1);
  print_asm_template3(srai);
}

static inline def_EHelper(sltu){
  rtl_setrelop(s, RELOP_LTU, ddest, dsrc1, dsrc2);
  print_asm_template3(sltu);
}

static inline def_EHelper(slt){
  rtl_setrelop(s, RELOP_LT, ddest, dsrc1, dsrc2);
  print_asm_template3(slt);
}

static inline def_EHelper(add){
  rtl_add(s, ddest, dsrc1, dsrc2);
  print_asm_template3(add);
}

static inline def_EHelper(and){
  rtl_and(s, ddest, dsrc1, dsrc2);
  print_asm_template3(and);
}

static inline def_EHelper(or){
  rtl_or(s, ddest, dsrc1, dsrc2);
  print_asm_template3(or);
}

static inline def_EHelper(xor){
  rtl_xor(s, ddest, dsrc1, dsrc2);
  print_asm_template3(xor);
}

static inline def_EHelper(sub){
  rtl_sub(s, ddest, dsrc1, dsrc2);
  print_asm_template3(sub);
}

static inline def_EHelper(sll){
  rtl_andi(s, s0, dsrc2, 0b11111);
  rtl_shl(s, ddest, dsrc1, s0);
  print_asm_template3(sll);
}

static inline def_EHelper(auipc){
  rtl_li(s, s0, cpu.pc);
  rtl_addi(s, ddest, s0, id_src1->imm);
  print_asm_template2(auipc);
}

