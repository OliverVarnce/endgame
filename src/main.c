#include <SDL2/SDL.h>
#include "SDL2_image/SDL_image.h"
#include "SDL2_rotozoom.h"
#include "SDL2_ttf/SDL_ttf.h"
#include "header.h"
#include <stdlib.h>
#include <stdio.h>


const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

void swap(char *x, char *y) 
{
    char t = *x; *x = *y; *y = t;
}

// function to reverse buffer[i..j]
char* reverse(char *buffer, int i, int j)
{
    while (i < j)
        swap(&buffer[i++], &buffer[j--]);

    return buffer;
}

// Iterative function to implement itoa() function in C
char* itoa(int value, char* buffer, int base)
{
    // invalid input
    if (base < 2 || base > 32)
        return buffer;

    // consider absolute value of number
    int n = abs(value);

    int i = 0;
    while (n)
    {
        int r = n % base;

        if (r >= 10) 
            buffer[i++] = 65 + (r - 10);
        else
            buffer[i++] = 48 + r;

        n = n / base;
    }

    // if number is 0
    if (i == 0)
        buffer[i++] = '0';

    // If base is 10 and value is negative, the resulting string 
    // is preceded with a minus sign (-)
    // With any other base, value is always considered unsigned
    if (value < 0 && base == 10)
        buffer[i++] = '-';

    buffer[i] = '\0'; // null terminate string

    // reverse the string and return it
    return reverse(buffer, 0, i - 1);
}

SDL_Surface *refresh_score(SDL_Surface *text, int score) {
    SDL_FreeSurface(text);
    char buff[3];
    char *score_string;
    TTF_Font* Sans = TTF_OpenFont("resources/OpenSans-Bold.ttf", 40);
    SDL_Color text_color = {247, 220, 111, 0};
    score_string = itoa(score, buff, 10);
    SDL_Surface *retext = TTF_RenderText_Solid(Sans, score_string, text_color);
    return retext;
}

void respawn_trash(SDL_Rect *trash)
{
    trash->x = random() % 1366;
    trash->y = random() % 800;
}
 
int main() {
    int running = 1;
    SDL_Event event;
    int score = 0;
    float speed = 0.0;
    
    float x = 200, y = 200;
    int angle = 0;

    SDL_Init(SDL_INIT_EVERYTHING);

    TTF_Init();
    char buff[3];
    char *score_string;
    TTF_Font* Sans = TTF_OpenFont("./resources/OpenSans-Bold.ttf", 40);
    SDL_Color text_color = {247, 220, 111, 0};
    score_string = itoa(score, buff, 10);
    SDL_Surface *text = TTF_RenderText_Solid(Sans, score_string, text_color);

    SDL_Window *window = SDL_CreateWindow("SAVE THE OCEAN", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Surface *surface = SDL_GetWindowSurface(window);  // создает sufrace для всего окна
    SDL_Surface *img = IMG_Load("./resources/water.jpg");
    SDL_Surface *ship = IMG_Load("./resources/ship.png"); // Эта функция загружвет изображение с любым расширением
    SDL_Surface *trash = IMG_Load("./resources/trash.jpg");
    SDL_Surface *mine = IMG_Load("./resources/mine.png");
    
   
    
    SDL_Rect rect = {0, 0, 0, 0}; // создаем прямоугольник с картинкой, которую будем вставлять. Первые две переменные x,y это начальные точки на экране  {x, y, h, w}
    SDL_Rect t = {500, 500, 0, 0};
    SDL_Rect txt = {1200, 30, 0, 0};
    SDL_Rect rec_mine = {100, 100, 0, 0};

    

    if (NULL == window)
        exit (1);
    
    while (running)
    {
        while(SDL_PollEvent(&event))
        {
            if((SDL_QUIT == event.type) || (SDL_KEYDOWN == event.type && SDL_SCANCODE_ESCAPE == event.key.keysym.scancode))
                running = 0;
            if (event.type == SDL_KEYDOWN && SDL_SCANCODE_UP == event.key.keysym.scancode) {
                speed = speed + 0.2;
            }
            if (event.type == SDL_KEYDOWN && SDL_SCANCODE_DOWN == event.key.keysym.scancode) {
                speed = speed - 0.5;
            }
            if (event.type == SDL_KEYDOWN && SDL_SCANCODE_LEFT == event.key.keysym.scancode)
                angle += 20;
            if (event.type == SDL_KEYDOWN && SDL_SCANCODE_RIGHT == event.key.keysym.scancode)
                angle -= 20;
        }

        if (speed < 0)
            speed = 0;
        if (speed > 3)
            speed = 3;

        x -= sin(angle*M_PI/180.0)*speed;
        y -= cos(angle*M_PI/180.0)*speed;

        if (angle >= 360) 
            angle -=360;
        if (angle <= -360)
            angle +=360;
        
        SDL_Surface * rotatedimage = rotozoomSurface(ship, angle, 1.0,0);
        SDL_Rect rec ={(int)x,(int)y,0,0}; //центровка корабля
        rec.x -= rotatedimage->w/2 - ship->w/2;
        rec.y -= rotatedimage->h/2 - ship->h/2;

        if (x < -50)
            x = 1920;
        if (x > 1920)
            x = -50;
        if (y < -50)
            y = 1080;
        if (y > 1080)
            y = -50;

        SDL_BlitSurface(img, NULL, surface, &rect); // вставляет картинку
        SDL_BlitSurface(rotatedimage, NULL, surface, &rec);
        SDL_BlitSurface(trash, NULL, surface, &t); // вставляет картинку
        SDL_BlitSurface(text, NULL, surface, &txt);
        SDL_BlitSurface(mine, NULL, surface, &rec_mine);

        SDL_UpdateWindowSurface(window);

        if(SDL_HasIntersection(&rec, &t) == SDL_TRUE)
        {
            respawn_trash(&t);
            score++;
            text = refresh_score(text, score);
        }
    }
    
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return (0);
}
