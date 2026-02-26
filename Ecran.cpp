#include "Ecran.h"
#include "Texture.h"
#include "Player.h"
#include "Enemy.h"

Ecran* Ecran::s_Instance = nullptr;

bool meniu, game_solo, game_coop, nivel_menu_solo, nivel_menu_coop, lost, info;
int Level, delay_click, delay_esc, delay_paused, delay_mute;
int MouseXPos, MouseYPos;
bool unlocked_solo[10];
bool unlocked_coop[10];
bool paused, muted;

bool Ecran::Init()
{
    unlocked_coop[1] = 1;
    unlocked_solo[1] = 1;
    SDL_INIT_AUDIO;
    meniu = 1;
    ok = 1;
    if (SDL_Init(SDL_INIT_VIDEO) != 0 && IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != 0) {
        SDL_Log("Eroare la initializare: %s", SDL_GetError());
        ok = 0;
    }

    fereastra = SDL_CreateWindow("Star Reacher", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);

    if (fereastra == nullptr) {
        SDL_Log("Eroare la crearea ferestrei: %s", SDL_GetError());
        ok = 0;
    }

    renderer = SDL_CreateRenderer(fereastra, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        SDL_Log("Eroare la crearea rendererului: %s", SDL_GetError());
        ok = 0;
    }

    Texture::GetInstance()->Load("player", "Images/Racheta1.png");
    Texture::GetInstance()->Load("player2", "Images/Racheta2.png");
    Texture::GetInstance()->Load("bullet", "Images/bullet.png");
    Texture::GetInstance()->Load("background_level_1", "Images/Background_1.jpg");
    Texture::GetInstance()->Load("meniu", "Images/Menu.jpg");
    Texture::GetInstance()->Load("asteroid", "Images/steroid.png");
    Texture::GetInstance()->Load("boom", "Images/boom.png");
    Texture::GetInstance()->Load("nivele", "Images/Level_Menu.jpg");
    Texture::GetInstance()->Load("healthbar3", "Images/full.png");
    Texture::GetInstance()->Load("healthbar2", "Images/2.png");
    Texture::GetInstance()->Load("healthbar1", "Images/1.png");
    Texture::GetInstance()->Load("healthbar0", "Images/0.png");
    Texture::GetInstance()->Load("dead", "Images/YOU DIED.png");
    Texture::GetInstance()->Load("won", "Images/LEVEL COMPLETE.png");
    Texture::GetInstance()->Load("laser", "Images/lazer.png");
    Texture::GetInstance()->Load("enemyBullet", "Images/enemy bullet.png");
    Texture::GetInstance()->Load("shooter", "Images/shooter.png");
    Texture::GetInstance()->Load("controls", "Images/CONTROLS.jpg");
    Texture::GetInstance()->Load("lock", "Images/lock.png");
    Texture::GetInstance()->Load("paused", "Images/Pause.png");
    return ok;
}

void Ecran::Quit()
{
    ok = 0;
}

