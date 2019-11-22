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

    const int kWidth = 640;
    const int kHeight = 480;
    const int kTolerance = 5;
    const int kRadius = 10;
    const int kOffset_x = 100;
    const int kOffset_y = 100;

    int findHue;
};
