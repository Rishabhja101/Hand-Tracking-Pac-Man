#include "Map.h"

vector<string> Map::map;

Map::Map() { 
	LoadMap(kFilePath); 
}

// Display the map along with all of its coins and powerups
void Map::Draw(bool blink) {
    ofSetColor(0, 0, 255);

    for (int i = 0; i < map.size(); i++) {
        for (int n = 0; n < map[i].length(); n++) {
            ofSetColor(0, 0, 0);

            ofDrawRectangle(n * kScale + kOffsetX, i * kScale + kOffsetY,
                            kScale, kScale);
            if (map[i][n] == kWall) {
                ofSetColor(0, 0, 255);
                ofDrawRectangle(n * kScale + kOffsetX, i * kScale + kOffsetY,
                                kScale, kScale);
            } else if (map[i][n] == kPowerup && !blink) {
                ofSetColor(255, 255, 255);
                ofDrawCircle(n * kScale + kOffsetX + kScale / 2,
                             i * kScale + kOffsetY + kScale / 2,
                             kPowerupRadius);
            } else if (map[i][n] == kCoin) {
                ofSetColor(255, 255, 255);
                ofDrawCircle(n * kScale + kOffsetX + kScale / 2,
                             i * kScale + kOffsetY + kScale / 2, kCoinRadius);
            } else if (map[i][n] == kTeleport) {
                ofSetColor(0, 0, 0);
                ofDrawRectangle(n * kScale + kOffsetX, i * kScale + kOffsetY,
                                kScale, kScale);
            }
        }
    }
}

// Load a map from the given text file
void Map::LoadMap(string file_path) {
    ifstream reader(file_path);
    while (!reader.eof()) {
        string line;
        getline(reader, line);
        map.push_back(line);
    }

    height = map.size();
    if (height > 0) {
        width = map[0].size();
    } else {
        width = 0;
    }
}

// Returns the character in the map at the given position
char Map::GetAtPosition(int x, int y) { return map[y][x]; }

// Returns the width of the map
int Map::GetWidth() { return width; }

// Returns the height of the map
int Map::GetHeight() { return height; }

// If there is a coin on the given coordinate, remove the coin and return true,
// otherwise return false
bool Map::CollectCoin(int x, int y) {
    if (map[y][x] == kCoin) {
        map[y][x] = kEmpty;
        return true;
    }
    return false;
}