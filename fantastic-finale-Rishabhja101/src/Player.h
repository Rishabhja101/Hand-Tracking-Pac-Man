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
    bool IsDead();

   private:
    const int kSpeed = 10;
    const int kRadius = 20;
    const int kSpawnPositionX = 680;
    const int kSpawnPositionY = 1040;
    const int kCoinValue = 10;
    const int kStartingLives = 3;
    const double kMaxMouthDegree = 0.8;
    const double kMouthSpeed = 0.1;
   
    int score;
    int lives;
    int position_x;
    int position_y;
    double mouth_angle;
    double mouth_direction;
    Direction current_direction;
    Direction next_direction;
    Direction last_direction;
    bool possible_directions[4];
    bool powerup;
    ofSoundPlayer eat_music;

    void Collisions(Map map);
    void Teleport(Map map);
    void ResetPosition();
};