void Ecran::Update()
{
    if (paused) return;
    if (lost) return;
    std::vector<std::pair<int, int> > aux;
    std::vector<std::pair<std::pair<int, int>, int> > aux_Booms;

    for (auto it : Booms) {
        it.second--;

        if (it.second > 0) aux_Booms.push_back({{it.first.first, it.first.second}, it.second});
    }
    Booms = aux_Booms;

    for (auto it : Bullets) {
        if (it.second <= 1) {
            continue;
        }
        else aux.push_back(std::make_pair(it.first, it.second-2));
    }
    Bullets.clear();

    std::vector<Steroid> aux_1;
    std::vector<bool> elim_1(E_1.size());
    for (int i = 0; i < E_1.size(); ++ i ) {
        Steroid V = E_1[i];

        V.W--;

        if (V.W == 0) {
            V.y++;
            V.W = WALK_DELAY;
        }

        if (V.y == HEIGHT) {
            if (Player::GetInstance()->lives > 0) Player::GetInstance()->lives--;
            else if (game_coop && Player_2::GetInstance()->lives > 0) Player_2::GetInstance()->lives--;
            continue;
        }
        aux_1.push_back(V);
    }
    E_1.clear();

    std::vector<WeakShooter> aux_2;
    std::vector<bool> elim_2(E_2.size());
    for (int i = 0; i < E_2.size(); ++ i ) {
        WeakShooter V = E_2[i];

        V.W--;

        if (V.W == 0) {
            V.y++;
            V.W = WALK_DELAY;
        }

        V.S--;
        if (V.S == 0) {
            V.S = SHOOT_DELAY_ENEMY;
            Weak_Bullets.push_back({V.x, V.y + 64});
        }

        if (V.y == HEIGHT) {
            if (Player::GetInstance()->lives > 0) Player::GetInstance()->lives--;
            else if (game_coop && Player_2::GetInstance()->lives > 0) Player_2::GetInstance()->lives--;
            continue;
        }
        aux_2.push_back(V);
    }
    E_2.clear();

    std::vector<StrongShooter> aux_3;
    std::vector<bool> elim_3(E_3.size());
    for (int i = 0; i < E_3.size(); ++ i ) {
        StrongShooter V = E_3[i];

        V.W--;

        if (V.W == 0) {
            V.y++;
            V.W = WALK_DELAY;
        }

        V.S--;
        if (V.S == 0) {
            V.S = SHOOT_DELAY_ENEMY;
            Strong_Bullets.push_back({V.x, V.y + 64});
        }

        if (V.y == HEIGHT) {
            if (Player::GetInstance()->lives > 0) Player::GetInstance()->lives--;
            else if (game_coop && Player_2::GetInstance()->lives > 0) Player_2::GetInstance()->lives--;
            continue;
        }
        aux_3.push_back(V);
    }
    E_3.clear();



    for (int i = 0; i < aux.size(); ++ i ) {
        bool elim_i = 0;
        for (int j = 0; j < aux_1.size(); ++ j ) {
            if (elim_1[j]) continue;

            if (Intersect_PlayerBullet_Enemy(aux[i].first, aux[i].second, aux_1[j].x, aux_1[j].y, aux_1[j].x+52, aux_1[j].y+54)) {
                Booms.push_back({{aux_1[j].x, aux_1[j].y}, 50});
                elim_1[j] = 1;
                elim_i = 1;
                break;
            }
        }

        if (elim_i) continue;

        for (int j = 0; j < aux_2.size(); ++ j ) {
            if (elim_2[j]) continue;

            bool ok = 0;
            ok |= Intersect_PlayerBullet_Enemy(aux[i].first, aux[i].second, aux_2[j].x, aux_2[j].y + 20, aux_2[j].x + 58, aux_2[j].y + 30);
            ok |= Intersect_PlayerBullet_Enemy(aux[i].first, aux[i].second, aux_2[j].x+15, aux_2[j].y + 30, aux_2[j].x + 40, aux_2[j].y + 45);
            ok |= Intersect_PlayerBullet_Enemy(aux[i].first, aux[i].second, aux_2[j].x+20, aux_2[j].y + 45, aux_2[j].x + 35, aux_2[j].y + 64);

            if (ok) {
                Booms.push_back({{aux_2[j].x, aux_2[j].y}, 50});
                elim_2[j] = 1;
                elim_i = 1;
                break;
            }
        }
        if (elim_i) continue;

        for (int j = 0; j < aux_3.size(); ++ j ) {
            if (elim_3[j]) continue;

            bool ok = 0;
            ok |= Intersect_PlayerBullet_Enemy(aux[i].first, aux[i].second, aux_3[j].x, aux_3[j].y + 15, aux_3[j].x + 10, aux_3[j].y + 50);
            ok |= Intersect_PlayerBullet_Enemy(aux[i].first, aux[i].second, aux_3[j].x+20, aux_3[j].y + 20, aux_3[j].x + 40, aux_3[j].y + 50);
            ok |= Intersect_PlayerBullet_Enemy(aux[i].first, aux[i].second, aux_3[j].x+40, aux_3[j].y + 20, aux_3[j].x + 58, aux_3[j].y + 50);

            if (ok) {
                Booms.push_back({{aux_3[j].x, aux_3[j].y}, 50});
                elim_3[j] = 1;
                elim_i = 1;
                break;
            }
        }
        if (elim_i == 0) Bullets.push_back(aux[i]);
    }

    for (int j = 0; j < aux_1.size(); ++ j )
        if (!elim_1[j]) {
            Steroid x = aux_1[j];
            bool ok_1 = 0, ok_2 = 0;
            bool distrus_player = 0;
            ok_1 |= DreptunghiIntersectDreptunghi(Player::GetInstance()->pozX + 25, Player::GetInstance()->pozY, Player::GetInstance()->pozX + 30, Player::GetInstance()->pozY+15, x.x, x.y, x.x+52, x.y+54);
            ok_1 |= DreptunghiIntersectDreptunghi(Player::GetInstance()->pozX + 20, Player::GetInstance()->pozY+20, Player::GetInstance()->pozX + 35, Player::GetInstance()->pozY+40, x.x, x.y, x.x+52, x.y+54);
            ok_1 |= DreptunghiIntersectDreptunghi(Player::GetInstance()->pozX, Player::GetInstance()->pozY + 30, Player::GetInstance()->pozX + 55, Player::GetInstance()->pozY+40, x.x, x.y, x.x+52, x.y+54);

            if (game_coop) {
                ok_2 |= DreptunghiIntersectDreptunghi(Player_2::GetInstance()->pozX + 25, Player_2::GetInstance()->pozY, Player_2::GetInstance()->pozX + 30, Player_2::GetInstance()->pozY+15, x.x, x.y, x.x+52, x.y+54);
                ok_2 |= DreptunghiIntersectDreptunghi(Player_2::GetInstance()->pozX + 20, Player_2::GetInstance()->pozY+20, Player_2::GetInstance()->pozX + 35, Player_2::GetInstance()->pozY+40, x.x, x.y, x.x+52, x.y+54);
                ok_2 |= DreptunghiIntersectDreptunghi(Player_2::GetInstance()->pozX, Player_2::GetInstance()->pozY + 30, Player_2::GetInstance()->pozX + 55, Player_2::GetInstance()->pozY+40, x.x, x.y, x.x+52, x.y+54);
            }

            if (game_solo) {
                if (ok_1) {
                    if (Player::GetInstance()->lives >= 1) Player::GetInstance()->lives--;
                    else E_1.push_back(aux_1[j]);
                }
                else E_1.push_back(aux_1[j]);
            }
            if (game_coop) {
                if (ok_1 && Player::GetInstance()->lives >= 1) Player::GetInstance()->lives--;
                else if (ok_2 && Player_2::GetInstance()->lives >= 1) Player_2::GetInstance()->lives--;
                else E_1.push_back(aux_1[j]);
            }
        }

    for (int j = 0; j < aux_2.size(); ++ j ) {
        if (elim_2[j]) continue;

        WeakShooter x = aux_2[j];
        bool ok_1 = 0, ok_2 = 0;
        ok_1 |= DreptunghiIntersectDreptunghi(Player::GetInstance()->pozX + 25, Player::GetInstance()->pozY, Player::GetInstance()->pozX + 30, Player::GetInstance()->pozY+15, x.x, x.y+20, x.x+58, x.y+30);
        ok_1 |= DreptunghiIntersectDreptunghi(Player::GetInstance()->pozX + 25, Player::GetInstance()->pozY, Player::GetInstance()->pozX + 30, Player::GetInstance()->pozY+15, x.x+15, x.y+30, x.x+40, x.y+45);
        ok_1 |= DreptunghiIntersectDreptunghi(Player::GetInstance()->pozX + 25, Player::GetInstance()->pozY, Player::GetInstance()->pozX + 30, Player::GetInstance()->pozY+15, x.x+20, x.y+45, x.x+35, x.y+64);

        ok_1 |= DreptunghiIntersectDreptunghi(Player::GetInstance()->pozX + 20, Player::GetInstance()->pozY+20, Player::GetInstance()->pozX + 35, Player::GetInstance()->pozY+40, x.x, x.y+20, x.x+58, x.y+30);
        ok_1 |= DreptunghiIntersectDreptunghi(Player::GetInstance()->pozX + 20, Player::GetInstance()->pozY+20, Player::GetInstance()->pozX + 35, Player::GetInstance()->pozY+40, x.x+15, x.y+30, x.x+40, x.y+45);
        ok_1 |= DreptunghiIntersectDreptunghi(Player::GetInstance()->pozX + 20, Player::GetInstance()->pozY+20, Player::GetInstance()->pozX + 35, Player::GetInstance()->pozY+40, x.x+20, x.y+45, x.x+35, x.y+64);

        ok_1 |= DreptunghiIntersectDreptunghi(Player::GetInstance()->pozX, Player::GetInstance()->pozY + 30, Player::GetInstance()->pozX + 55, Player::GetInstance()->pozY+40, x.x, x.y+20, x.x+58, x.y+30);
        ok_1 |= DreptunghiIntersectDreptunghi(Player::GetInstance()->pozX, Player::GetInstance()->pozY + 30, Player::GetInstance()->pozX + 55, Player::GetInstance()->pozY+40, x.x+15, x.y+30, x.x+40, x.y+45);
        ok_1 |= DreptunghiIntersectDreptunghi(Player::GetInstance()->pozX, Player::GetInstance()->pozY + 30, Player::GetInstance()->pozX + 55, Player::GetInstance()->pozY+40, x.x+20, x.y+45, x.x+35, x.y+64);

        if (game_coop) {
            ok_2 |= DreptunghiIntersectDreptunghi(Player_2::GetInstance()->pozX + 25, Player_2::GetInstance()->pozY, Player_2::GetInstance()->pozX + 30, Player_2::GetInstance()->pozY+15, x.x, x.y+20, x.x+58, x.y+30);
            ok_2 |= DreptunghiIntersectDreptunghi(Player_2::GetInstance()->pozX + 25, Player_2::GetInstance()->pozY, Player_2::GetInstance()->pozX + 30, Player_2::GetInstance()->pozY+15, x.x+15, x.y+30, x.x+40, x.y+45);
            ok_2 |= DreptunghiIntersectDreptunghi(Player_2::GetInstance()->pozX + 25, Player_2::GetInstance()->pozY, Player_2::GetInstance()->pozX + 30, Player_2::GetInstance()->pozY+15, x.x+20, x.y+45, x.x+35, x.y+64);

            ok_2 |= DreptunghiIntersectDreptunghi(Player_2::GetInstance()->pozX + 20, Player_2::GetInstance()->pozY+20, Player_2::GetInstance()->pozX + 35, Player_2::GetInstance()->pozY+40, x.x, x.y+20, x.x+58, x.y+30);
            ok_2 |= DreptunghiIntersectDreptunghi(Player_2::GetInstance()->pozX + 20, Player_2::GetInstance()->pozY+20, Player_2::GetInstance()->pozX + 35, Player_2::GetInstance()->pozY+40, x.x+15, x.y+30, x.x+40, x.y+45);
            ok_2 |= DreptunghiIntersectDreptunghi(Player_2::GetInstance()->pozX + 20, Player_2::GetInstance()->pozY+20, Player_2::GetInstance()->pozX + 35, Player_2::GetInstance()->pozY+40, x.x+20, x.y+45, x.x+35, x.y+64);

            ok_2 |= DreptunghiIntersectDreptunghi(Player_2::GetInstance()->pozX, Player_2::GetInstance()->pozY + 30, Player_2::GetInstance()->pozX + 55, Player_2::GetInstance()->pozY+40, x.x, x.y+20, x.x+58, x.y+30);
            ok_2 |= DreptunghiIntersectDreptunghi(Player_2::GetInstance()->pozX, Player_2::GetInstance()->pozY + 30, Player_2::GetInstance()->pozX + 55, Player_2::GetInstance()->pozY+40, x.x+15, x.y+30, x.x+40, x.y+45);
            ok_2 |= DreptunghiIntersectDreptunghi(Player_2::GetInstance()->pozX, Player_2::GetInstance()->pozY + 30, Player_2::GetInstance()->pozX + 55, Player_2::GetInstance()->pozY+40, x.x+20, x.y+45, x.x+35, x.y+64);
        }

        if (game_solo) {
            if (ok_1) {
                if (Player::GetInstance()->lives >= 1) Player::GetInstance()->lives--;
                else E_2.push_back(aux_2[j]);
            }
            else E_2.push_back(aux_2[j]);
        }
        if (game_coop) {
            if (ok_1 && Player::GetInstance()->lives >= 1) Player::GetInstance()->lives--;
            else if (ok_2 && Player_2::GetInstance()->lives >= 1) Player_2::GetInstance()->lives--;
            else E_2.push_back(aux_2[j]);
        }
    }

    for (int j = 0; j < aux_3.size(); ++ j ) {
        if (elim_3[j]) continue;

        StrongShooter x = aux_3[j];
        bool ok_1 = 0, ok_2 = 0;
        bool distrus_player = 0;
        ok_1 |= DreptunghiIntersectDreptunghi(Player::GetInstance()->pozX + 25, Player::GetInstance()->pozY, Player::GetInstance()->pozX + 30, Player::GetInstance()->pozY+15, x.x, x.y+15, x.x+10, x.y+50);
        ok_1 |= DreptunghiIntersectDreptunghi(Player::GetInstance()->pozX + 25, Player::GetInstance()->pozY, Player::GetInstance()->pozX + 30, Player::GetInstance()->pozY+15, x.x+20, x.y+20, x.x+40, x.y+50);
        ok_1 |= DreptunghiIntersectDreptunghi(Player::GetInstance()->pozX + 25, Player::GetInstance()->pozY, Player::GetInstance()->pozX + 30, Player::GetInstance()->pozY+15, x.x+40, x.y+20, x.x+58, x.y+50);

        ok_1 |= DreptunghiIntersectDreptunghi(Player::GetInstance()->pozX + 20, Player::GetInstance()->pozY+20, Player::GetInstance()->pozX + 35, Player::GetInstance()->pozY+40, x.x, x.y+15, x.x+10, x.y+50);
        ok_1 |= DreptunghiIntersectDreptunghi(Player::GetInstance()->pozX + 20, Player::GetInstance()->pozY+20, Player::GetInstance()->pozX + 35, Player::GetInstance()->pozY+40, x.x+20, x.y+20, x.x+40, x.y+50);
        ok_1 |= DreptunghiIntersectDreptunghi(Player::GetInstance()->pozX + 20, Player::GetInstance()->pozY+20, Player::GetInstance()->pozX + 35, Player::GetInstance()->pozY+40, x.x+40, x.y+20, x.x+58, x.y+50);

        ok_1 |= DreptunghiIntersectDreptunghi(Player::GetInstance()->pozX, Player::GetInstance()->pozY + 30, Player::GetInstance()->pozX + 55, Player::GetInstance()->pozY+40, x.x, x.y+15, x.x+10, x.y+50);
        ok_1 |= DreptunghiIntersectDreptunghi(Player::GetInstance()->pozX, Player::GetInstance()->pozY + 30, Player::GetInstance()->pozX + 55, Player::GetInstance()->pozY+40, x.x+20, x.y+20, x.x+40, x.y+50);
        ok_1 |= DreptunghiIntersectDreptunghi(Player::GetInstance()->pozX, Player::GetInstance()->pozY + 30, Player::GetInstance()->pozX + 55, Player::GetInstance()->pozY+40, x.x+40, x.y+20, x.x+58, x.y+50);

        if (game_coop) {
            ok_2 |= DreptunghiIntersectDreptunghi(Player_2::GetInstance()->pozX + 25, Player_2::GetInstance()->pozY, Player_2::GetInstance()->pozX + 30, Player_2::GetInstance()->pozY+15, x.x, x.y+15, x.x+10, x.y+50);
            ok_2 |= DreptunghiIntersectDreptunghi(Player_2::GetInstance()->pozX + 25, Player_2::GetInstance()->pozY, Player_2::GetInstance()->pozX + 30, Player_2::GetInstance()->pozY+15, x.x+20, x.y+20, x.x+40, x.y+50);
            ok_2 |= DreptunghiIntersectDreptunghi(Player_2::GetInstance()->pozX + 25, Player_2::GetInstance()->pozY, Player_2::GetInstance()->pozX + 30, Player_2::GetInstance()->pozY+15, x.x+40, x.y+20, x.x+58, x.y+50);

            ok_2 |= DreptunghiIntersectDreptunghi(Player_2::GetInstance()->pozX + 20, Player_2::GetInstance()->pozY+20, Player_2::GetInstance()->pozX + 35, Player_2::GetInstance()->pozY+40, x.x, x.y+15, x.x+10, x.y+50);
            ok_2 |= DreptunghiIntersectDreptunghi(Player_2::GetInstance()->pozX + 20, Player_2::GetInstance()->pozY+20, Player_2::GetInstance()->pozX + 35, Player_2::GetInstance()->pozY+40, x.x+20, x.y+20, x.x+40, x.y+50);
            ok_2 |= DreptunghiIntersectDreptunghi(Player_2::GetInstance()->pozX + 20, Player_2::GetInstance()->pozY+20, Player_2::GetInstance()->pozX + 35, Player_2::GetInstance()->pozY+40, x.x+40, x.y+20, x.x+58, x.y+50);

            ok_2 |= DreptunghiIntersectDreptunghi(Player_2::GetInstance()->pozX, Player_2::GetInstance()->pozY + 30, Player_2::GetInstance()->pozX + 55, Player_2::GetInstance()->pozY+40, x.x, x.y+15, x.x+10, x.y+50);
            ok_2 |= DreptunghiIntersectDreptunghi(Player_2::GetInstance()->pozX, Player_2::GetInstance()->pozY + 30, Player_2::GetInstance()->pozX + 55, Player_2::GetInstance()->pozY+40, x.x+20, x.y+20, x.x+40, x.y+50);
            ok_2 |= DreptunghiIntersectDreptunghi(Player_2::GetInstance()->pozX, Player_2::GetInstance()->pozY + 30, Player_2::GetInstance()->pozX + 55, Player_2::GetInstance()->pozY+40, x.x+40, x.y+20, x.x+58, x.y+50);
        }

        if (game_solo) {
            if (ok_1) {
                if (Player::GetInstance()->lives >= 1) Player::GetInstance()->lives--;
                else E_3.push_back(aux_3[j]);
            }
            else E_3.push_back(aux_3[j]);
        }
        if (game_coop) {
            if (ok_1 && Player::GetInstance()->lives >= 1) Player::GetInstance()->lives--;
            else if (ok_2 && Player_2::GetInstance()->lives >= 1) Player_2::GetInstance()->lives--;
            else E_3.push_back(aux_3[j]);
        }
    }


    std::vector<std::pair<int, int> > aux_Weak_Bullets;
    for (int j = 0; j < Weak_Bullets.size(); ++ j ) {
        std::pair <int, int> V = Weak_Bullets[j];
        V.second += 1;

        if (V.second >= HEIGHT) continue;

        bool ok_1 = 0, ok_2 = 0;
        ok_1 |= Intersect_PlayerBullet_Enemy(V.first, V.second + 18, Player::GetInstance()->pozX + 25, Player::GetInstance()->pozY, Player::GetInstance()->pozX + 30, Player::GetInstance()->pozY+15);
        ok_1 |= Intersect_PlayerBullet_Enemy(V.first, V.second + 18, Player::GetInstance()->pozX + 20, Player::GetInstance()->pozY+20, Player::GetInstance()->pozX + 35, Player::GetInstance()->pozY+40);
        ok_1 |= Intersect_PlayerBullet_Enemy(V.first, V.second + 18, Player::GetInstance()->pozX, Player::GetInstance()->pozY + 30, Player::GetInstance()->pozX + 55, Player::GetInstance()->pozY+40);

        if (game_coop) {
            ok_2 |= Intersect_PlayerBullet_Enemy(V.first, V.second + 18, Player_2::GetInstance()->pozX + 25, Player_2::GetInstance()->pozY, Player_2::GetInstance()->pozX + 30, Player_2::GetInstance()->pozY+15);
            ok_2 |= Intersect_PlayerBullet_Enemy(V.first, V.second + 18, Player_2::GetInstance()->pozX + 20, Player_2::GetInstance()->pozY+20, Player_2::GetInstance()->pozX + 35, Player_2::GetInstance()->pozY+40);
            ok_2 |= Intersect_PlayerBullet_Enemy(V.first, V.second + 18, Player_2::GetInstance()->pozX, Player_2::GetInstance()->pozY + 30, Player_2::GetInstance()->pozX + 55, Player_2::GetInstance()->pozY+40);
        }

        if (game_solo) {
            if (ok_1) {
                if (Player::GetInstance()->lives >= 1)
                    Player::GetInstance()->lives--;
                else aux_Weak_Bullets.push_back(V);
            }
            else aux_Weak_Bullets.push_back(V);
        }
        else if (game_coop) {
            if (ok_1 && Player::GetInstance()->lives >= 1) Player::GetInstance()->lives--;
            else if (ok_2 && Player_2::GetInstance()->lives >= 1) Player_2::GetInstance()->lives--;
            else aux_Weak_Bullets.push_back(V);
        }
    }

    Weak_Bullets = aux_Weak_Bullets;

    std::vector<std::pair<int, int> > aux_Strong_Bullets;
    for (int j = 0; j < Strong_Bullets.size(); ++ j ) {
        std::pair <int, int> V = Strong_Bullets[j];
        V.second += 2;

        if (V.second >= HEIGHT) continue;

        bool ok_1 = 0, ok_2 = 0;
        ok_1 |= Intersect_PlayerBullet_Enemy(V.first, V.second + 18, Player::GetInstance()->pozX + 25, Player::GetInstance()->pozY, Player::GetInstance()->pozX + 30, Player::GetInstance()->pozY+15);
        ok_1 |= Intersect_PlayerBullet_Enemy(V.first, V.second + 18, Player::GetInstance()->pozX + 20, Player::GetInstance()->pozY+20, Player::GetInstance()->pozX + 35, Player::GetInstance()->pozY+40);
        ok_1 |= Intersect_PlayerBullet_Enemy(V.first, V.second + 18, Player::GetInstance()->pozX, Player::GetInstance()->pozY + 30, Player::GetInstance()->pozX + 55, Player::GetInstance()->pozY+40);

        if (game_coop) {
            ok_2 |= Intersect_PlayerBullet_Enemy(V.first, V.second + 18, Player_2::GetInstance()->pozX + 25, Player_2::GetInstance()->pozY, Player_2::GetInstance()->pozX + 30, Player_2::GetInstance()->pozY+15);
            ok_2 |= Intersect_PlayerBullet_Enemy(V.first, V.second + 18, Player_2::GetInstance()->pozX + 20, Player_2::GetInstance()->pozY+20, Player_2::GetInstance()->pozX + 35, Player_2::GetInstance()->pozY+40);
            ok_2 |= Intersect_PlayerBullet_Enemy(V.first, V.second + 18, Player_2::GetInstance()->pozX, Player_2::GetInstance()->pozY + 30, Player_2::GetInstance()->pozX + 55, Player_2::GetInstance()->pozY+40);
        }

        if (game_solo) {
            if (ok_1) {
                if (Player::GetInstance()->lives >= 1)
                    Player::GetInstance()->lives--;
                else aux_Strong_Bullets.push_back(V);
            }
            else aux_Strong_Bullets.push_back(V);
        }
        else if (game_coop) {
            if (ok_1 && Player::GetInstance()->lives >= 1) Player::GetInstance()->lives--;
            else if (ok_2 && Player_2::GetInstance()->lives >= 1) Player_2::GetInstance()->lives--;
            else aux_Strong_Bullets.push_back(V);
        }
    }

    Strong_Bullets = aux_Strong_Bullets;

    if (FaraInamici()) {
        if (game_solo)
            unlocked_solo[Level+1] = 1;
        else if (game_coop) unlocked_coop[Level+1] = 1;
    }
}

