static inline def_EHelper(mul){
  rtl_mul_lo(s, ddest, dsrc1, dsrc2);
  print_asm_template3(mul);
}

static inline def_EHelper(div){
  if (*dsrc2 == 0)
    rtl_li(s, ddest, 0xFFFFFFFF);
  else if (*dsrc1 == 0x80000000 && *dsrc2 == -1)
    rtl_li(s, ddest, 0x80000000);
  else
    rtl_idiv_q(s, ddest, dsrc1, dsrc2);
  print_asm_template3(div);
}

static inline def_EHelper(rem){
  if (*dsrc2 == 0)
    rtl_mv(s, ddest, dsrc1);
  else if (*dsrc1 == 0x80000000 && *dsrc2 == -1)
    rtl_li(s, ddest, 0);
  else
    rtl_idiv_r(s, ddest, dsrc1, dsrc2);
  print_asm_template3(rem);
}