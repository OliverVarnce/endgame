#include <SDL2/SDL.h>
#include "SDL2_image/SDL_image.h"
#include "header.h"
#include <stdlib.h>
#include <stdio.h>

const int SCREEN_WIDTH = 1366;
const int SCREEN_HEIGHT = 800;

 
int main() {
    int running = 1;
    SDL_Event event;
    
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("SAVE THE OCEAN", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Surface *surface = SDL_GetWindowSurface(window);  // создает sufrace для всего окна
    SDL_Surface *img = IMG_Load("resources/water1.gif");
    SDL_Surface *ship = IMG_Load("resources/ship.png"); // Эта функция загружвет изображение с любым расширением
    SDL_Rect rect = {0, 0, 0, 0}; // создаем прямоугольник с картинкой, которую будем вставлять. Первые две переменные x,y это начальные точки на экране  {x, y, h, w}

    if (NULL == window)
        exit (1);
    
    while (running)
    {
        while(SDL_PollEvent(&event))
        {
            if((SDL_QUIT == event.type) || (SDL_KEYDOWN == event.type && SDL_SCANCODE_ESCAPE == event.key.keysym.scancode))
                running = 0;
        }
        SDL_BlitSurface(img, NULL, surface, &rect); // вставляет картинку
        SDL_BlitSurface(ship, NULL, surface, &rect); // вставляет картинку
        SDL_UpdateWindowSurface(window);
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    return (0);
}
