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
    modi = pressed ? modi | mbit::alt_left : modi & ~mbit::alt_left;
}

void Key::alt_right(bool pressed) {
    //TODO
    modi = pressed ? modi | mbit::alt_right : modi & ~mbit::alt_right;
}

void Key::ctrl_left(bool pressed) {
    //TODO
     modi = pressed ? modi | mbit::ctrl_left : modi & ~mbit::ctrl_left;
}

void Key::ctrl_right(bool pressed) {
   //TODO
    modi = pressed ? modi | mbit::ctrl_right : modi & ~mbit::ctrl_right;
}

void Key::caps_lock(bool pressed) {
    //TODO
     modi = pressed ? modi | mbit::caps_lock : modi & ~mbit::caps_lock;
}

void Key::num_lock(bool pressed) {
    //TODO
     modi = pressed ? modi | mbit::num_lock : modi & ~mbit::num_lock;
}

void Key::scroll_lock(bool pressed) {
    //TODO
     modi = pressed ? modi | mbit::num_lock : modi & ~mbit::num_lock;
}

bool Key::SHIFT() {
    return modi & mbit::SHIFT;
}

bool Key::alt_left() {
    //TODO
    return modi & mbit::alt_left;
}

bool Key::alt_right() {
    //TODO
    return modi & mbit::alt_right;
}

bool Key::ctrl_left() {
    //TODO
    return modi & mbit::ctrl_left;
}

bool Key::ctrl_right() {
    //TODO
    return modi & mbit::ctrl_right;
}

bool Key::caps_lock() {
    //TODO
    return modi & mbit::caps_lock;
}

bool Key::num_lock() {
    //TODO
    return modi & mbit::num_lock;
}

bool Key::scroll_lock() {
    //TODO
    return modi & mbit::scroll_lock;
}

bool Key::alt() {
    //TODO
    return (modi & mbit::alt_left) | (modi & mbit::alt_right);
}

bool Key::ctrl() {
    //TODO
    return (modi & mbit::ctrl_left) | (modi & mbit::ctrl_right);
}
