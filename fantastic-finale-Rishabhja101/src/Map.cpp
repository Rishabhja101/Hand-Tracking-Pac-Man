#include "Map.h"

vector<string> Map::map;

Map::Map() { 
	LoadMap(); 
}

// Display the map along with all of its coins and powerups
void Map::Draw(bool blink) {
    ofSetColor(0, 0, 255);

	// iterate through each position on the map and display whatever is in that spot
    for (int i = 0; i < map.size(); i++) {
        for (int n = 0; n < map[i].length(); n++) {
            // set the tile to be black
			ofSetColor(0, 0, 0);
            ofDrawRectangle(n * kScale + kOffsetX, i * kScale + kOffsetY, kScale, kScale);
            
			if (map[i][n] == kWall) {
				// if the tile has a wall, make it a blue square
                ofSetColor(0, 0, 255);
                ofDrawRectangle(n * kScale + kOffsetX, i * kScale + kOffsetY, kScale, kScale);
            } else if (map[i][n] == kPowerup && !blink) {
				// if the tile has a powerup, display the powerup
                ofSetColor(255, 255, 255);
                ofDrawCircle(n * kScale + kOffsetX + kScale / 2, i * kScale + kOffsetY + kScale / 2, kPowerupRadius);
            } else if (map[i][n] == kCoin) {
				// if the tile has a coin, display the coin
                ofSetColor(255, 255, 255);
                ofDrawCircle(n * kScale + kOffsetX + kScale / 2, i * kScale + kOffsetY + kScale / 2, kCoinRadius);
            }
        }
    }
}

// Load a map from the given text file
void Map::LoadMap() {
    map.clear();

    ifstream reader(kFilePath);
    while (!reader.eof()) {
        string line;
        getline(reader, line);
        map.push_back(line);
    }
    reader.close();

    height = map.size();
    if (height > 0) {
        width = map[0].size();
    } else {
        width = 0;
    }
}

// Returns the character in the map at the given position
char Map::GetAtPosition(int x, int y) { 
	return map[y][x]; 
}

// Returns the width of the map
int Map::GetWidth() { 
	return width; 
}

// Returns the height of the map
int Map::GetHeight() { 
	return height; 
}

// If the given item is on the given coordinate, remove the item and return true,
// otherwise return false
bool Map::GetItem(int x, int y, char item) {
    if (map[y][x] == item) {
        map[y][x] = kEmpty;
        return true;
    }
    return false;
}

// If there is a coin on the given coordinate, remove the coin and return true,
// otherwise return false
bool Map::CollectCoin(int x, int y) {
    return GetItem(x, y, kCoin);
}

// If there is a powerup on the given coordinate, remove the powerup and return
// true, otherwise return false
bool Map::GetPowerup(int x, int y) {
    return GetItem(x, y, kPowerup);
}

// Returns true if the map is empty (has no more coins or powerups), otherwise returns false
bool Map::IsEmpty() { 
	 for (int i = 0; i < map.size(); i++) {
        for (int n = 0; n < map[i].length(); n++) {
            if (map[i][n] == kCoin || map[i][n] == kPowerup) {
                return false;
			}
		}
	}
    return true;
}