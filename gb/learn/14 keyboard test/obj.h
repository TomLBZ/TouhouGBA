#if !defined(OBJ_MACROS)
#define OBJ_MACROS

#include <gb/gb.h>

#define OFFDIRX(x)  (x)
#define OFFDIRY(x)  ((x) << 4)
#define VFLIP       (1 << 6)
#define HFLIP       (1 << 5)

typedef int8_t (*fptr)(void* obj, uint8_t x, uint8_t y);

typedef struct gameObj_t{
    uint8_t tlx; // x of the tl corner, need not be on a tile
    uint8_t tly; // y of the tl corner, need not be on a tile
    uint8_t cdx;  // center x from tlx, used for collision detection
    uint8_t cdy;  // center y from tly, used for collision detection
    fptr distfunc; // sdf for collision detection
    uint8_t len;   // len of consequent sprites
    uint8_t* oamIds; // ids in OAM of sprites
    uint8_t* sprIds;  // ids of sprites
    uint8_t* offsets; // offsets relative to first sprite: 4 bit x and y components
    uint8_t* props; // properties of sprites
} GameObject;


extern GameObject Oodama;

void initGameObjects();
void setGameObjectTiles(GameObject* obj);
void moveGameObject(GameObject* obj, uint8_t x, uint8_t y);
int8_t sdfOodama(void* obj, uint8_t x, uint8_t y);

#endif // OBJ_MACROS


