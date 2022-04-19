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
	.globl _set_sprite_data
	.globl _joypad
	.globl _delay
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
;main.c:9: void main(){
;	---------------------------------
; Function main
; ---------------------------------
_main::
;main.c:10: uint8_t spriteIndex = 0;
	ld	c, #0x00
;main.c:11: set_sprite_data(0, 2, RM);
	ld	de, #_RM
	push	de
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
;main.c:14: SHOW_SPRITES;
	ldh	a, (_LCDC_REG + 0)
	or	a, #0x02
	ldh	(_LCDC_REG + 0), a
;main.c:16: while(1){
00112$:
;c:/gbdk/include/gb/gb.h:1447: shadow_OAM[nb].tile=tile;
	ld	hl, #(_shadow_OAM + 2)
	ld	(hl), c
;main.c:18: switch (joypad())
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
	jr	Z, 00110$
	cp	a, #0x20
	jr	Z, 00110$
	sub	a, #0x40
	jr	Z, 00107$
	jr	00110$
;main.c:20: case J_LEFT:
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
;main.c:22: break;
	jr	00110$
;main.c:23: case J_RIGHT:
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
;main.c:25: break;
	jr	00110$
;main.c:26: case J_UP:
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
;main.c:28: break;
	jr	00110$
;main.c:29: case J_DOWN:
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
;main.c:31: break;
	jr	00110$
;main.c:38: case J_SELECT:
00107$:
;main.c:39: spriteIndex = spriteIndex ? 0 : 1;
	ld	a, c
	or	a, a
	jr	Z, 00123$
	ld	bc, #0x0000
	jr	00124$
00123$:
	ld	bc, #0x0001
00124$:
;main.c:46: }
00110$:
;main.c:47: delay(200);
	push	bc
	ld	de, #0x00c8
	push	de
	call	_delay
	pop	hl
	pop	bc
;main.c:49: }
	jr	00112$
	.area _CODE
	.area _INITIALIZER
	.area _CABS (ABS)
