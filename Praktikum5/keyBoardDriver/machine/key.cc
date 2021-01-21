#include "machine/key.h"

bool Key::valid() {
    return scan != 0;
}

void Key::invalidate() {
    scan = 0;
}

void Key::ascii(unsigned char a) {
    asc = a;
}

void Key::scancode(unsigned char s) {
    scan = s;
}

unsigned char Key::ascii() {
    return asc;
}

unsigned char Key::scancode() {
    return scan;
}

void Key::SHIFT(bool pressed) {
    modi = pressed ? modi | mbit::SHIFT : modi & ~mbit::SHIFT;
}

void Key::alt_left(bool pressed) {
    //TODO
}

void Key::alt_right(bool pressed) {
    //TODO
}

void Key::ctrl_left(bool pressed) {
    //TODO
}

void Key::ctrl_right(bool pressed) {
   //TODO
}

void Key::caps_lock(bool pressed) {
    //TODO
}

void Key::num_lock(bool pressed) {
    //TODO
}

void Key::scroll_lock(bool pressed) {
    //TODO
}

bool Key::SHIFT() {
    return modi & mbit::SHIFT;
}

bool Key::alt_left() {
    //TODO
}

bool Key::alt_right() {
    //TODO
}

bool Key::ctrl_left() {
    //TODO
}

bool Key::ctrl_right() {
    //TODO
}

bool Key::caps_lock() {
    //TODO
}

bool Key::num_lock() {
    //TODO
}

bool Key::scroll_lock() {
    //TODO
}

bool Key::alt() {
    //TODO
}

bool Key::ctrl() {
    //TODO
}
