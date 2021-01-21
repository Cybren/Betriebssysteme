
#include "stub.h"
#include "debug/assert.h"
#include "debug/output.h"

static const char digits[] = "0123456789abcdef";

static char get_digit(int val) {
    return (val >= 0) && (val <= 0xf) ? digits[val] : -1;
}

static int get_val(char digit, int base) {
    int value;
    if ((digit >= '0') && (digit <= '9'))
        value = digit - '0';
    else if ((digit >= 'a') && (digit <= 'f'))
        value = digit - 'a' + 0xa;
    else if ((digit >= 'A') && (digit <= 'F'))
        value = digit - 'A' + 0xa;
    else
        return -1;
    return (value < base) ? value : -1;
}

static bool is_printable_char(char ch) {
    return (ch >= 0x20 && ch <= 0x7e);
}

static int strtol(const char *str, size_t len, int base, const char **endptr) {
    int value = 0;
    size_t pos = 0;
    int sign = 1;
    int valid = 0;
    if (endptr)
        *endptr = NULL;
    if (len < 1)
        return 0;
    if (str[pos] == '-') {
        sign = -1;
        pos += 1;
    } else if (str[pos] == '+') {
        sign = 1;
        pos += 1;
    }
    if ((pos + 2 < len) && (str[pos] == '0') &&
        ((str[pos + 1] == 'x') || (str[pos + 1] == 'X'))) {
        base = 16;
        pos += 2;
    }
    if (base == 0)
        base = 10;
    for (; (pos < len) && (str[pos] != '\x00'); pos++) {
        int tmp = get_val(str[pos], base);
        if (tmp == -1)
            break;
        value = value * base + tmp;
        valid = 1;
    }
    if (!valid)
        return 0;
    if (endptr)
        *endptr = str + pos;
    value *= sign;
    return value;
}

int enc_hex(char *buf, size_t buf_len, const char *data, size_t data_len) {
    if (buf_len < data_len * 2)
        return -1;
    for (size_t pos = 0; pos < data_len; pos++) {
        *buf++ = get_digit((data[pos] >> 4) & 0xf);
        *buf++ = get_digit((data[pos]) & 0xf);
    }
    return data_len * 2;
}

static int dec_hex(const char *buf, size_t buf_len, char *data, size_t data_len) {
    if (buf_len != data_len * 2)
        return -1;
    for (size_t pos = 0; pos < data_len; pos++) {
        int tmp = get_val(*buf++, 16);
        if (tmp == -1) {
            assert(0);
            return -1;
        }
        data[pos] = (tmp << 4);
        tmp = get_val(*buf++, 16);
        if (tmp == -1) {
            assert(0);
            return -1;
        }
        data[pos] |= tmp;
    }
    return 0;
}

static int enc_bin(char *buf, size_t buf_len, const char *data, size_t data_len) {
    size_t buf_pos = 0;
    for (size_t data_pos = 0; data_pos < data_len; data_pos++) {
        if (data[data_pos] == '$' || data[data_pos] == '#' || data[data_pos] == '}' || data[data_pos] == '*') {
            if (buf_pos + 1 >= buf_len) {
                assert(0);
                return -1;
            }
            buf[buf_pos++] = '}';
            buf[buf_pos++] = data[data_pos] ^ 0x20;
        } else {
            if (buf_pos >= buf_len) {
                assert(0);
                return -1;
            }
            buf[buf_pos++] = data[data_pos];
        }
    }
    return buf_pos;
}

static int dec_bin(const char *buf, size_t buf_len, char *data, size_t data_len) {
    size_t data_pos = 0;
    for (size_t buf_pos = 0; buf_pos < buf_len; buf_pos++) {
        if (data_pos >= data_len) {
            assert(0);
            return -1;
        }
        if (buf[buf_pos] == '}') {
            if (buf_pos + 1 >= buf_len) {
                assert(0);
                return -1;
            }
            buf_pos += 1;
            data[data_pos++] = buf[buf_pos] ^ 0x20;
        } else {
            data[data_pos++] = buf[buf_pos];
        }
    }
    return data_pos;
}

int GDB_Stub::recv_ack(void) {
    int response = read();
    switch (response) {
        case '+':
            return 0;
        case '-':
            return 1;
        case 3:
            DBG << "GDB: [ctrl] + [c] pressed" << endl;
            return recv_ack();
        default:
            DBG << "GDB: received bad packet response: " << hex << response << endl;
            return -1;
    }
}

