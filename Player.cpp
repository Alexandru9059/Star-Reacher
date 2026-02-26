#include "Player.h"
#include "Ecran.h"
#include "Texture.h"

Player* Player::s_Instance = nullptr;
Player_2* Player_2::s_Instance = nullptr;
std::vector <std::pair <int, int> > Bullets;
std::vector <std::pair <std::pair<int, int>, int> > Booms;

void Player::Init () {
    pozX = 200;
    pozY = 500;
    lives = 3;
}

void Player::Shoot() {
    Bullets.push_back({pozX, pozY-1});
}

void Player::Move(int X, int Y)
{
    int newX = pozX + X;
    int newY = pozY + Y;

    if (0 <= newX && newX <= WIDTH - 58  && 0 <= newY && newY <= HEIGHT - 64) {
        pozX = newX;
        pozY = newY;
    }
}

void Player::Image() {
    Texture::GetInstance()->Draw("player", pozX, pozY, 300, 400);
}

void Player::Clean() {
    Bullets.clear();
    Booms.clear();
    pozX = 200;
    pozY = 500;
    lives = 3;
}


void Player_2::Init () {
    pozX = 300;
    pozY = 500;
    lives = 3;
}

void Player_2::Shoot() {
    Bullets.push_back({pozX, pozY-1});
}

void Player_2::Move(int X, int Y)
{
    int newX = pozX + X;
    int newY = pozY + Y;

    if (0 <= newX && newX <= WIDTH - 58  && 0 <= newY && newY <= HEIGHT - 64) {
        pozX = newX;
        pozY = newY;
    }
}

void Player_2::Image() {
    Texture::GetInstance()->Draw("player", pozX, pozY, 300, 400);
}

void Player_2::Clean() {
    Bullets.clear();
    Booms.clear();
    pozX = 300;
    lives = 3;
    pozY = 500;
}
