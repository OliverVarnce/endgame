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
    trash->x = random() % 1920;
    trash->y = random() % 1080;
}

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
 
int main() {
    int running = 1;
    SDL_Event event;
    int score = 0;
    int lives = 3;
    float ship_speed = 0.0;
    
    int random_seed;
    int kostil = 0;

    float ship_x = 200, ship_y = 200;
    float mine1_x = 200, mine1_y = 1000;
    float torpeda1_x = 2000, torpeda1_y = 100;
    int ship_angle = 0;
    int mine1_angle = 0;
    int torpeda1_angle = 0;

    SDL_Init(SDL_INIT_EVERYTHING);

    TTF_Init();
    char buff[3];
    char *score_string;
    TTF_Font* Sans = TTF_OpenFont("./resources/OpenSans-Bold.ttf", 40);
    SDL_Color text_color = {247, 220, 111, 0};
    score_string = itoa(score, buff, 10);
    SDL_Surface *text = TTF_RenderText_Solid(Sans, score_string, text_color);
    SDL_Surface *text2 = TTF_RenderText_Solid(Sans, "Score:", text_color);

    SDL_Window *window = SDL_CreateWindow("SAVE THE OCEAN", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Surface *surface = SDL_GetWindowSurface(window);  // создает sufrace для всего окна

    // текстуры
    SDL_Surface *img = IMG_Load("./resources/water.jpg");
    SDL_Surface *ship = IMG_Load("./resources/ship.png"); // Эта функция загружвет изображение с любым расширением
    SDL_Surface *trash = IMG_Load("./resources/trash.jpg");
    SDL_Surface *mine1 = IMG_Load("./resources/mine.png");
    SDL_Surface *torpeda1 = IMG_Load("./resources/torpeda.png");
    SDL_Surface *lives1 = IMG_Load("./resources/lives.png");
    SDL_Surface *lives2 = IMG_Load("./resources/lives.png");
    SDL_Surface *lives3 = IMG_Load("./resources/lives.png");

    //начальное положение текстур на карте
    SDL_Rect rect = {0, 0, 0, 0}; // создаем прямоугольник с картинкой, которую будем вставлять. Первые две переменные x,y это начальные точки на экране  {x, y, h, w}
    SDL_Rect t = {500, 500, 0, 0};
    SDL_Rect txt = {1800, 20, 0, 0};
    SDL_Rect txt2 = {1650, 20, 0, 0};
    SDL_Rect lives1_rect = {20, 20, 0, 0};
    SDL_Rect lives2_rect = {50, 20, 0, 0};
    SDL_Rect lives3_rect = {80, 20, 0, 0};

    if (NULL == window)
        exit (1);
    
    while (running)
    {
        // обработка нажатий клавиш
        while(SDL_PollEvent(&event))
        {
            if((SDL_QUIT == event.type) || (SDL_KEYDOWN == event.type && SDL_SCANCODE_ESCAPE == event.key.keysym.scancode))
                running = 0;
            if (event.type == SDL_KEYDOWN && SDL_SCANCODE_UP == event.key.keysym.scancode) {
                ship_speed = ship_speed + 0.2;
            }
            if (event.type == SDL_KEYDOWN && SDL_SCANCODE_DOWN == event.key.keysym.scancode) {
                ship_speed = ship_speed - 0.5;
            }
            if (event.type == SDL_KEYDOWN && SDL_SCANCODE_LEFT == event.key.keysym.scancode)
                ship_angle += 20;
            if (event.type == SDL_KEYDOWN && SDL_SCANCODE_RIGHT == event.key.keysym.scancode)
                ship_angle -= 20;
        }

        // ограничение скорости корабля
        if (ship_speed < 0)
            ship_speed = 0;
        if (ship_speed > 3)
            ship_speed = 3;

        //"Двигатель" корабля
        ship_x -= sin(ship_angle*M_PI/180.0)*ship_speed;
        ship_y -= cos(ship_angle*M_PI/180.0)*ship_speed;

        // ограничение корабля по поворотам
        if (ship_angle >= 360) 
            ship_angle -=360;
        if (ship_angle <= -360)
            ship_angle +=360;
        
        //Магия для корабля, не трогать
        SDL_Surface * rotatedimage = rotozoomSurface(ship, ship_angle, 1.0,0);
        SDL_Rect rec ={(int)ship_x,(int)ship_y,0,0}; //центровка корабля
        rec.x -= rotatedimage->w/2 - ship->w/2;
        rec.y -= rotatedimage->h/2 - ship->h/2;

        // mine1
        SDL_Surface * rotated_mine1 = rotozoomSurface(mine1, mine1_angle, 1.0,0);
        SDL_Rect mine1_rec ={(int)mine1_x,(int)mine1_y,0,0};
        mine1_rec.x -= rotated_mine1->w/2 - mine1->w/2;
        mine1_rec.y -= rotated_mine1->h/2 - mine1->h/2;
        move_mine(&mine1_x, &mine1_y, &mine1_angle);
        /*
        mine1_x -= sin(mine1_angle*M_PI/180.0)*0.1;
        mine1_y -= cos(mine1_angle*M_PI/180.0)*0.1;
        if (mine1_x < -50)
            mine1_x = 1920;
        if (mine1_x > 1920)
            mine1_x = -50;
        if (mine1_y < -50)
            mine1_y = 1080;
        if (mine1_y > 1080)
            mine1_y = -50;
        random_seed = random() % 11;
        if ((random() % 100) % 88 == 0) {
            if (random_seed % 2 == 0)
                mine1_angle += random_seed;
            else
                mine1_angle -= random_seed;
        }
        */
        
        // torpeda1
        SDL_Surface * rotated_torpeda1 = rotozoomSurface(torpeda1, torpeda1_angle, 1.0,0);
        SDL_Rect torpeda1_rec ={(int)torpeda1_x,(int)torpeda1_y,0,0};
        torpeda1_rec.x -= rotated_torpeda1->w/2 - torpeda1->w/2;
        torpeda1_rec.y -= rotated_torpeda1->h/2 - torpeda1->h/2;
        if (score % 3 == 0 && kostil == 0 && score != 0) {
            random_seed = random() % 4;
            kostil = 1;
            if (random_seed == 0) {
                torpeda1_x = random() % 1920;
                torpeda1_y = 1080;
                torpeda1_angle = 0;
            }
            else if (random_seed == 1) {
                torpeda1_x = random() % 1920;
                torpeda1_y = 0;
                torpeda1_angle = 180;
            }
            else if (random_seed == 2) {
                torpeda1_x = 0;
                torpeda1_y = random() % 1080;
                torpeda1_angle = 270;
            }
            else if (random_seed == 3) {
                torpeda1_x = 1920;
                torpeda1_y = random() % 1080;
                torpeda1_angle = 90;
            }
        }
        else if (score % 3 != 0)
            kostil = 0;
        torpeda1_x -= sin(torpeda1_angle*M_PI/180.0)*3;
        torpeda1_y -= cos(torpeda1_angle*M_PI/180.0)*3;
        // возврат корабля обратно на карту (если он выходит за карту)
        if (ship_x < -50)
            ship_x = 1920;
        if (ship_x > 1920)
            ship_x = -50;
        if (ship_y < -50)
            ship_y = 1080;
        if (ship_y > 1080)
            ship_y = -50;

        SDL_BlitSurface(img, NULL, surface, &rect); // вставляет картинку
        SDL_BlitSurface(rotatedimage, NULL, surface, &rec);
        SDL_BlitSurface(trash, NULL, surface, &t); 
        SDL_BlitSurface(text, NULL, surface, &txt);
        SDL_BlitSurface(text2, NULL, surface, &txt2);
        SDL_BlitSurface(rotated_mine1, NULL, surface, &mine1_rec);
        SDL_BlitSurface(rotated_torpeda1, NULL, surface, &torpeda1_rec);

        SDL_BlitSurface(lives1, NULL, surface, &lives1_rect);
        SDL_BlitSurface(lives2, NULL, surface, &lives2_rect);
        SDL_BlitSurface(lives3, NULL, surface, &lives3_rect);

        // update screen
        SDL_UpdateWindowSurface(window);

        // score
        if(SDL_HasIntersection(&rec, &t) == SDL_TRUE)
        {
            respawn_trash(&t);
            score++;
            text = refresh_score(text, score);
        }
        if(SDL_HasIntersection(&rec, &torpeda1_rec) == SDL_TRUE || SDL_HasIntersection(&rec, &mine1_rec) == SDL_TRUE) {
            lives--;
            if (lives == 2)
                lives3_rect.x = 2000;
            if (lives == 1)
                lives2_rect.x = 2000;
            if (lives == 0)
                lives1_rect.x = 2000;
            ship_x = 900;
            ship_y = 600;
        }
    }
    
    // free memory
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return (0);
}
