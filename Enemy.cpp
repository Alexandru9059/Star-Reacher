#include "Enemy.h"

std::vector<Steroid> E_1;
std::vector<WeakShooter> E_2;
std::vector<StrongShooter> E_3;
std::vector <std::pair<int, int> > Weak_Bullets;
std::vector <std::pair<int, int> > Strong_Bullets;

bool Intersect_PlayerBullet_Enemy (int x, int y, int St_x, int St_y, int Dr_x, int Dr_y) {
    x = x + 25;
    if (St_x <= x && x <= Dr_x && St_y <= y && y <= Dr_y)
        return 1;

    x = x + 10;
    if (St_x <= x && x <= Dr_x && St_y <= y && y <= Dr_y)
        return 1;

    return 0;
}

bool PunctInDreptunghi (int x, int y, int ST_X, int ST_Y, int DR_X, int DR_Y) {
    return ST_X <= x && x <= DR_X && ST_Y <= y && y <= DR_Y;
}

bool DreptunghiIntersectDreptunghi (int ST_X, int ST_Y, int DR_X, int DR_Y, int SUS_X, int SUS_Y, int JOS_X, int JOS_Y) {
    if (PunctInDreptunghi(ST_X, ST_Y, SUS_X, SUS_Y, JOS_X, JOS_Y))
        return 1;
    if (PunctInDreptunghi(ST_X, DR_Y, SUS_X, SUS_Y, JOS_X, JOS_Y))
        return 1;
    if (PunctInDreptunghi(DR_X, ST_Y, SUS_X, SUS_Y, JOS_X, JOS_Y))
        return 1;
    if (PunctInDreptunghi(DR_X, DR_Y, SUS_X, SUS_Y, JOS_X, JOS_Y))
        return 1;
    if (PunctInDreptunghi(SUS_X, SUS_Y, ST_X, ST_Y, DR_X, DR_Y))
        return 1;
    if (PunctInDreptunghi(SUS_X, JOS_Y, ST_X, ST_Y, DR_X, DR_Y))
        return 1;
    if (PunctInDreptunghi(JOS_X, SUS_Y, ST_X, ST_Y, DR_X, DR_Y))
        return 1;
    if (PunctInDreptunghi(JOS_X, JOS_Y, ST_X, ST_Y, DR_X, DR_Y))
        return 1;

    return 0;
}

void Clean_Inamici() {
    E_1.clear();
    E_2.clear();
    E_3.clear();
    Weak_Bullets.clear();
    Strong_Bullets.clear();
}

