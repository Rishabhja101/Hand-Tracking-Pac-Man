#include "CameraInput.h"

CameraInput::CameraInput() {
	camera.initGrabber(kWidth, kHeight, true);

	//reserve memory for cv images
	rgb.allocate(kWidth, kHeight);
	hsb.allocate(kWidth, kHeight);
	hue.allocate(kWidth, kHeight);
	sat.allocate(kWidth, kHeight);
	bri.allocate(kWidth, kHeight);
    filtered.allocate(kWidth, kHeight);
}

// Displays the camera frame with a red circle at the point that is currently being tracked
void CameraInput::Draw() {
    ofSetColor(255, 255, 255);

	// display what is seen through the camera
    rgb.draw(kOffset_x, kOffset_y);

    ofSetColor(255, 0, 0);
    ofFill();

    // draw red circles for found blobs
    for (int i = 0; i < contours.nBlobs; i++) {
        ofCircle(contours.blobs[i].centroid.x - kRadius / 2 + kOffset_x, contours.blobs[i].centroid.y - kRadius / 2 + kOffset_y, kRadius);
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
        for (int i = 0; i < kWidth * kHeight; i++) {
            if (ofInRange(hue.getPixels()[i], findHue - kTolerance,
                          findHue + kTolerance)) {
                filtered.getPixels()[i] = 255;
            } else {
                filtered.getPixels()[i] = 0;
            }
        }
        filtered.flagImageChanged();

        // run the contour finder on the filtered image to find blobs with a
        // certain hue
        contours.findContours(filtered, 50, kWidth * kHeight / 2, 1, false);
    }
}

// Changes the color that is being tracked to the color at the pixel where the mouse was clicked
void CameraInput::CalibrateCamera(int x, int y) {
    // calculate local mouse x,y in image
    int mx = (x - kOffset_x) % kWidth;
    int my = (y - kOffset_y) % kHeight;

    // get hue value on mouse position
    findHue = hue.getPixels()[my * kWidth + mx];
}

// Returns the direction that the tracked color indicates
Direction CameraInput::GetDirection() {
    for (int i = 0; i < contours.nBlobs; i++) {
        int x = contours.blobs[i].centroid.x - kOffset_x;
        int y = -contours.blobs[i].centroid.y + kOffset_y;
        int height = camera.getHeight();
       
		// up
        if (y > -x && y > x - height) {
            return Direction::up;
        }
        // right
        else if (y > -x && y < x - height) {
            return Direction::right;
        }
        // down
        else if (y < -x && y < x - height) {
            return Direction::down;
        }
        // left
        else {
            return Direction::left;
        }
    }

	// hand is not in frame
	return Direction::none;
}
