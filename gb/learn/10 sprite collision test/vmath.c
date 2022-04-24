#include "vmath.h"

uint8_t vlen(uint8_t x, uint8_t y){ // integer square root with no division
    uint16_t n = x * x + y * y;
    uint16_t c = 0;
    uint16_t d = 1 << 14; // 0100 0000 0000 0000
    while (d > n) d >>= 2; // div by 4 until <= n
    while (d){ // d != 0
        if (n >= c + d){
            n -= c + d;
            c = (c >> 1) + d;
        } else c >>= 1;
        d >>= 2;
    }
    return (uint8_t)c;
}