void Ecran::Render()
{
    SDL_SetRenderDrawColor(renderer, 94, 3, 252, 155);
    SDL_RenderClear(renderer);

    if (meniu) {
        Texture::GetInstance()->Draw("meniu", 0, 0, WIDTH, HEIGHT);
        //Texture::GetInstance()->Draw("shooter", 0, 0, 58, 64);
        //Texture::GetInstance()->Draw("laser", 0, 0, 58, 64);
    }
    else if (nivel_menu_solo || nivel_menu_coop) {
        Texture::GetInstance()->Draw("nivele", 0, 0, WIDTH, HEIGHT);

        if (nivel_menu_solo) {
            if (!unlocked_solo[1]) Texture::GetInstance()->Draw("lock", 170, 275, 27, 30);
            if (!unlocked_solo[2]) Texture::GetInstance()->Draw("lock", 170, 330, 27, 30);
            if (!unlocked_solo[3]) Texture::GetInstance()->Draw("lock", 170, 385, 27, 30);
            if (!unlocked_solo[4]) Texture::GetInstance()->Draw("lock", 170, 440, 27, 30);
            if (!unlocked_solo[5]) Texture::GetInstance()->Draw("lock", 170, 495, 27, 30);
        }
        else {
            if (!unlocked_coop[1]) Texture::GetInstance()->Draw("lock", 170, 275, 27, 30);
            if (!unlocked_coop[2]) Texture::GetInstance()->Draw("lock", 170, 330, 27, 30);
            if (!unlocked_coop[3]) Texture::GetInstance()->Draw("lock", 170, 385, 27, 30);
            if (!unlocked_coop[4]) Texture::GetInstance()->Draw("lock", 170, 440, 27, 30);
            if (!unlocked_coop[5]) Texture::GetInstance()->Draw("lock", 170, 495, 27, 30);
        }
    }
    else if (info) {
        Texture::GetInstance()->Draw("controls", 0, 0, WIDTH, HEIGHT);
    }
    else if (game_solo) {
        Texture::GetInstance()->Draw("background_level_1", 0, 0, WIDTH, HEIGHT);
        if (Player::GetInstance()->lives == 0) {
            Texture::GetInstance()->Draw("dead", 172, 338, 156, 24);
            lost = 1;
        }
        else if (FaraInamici()) {
            Texture::GetInstance()->Draw("won", 117, 338, 266, 24);
        }
        else {
            if (Player::GetInstance()->lives == 3) {
                Texture::GetInstance()->Draw("healthbar3", 0, 300, 50, 100);
            }
            else if (Player::GetInstance()->lives == 2) {
                Texture::GetInstance()->Draw("healthbar2", 0, 300, 50, 100);
            }
            else if (Player::GetInstance()->lives == 1) {
                Texture::GetInstance()->Draw("healthbar1", 0, 300, 50, 100);
            }

            if (Player::GetInstance()->lives > 0) {
                Texture::GetInstance()->Draw("player", Player::GetInstance()->pozX, Player::GetInstance()->pozY, 58, 64);
                for (auto it : Bullets) {
                    Texture::GetInstance()->Draw("bullet", it.first, it.second, 58, 18);
                }

                for (auto it : E_1) {
                    Texture::GetInstance()->Draw("asteroid", it.x, it.y, 52, 54);
                }

                for (auto it : E_2) {
                    Texture::GetInstance()->Draw("shooter", it.x, it.y, 58, 64);
                }

                for (auto it : E_3) {
                    Texture::GetInstance()->Draw("laser", it.x, it.y, 58, 64);
                }

                for (auto it : Strong_Bullets) {
                    Texture::GetInstance()->Draw("enemyBullet", it.first, it.second, 58, 18);
                }

                for (auto it : Weak_Bullets) {
                    Texture::GetInstance()->Draw("enemyBullet", it.first, it.second, 58, 18);
                }

                for (auto it : Booms) {
                    Texture::GetInstance()->Draw("boom", it.first.first, it.first.second, 66, 66);
                }
            }
        }
        if (paused) Texture::GetInstance()->Draw("paused", 0, 0, 170, 35);
    }

    else if (game_coop) {
        Texture::GetInstance()->Draw("background_level_1", 0, 0, WIDTH, HEIGHT);

        if (Player_2::GetInstance()->lives <= 0 && Player::GetInstance()->lives <= 0) {
            ///----------------------------------
            Texture::GetInstance()->Draw("dead", 172, 338, 156, 24);
            lost = 1;
        }
        else if (FaraInamici()) {
            Texture::GetInstance()->Draw("won", 117, 338, 266, 24);
        }
        else {
            if (Player::GetInstance()->lives == 3) {
                Texture::GetInstance()->Draw("healthbar3", 0, 300, 50, 100);
            }
            else if (Player::GetInstance()->lives == 2) {
                Texture::GetInstance()->Draw("healthbar2", 0, 300, 50, 100);
            }
            else if (Player::GetInstance()->lives == 1) {
                Texture::GetInstance()->Draw("healthbar1", 0, 300, 50, 100);
            }
            else if (Player::GetInstance()->lives == 0) {
                Texture::GetInstance()->Draw("healthbar0", 0, 300, 50, 100);
            }
            if (Player::GetInstance()->lives >= 1)
                Texture::GetInstance()->Draw("player", Player::GetInstance()->pozX, Player::GetInstance()->pozY, 58, 64);

            if (Player_2::GetInstance()->lives == 3) {
                Texture::GetInstance()->Draw("healthbar3", 449, 300, 50, 100);
            }
            else if (Player_2::GetInstance()->lives == 2) {
                Texture::GetInstance()->Draw("healthbar2", 449, 300, 50, 100);
            }
            else if (Player_2::GetInstance()->lives == 1) {
                Texture::GetInstance()->Draw("healthbar1", 449, 300, 50, 100);
            }
            else if (Player_2::GetInstance()->lives == 0) {
                Texture::GetInstance()->Draw("healthbar0", 449, 300, 50, 100);
            }
            if (Player_2::GetInstance()->lives >= 1)
                Texture::GetInstance()->Draw("player2", Player_2::GetInstance()->pozX, Player_2::GetInstance()->pozY, 58, 64);

            if (Player_2::GetInstance()->lives <= 0 && Player::GetInstance()->lives <= 0) {
                ///----------------------------------
                Texture::GetInstance()->Draw("dead", 172, 338, 156, 24);
            }
            else {
                for (auto it : Bullets) {
                    Texture::GetInstance()->Draw("bullet", it.first, it.second, 58, 18);
                }
                for (auto it : E_1) {
                    Texture::GetInstance()->Draw("asteroid", it.x, it.y, 52, 54);
                }
                for (auto it : E_2) {
                    Texture::GetInstance()->Draw("shooter", it.x, it.y, 58, 64);
                }
                for (auto it : Booms) {
                    Texture::GetInstance()->Draw("boom", it.first.first, it.first.second, 66, 66);
                }
                for (auto it : E_3) {
                    Texture::GetInstance()->Draw("laser", it.x, it.y, 58, 64);
                }
                for (auto it : Strong_Bullets) {
                    Texture::GetInstance()->Draw("enemyBullet", it.first, it.second, 58, 18);
                }
                for (auto it : Weak_Bullets) {
                    Texture::GetInstance()->Draw("enemyBullet", it.first, it.second, 58, 18);
                }
            }
        }

        if (paused) Texture::GetInstance()->Draw("paused", 0, 0, 170, 35);
    }
    SDL_RenderPresent(renderer);
}

