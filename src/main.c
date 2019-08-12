#include <SDL2/SDL.h>
#include "SDL2_image/SDL_image.h"
#include "header.h"
#include <stdlib.h>
#include <stdio.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;





    
int main() {
    int running = 1;
    SDL_Event event;
    
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("SAVE THE OCEAN", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    if (NULL == window)
        exit (1);
    
    while (running)
        while(SDL_PollEvent(&event))
            if((SDL_QUIT == event.type) || (SDL_KEYDOWN == event.type && SDL_SCANCODE_ESCAPE == event.key.keysym.scancode))
                running = 0;
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    return (0);
}
