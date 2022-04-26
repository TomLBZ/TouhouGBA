#include "fade.h"

const uint8_t FadePalette[] = {0xE4u, 0xF9u, 0xFEu, 0xFFu, 0xE4u, 0x90u, 0x40u, 0x00u};

void blackOut(){
    BGP_REG = FadePalette[3];
    wait_vbl_done();
}

void whiteOut(){
    BGP_REG = FadePalette[7];
    wait_vbl_done();
}

void fadeToBlack(){
    for (uint8_t i = 0u; i < 4u; i++){
        BGP_REG = FadePalette[i];
        for (uint8_t j = 0; j < FADE_FRAME_DELAY; j++) wait_vbl_done();
    }
}

void fadeFromBlack(){
    for (uint8_t i = 0u; i < 4u; i++){
        BGP_REG = FadePalette[3u - i];
        for (uint8_t j = 0; j < FADE_FRAME_DELAY; j++) wait_vbl_done();
    }
}

void fadeToWhite(){
    for (uint8_t i = 0u; i < 4u; i++){
        BGP_REG = FadePalette[4u + i];
        for (uint8_t j = 0; j < FADE_FRAME_DELAY; j++) wait_vbl_done();
    }
}

void fadeFromWhite(){
    for (uint8_t i = 0u; i < 4u; i++){
        BGP_REG = FadePalette[7u - i];
        for (uint8_t j = 0; j < FADE_FRAME_DELAY; j++) wait_vbl_done();
    }
}