#include "header.h"

void respawn_trash(SDL_Rect *trash) {
    trash->x = random() % 1920;
    trash->y = random() % 1080;
}
