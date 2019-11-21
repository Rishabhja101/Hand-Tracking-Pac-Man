#include "Map.h"

Map::Map() { 
	LoadMap(kFilePath); 
}

// Display the map along with all of its coins and powerups
void Map::Draw() {
    ofSetColor(0, 0, 0);
    ofDrawRectangle(kOffsetX, kOffsetY, map[0].size() * kScale, map.size() * kScale);
    
	ofSetColor(0, 0, 255);

    for (int i = 0; i < map.size(); i++) {
        for (int n = 0; n < map[i].length(); n++) {
            if (map[i].at(n) == 'x') {
                ofSetColor(0, 0, 255);
                ofDrawRectangle(n * kScale + kOffsetX, i * kScale + kOffsetY, kScale, kScale);
            } else if (map[i].at(n) == 'O') {
				ofSetColor(255, 255, 255);
                ofDrawCircle(n * kScale + kOffsetX + kScale / 2, i * kScale + kOffsetY + kScale / 2, kPowerupRadius);
            } else if (map[i].at(n) == 'o') {
                ofSetColor(255, 255, 255);
                ofDrawCircle(n * kScale + kOffsetX + kScale / 2, i * kScale + kOffsetY + kScale / 2, kCoinRadius);
            } else if (map[i].at(n) == 'T') {
                ofSetColor(0, 0, 0);
                ofDrawRectangle(n * kScale + kOffsetX, i * kScale + kOffsetY, kScale, kScale);
            }
        }
    }
}

// load a map from the given text file
void Map::LoadMap(string file_path) { 
	ifstream reader(file_path);
    while (!reader.eof()) {
        string line;
        getline(reader, line);
        map.push_back(line);
	}
}