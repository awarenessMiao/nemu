static inline def_EHelper(jal)
{
    rtl_li(s, s0, cpu.pc);
    rtl_addi(s, s1, s0, id_src1->simm);
    rtl_addi(s, ddest, s0, 4);
    rtl_jr(s, s1);
    print_asm_template2(jal);
}