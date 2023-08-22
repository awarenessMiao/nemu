static inline def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
  print_asm_template2(lui);
}

static inline def_EHelper(addi){
  rtl_addi(s, ddest, dsrc1, id_src2->simm);
  print_asm_template3(addi);
}

static inline def_EHelper(add){
  rtl_add(s, ddest, dsrc1, dsrc2);
  print_asm_template3(add);
}

static inline def_EHelper(sub){
  rtl_sub(s, ddest, dsrc1, dsrc2);
  print_asm_template3(sub);
}

static inline def_EHelper(auipc){
  rtl_li(s, s0, cpu.pc);
  rtl_addi(s, ddest, s0, id_src1->imm);
  print_asm_template2(auipc);
}

