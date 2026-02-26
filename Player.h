#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

constexpr int DELAY_SHOOT = 50;

class Player
{
public :
    static Player* GetInstance(){ return s_Instance = (s_Instance != nullptr)? s_Instance : new Player();}
    void Move(int X, int Y);
    void Shoot();
    void Image();
    void Init();
    void Clean();
    int pozX, pozY;
    int lives;
private :
    Player(){}
    static Player* s_Instance;
};

class Player_2
{
public :
    static Player_2* GetInstance(){ return s_Instance = (s_Instance != nullptr)? s_Instance : new Player_2();}
    void Move(int X, int Y);
    void Shoot();
    void Image();
    void Init();
    void Clean();
    int pozX, pozY;
    int lives;
private :
    Player_2(){}
    static Player_2* s_Instance;
};

extern std::vector <std::pair <int, int> > Bullets;
extern std::vector<std::pair<std::pair<int, int>, int> > Booms;
#endif // PLAYER_H