int GDB_Stub::checksum(const char *buf, size_t len) {
    unsigned char csum = 0;
    while (len--)
        csum += *buf++;
    return csum;
}

int GDB_Stub::send_packet(const char *pkt_data, size_t pkt_len) {
    if (write('$') == -1)
        return -1;
    if (debug) {
        DBG << "GDB: ->";
        for (size_t p = 0; p < pkt_len; p++) {
            if (is_printable_char(pkt_data[p]))
                DBG << (char) pkt_data[p];
            else
                DBG << hex << (pkt_data[p] & 0xff);
        }
        DBG << endl;
    }
    if (writeString(pkt_data, pkt_len) == -1)
        return -1;
    char buf[3];
    buf[0] = '#';
    char csum = checksum(pkt_data, pkt_len);
    if ((enc_hex(buf + 1, sizeof(buf) - 1, &csum, 1) == -1) || (writeString(buf, sizeof(buf)) == -1)) {
        return -1;
    }
    return recv_ack();
}

int GDB_Stub::recv_packet(char *pkt_buf, size_t pkt_buf_len, size_t *pkt_len) {
    int data;
    while ((data = read()) != '$');
    *pkt_len = 0;
    while (1) {
        if ((data = read()) == -1)
            return -1;
        else if (data == '#')
            break;
        else {
            if (*pkt_len >= pkt_buf_len) {
                DBG << "GDB: packet buffer overflow" << endl;
                return -1;
            }
            pkt_buf[(*pkt_len)++] = (char) data;
        }
    }
    if (debug) {
        DBG << "GDB: <- ";
        for (size_t p = 0; p < *pkt_len; p++) {
            if (is_printable_char(pkt_buf[p]))
                DBG << (char) pkt_buf[p];
            else
                DBG << hex << (pkt_buf[p] & 0xff);
        }
        DBG << endl;;
    }
    char buf[2];
    char actual_csum, expected_csum;
    if ((readString(buf, sizeof(buf), 2) == -1) || (dec_hex(buf, 2, &expected_csum, 1) == -1))
        return -1;
    else if ((actual_csum = checksum(pkt_buf, *pkt_len)) != expected_csum) {
        DBG << "GDB: received packet with bad checksum (" << (int) actual_csum << " instead of " << (int) expected_csum
            << ")" << endl;
        write('-');
        return -1;
    } else {
        write('+');
        return 0;
    }
}

int GDB_Stub::mem_read(char *buf, size_t buf_len, address addr, size_t len, bool hex) {
    char data[64];
    if (len > sizeof(data))
        return -1;
    for (size_t pos = 0; pos < len; pos++)
        data[pos] = *(volatile char *) (addr + pos);
    return hex ? enc_hex(buf, buf_len, data, len) : enc_bin(buf, buf_len, data, len);
}

int GDB_Stub::mem_write(const char *buf, size_t buf_len, address addr, size_t len, bool hex) {
    char data[64];
    if (len > sizeof(data))
        return -1;
    if ((hex ? dec_hex(buf, buf_len, data, len) : dec_bin(buf, buf_len, data, len)) == -1)
        return -1;
    for (size_t pos = 0; pos < len; pos++)
        *(volatile char *) (addr + pos) = data[pos];
    return 0;
}

void GDB_Stub::sys_continue(void) {
    state.registers[REG_PS] &= ~(1 << 8);
}

void GDB_Stub::sys_step(void) {
    state.registers[REG_PS] |= 1 << 8;
}

int GDB_Stub::send_ok_packet() {
    return send_packet("OK", 2);
}

int GDB_Stub::send_signal_packet(char *buf, size_t buf_len, char signal) {
    if (buf_len < 4)
        return -1;
    buf[0] = 'S';
    int size = enc_hex(&buf[1], buf_len - 1, &signal, 1);
    return size == -1 ? -1 : send_packet(buf, 1 + size);
}

int GDB_Stub::send_error_packet(char *buf, size_t buf_len, char error) {
    if (buf_len < 4)
        return -1;
    buf[0] = 'E';
    int size = enc_hex(&buf[1], buf_len - 1, &error, 1);
    return size == -1 ? -1 : send_packet(buf, 1 + size);
}

