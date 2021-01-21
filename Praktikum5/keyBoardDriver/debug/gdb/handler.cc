
#include "stub.h"
#include "debug/output.h"
#include "debug/assert.h"
#include "machine/lapic.h"
#include "machine/plugbox.h"

static GDB_Stub *instance = 0;

static void *memset(void *ptr, int data, size_t len) {
    char *p = (char *) ptr;
    while (len--)
        *p++ = (char) data;
    return ptr;
}

extern "C" void debug_handler(struct debug_context *context) {
    if (instance == 0)
        return;
    struct GDB_Stub::state *state = &(instance->state);
    memset(&state->registers, 0, sizeof(state->registers));
    state->signum = context->vector;
    state->registers[GDB_Stub::REG_EAX] = context->eax;
    state->registers[GDB_Stub::REG_ECX] = context->ecx;
    state->registers[GDB_Stub::REG_EDX] = context->edx;
    state->registers[GDB_Stub::REG_EBX] = context->ebx;
    state->registers[GDB_Stub::REG_ESP] = context->esp;
    state->registers[GDB_Stub::REG_EBP] = context->ebp;
    state->registers[GDB_Stub::REG_ESI] = context->esi;
    state->registers[GDB_Stub::REG_EDI] = context->edi;
    state->registers[GDB_Stub::REG_PC] = context->eip;
    state->registers[GDB_Stub::REG_CS] = context->cs;
    state->registers[GDB_Stub::REG_PS] = context->eflags;
    state->registers[GDB_Stub::REG_SS] = context->ss;
    state->registers[GDB_Stub::REG_DS] = context->ds;
    state->registers[GDB_Stub::REG_ES] = context->es;
    state->registers[GDB_Stub::REG_FS] = context->fs;
    state->registers[GDB_Stub::REG_GS] = context->gs;
    instance->handle();
    context->eax = state->registers[GDB_Stub::REG_EAX];
    context->ecx = state->registers[GDB_Stub::REG_ECX];
    context->edx = state->registers[GDB_Stub::REG_EDX];
    context->ebx = state->registers[GDB_Stub::REG_EBX];
    context->esp = state->registers[GDB_Stub::REG_ESP];
    context->ebp = state->registers[GDB_Stub::REG_EBP];
    context->esi = state->registers[GDB_Stub::REG_ESI];
    context->edi = state->registers[GDB_Stub::REG_EDI];
    context->eip = state->registers[GDB_Stub::REG_PC];
    context->cs = state->registers[GDB_Stub::REG_CS];
    context->eflags = state->registers[GDB_Stub::REG_PS];
    context->ss = state->registers[GDB_Stub::REG_SS];
    context->ds = state->registers[GDB_Stub::REG_DS];
    context->es = state->registers[GDB_Stub::REG_ES];
    context->fs = state->registers[GDB_Stub::REG_FS];
    context->gs = state->registers[GDB_Stub::REG_GS];
    lapic.ackIRQ();
}

GDB_Stub::GDB_Stub(bool wait, bool debugOutput, Serial::comPort port, Serial::baudRate baudrate) : Serial(port,
                                                                                                          baudrate,
                                                                                                          DATA_8BIT,
                                                                                                          STOP_1BIT,
                                                                                                          PARITY_NONE),
                                                                                                   debug(debugOutput) {
    assert(instance == 0);
    instance = this;
    for (int i = 0; i < 17; i++)
        if (i != 2
            && i != 15
                ) {
            install_handler(i);
        }
    install_handler(Plugbox::serial);
    if (wait) {
        breakpoint();
    }
}
