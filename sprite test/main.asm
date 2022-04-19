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
00102$:
;main.c:17: spriteIndex = spriteIndex ? 0 : 1;
	ld	a, c
	or	a, a
	jr	Z, 00110$
	ld	bc, #0x0000
	jr	00111$
00110$:
	ld	bc, #0x0001
00111$:
;c:/gbdk/include/gb/gb.h:1447: shadow_OAM[nb].tile=tile;
	ld	hl, #(_shadow_OAM + 2)
	ld	(hl), c
;main.c:19: delay(1000);
	push	bc
	ld	de, #0x03e8
	push	de
	call	_delay
	pop	hl
	pop	bc
;c:/gbdk/include/gb/gb.h:1536: OAM_item_t * itm = &shadow_OAM[nb];
	ld	de, #_shadow_OAM
;c:/gbdk/include/gb/gb.h:1537: itm->y+=y, itm->x+=x;
	ld	a, (de)
	ld	(de), a
	inc	de
	ld	a, (de)
	add	a, #0x0a
	ld	(de), a
;main.c:20: scroll_sprite(0, 10, 0);
;main.c:22: }
	jr	00102$
	.area _CODE
	.area _INITIALIZER
	.area _CABS (ABS)
