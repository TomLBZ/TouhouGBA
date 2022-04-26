#if !defined(FADE_MACROS)
#define FADE_MACROS

#include <gb/gb.h>

#define FADE_FRAME_DELAY    5u

void blackOut();
void whiteOut();
void fadeToBlack();
void fadeFromBlack();
void fadeToWhite();
void fadeFromWhite();

void inversePalette();

#endif // FADE_MACROS
