#include "Direction.h"
#include "ofCamera.h"
#include "ofxOpenCv.h"

// code for color tracking from:
// https://sites.google.com/site/ofauckland/examples/10-testing
class CameraInput {
   public:
    CameraInput();
    void Draw();
    void Update();
    void CalibrateCamera(int x, int y);
    Direction GetDirection();

   private:
    ofVideoGrabber camera;
    ofxCvColorImage rgb;
    ofxCvColorImage hsb;
    ofxCvGrayscaleImage hue;
    ofxCvGrayscaleImage sat;
    ofxCvGrayscaleImage bri;
    ofxCvGrayscaleImage filtered;
    ofxCvContourFinder contours;

    const int kWidth = 320;
    const int kHeight = 240;
    const int kTolerance = 5;
    const int kRadius = 10;
    const int kOffset_x = 1340;
    const int kOffset_y = 1125;

    int findHue;
};
