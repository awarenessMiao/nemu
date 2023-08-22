static inline def_EHelper(jal)
{
    rtl_li(s, s0, s->seq_pc);
    rtl_addi(s, s1, s0, id_src1->simm);
    rtl_jr(s, s1);
    rtl_addi(s, ddest, s0, 4);
    print_asm_template2(jal);
}