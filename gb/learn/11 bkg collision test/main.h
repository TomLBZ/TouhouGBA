#include <gb/gb.h>
#include <gbdk/font.h>
#include <stdio.h>

#if !defined(MAIN_MACROS)
#define MAIN_MACROS

#define MAP_W_TILES 32u
#define SCR_W_TILES 20u
#define SCR_H_TILES 18u
#define SCR_W_PX    (SCR_W_TILES << 3u)
#define SCR_H_PX    (SCR_H_TILES << 3u)
#define SCR_TLX_PX  8u
#define SCR_TLY_PX  16u
#define SCR_MAXX    (SCR_W_PX + SCR_TLX_PX)
#define SCR_MAXY    (SCR_H_PX + SCR_TLY_PX)
#define MAX_SCROLL  ((MAP_W_TILES - SCR_W_TILES) << 3u)
#define PLAYER_CDX  4u
#define PLAYER_CDY  4u
#define CENT_X      (SCR_TLX_PX + (SCR_W_TILES << 2u))
#define CENT_Y      (SCR_TLY_PX + (SCR_H_TILES << 2u))
#define PCENT_X     (CENT_X - PLAYER_CDX)
#define PCENT_Y     (CENT_Y - PLAYER_CDY)

#define T_TOU       0x33u
#define T_HOU       0x34u
#define T_KOU       0x35u
#define T_MA        0x36u
#define T_KYOU      0x37u
#define T_EMPTY     0x0Eu
#define T_T         0x2Cu
#define T_O         0x27u
#define T_U         0x2Du
#define T_H         0x20u
#define T_G         0x1Fu
#define T_B         0x1Au
#define T_A         0x19u

#endif // MAIN_MACROS


