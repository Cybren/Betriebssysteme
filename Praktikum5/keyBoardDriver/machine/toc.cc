
#include "machine/toc.h"

void toc_settle(struct toc *regs, void *tos, void (*kickoff)(Thread *), Thread *object) {
    void **sp = (void **) tos;
    *(--sp) = object;
    *(--sp) = (void *) 0xdeadbeef;
    *(--sp) = (void *) kickoff;
    regs->ebx = 0;
    regs->esi = 0;
    regs->edi = 0;
    regs->ebp = 0;
    regs->esp = sp;
}

