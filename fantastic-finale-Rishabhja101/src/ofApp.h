#pragma once

#include "CameraInput.h"
#include "Ghost.h"

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void mousePressed(int x, int y, int button);
    void NewGame();

   private:
    const int kBlinkSpeed = 1;
    const int kScaredTime = 10;
    const int kUnscaringTime = 5;
    const int kStartingTime = 5;
    const int kDyingTime = 2;
    const string kGameMusicPath = "pacman_beginning.wav";
    const string kDeathMusicPath = "pacman_death.wav";
    const string kEatMusicPath = "pacman_chomp.wav";
    const string kScaredMusicPath = "Pacman_Siren.wav";
    const string kBackgroundMusicPath = "Pacman_Waka_Waka.wav";
    const int kScoreLength = 5;
    const string kFontPath = "Gravedigger-8BOZ.ttf";
    const string kTitleFontPath = "FridayNightLights-l5pe.ttf";
    const string kSecondFontPath = "HeadlinerNo45-59y8.ttf";
    const string kHighScorePath = "high_score.txt";
    const bool kCameraInput = true;

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
    ofSoundPlayer scared_music;
    ofSoundPlayer background_music;
    vector<Ghost> ghosts;
    ofTrueTypeFont main_font;
    ofTrueTypeFont title_font;
    ofTrueTypeFont secondary_font;
    int high_score;
};
