#pragma once

#include "Player.h"

class Ghost {
   public:
    Ghost();
    void Draw(State game_state, bool blink);
    void Update(Map map);

   private:
    const int kSpeed = 5;
    const int kRadius = 20;
    const int kSpawnPositionX = 380;
    const int kSpawnPositionY = 200;

    int position_x;
    int position_y;
    Direction current_direction;
    bool possible_directions[4];

    void Collisions(Map map);
    void CalculateNextDirection();
    void Teleport(Map map);
};