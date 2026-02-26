#ifndef ENGINE_H
#define ENGINE_H
#include "SDL.h"
#include "Player.h"
#include "SDL_image.h"
#include "Enemy.h"

#include<cassert>
#include<iostream>

constexpr int WIDTH = 500;
constexpr int HEIGHT = 700;
constexpr int DELAY_TIME = 30;
class Ecran
{
public:
    static Ecran* GetInstance(){ return s_Instance = (s_Instance != nullptr)? s_Instance : new Ecran();}
    bool Init();
    void Quit();

    void Update();
    void Render();
    void Events();

    inline bool Running(){return ok;}
    inline SDL_Renderer* GetRenderer(){return renderer;}

private:
    Ecran(){}
    static Ecran* s_Instance;
    bool ok;
    SDL_Window* fereastra;
    SDL_Renderer* renderer;
};

extern bool meniu, game_solo, game_coop, settings, nivel_menu_solo, nivel_menu_coop, info;
extern int Level;
extern int MouseXPos, MouseYPos;
extern int delay_click;
extern int delay_esc;
extern int delay_paused;
extern int delay_mute;
extern bool lost, paused, muted;
extern bool unlocked_solo[10];
extern bool unlocked_coop[10];

#endif //ECRAN_H
