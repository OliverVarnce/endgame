#ifndef __HEADER_H__
#define __HEADER_H__

#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include "SDL2_image/SDL_image.h"
#include "SDL2_mixer/SDL_mixer.h"
#include "SDL2_rotozoom.h"
#include "SDL2_ttf/SDL_ttf.h"












void mx_printerr(const char *s);
void respawn_trash(SDL_Rect *trash);
int mx_strlen(const char *s);
void swap(char *x, char *y);
char* reverse(char *buffer, int i, int j);
char* itoa(int value, char* buffer, int base);
void move_mine(float *x, float *y, int *angle);




#endif
