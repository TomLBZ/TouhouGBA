;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 4.1.6 #12539 (MINGW32)
;--------------------------------------------------------
	.module main
	.optsdcc -mgbz80
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _main
	.globl _init
	.globl _font_set
	.globl _font_load
	.globl _font_init
	.globl _set_sprite_data
	.globl _set_win_tiles
	.globl _set_bkg_tiles
	.globl _set_bkg_data
	.globl _joypad
	.globl _delay
	.globl _windowmap
	.globl _map
	.globl _tiles
;--------------------------------------------------------
; special function registers
;--------------------------------------------------------
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area _DATA
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area _INITIALIZED
_tiles::
	.ds 192
_map::
	.ds 720
_windowmap::
	.ds 5
;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area _DABS (ABS)
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area _HOME
	.area _GSINIT
	.area _GSFINAL
	.area _GSINIT
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area _HOME
	.area _HOME
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area _CODE
;main.c:11: void init(){
;	---------------------------------
; Function init
; ---------------------------------
_init::
;main.c:13: NR52_REG = 0x80; // 1000 0000 turns on sound
	ld	a, #0x80
	ldh	(_NR52_REG + 0), a
;main.c:14: NR50_REG = 0x77; // 0111 0111 sets L and R channels to 0x77 (max)
	ld	a, #0x77
	ldh	(_NR50_REG + 0), a
;main.c:15: NR51_REG = 0xFF; // 1111 1111 select both channels
	ld	a, #0xff
	ldh	(_NR51_REG + 0), a
;main.c:17: font_init();
	call	_font_init
;main.c:18: font_set(font_load(font_min));
	ld	de, #_font_min
	push	de
	call	_font_load
	pop	hl
	push	de
	call	_font_set
	pop	hl
;main.c:20: set_bkg_data(37, 12, tiles);
	ld	bc, #_tiles+0
	push	bc
	ld	hl, #0xc25
	push	hl
	call	_set_bkg_data
	add	sp, #4
;main.c:21: set_bkg_tiles(0,0,40,18,map);
	ld	de, #_map
	push	de
	ld	hl, #0x1228
	push	hl
	xor	a, a
	rrca
	push	af
	call	_set_bkg_tiles
	add	sp, #6
;main.c:22: SHOW_BKG;
	ldh	a, (_LCDC_REG + 0)
	or	a, #0x01
	ldh	(_LCDC_REG + 0), a
;main.c:23: DISPLAY_ON;
	ldh	a, (_LCDC_REG + 0)
	or	a, #0x80
	ldh	(_LCDC_REG + 0), a
;main.c:25: set_sprite_data(0, 2, tiles);
	push	bc
	ld	hl, #0x200
	push	hl
	call	_set_sprite_data
	add	sp, #4
;c:/gbdk/include/gb/gb.h:1447: shadow_OAM[nb].tile=tile;
	ld	hl, #(_shadow_OAM + 2)
	ld	(hl), #0x00
;c:/gbdk/include/gb/gb.h:1520: OAM_item_t * itm = &shadow_OAM[nb];
	ld	hl, #_shadow_OAM
;c:/gbdk/include/gb/gb.h:1521: itm->y=y, itm->x=x;
	ld	a, #0x4e
	ld	(hl+), a
	ld	(hl), #0x58
;main.c:28: SHOW_SPRITES;
	ldh	a, (_LCDC_REG + 0)
	or	a, #0x02
	ldh	(_LCDC_REG + 0), a
;main.c:30: set_win_tiles(0,0,5,1,windowmap);
	ld	de, #_windowmap
	push	de
	ld	hl, #0x105
	push	hl
	xor	a, a
	rrca
	push	af
	call	_set_win_tiles
	add	sp, #6
;c:/gbdk/include/gb/gb.h:1316: WX_REG=x, WY_REG=y;
	ld	a, #0x07
	ldh	(_WX_REG + 0), a
	ld	a, #0x88
	ldh	(_WY_REG + 0), a
;main.c:32: SHOW_WIN;
	ldh	a, (_LCDC_REG + 0)
	or	a, #0x20
	ldh	(_LCDC_REG + 0), a
;main.c:33: }
	ret
