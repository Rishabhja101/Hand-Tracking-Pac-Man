#pragma once

#include "ofMain.h"
#include "Direction.h"
#include "Map.h"

class Player {
   public:
    Player();
    void Draw();
    void Update(Map map);
    void ChangeDirection(Direction new_direction);
    bool HasPowerup();

   private:
    const int kSpeed = 10;
    const int kRadius = 20;
    const int kSpawnPositionX = 380;
    const int kSpawnPositionY = 380;
    const int kCoinValue = 10;

	int score;
	int position_x;
    int position_y;
    Direction current_direction;
    Direction next_direction;
    bool possible_directions[4];
    bool powerup;
    
	void Collisions(Map map);
    void Teleport(Map map);
};