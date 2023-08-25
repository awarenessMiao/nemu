static inline def_EHelper(jal)
{
    rtl_li(s, s0, cpu.pc);
    rtl_addi(s, s1, s0, id_src1->simm);
    rtl_addi(s, ddest, s0, 4);
    rtl_jr(s, s1);
    print_asm_template2(jal);
}

static inline def_EHelper(jalr){
    rtl_li(s, s1, cpu.pc);
    rtl_addi(s, ddest, s1, 4);
    rtl_addi(s, s0, dsrc1, id_src2->simm);
    rtl_andi(s, s1, s0, 0xfffffffe);
    rtl_jr(s, s1);
    print_asm_template3(jalr);
}

static inline def_EHelper(beq){
    vaddr_t tmp = id_dest->simm + cpu.pc;
    rtl_jrelop(s, RELOP_EQ, dsrc1, dsrc2, tmp);
    print_asm_template3(beq);
}

static inline def_EHelper(bne){
    vaddr_t tmp = id_dest->simm + cpu.pc;
    rtl_jrelop(s, RELOP_NE, dsrc1, dsrc2, tmp);
    print_asm_template3(bne);
}

static inline def_EHelper(bge){
    vaddr_t tmp = id_dest->simm + cpu.pc;
    rtl_jrelop(s, RELOP_GE, dsrc1, dsrc2, tmp);
    print_asm_template3(bge);
}

static inline def_EHelper(bgeu){
    vaddr_t tmp = id_dest->simm + cpu.pc;
    rtl_jrelop(s, RELOP_GEU, dsrc1, dsrc2, tmp);
    print_asm_template3(bgeu);
}

static inline def_EHelper(blt){
    vaddr_t tmp = id_dest->simm + cpu.pc;
    rtl_jrelop(s, RELOP_LT, dsrc1, dsrc2, tmp);
    print_asm_template3(blt);
}

static inline def_EHelper(bltu){
    vaddr_t tmp = id_dest->simm + cpu.pc;
    rtl_jrelop(s, RELOP_LTU, dsrc1, dsrc2, tmp);
    print_asm_template3(bltu);
}