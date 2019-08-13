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
    int start = 1;
    int menu_up_down = 0;
    int pause = 1;
    int pause_up_down = 0;
    int running = 1;
    SDL_Event event;
    int score = 0;
    int lives = 3;
    float ship_speed = 0.0;
    
    int random_seed;
    int kostil = 0;

    float ship_x = 200, ship_y = 200;
    float mine1_x = 100, mine1_y = 1900;
    float mine2_x = 500, mine2_y = 1500;
    float mine3_x = 1000, mine3_y = 1000;
    float mine4_x = 1500, mine4_y = 500;
    float mine5_x = 1900, mine5_y = 100;
    float torpeda1_x = 2000, torpeda1_y = 100;
    int count_w = 1;
    int ship_angle = 0;
    int mine1_angle = 0;
    int mine2_angle = 0;
    int mine3_angle = 0;    
    int mine4_angle = 0;
    int mine5_angle = 0;
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
    SDL_Surface *img1 = IMG_Load("./resources/water/1.jpg");
    SDL_Surface *img2 = IMG_Load("./resources/water/2.jpg");
    SDL_Surface *img3 = IMG_Load("./resources/water/3.jpg");
    SDL_Surface *img4 = IMG_Load("./resources/water/4.jpg");

    SDL_Surface *ship = IMG_Load("./resources/ship.png"); // Эта функция загружвет изображение с любым расширением

    SDL_Surface *mine1 = IMG_Load("./resources/mine.png");
    SDL_Surface *mine2 = IMG_Load("./resources/mine.png");
    SDL_Surface *mine3 = IMG_Load("./resources/mine.png");
    SDL_Surface *mine4 = IMG_Load("./resources/mine.png");
    SDL_Surface *mine5 = IMG_Load("./resources/mine.png");

    SDL_Surface *trash = IMG_Load("./resources/trash.png");
    SDL_Surface *trash1 = IMG_Load("./resources/trash1.png");
    SDL_Surface *trash2 = IMG_Load("./resources/_trash2.png");

    SDL_Surface *torpeda1 = IMG_Load("./resources/torpeda.png");
    SDL_Surface *lives1 = IMG_Load("./resources/lives.png");
    SDL_Surface *lives2 = IMG_Load("./resources/lives.png");
    SDL_Surface *lives3 = IMG_Load("./resources/lives.png");

    SDL_Surface *pause_m1 = IMG_Load("./resources/pause1.jpg");
    SDL_Surface *pause_m2 = IMG_Load("./resources/pause2.jpg");

    SDL_Surface *start_m1 = IMG_Load("./resources/stat_quit1.jpg");
    SDL_Surface *start_m2 = IMG_Load("./resources/stat_quit2.jpg");

    //начальное положение текстур на карте
    SDL_Rect rect = {0, 0, 0, 0}; // создаем прямоугольник с картинкой, которую будем вставлять. Первые две переменные x,y это начальные точки на экране  {x, y, h, w}
    SDL_Rect t = {500, 500, 0, 0};

    SDL_Rect trash1_rec = {1000, 1000, 0, 0};
    SDL_Rect trash2_rec = {600, 600, 0, 0};

    SDL_Rect txt = {1800, 20, 0, 0};
    SDL_Rect txt2 = {1650, 20, 0, 0};
    SDL_Rect lives1_rect = {20, 20, 0, 0};
    SDL_Rect lives2_rect = {50, 20, 0, 0};
    SDL_Rect lives3_rect = {80, 20, 0, 0};

    SDL_Rect pause1_rect = {0, 0, 0, 0};
    SDL_Rect pause2_rect = {0, 0, 0, 0};

    SDL_Rect start1_rect = {0, 0, 0, 0};
    SDL_Rect start2_rect = {0, 0, 0, 0};

    if (NULL == window)
        exit (1);

    while(start)
    {
        while(SDL_PollEvent(&event))
        {

            if (event.type == SDL_KEYDOWN && SDL_SCANCODE_RETURN == event.key.keysym.scancode && 
                menu_up_down % 2 == 0)
            {
                start = 0;
            }
            else if (event.type == SDL_KEYDOWN && SDL_SCANCODE_RETURN == event.key.keysym.scancode && 
                menu_up_down % 2 != 0)
            {
                start = 0;
                running = 0;
            }
            if ((event.type == SDL_KEYDOWN && SDL_SCANCODE_UP == event.key.keysym.scancode) || 
                (event.type == SDL_KEYDOWN && SDL_SCANCODE_DOWN == event.key.keysym.scancode))
            {
                menu_up_down++;
            }
            if(menu_up_down % 2 == 0)
            {
                SDL_BlitSurface(start_m1, NULL, surface, &start1_rect);
            }
            else
            {
                SDL_BlitSurface(start_m2, NULL, surface, &start2_rect);
            }
            SDL_UpdateWindowSurface(window);
        }

    }
    start = 1;
    
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
            if (event.type == SDL_KEYDOWN && SDL_SCANCODE_P == event.key.keysym.scancode)
            {
                pause = 1;

                while(pause == 1)
                {
                    while(SDL_PollEvent(&event))
                    {
                        if (event.type == SDL_KEYDOWN && SDL_SCANCODE_RETURN == event.key.keysym.scancode && 
                            pause_up_down % 2 == 0)
                        {
                            pause = 0;
                        }
                        else if (event.type == SDL_KEYDOWN && SDL_SCANCODE_RETURN == event.key.keysym.scancode && 
                            pause_up_down % 2 != 0)
                        {
                            pause = 0;
                            running = 0;
                        }
                        if ((event.type == SDL_KEYDOWN && SDL_SCANCODE_UP == event.key.keysym.scancode) || 
                            (event.type == SDL_KEYDOWN && SDL_SCANCODE_DOWN == event.key.keysym.scancode))
                        {
                            pause_up_down++;
                        }
                        if(pause_up_down % 2 == 0)
                        {
                            SDL_BlitSurface(pause_m1, NULL, surface, &pause1_rect);
                        }
                        else
                        {
                            SDL_BlitSurface(pause_m2, NULL, surface, &pause2_rect);
                        }
                        SDL_UpdateWindowSurface(window);
                    }
                }
            }
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

        //счетчик дл анимации воды
        if(count_w > 120)
            count_w = 1;
        
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

        // mine2
        SDL_Surface * rotated_mine2 = rotozoomSurface(mine2, mine2_angle, 1.0,0);
        SDL_Rect mine2_rec ={(int)mine2_x,(int)mine2_y,0,0};
        mine2_rec.x -= rotated_mine2->w/2 - mine2->w/2;
        mine2_rec.y -= rotated_mine2->h/2 - mine2->h/2;
        move_mine(&mine2_x, &mine2_y, &mine2_angle);

        // mine3
        SDL_Surface * rotated_mine3 = rotozoomSurface(mine3, mine3_angle, 1.0,0);
        SDL_Rect mine3_rec ={(int)mine3_x,(int)mine3_y,0,0};
        mine3_rec.x -= rotated_mine3->w/2 - mine3->w/2;
        mine3_rec.y -= rotated_mine3->h/2 - mine3->h/2;
        move_mine(&mine3_x, &mine3_y, &mine3_angle);

        // mine4
        SDL_Surface * rotated_mine4 = rotozoomSurface(mine4, mine4_angle, 1.0,0);
        SDL_Rect mine4_rec ={(int)mine4_x,(int)mine4_y,0,0};
        mine4_rec.x -= rotated_mine4->w/2 - mine4->w/2;
        mine4_rec.y -= rotated_mine4->h/2 - mine4->h/2;
        move_mine(&mine4_x, &mine4_y, &mine4_angle);

        // mine5
        SDL_Surface * rotated_mine5 = rotozoomSurface(mine5, mine5_angle, 1.0,0);
        SDL_Rect mine5_rec ={(int)mine5_x,(int)mine5_y,0,0};
        mine5_rec.x -= rotated_mine5->w/2 - mine5->w/2;
        mine5_rec.y -= rotated_mine5->h/2 - mine5->h/2;
        move_mine(&mine5_x, &mine5_y, &mine5_angle);
        
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


        if (count_w > 1 && count_w < 20)
            SDL_BlitSurface(img1, NULL, surface, &rect); // вставляет картинку
        if (count_w > 20 && count_w < 40)
            SDL_BlitSurface(img2, NULL, surface, &rect); // вставляет картинку
        if (count_w > 40 && count_w < 60)
            SDL_BlitSurface(img3, NULL, surface, &rect); // вставляет картинку
        if (count_w > 60 && count_w < 80)
            SDL_BlitSurface(img4, NULL, surface, &rect); // вставляет картинку
        if (count_w > 80 && count_w < 100)
            SDL_BlitSurface(img3, NULL, surface, &rect); // вставляет картинку
        if (count_w > 100  && count_w < 120)
            SDL_BlitSurface(img2, NULL, surface, &rect); // вставляет картинку
        

        count_w++;

        SDL_BlitSurface(rotatedimage, NULL, surface, &rec);
        
        SDL_BlitSurface(trash, NULL, surface, &t);
        SDL_BlitSurface(trash1, NULL, surface, &trash1_rec);
        SDL_BlitSurface(trash2, NULL, surface, &trash2_rec);

        SDL_BlitSurface(text, NULL, surface, &txt);
        SDL_BlitSurface(text2, NULL, surface, &txt2);
        SDL_BlitSurface(rotated_torpeda1, NULL, surface, &torpeda1_rec);

        SDL_BlitSurface(rotated_mine1, NULL, surface, &mine1_rec);
        SDL_BlitSurface(rotated_mine2, NULL, surface, &mine2_rec);
        SDL_BlitSurface(rotated_mine3, NULL, surface, &mine3_rec);
        SDL_BlitSurface(rotated_mine4, NULL, surface, &mine4_rec);
        SDL_BlitSurface(rotated_mine5, NULL, surface, &mine5_rec);

        SDL_BlitSurface(lives1, NULL, surface, &lives1_rect);
        SDL_BlitSurface(lives2, NULL, surface, &lives2_rect);
        SDL_BlitSurface(lives3, NULL, surface, &lives3_rect);

        // update screen
        SDL_UpdateWindowSurface(window);

        // score && trash catch
        // trash 1
        if(SDL_HasIntersection(&rec, &t) == SDL_TRUE)
        {
            respawn_trash(&t);
            score++;
            text = refresh_score(text, score);
        }
        //trash 2
        if(SDL_HasIntersection(&rec, &trash1_rec) == SDL_TRUE)
        {
            respawn_trash(&trash1_rec);
            score++;
            text = refresh_score(text, score);
        }
        // trash 3
        if(SDL_HasIntersection(&rec, &trash2_rec) == SDL_TRUE)
        {
            respawn_trash(&trash2_rec);
            score++;
            text = refresh_score(text, score);
        }

        if(SDL_HasIntersection(&rec, &torpeda1_rec) == SDL_TRUE || SDL_HasIntersection(&rec, &mine1_rec) == SDL_TRUE || 
            SDL_HasIntersection(&rec, &mine2_rec) == SDL_TRUE || SDL_HasIntersection(&rec, &mine3_rec) == SDL_TRUE || 
            SDL_HasIntersection(&rec, &mine4_rec) == SDL_TRUE || SDL_HasIntersection(&rec, &mine5_rec) == SDL_TRUE) {
            lives--;
            if (lives == 2)
                lives3_rect.x = 2000;
            if (lives == 1)
                lives2_rect.x = 2000;
            if (lives == 0)
            {

                running = 0;
                
            }
            ship_x = 900;
            ship_y = 400;
            ship_speed = 0;
        }

        TTF_CloseFont(Sans);
        Sans = NULL;
        // SDL_FreeSurface(torpeda1);
        // torpeda1 = NULL;

    }
    
    while(start)
    {
        while(SDL_PollEvent(&event))
        {

            if (event.type == SDL_KEYDOWN && SDL_SCANCODE_RETURN == event.key.keysym.scancode && 
                menu_up_down % 2 == 0)
            {
                start = 0;
            }
            else if (event.type == SDL_KEYDOWN && SDL_SCANCODE_RETURN == event.key.keysym.scancode && 
                menu_up_down % 2 != 0)
            {
                start = 0;
                running = 0;
            }
            if ((event.type == SDL_KEYDOWN && SDL_SCANCODE_UP == event.key.keysym.scancode) || 
                (event.type == SDL_KEYDOWN && SDL_SCANCODE_DOWN == event.key.keysym.scancode))
            {
                menu_up_down++;
            }
            if(menu_up_down % 2 == 0)
            {
                SDL_BlitSurface(start_m1, NULL, surface, &start1_rect);
            }
            else
            {
                SDL_BlitSurface(start_m2, NULL, surface, &start2_rect);
            }
            SDL_UpdateWindowSurface(window);
        }

    }

    // free memory
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return (0);
}
