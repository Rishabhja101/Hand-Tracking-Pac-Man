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
    string GetDirection();

   private:
    ofVideoGrabber camera;
    ofxCvColorImage rgb;
    ofxCvColorImage hsb;
    ofxCvGrayscaleImage hue;
    ofxCvGrayscaleImage sat;
    ofxCvGrayscaleImage bri;
    ofxCvGrayscaleImage filtered;
    ofxCvContourFinder contours;

    const int width = 640;
    const int height = 480;
    const int tolerance = 5;
    const int radius = 10;

    int findHue;
};
