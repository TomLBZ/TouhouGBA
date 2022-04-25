#include "sound.h"

void spark(){
    NR41_REG = 0x1F; // 0001 1111 sound len max 11111
    NR42_REG = 0xF1; // 1111 0001 vol start full 1111, fade down by 1 sweep
    NR43_REG = 0x30; // 0011 0000: shift freq 3, setp 0, div ratio 0
    NR44_REG = 0xC0; // 1100 0000: start sound, not continuous
}

void seal(){
    NR10_REG = 0x16; // 0001 0110: freq sweep: time 1, dir increase, shift 110 (6 decimal)
    NR11_REG = 0x40; // 0100 0000: dut cycle 01 (25%), wave len 000000 (longest).
    NR12_REG = 0x73; // 0111 0011: vol 7, sweep down, step=3
    NR13_REG = 0x00; // lsbs for channel 1
    NR14_REG = 0xC3; // 1100 0011: init, no conseq, freq = 011 [NR13] = 0x300
}

void biu(){
    NR10_REG = 0x1E; // 0001 1110: freq sweep: time 1, dir decrease, shift 110 (6 decimal)
    NR11_REG = 0x80; // 1000 0000: dut cycle 10 (50%), wave len 000000 (longest).
    NR12_REG = 0x72; // 0111 0010: vol 7, sweep down, step=2
    NR13_REG = 0x00; // lsbs for channel 1
    NR14_REG = 0xC7; // 1100 0111: init, no conseq, freq = 111 [NR13] = 0x700
}