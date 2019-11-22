#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() { 
	
}

//--------------------------------------------------------------
void ofApp::update() { 
	input.Update(); 
	player.Update(map);
}

//--------------------------------------------------------------
void ofApp::draw() { 
	input.Draw();
    map.Draw();
    player.Draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == OF_KEY_RIGHT) {
        player.ChangeDirection(Direction::right);
    } else if (key == OF_KEY_LEFT) {
        player.ChangeDirection(Direction::left);
    } else if (key == OF_KEY_UP) {
        player.ChangeDirection(Direction::up);
    } else if (key == OF_KEY_DOWN) {
        player.ChangeDirection(Direction::down);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) { 
	input.CalibrateCamera(x, y); 
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
