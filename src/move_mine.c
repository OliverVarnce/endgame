#include "header.h"

void move_mine(float *x, float *y, int *angle) {
    *x -= sin(*angle*M_PI/180.0)*0.1;
    *y -= cos(*angle*M_PI/180.0)*0.1;
    if (*x < -50)
        *x = 1920;
    if (*x > 1920)
        *x = -50;
    if (*y < -50)
        *y = 1080;
    if (*y > 1080)
        *y = -50;
    int random_seed = random() % 11;
    if ((random() % 100) % 88 == 0) {
        if (random_seed % 2 == 0)
            *angle += random_seed;
        else
            *angle -= random_seed;
    }    
}
