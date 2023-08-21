static inline def_EHelper(jal)
{
    rtl_addi(s, ddest, s0, id_src1->imm);
    print_asm_template2(jal);
}