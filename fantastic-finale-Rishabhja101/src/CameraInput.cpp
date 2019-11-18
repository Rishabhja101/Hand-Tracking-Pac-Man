#include "CameraInput.h"

CameraInput::CameraInput() {
	camera.initGrabber(width, height, true);

	//reserve memory for cv images
	rgb.allocate(width, height);
	hsb.allocate(width, height);
	hue.allocate(width, height);
	sat.allocate(width, height);
	bri.allocate(width, height);
	filtered.allocate(width , height);
}

// Displays the camera frame with a red circle at the point that is currently being tracked
void CameraInput::Draw() {
    ofSetColor(255, 255, 255);
    
	// display what is seen through the camera
	rgb.draw(0, 0);

    ofSetColor(255, 0, 0);
    ofFill();

    // draw red circles for found blobs
    for (int i = 0; i < contours.nBlobs; i++) {
        ofCircle(contours.blobs[i].centroid.x - radius / 2,
                 contours.blobs[i].centroid.y - radius / 2, radius);
    }
}

void CameraInput::Update() {
    camera.update();

    if (camera.isFrameNew()) {
        // copy webcam pixels to rgb image
        rgb.setFromPixels(camera.getPixels());

        // mirror horizontal
        rgb.mirror(false, true);

        // duplicate rgb
        hsb = rgb;

        // convert to hsb
        hsb.convertRgbToHsv();

        // store the three channels as grayscale images
        hsb.convertToGrayscalePlanarImages(hue, sat, bri);

        // filter image based on the hue value were looking for
        for (int i = 0; i < width * height; i++) {
            if (ofInRange(hue.getPixels()[i], findHue - tolerance,
                          findHue + tolerance)) {
                filtered.getPixels()[i] = 255;
            } else {
                filtered.getPixels()[i] = 0;
            }
        }
        filtered.flagImageChanged();

        // run the contour finder on the filtered image to find blobs with a
        // certain hue
        contours.findContours(filtered, 50, width * height / 2, 1, false);
    }
}

// Changes the color that is being tracked to the color at the pixel where the mouse was clicked
void CameraInput::CalibrateCamera(int x, int y) {
    // calculate local mouse x,y in image
    int mx = x % width;
    int my = y % height;

    // get hue value on mouse position
    findHue = hue.getPixels()[my * width + mx];
}

// Returns the direction that the tracked color indicates
string CameraInput::GetDirection() {
    for (int i = 0; i < contours.nBlobs; i++) {
        int x = contours.blobs[i].centroid.x;
        int y = -contours.blobs[i].centroid.y;
        int height = camera.getHeight();
       
		// up
        if (y > -x && y > x - height) {
            return "up";
        }
        // right
        else if (y > -x && y < x - height) {
            return "right";
        }
        // down
        else if (y < -x && y < x - height) {
            return "down";
        }
        // left
        else {
            return "left";
        }
    }

	// hand is not in frame
	return "";
}