;main.c:35: void main(){
;	---------------------------------
; Function main
; ---------------------------------
_main::
;main.c:36: init();
	call	_init
;main.c:37: uint8_t spriteIndex = 0;
	ld	c, #0x00
;main.c:38: while(1){
00112$:
;c:/gbdk/include/gb/gb.h:1094: SCX_REG+=x, SCY_REG+=y;
	ldh	a, (_SCX_REG + 0)
	inc	a
	ldh	(_SCX_REG + 0), a
;c:/gbdk/include/gb/gb.h:1447: shadow_OAM[nb].tile=tile;
	ld	hl, #(_shadow_OAM + 2)
	ld	(hl), c
;main.c:42: switch (joypad())
	call	_joypad
	ld	a, e
	cp	a, #0x01
	jr	Z, 00102$
	cp	a, #0x02
	jr	Z, 00101$
	cp	a, #0x04
	jr	Z, 00103$
	cp	a, #0x08
	jr	Z, 00104$
	cp	a, #0x10
	jr	Z, 00105$
	cp	a, #0x20
	jr	Z, 00110$
	sub	a, #0x40
	jr	Z, 00107$
	jr	00110$
;main.c:44: case J_LEFT:
00101$:
;c:/gbdk/include/gb/gb.h:1536: OAM_item_t * itm = &shadow_OAM[nb];
	ld	de, #_shadow_OAM+0
;c:/gbdk/include/gb/gb.h:1537: itm->y+=y, itm->x+=x;
	ld	a, (de)
	ld	(de), a
	inc	de
	ld	a, (de)
	add	a, #0xfb
	ld	(de), a
;main.c:46: break;
	jr	00110$
;main.c:47: case J_RIGHT:
00102$:
;c:/gbdk/include/gb/gb.h:1536: OAM_item_t * itm = &shadow_OAM[nb];
	ld	de, #_shadow_OAM
;c:/gbdk/include/gb/gb.h:1537: itm->y+=y, itm->x+=x;
	ld	a, (de)
	ld	(de), a
	inc	de
	ld	a, (de)
	add	a, #0x05
	ld	(de), a
;main.c:49: break;
	jr	00110$
;main.c:50: case J_UP:
00103$:
;c:/gbdk/include/gb/gb.h:1536: OAM_item_t * itm = &shadow_OAM[nb];
	ld	de, #_shadow_OAM
;c:/gbdk/include/gb/gb.h:1537: itm->y+=y, itm->x+=x;
	ld	a, (de)
	add	a, #0xfb
	ld	(de), a
	inc	de
	ld	a, (de)
	ld	(de), a
;main.c:52: break;
	jr	00110$
;main.c:53: case J_DOWN:
00104$:
;c:/gbdk/include/gb/gb.h:1536: OAM_item_t * itm = &shadow_OAM[nb];
	ld	de, #_shadow_OAM
;c:/gbdk/include/gb/gb.h:1537: itm->y+=y, itm->x+=x;
	ld	a, (de)
	add	a, #0x05
	ld	(de), a
	inc	de
	ld	a, (de)
	ld	(de), a
;main.c:55: break;
	jr	00110$
;main.c:56: case J_A:
00105$:
;main.c:57: NR10_REG = 0x16; // 0001 0110: freq sweep: time 1, dir increase, shift 110 (6 decimal)
	ld	a, #0x16
	ldh	(_NR10_REG + 0), a
;main.c:58: NR11_REG = 0x40; // 0100 0000: dut cycle 01 (25%), wave len 000000 (longest).
	ld	a, #0x40
	ldh	(_NR11_REG + 0), a
;main.c:59: NR12_REG = 0x73; // 0111 0011: vol 7, sweep down, step=3
	ld	a, #0x73
	ldh	(_NR12_REG + 0), a
;main.c:60: NR13_REG = 0x00; // lsbs for channel 1
	xor	a, a
	ldh	(_NR13_REG + 0), a
;main.c:61: NR14_REG = 0xC3; // 1100 0011: init, no conseq, freq = 011 [NR13] = 0x300
	ld	a, #0xc3
	ldh	(_NR14_REG + 0), a
;main.c:62: break;
	jr	00110$
;main.c:65: case J_SELECT:
00107$:
;main.c:66: spriteIndex = spriteIndex ? 0 : 1;
	ld	a, c
	or	a, a
	jr	Z, 00122$
	ld	bc, #0x0000
	jr	00123$
00122$:
	ld	bc, #0x0001
00123$:
;main.c:72: }
00110$:
;main.c:73: delay(200);
	push	bc
	ld	de, #0x00c8
	push	de
	call	_delay
	pop	hl
	pop	bc
;main.c:76: }
	jp	00112$
	.area _CODE
	.area _INITIALIZER
