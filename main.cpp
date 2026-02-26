#include <bits/stdc++.h>
#include "Player.h"
#include "Ecran.h"
#include "Texture.h"
#include "Windows.h"
#include "mmsystem.h"
#include "SDL_mixer.h"

using namespace std;

int cnt_1 = 0, cnt_2 = 0;
Mix_Music*muzica = NULL;

void Update_Delays () {
    if (delay_click > 0) delay_click --;
    if (delay_esc > 0) delay_esc --;
    if (delay_paused > 0) delay_paused --;
    if (delay_mute > 0) delay_mute --;
    if (cnt_1 != 0) cnt_1++;
    if (cnt_2 != 0) cnt_2++;
    if (cnt_1 > DELAY_SHOOT) cnt_1 = 0;
    if (cnt_2 > DELAY_SHOOT) cnt_2 = 0;
}

void Joc () {
    if (GetAsyncKeyState('P') && delay_paused == 0) {
        paused = !paused;
        delay_paused = DELAY_TIME;
    }
    if (GetAsyncKeyState('A'))
        Player::GetInstance()->Move(-1, 0);
    if (GetAsyncKeyState('D'))
        Player::GetInstance()->Move(1, 0);
    if (GetAsyncKeyState('W'))
        Player::GetInstance()->Move(0, -1);
    if (GetAsyncKeyState('S'))
        Player::GetInstance()->Move(0, 1);
    if (GetAsyncKeyState(' '))
        if (cnt_1 == 0) {
            Player::GetInstance()->Shoot();
            cnt_1 = 1;
        }
    if (GetAsyncKeyState(VK_ESCAPE) && delay_esc == 0) {
        delay_esc = DELAY_TIME;
        nivel_menu_solo = 1;
        Player::GetInstance()->Clean();
        Clean_Inamici();
        game_solo = 0;
    }

    //system("PAUSE");
}

void Joc_CoOp () {
    if (GetAsyncKeyState('P') && delay_paused == 0) {
        paused = !paused;
        delay_paused = DELAY_TIME;
    }
    if (GetAsyncKeyState('A'))
        Player::GetInstance()->Move(-1, 0);
    if (GetAsyncKeyState('D'))
        Player::GetInstance()->Move(1, 0);
    if (GetAsyncKeyState('W'))
        Player::GetInstance()->Move(0, -1);
    if (GetAsyncKeyState('S'))
        Player::GetInstance()->Move(0, 1);
    if (GetAsyncKeyState(' '))
        if (cnt_1 == 0 && Player::GetInstance()->lives >= 1) {
            Player::GetInstance()->Shoot();
            cnt_1 = 1;
        }

    if (GetAsyncKeyState(VK_LEFT))
        Player_2::GetInstance()->Move(-1, 0);
    if (GetAsyncKeyState(VK_RIGHT))
        Player_2::GetInstance()->Move(1, 0);
    if (GetAsyncKeyState(VK_UP))
        Player_2::GetInstance()->Move(0, -1);
    if (GetAsyncKeyState(VK_DOWN))
        Player_2::GetInstance()->Move(0, 1);
    if (GetAsyncKeyState(VK_RETURN))
        if (cnt_2 == 0 && Player_2::GetInstance()->lives >= 1) {
            Player_2::GetInstance()->Shoot();
            cnt_2 = 1;
        }
    if (GetAsyncKeyState(VK_ESCAPE) && delay_esc == 0) {
        delay_esc = DELAY_TIME;
        nivel_menu_coop = 1;
        Player::GetInstance()->Clean();
        Player_2::GetInstance()->Clean();
        Clean_Inamici();
        game_coop = 0;
    }

    //system("PAUSE");
}
int main(int argc, char *args[])
{
    Ecran::GetInstance()->Init();
    Player::GetInstance()->Init();
    Player_2::GetInstance()->Init();

    SDL_ShowCursor(SDL_ENABLE);
    Mix_OpenAudio (22050, MIX_DEFAULT_FORMAT, 2, 4096);
    muzica = Mix_LoadMUS("song.wav");
    Mix_PlayMusic(muzica, -1);
    //PlaySound(TEXT("song.wav"), NULL, SND_FILENAME|SND_LOOP|SND_ASYNC);
    while (Ecran::GetInstance()->Running()) {
        Update_Delays();

        if (meniu) {
            cnt_1 = 0;
            cnt_2 = 0;
            SDL_ShowCursor(SDL_ENABLE);
            SDL_GetMouseState(&MouseXPos, &MouseYPos);
            Ecran::GetInstance()->Events();
            Ecran::GetInstance()->Update();
            //cout << MouseXPos << " " << MouseYPos << '\n';
        }
        else if (nivel_menu_solo) {
            SDL_ShowCursor(SDL_ENABLE);
            SDL_GetMouseState(&MouseXPos, &MouseYPos);

            Ecran::GetInstance()->Events();

            if (GetAsyncKeyState(VK_ESCAPE) && delay_esc == 0) {
                delay_esc = DELAY_TIME;
                nivel_menu_solo = 0;
                meniu = 1;
            }
            //cout << MouseXPos << " " << MouseYPos << '\n';
        }
        else if (nivel_menu_coop) {
            SDL_ShowCursor(SDL_ENABLE);
            SDL_GetMouseState(&MouseXPos, &MouseYPos);

            if (GetAsyncKeyState(VK_ESCAPE) && delay_esc == 0) {
                delay_esc = DELAY_TIME;
                nivel_menu_coop = 0;
                meniu = 1;
            }
            Ecran::GetInstance()->Events();
        }
        else if (game_coop) {
            Ecran::GetInstance()->Events();
            Ecran::GetInstance()->Update();
            Joc_CoOp();
        }
        else if (game_solo) {
            Ecran::GetInstance()->Events();
            Ecran::GetInstance()->Update();
            Joc();
            //cout << Player::GetInstance()->pozX << " " << Player::GetInstance()->pozY << " " << cnt << '\n';
        }
        else if (info) {
            Ecran::GetInstance()->Events();
            Ecran::GetInstance()->Update();
            if (GetAsyncKeyState(VK_ESCAPE) && delay_esc == 0) {
                delay_esc = DELAY_TIME;
                info = 0;
                meniu = 1;
            }
        }

        if (GetAsyncKeyState('M') && delay_mute == 0) {
            delay_mute = DELAY_TIME;

            muted = !muted;

            if (muted == 0)
                Mix_ResumeMusic();
            else Mix_PauseMusic();
        }
        Ecran::GetInstance()->Render();

        //cout << Player::GetInstance()->pozX << " " << Player::GetInstance()->pozY << " " << cnt << '\n';
    }

    return 0;
}
