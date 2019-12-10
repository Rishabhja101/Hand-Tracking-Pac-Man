#pragma once

#include "Player.h"

class Ghost {
   public:
    Ghost();
    void Draw(State game_state, bool blink);
    void Update(Map map, State game_state);
    bool PlayerCollision(int position_x, int position_y, State game_state, Map map);
    void ResetPosition();
    void SetColor(int r, int g, int b);
    void SetRespawnTime(int delay);

   private:
    const int kSpeed = 5;
    const int kRadius = 20;
    const int kSpawnPositionX = 680;
    const int kSpawnPositionY = 680;
    const string kMusicPath = "pacman_eatghost.wav";
    const int kRespawnTime = 5;

    int position_x;
    int position_y;
    Direction current_direction;
    bool possible_directions[4];
    bool escape;
    ofSoundPlayer death_music;
    time_t respawn_time;
    int color_r;
    int color_g;
    int color_b;
    int respawn_delay;

    void Collisions(Map map);
    void CalculateNextDirection(Map map);
    void Teleport(Map map);
    void Kill();
};