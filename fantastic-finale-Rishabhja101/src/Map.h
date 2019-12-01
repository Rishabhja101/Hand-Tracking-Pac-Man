#pragma once

#include "fstream"
#include "ofMain.h"

class Map {
   public:
    const int kScale = 60;
    const int kOffsetX = 50;
    const int kOffsetY = 50;
    const char kWall = 'x';
    const char kPowerup = 'O';
    const char kCoin = 'o';
    const char kTeleport = 'T';

    Map();
    void Draw();
    char GetAtPosition(int x, int y);
    int GetWidth();
    int GetHeight();

   private:
    vector<string> map;
    const int kPowerupRadius = 10;
    const int kCoinRadius = 5;
    const string kFilePath =
        "C:\\Users\\risha\\OneDrive\\Documents\\GitHub\\fantastic-finale-"
        "Rishabhja101\\fantastic-finale-Rishabhja101\\src\\map_data.txt";

    int width;
    int height;

    void LoadMap(string file_path);
};