#pragma once

#include "Direction.h"
#include "Map.h"
#include "ofMain.h"

class Player {
   public:
    Player();
    void Draw();
    void Update(Map map, State game_state);
    void ChangeDirection(Direction new_direction);
    bool HasPowerup();
    void Kill();
    int GetPositionX();
    int GetPositionY();
    void LoadMusic(ofSoundPlayer eat_music);

   private:
    const int kSpeed = 10;
    const int kRadius = 20;
    const int kSpawnPositionX = 680;
    const int kSpawnPositionY = 1040;
    const int kCoinValue = 10;
    const int kStartingLives = 3;
   
    int score;
    int lives;
    int position_x;
    int position_y;
    Direction current_direction;
    Direction next_direction;
    bool possible_directions[4];
    bool powerup;
    ofSoundPlayer eat_music;

    void Collisions(Map map);
    void Teleport(Map map);
    void ResetPosition();
};