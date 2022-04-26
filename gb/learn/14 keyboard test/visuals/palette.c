#include "palette.h"

const uint8_t FadePalette[] = {0xE4u, 0xF9u, 0xFEu, 0xFFu, 0xE4u, 0x90u, 0x40u, 0x00u};
const uint8_t FadeInversePalette[] = {0x1Bu, 0x6Fu, 0xBFu, 0xFFu, 0x1Bu, 0x06u, 0x01u, 0x00u};
uint8_t isInversePalette = 0;

void blackOut(){
    BGP_REG = FadePalette[3];
    wait_vbl_done();
}

void whiteOut(){
    BGP_REG = FadePalette[7];
    wait_vbl_done();
}

void fadeToBlack(){
    const uint8_t* ptr = isInversePalette ? FadeInversePalette : FadePalette;
    for (uint8_t i = 0u; i < 4u; i++){
        BGP_REG = ptr[i];
        for (uint8_t j = 0; j < FADE_FRAME_DELAY; j++) wait_vbl_done();
    }
}

void fadeFromBlack(){
    const uint8_t* ptr = isInversePalette ? FadeInversePalette : FadePalette;
    for (uint8_t i = 0u; i < 4u; i++){
        BGP_REG = ptr[3u - i];
        for (uint8_t j = 0; j < FADE_FRAME_DELAY; j++) wait_vbl_done();
    }
}

void fadeToWhite(){
    const uint8_t* ptr = isInversePalette ? FadeInversePalette : FadePalette;
    for (uint8_t i = 0u; i < 4u; i++){
        BGP_REG = ptr[4u + i];
        for (uint8_t j = 0; j < FADE_FRAME_DELAY; j++) wait_vbl_done();
    }
}

void fadeFromWhite(){
    const uint8_t* ptr = isInversePalette ? FadeInversePalette : FadePalette;
    for (uint8_t i = 0u; i < 4u; i++){
        BGP_REG = ptr[7u - i];
        for (uint8_t j = 0; j < FADE_FRAME_DELAY; j++) wait_vbl_done();
    }
}

void inversePalette(){
    if (isInversePalette) isInversePalette = 0;
    else isInversePalette = 1;
}