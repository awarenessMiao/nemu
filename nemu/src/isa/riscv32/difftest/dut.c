#include <isa.h>
#include <monitor/difftest.h>
#include "../local-include/reg.h"
#include "difftest.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  // if (ref_r->pc != cpu.pc)
  //   return false;
  // for (int i = 0; i < 32; i++) {
  //   if (ref_r->gpr[i]._32 != cpu.gpr[i]._32)
  //     return false;
  // }
  return false;
}

void isa_difftest_attach() {
}
