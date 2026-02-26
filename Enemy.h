#ifndef ENEMY.H
#define ENEMY.H

#include <vector>
#include <iostream>

constexpr int WALK_DELAY = 3;
constexpr int SHOOT_DELAY_ENEMY = 250;
///Steroid -> se misca drept
///Nava Inamica -> trage
///UFO -> dreapta-stanga
///Superior Nava -> laser


struct Steroid {
    int x, y;
    int W;
};

struct WeakShooter{
    int x, y;
    int W;
    int S;
};

struct StrongShooter {
    int x, y;
    int W;
    int S;
};

extern std::vector <Steroid> E_1;
extern std::vector <WeakShooter> E_2;
extern std::vector <StrongShooter> E_3;
extern std::vector <std::pair<int, int> > Weak_Bullets;
extern std::vector <std::pair<int, int> > Strong_Bullets;
bool Intersect_PlayerBullet_Enemy (int x, int y, int St_x, int St_y, int Dr_x, int Dr_y);
void Clean_Inamici();
void Init_Level(int level);
bool PunctInDreptunghi (int x, int y, int ST_X, int ST_Y, int DR_X, int DR_Y);
bool DreptunghiIntersectDreptunghi (int ST_X, int ST_Y, int DR_X, int DR_Y, int SUS_X, int SUS_Y, int JOS_X, int JOS_Y);
bool FaraInamici();

#endif // ENEMY_H
