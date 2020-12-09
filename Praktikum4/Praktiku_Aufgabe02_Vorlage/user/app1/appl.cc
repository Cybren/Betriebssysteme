#include "user/app1/appl.h"
#include "debug/output.h"
#include "types.h"
#include "device/cga_stream.h"
extern CGA_Stream kout;

void Application::action () {
    kout << "bool:       " << true << " -> true" << endl;
    kout << "zero:       " << 0 << " -> 0" << endl;
    kout << "ten:        " << (10) << " -> 10" << endl;
    kout << "uint max:   " << ~((unsigned int) 0) << " -> 4294967295" << endl;
    kout << "int max:    " << ~(1 << 31) << " -> 2147483647" << endl;
    kout << "int min:    " << (1 << 31) << " -> -2147483648" << endl;
    kout << "some int:   " << (-123456789) << " -> -123456789" << endl;
    kout << "some int:   " << (123456789) << " -> 123456789" << endl;
    kout << "binary:     " << bin << 42 << dec << " -> 0b101010" << endl;
    kout << "octal:      " << oct << 42 << dec << " -> 052" << endl;
    kout << "hex:        " << hex << 42 << dec << " -> 0x2a" << endl;
    kout << "pointer:    " << ((void *) (3735928559u)) << " -> 0xdeadbeef" << endl;
}