__xinit__tiles:
	.db #0x66	; 102	'f'
	.db #0x24	; 36
	.db #0x3c	; 60
	.db #0x18	; 24
	.db #0x18	; 24
	.db #0x24	; 36
	.db #0x5a	; 90	'Z'
	.db #0x00	; 0
	.db #0xdb	; 219
	.db #0x18	; 24
	.db #0x3c	; 60
	.db #0x3c	; 60
	.db #0x7e	; 126
	.db #0x7e	; 126
	.db #0x7e	; 126
	.db #0x7e	; 126
	.db #0x18	; 24
	.db #0x18	; 24
	.db #0x18	; 24
	.db #0x18	; 24
	.db #0x3c	; 60
	.db #0x3c	; 60
	.db #0x18	; 24
	.db #0x00	; 0
	.db #0x3c	; 60
	.db #0x00	; 0
	.db #0x3c	; 60
	.db #0x3c	; 60
	.db #0x7e	; 126
	.db #0x7e	; 126
	.db #0x7e	; 126
	.db #0x7e	; 126
	.db #0x12	; 18
	.db #0x12	; 18
	.db #0x61	; 97	'a'
	.db #0x61	; 97	'a'
	.db #0x96	; 150
	.db #0x96	; 150
	.db #0x1c	; 28
	.db #0x1c	; 28
	.db #0x62	; 98	'b'
	.db #0x62	; 98	'b'
	.db #0x53	; 83	'S'
	.db #0x53	; 83	'S'
	.db #0x9e	; 158
	.db #0x9e	; 158
	.db #0x22	; 34
	.db #0x22	; 34
	.db #0xff	; 255
	.db #0xff	; 255
	.db #0x01	; 1
	.db #0x01	; 1
	.db #0x01	; 1
	.db #0x01	; 1
	.db #0x01	; 1
	.db #0x01	; 1
	.db #0xff	; 255
	.db #0xff	; 255
	.db #0x10	; 16
	.db #0x10	; 16
	.db #0x10	; 16
	.db #0x10	; 16
	.db #0x10	; 16
	.db #0x10	; 16
	.db #0xff	; 255
	.db #0xff	; 255
	.db #0x01	; 1
	.db #0x01	; 1
	.db #0x01	; 1
	.db #0x01	; 1
	.db #0x01	; 1
	.db #0x01	; 1
	.db #0xff	; 255
	.db #0xff	; 255
	.db #0x11	; 17
	.db #0x11	; 17
	.db #0x11	; 17
	.db #0x11	; 17
	.db #0x11	; 17
	.db #0x11	; 17
	.db #0xff	; 255
	.db #0xff	; 255
	.db #0x80	; 128
	.db #0x80	; 128
	.db #0x80	; 128
	.db #0x80	; 128
	.db #0x80	; 128
	.db #0x80	; 128
	.db #0xff	; 255
	.db #0xff	; 255
	.db #0x88	; 136
	.db #0x88	; 136
	.db #0x88	; 136
	.db #0x88	; 136
	.db #0x88	; 136
	.db #0x88	; 136
	.db #0x81	; 129
	.db #0x00	; 0
	.db #0x42	; 66	'B'
	.db #0x00	; 0
	.db #0x24	; 36
	.db #0x00	; 0
	.db #0x18	; 24
	.db #0x00	; 0
	.db #0x18	; 24
	.db #0x00	; 0
	.db #0x24	; 36
	.db #0x00	; 0
	.db #0x42	; 66	'B'
	.db #0x00	; 0
	.db #0x81	; 129
	.db #0x00	; 0
	.db #0xff	; 255
	.db #0x00	; 0
	.db #0x81	; 129
	.db #0x00	; 0
	.db #0xbd	; 189
	.db #0x00	; 0
	.db #0xa5	; 165
	.db #0x00	; 0
	.db #0xa5	; 165
	.db #0x00	; 0
	.db #0xbd	; 189
	.db #0x00	; 0
	.db #0x81	; 129
	.db #0x00	; 0
	.db #0xff	; 255
	.db #0x00	; 0
	.db #0x55	; 85	'U'
	.db #0x00	; 0
	.db #0xaa	; 170
	.db #0x00	; 0
	.db #0x55	; 85	'U'
	.db #0x00	; 0
	.db #0xaa	; 170
	.db #0x00	; 0
	.db #0x55	; 85	'U'
	.db #0x00	; 0
	.db #0xaa	; 170
	.db #0x00	; 0
	.db #0x55	; 85	'U'
	.db #0x00	; 0
	.db #0xaa	; 170
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0xff	; 255
	.db #0xff	; 255
	.db #0x00	; 0
	.db #0xff	; 255
	.db #0xff	; 255
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0x00	; 0
	.db #0xaa	; 170
	.db #0x00	; 0
	.db #0x66	; 102	'f'
	.db #0x00	; 0
	.db #0x33	; 51	'3'
	.db #0xcc	; 204
	.db #0xff	; 255
	.db #0xff	; 255
	.db #0x00	; 0
	.db #0xff	; 255
	.db #0x00	; 0
	.db #0xdd	; 221
	.db #0x00	; 0
	.db #0xbb	; 187
	.db #0x00	; 0
	.db #0xff	; 255
	.db #0x00	; 0
	.db #0xff	; 255
	.db #0x00	; 0
	.db #0xdd	; 221
	.db #0x00	; 0
	.db #0xbb	; 187
	.db #0x00	; 0
	.db #0xff	; 255