int GDB_Stub::writeString(const char *buf, size_t len) {
    while (len--)
        if (write(*buf++) == -1)
            return -1;
    return 0;
}

int GDB_Stub::readString(char *buf, size_t buf_len, size_t len) {
    if (buf_len < len)
        return -1;
    while (len--) {
        char c = read();
        if (c == -1)
            return -1;
        *buf++ = c;
    }
    return 0;
}

void GDB_Stub::handle(void) {
    address addr;
    char pkt_buf[256];
    int status;
    size_t length, pkt_len;
    const char *ptr_next;
    send_signal_packet(pkt_buf, sizeof(pkt_buf), 0);
    while (recv_packet(pkt_buf, sizeof(pkt_buf), &pkt_len) != -1) {
        if (pkt_len == 0)
            continue;
        ptr_next = pkt_buf;
        switch (pkt_buf[0]) {
#define token_remaining_buf (pkt_len-(ptr_next-pkt_buf))
#define token_expect_seperator(c) \
                do { \
                    if (!ptr_next || *ptr_next != c) \
                        goto error; \
                    else \
                        ptr_next += 1; \
                } while(0)
#define token_expect_integer_arg(arg) \
                do { \
                    arg = strtol(ptr_next, token_remaining_buf, 16, &ptr_next); \
                    if (!ptr_next) \
                        goto error; \
                } while(0)
            case 'g':
                status = enc_hex(pkt_buf, sizeof(pkt_buf), (char *) &(state.registers), sizeof(state.registers));
                if (status == -1)
                    goto error;
                pkt_len = status;
                send_packet(pkt_buf, pkt_len);
                break;
            case 'G':
                status = dec_hex(pkt_buf + 1, pkt_len - 1, (char *) &(state.registers), sizeof(state.registers));
                if (status == -1)
                    goto error;
                send_ok_packet();
                break;
            case 'p':
                ptr_next += 1;
                token_expect_integer_arg(addr);
                if (addr >= NUM_REGISTERS)
                    goto error;
                status = enc_hex(pkt_buf, sizeof(pkt_buf), (char *) &(state.registers[addr]),
                                 sizeof(state.registers[addr]));
                if (status == -1)
                    goto error;
                send_packet(pkt_buf, status);
                break;
            case 'P':
                ptr_next += 1;
                token_expect_integer_arg(addr);
                token_expect_seperator('=');
                if (addr >= NUM_REGISTERS)
                    goto error;
                status = dec_hex(ptr_next, token_remaining_buf, (char *) &(state.registers[addr]),
                                 sizeof(state.registers[addr]));
                if (status == -1)
                    goto error;
                send_ok_packet();
                break;
            case 'm':
                ptr_next += 1;
                token_expect_integer_arg(addr);
                token_expect_seperator(',');
                token_expect_integer_arg(length);
                status = mem_read(pkt_buf, sizeof(pkt_buf), addr, length, true);
                if (status == -1)
                    goto error;
                send_packet(pkt_buf, status);
                break;
            case 'M':
                ptr_next += 1;
                token_expect_integer_arg(addr);
                token_expect_seperator(',');
                token_expect_integer_arg(length);
                token_expect_seperator(':');
                status = mem_write(ptr_next, token_remaining_buf, addr, length, true);
                if (status == -1)
                    goto error;
                send_ok_packet();
                break;
            case 'X':
                ptr_next += 1;
                token_expect_integer_arg(addr);
                token_expect_seperator(',');
                token_expect_integer_arg(length);
                token_expect_seperator(':');
                status = mem_write(ptr_next, token_remaining_buf, addr, length, false);
                if (status == -1)
                    goto error;
                send_ok_packet();
                break;
            case 'c':
                sys_continue();
                return;
            case 's':
                sys_step();
                return;
            case '?':
                send_signal_packet(pkt_buf, sizeof(pkt_buf), 0);
                break;
            default:
                if (debug)
                    DBG << "GDB: Unsupported Command '" << pkt_buf[0] << "'" << endl;
                send_packet(NULL, 0);
        }
        continue;
        error:
        send_error_packet(pkt_buf, sizeof(pkt_buf), 0x00);
#undef token_remaining_buf
#undef token_expect_seperator
#undef token_expect_integer_arg
    }
}
