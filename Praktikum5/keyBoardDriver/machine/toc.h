
#pragma once
struct toc {
public:
    void *ebx;
    void *esi;
    void *edi;
    void *ebp;
    void *esp;
} __attribute__ ((packed));

class Thread;

void toc_settle(struct toc *regs, void *tos, void (*kickoff)(Thread *),
                Thread *object);
extern "C" {
void toc_go(struct toc *regs);
void toc_switch(struct toc *regs_now, struct toc *reg_then);
}