__xinit__map:
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x25	; 37
	.db #0x26	; 38
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x2c	; 44
	.db #0x2c	; 44
	.db #0x2c	; 44
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x2a	; 42
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x29	; 41
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x2c	; 44
	.db #0x2c	; 44
	.db #0x2c	; 44
	.db #0x2c	; 44
	.db #0x2c	; 44
	.db #0x2c	; 44
	.db #0x2c	; 44
	.db #0x2c	; 44
	.db #0x2b	; 43
	.db #0x2c	; 44
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x2a	; 42
	.db #0x30	; 48	'0'
	.db #0x30	; 48	'0'
	.db #0x30	; 48	'0'
	.db #0x29	; 41
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x2c	; 44
	.db #0x2c	; 44
	.db #0x2b	; 43
	.db #0x2b	; 43
	.db #0x2b	; 43
	.db #0x2c	; 44
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x2a	; 42
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x29	; 41
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x2c	; 44
	.db #0x2c	; 44
	.db #0x2c	; 44
	.db #0x2c	; 44
	.db #0x2c	; 44
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x2a	; 42
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x29	; 41
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x2a	; 42
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x29	; 41
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x2a	; 42
	.db #0x28	; 40
	.db #0x2c	; 44
	.db #0x28	; 40
	.db #0x2c	; 44
	.db #0x28	; 40
	.db #0x2c	; 44
	.db #0x28	; 40
	.db #0x2c	; 44
	.db #0x28	; 40
	.db #0x2c	; 44
	.db #0x28	; 40
	.db #0x2c	; 44
	.db #0x27	; 39
	.db #0x2c	; 44
	.db #0x28	; 40
	.db #0x2c	; 44
	.db #0x28	; 40
	.db #0x29	; 41
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x2a	; 42
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x27	; 39
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x27	; 39
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x27	; 39
	.db #0x27	; 39
	.db #0x27	; 39
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x29	; 41
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x31	; 49	'1'
	.db #0x2f	; 47
	.db #0x2f	; 47
	.db #0x2f	; 47
	.db #0x2f	; 47
	.db #0x2f	; 47
	.db #0x2f	; 47
	.db #0x2f	; 47
	.db #0x2f	; 47
	.db #0x2f	; 47
	.db #0x2f	; 47
	.db #0x2f	; 47
	.db #0x2f	; 47
	.db #0x2f	; 47
	.db #0x2f	; 47
	.db #0x2f	; 47
	.db #0x2f	; 47
	.db #0x2e	; 46
	.db #0x2e	; 46
	.db #0x2a	; 42
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x27	; 39
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x27	; 39
	.db #0x27	; 39
	.db #0x27	; 39
	.db #0x28	; 40
	.db #0x27	; 39
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x28	; 40
	.db #0x29	; 41
	.db #0x2e	; 46
	.db #0x2e	; 46
	.db #0x2e	; 46
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
	.db #0x2d	; 45
__xinit__windowmap:
	.db #0x12	; 18
	.db #0x0f	; 15
	.db #0x16	; 22
	.db #0x16	; 22
	.db #0x19	; 25
	.area _CABS (ABS)