void Init_Level (int level) {
    if (level == 1) {
        E_1.push_back({75, 30, WALK_DELAY});
        E_1.push_back({150, 30, WALK_DELAY});
        E_1.push_back({225, 30, WALK_DELAY});
        E_1.push_back({300, 30, WALK_DELAY});
        E_1.push_back({375, 30, WALK_DELAY});

        E_1.push_back({75, 100, WALK_DELAY});
        E_1.push_back({150, 100, WALK_DELAY});
        E_1.push_back({225, 100, WALK_DELAY});
        E_1.push_back({300, 100, WALK_DELAY});
        E_1.push_back({375, 100, WALK_DELAY});

        E_1.push_back({75, 170, WALK_DELAY});
        E_1.push_back({150, 170, WALK_DELAY});
        E_1.push_back({225, 170, WALK_DELAY});
        E_1.push_back({300, 170, WALK_DELAY});
        E_1.push_back({375, 170, WALK_DELAY});
    }
    else if (level == 2) {
        E_1.push_back({75, 30, WALK_DELAY});
        E_2.push_back({150, 30, WALK_DELAY, SHOOT_DELAY_ENEMY});
        E_2.push_back({225, 30, WALK_DELAY, SHOOT_DELAY_ENEMY});
        E_1.push_back({300, 30, WALK_DELAY});
        E_2.push_back({375, 30, WALK_DELAY, SHOOT_DELAY_ENEMY});

        E_1.push_back({75, 100, WALK_DELAY});
        E_2.push_back({150, 100, WALK_DELAY, SHOOT_DELAY_ENEMY});
        E_1.push_back({225, 100, WALK_DELAY});
        E_1.push_back({300, 100, WALK_DELAY});
        E_2.push_back({375, 100, WALK_DELAY, SHOOT_DELAY_ENEMY});

        E_2.push_back({75, 170, WALK_DELAY, SHOOT_DELAY_ENEMY});
        E_1.push_back({150, 170, WALK_DELAY});
        E_2.push_back({225, 170, WALK_DELAY, SHOOT_DELAY_ENEMY});
        E_2.push_back({300, 170, WALK_DELAY, SHOOT_DELAY_ENEMY});
        E_1.push_back({375, 170, WALK_DELAY});
    }
    else if (level == 3) {
        E_3.push_back({75, 30, WALK_DELAY, SHOOT_DELAY_ENEMY});
        E_1.push_back({150, 30, WALK_DELAY});
        E_1.push_back({225, 30, WALK_DELAY});
        E_1.push_back({300, 30, WALK_DELAY});
        E_3.push_back({375, 30, WALK_DELAY, SHOOT_DELAY_ENEMY});

        E_1.push_back({75, 100, WALK_DELAY});
        E_1.push_back({150, 100, WALK_DELAY});
        E_3.push_back({225, 100, WALK_DELAY, SHOOT_DELAY_ENEMY});
        E_3.push_back({300, 100, WALK_DELAY, SHOOT_DELAY_ENEMY});
        E_1.push_back({375, 100, WALK_DELAY});

        E_1.push_back({75, 170, WALK_DELAY});
        E_3.push_back({150, 170, WALK_DELAY, SHOOT_DELAY_ENEMY});
        E_1.push_back({225, 170, WALK_DELAY});
        E_1.push_back({300, 170, WALK_DELAY});
        E_3.push_back({375, 170, WALK_DELAY, SHOOT_DELAY_ENEMY});
    }

    else if (level == 4) {
        E_3.push_back({75, 30, WALK_DELAY, SHOOT_DELAY_ENEMY});
        E_1.push_back({150, 30, WALK_DELAY});
        E_2.push_back({225, 30, WALK_DELAY, SHOOT_DELAY_ENEMY});
        E_1.push_back({300, 30, WALK_DELAY});
        E_2.push_back({375, 30, WALK_DELAY, SHOOT_DELAY_ENEMY});

        E_2.push_back({75, 100, WALK_DELAY, SHOOT_DELAY_ENEMY});
        E_1.push_back({150, 100, WALK_DELAY});
        E_3.push_back({225, 100, WALK_DELAY, SHOOT_DELAY_ENEMY});
        E_2.push_back({300, 100, WALK_DELAY, SHOOT_DELAY_ENEMY});
        E_1.push_back({375, 100, WALK_DELAY});

        E_1.push_back({75, 170, WALK_DELAY});
        E_2.push_back({150, 170, WALK_DELAY, SHOOT_DELAY_ENEMY});
        E_1.push_back({225, 170, WALK_DELAY});
        E_2.push_back({300, 170, WALK_DELAY, SHOOT_DELAY_ENEMY});
        E_3.push_back({375, 170, WALK_DELAY, SHOOT_DELAY_ENEMY});
    }

    else if (level == 5) {
        E_3.push_back({75, 30, WALK_DELAY, SHOOT_DELAY_ENEMY});
        E_3.push_back({150, 30, WALK_DELAY, SHOOT_DELAY_ENEMY});
        E_3.push_back({225, 30, WALK_DELAY, SHOOT_DELAY_ENEMY});
        E_3.push_back({300, 30, WALK_DELAY, SHOOT_DELAY_ENEMY});
        E_3.push_back({375, 30, WALK_DELAY, SHOOT_DELAY_ENEMY});

        E_3.push_back({75, 100, WALK_DELAY, SHOOT_DELAY_ENEMY});
        E_3.push_back({150, 100, WALK_DELAY, SHOOT_DELAY_ENEMY});
        E_2.push_back({225, 100, WALK_DELAY, SHOOT_DELAY_ENEMY});
        E_2.push_back({300, 100, WALK_DELAY, SHOOT_DELAY_ENEMY});
        E_3.push_back({375, 100, WALK_DELAY, SHOOT_DELAY_ENEMY});

        E_2.push_back({75, 170, WALK_DELAY, SHOOT_DELAY_ENEMY});
        E_2.push_back({150, 170, WALK_DELAY, SHOOT_DELAY_ENEMY});
        E_2.push_back({225, 170, WALK_DELAY, SHOOT_DELAY_ENEMY});
        E_2.push_back({300, 170, WALK_DELAY, SHOOT_DELAY_ENEMY});
        E_2.push_back({375, 170, WALK_DELAY, SHOOT_DELAY_ENEMY});
    }
}

bool FaraInamici() {
    return E_1.empty() && E_2.empty() && E_3.empty();
}
