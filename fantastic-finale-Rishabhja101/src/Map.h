#include "fstream"
#include "ofMain.h"

class Map {
   public:
    Map();
    void Draw();

   private:
    vector<string> map;
    const int kScale = 60;
    const int kOffsetX = 50;
    const int kOffsetY = 50;
    const int kPowerupRadius = 10;
    const int kCoinRadius = 5;
    const string kFilePath = "C:\\Users\\risha\\OneDrive\\Documents\\GitHub\\fantastic-finale-Rishabhja101\\fantastic-finale-Rishabhja101\\src\\map_data.txt";

    void LoadMap(string file_path);
};