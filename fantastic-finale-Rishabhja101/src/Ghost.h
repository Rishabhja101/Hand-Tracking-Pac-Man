#pragma once

#include "Player.h"

class Ghost {
   public:
    Ghost();
    void Draw(State game_state, bool blink);
    void Update(Map map);
    bool PlayerCollision(int position_x, int position_y, State game_state, Map map);
    void ResetPosition();

   private:
    const int kSpeed = 5;
    const int kRadius = 20;
    const int kSpawnPositionX = 680;
    const int kSpawnPositionY = 680;
    const string kMusicPath = "pacman_eatghost.wav";

    int position_x;
    int position_y;
    Direction current_direction;
    bool possible_directions[4];
    bool escape;
    ofSoundPlayer death_music;

    void Collisions(Map map);
    void CalculateNextDirection(Map map);
    void Teleport(Map map);
    void Kill();
};