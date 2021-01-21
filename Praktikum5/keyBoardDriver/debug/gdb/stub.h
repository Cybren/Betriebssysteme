
#pragma once
#include "machine/serial.h"

inline void breakpoint(void) {
    asm volatile("int $3;\r\n":: : "memory");
}

struct debug_context {
    uint32_t ss;
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t vector;
    uint32_t error_code;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
} __attribute__((packed));


extern "C" void debug_handler(struct debug_context *context);

class GDB_Stub
        : public Serial {
    GDB_Stub(const GDB_Stub &) = delete;
    GDB_Stub &operator=(const GDB_Stub &) = delete;
    const bool debug;
public:
    GDB_Stub(bool wait = false, bool debugOutput = false, Serial::comPort port = COM1,
             Serial::baudRate baudrate = BAUD_9600);
protected:
    typedef unsigned int address;
    typedef unsigned int reg;
    typedef enum {
        REG_EAX = 0,
        REG_ECX = 1,
        REG_EDX = 2,
        REG_EBX = 3,
        REG_ESP = 4,
        REG_EBP = 5,
        REG_ESI = 6,
        REG_EDI = 7,
        REG_PC = 8,
        REG_PS = 9,
        REG_CS = 10,
        REG_SS = 11,
        REG_DS = 12,
        REG_ES = 13,
        REG_FS = 14,
        REG_GS = 15,
        NUM_REGISTERS = 16
    } Registers;
    struct state {
        int signum;
        reg registers[NUM_REGISTERS];
    };
    struct state state;
    void handle(void);
    friend void debug_handler(struct debug_context *context);
    int writeString(const char *buf, size_t len);
    int readString(char *buf, size_t buf_len, size_t len);
    int send_packet(const char *pkt, size_t pkt_len);
    int recv_packet(char *pkt_buf, size_t pkt_buf_len, size_t *pkt_len);
    int checksum(const char *buf, size_t len);
    int recv_ack(void);
    int send_ok_packet();
    int send_signal_packet(char *buf, size_t buf_len, char signal);
    int send_error_packet(char *buf, size_t buf_len, char error);
    int mem_read(char *buf, size_t buf_len, address addr, size_t len, bool hex);
    int mem_write(const char *buf, size_t buf_len, address addr, size_t len, bool hex);
    void sys_continue(void);
    void sys_step(void);
    bool install_handler(int vector);
};
