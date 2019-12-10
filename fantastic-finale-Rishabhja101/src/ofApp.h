#pragma once

#include "CameraInput.h"
#include "Ghost.h"

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void draw();

	void NewGame();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

   private:
    const int kBlinkSpeed = 1;
    const int kScaredTime = 10;
    const int kUnscaringTime = 10;
    const int kStartingTime = 5;
    const int kDyingTime = 2;
    const string kGameMusicPath = "pacman_beginning.wav";
    const string kDeathMusicPath = "pacman_death.wav";
    const string kEatMusicPath = "pacman_chomp.wav";

	time_t scared_timer;
    time_t blink_timer;
	time_t sound_delay_time;
    bool blink;
    static Map map;
    static Player player;
    CameraInput input;
    State game_state;
    ofSoundPlayer game_music;
    ofSoundPlayer death_music;
    vector<Ghost> ghosts;

};