void Ecran::Events()
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch(event.type) {
        case SDL_MOUSEBUTTONDOWN:
            if (delay_click == 0) {
            if (meniu == 1) {
                if (200 <= MouseXPos && MouseXPos <= 295 && 265 <= MouseYPos && MouseYPos <= 310) {   /// X:45 Y:45
                    meniu = 0;
                    nivel_menu_solo = 1;
                    delay_click = DELAY_TIME;
                }
                else if (200 <= MouseXPos && MouseXPos <= 295 && 325 <= MouseYPos && MouseYPos <= 370) {
                    meniu = 0;
                    nivel_menu_coop = 1;
                    delay_click = DELAY_TIME;
                }
                else if (28 <= MouseXPos && MouseXPos <= 70 && 628 <= MouseYPos && MouseYPos <= 670) {
                    meniu = 0;
                    info = 1;
                    delay_click = DELAY_TIME;
                }
                else if (200 <= MouseXPos && MouseXPos <= 295 && 385 <= MouseYPos && MouseYPos <= 430) {
                    //std::cout << "GIGEL";
                    Quit();
                }
            }
            else if (nivel_menu_solo == 1) {
                if (200 <= MouseXPos && MouseXPos <= 290) {
                    if (270 <= MouseYPos && MouseYPos <= 305 && unlocked_solo[1]) {
                        Level = 1;
                        nivel_menu_solo = 0;
                        game_solo = 1;
                        lost = 0;
                        Init_Level(Level);
                        delay_click = DELAY_TIME;
                    }
                    else if (325 <= MouseYPos && MouseYPos <= 360 && unlocked_solo[2]) {
                        Level = 2;
                        nivel_menu_solo = 0;
                        game_solo = 1;
                        lost = 0;
                        Init_Level(Level);
                        delay_click = DELAY_TIME;
                    }
                    else if (380 <= MouseYPos && MouseYPos <= 415 && unlocked_solo[3]) {
                        Level = 3;
                        nivel_menu_solo = 0;
                        game_solo = 1;
                        lost = 0;
                        Init_Level(Level);
                        delay_click = DELAY_TIME;
                    }
                    else if (435 <= MouseYPos && MouseYPos <= 470 && unlocked_solo[4]) {
                        Level = 4;
                        nivel_menu_solo = 0;
                        game_solo = 1;
                        lost = 0;
                        Init_Level(Level);
                        delay_click = DELAY_TIME;
                    }
                    else if (490 <= MouseYPos && MouseYPos <= 525 && unlocked_solo[5]) {
                        Level = 5;
                        nivel_menu_solo = 0;
                        game_solo = 1;
                        lost = 0;
                        Init_Level(Level);
                        delay_click = DELAY_TIME;
                    }
                }
            }
            else if (nivel_menu_coop == 1) {
                if (200 <= MouseXPos && MouseXPos <= 290) {
                    if (270 <= MouseYPos && MouseYPos <= 305 && unlocked_coop[1]) {
                        Level = 1;
                        nivel_menu_coop = 0;
                        game_coop = 1;
                        lost = 0;
                        Init_Level(Level);
                        delay_click = DELAY_TIME;
                    }
                    else if (325 <= MouseYPos && MouseYPos <= 360 && unlocked_coop[2]) {
                        Level = 2;
                        nivel_menu_coop = 0;
                        game_coop = 1;
                        lost = 0;
                        Init_Level(Level);
                        delay_click = DELAY_TIME;
                    }
                    else if (380 <= MouseYPos && MouseYPos <= 415 && unlocked_coop[3]) {
                        Level = 3;
                        nivel_menu_coop = 0;
                        game_coop = 1;
                        lost = 0;
                        Init_Level(Level);
                        delay_click = DELAY_TIME;
                    }
                    else if (435 <= MouseYPos && MouseYPos <= 470 && unlocked_coop[4]) {
                        Level = 4;
                        nivel_menu_coop = 0;
                        game_coop = 1;
                        lost = 0;
                        Init_Level(Level);
                        delay_click = DELAY_TIME;
                    }
                    else if (490 <= MouseYPos && MouseYPos <= 525 && unlocked_coop[5]) {
                        Level = 5;
                        nivel_menu_coop = 0;
                        game_coop = 1;
                        lost = 0;
                        Init_Level(Level);
                        delay_click = DELAY_TIME;
                    }
                }
            }
            }
            break;
    }
}
