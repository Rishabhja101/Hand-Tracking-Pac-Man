#pragma once

#include "CameraInput.h"
#include "Ghost.h"

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void draw();

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

	time_t scared_timer;
    time_t blink_timer;
    bool blink;
    static Map map;
    static Player player;
    CameraInput input;
    Ghost ghost;
    State game_state;
};
