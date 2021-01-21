
#pragma once
#include "types.h"

class CPU {
private:
    CPU(const CPU &) = delete;
    CPU &operator=(const CPU &) = delete;

    CPU() {}

public:
    static void enable_int() {
        asm volatile("sti\n\t nop\n\t" : : : "memory");
    }

    static bool disable_int() {
        uint32_t out;
        asm volatile (
        "pushf\n\t"
        "pop %0\n\t"
        "cli\n\t"
        : "=r"(out)
        :
        : "memory"
        );

        bool enabled = (out & 0x200) != 0;
        return enabled;
    }

    static void restore_int(bool val) {
        if (val) {
            enable_int();
        }
    }

    static void idle() {
        asm volatile("sti\n\t hlt\n\t" : : : "memory");
    }

    static void die() {
        asm volatile("cli\n\t hlt\n\t" : : : "memory");
    }

    static void halt() {
        asm volatile("hlt\n\t" : : : "memory");
    }

    static uint64_t rdmsr(uint32_t id) {
        uint64_t retval;
        asm volatile("rdmsr" : "=A"(retval) : "c"(id) : "memory");
        return retval;
    }

    static void wrmsr(uint32_t id, uint64_t val) {
        asm volatile("wrmsr" : : "A"(val), "c"(id) : "memory");
    }
};

struct irq_context {
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
} __attribute__((packed));
struct irq_context_without_error_code {
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t user_esp;
    uint32_t user_ss;
} __attribute__((packed));
struct irq_context_with_error_code {
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t errorcode;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t user_esp;
    uint32_t user_ss;
} __attribute__((packed));

