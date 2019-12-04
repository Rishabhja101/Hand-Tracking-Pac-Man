#include "ofApp.h"

Player ofApp::player;
Map ofApp::map;

//--------------------------------------------------------------
void ofApp::setup() { 
	blink_timer = chrono::system_clock::to_time_t(chrono::system_clock::now());
    blink = false;
    game_state = State::starting;
    game_music.load(kGameMusicPath);
    death_music.load(kDeathMusicPath);
    game_music.play();

    ofSoundPlayer eat_music;
    eat_music.load(kEatMusicPath);
    player.LoadMusic(eat_music);
    
	sound_delay_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
}

//--------------------------------------------------------------
void ofApp::update() {
	if (game_state == State::starting && sound_delay_time == chrono::system_clock::to_time_t(chrono::system_clock::now()) - kStartingTime) {
        game_state = State::regular;
	} 
	else if (game_state == State::death && sound_delay_time == chrono::system_clock::to_time_t(chrono::system_clock::now()) - kDyingTime) {
		game_state = State::starting;
        game_music.play();
        ghost.ResetPosition();
        player.Kill();
		sound_delay_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
	}

	// flip the bool for blinking if the blink time has elapsed
	if (blink_timer == chrono::system_clock::to_time_t(chrono::system_clock::now()) - kBlinkSpeed) {
        blink_timer = chrono::system_clock::to_time_t(chrono::system_clock::now());
        blink = !blink;
	}
    
	// check if it is time to unscare
	if (game_state == State::scared && scared_timer == chrono::system_clock::to_time_t(chrono::system_clock::now()) - kScaredTime) {
		game_state = State::unscaring;
		scared_timer = chrono::system_clock::to_time_t(chrono::system_clock::now());
	}
    else if (game_state == State::unscaring && scared_timer == chrono::system_clock::to_time_t(chrono::system_clock::now()) - kUnscaringTime) {
        game_state = State::regular;
    }
	

	input.Update();
	player.Update(map, game_state);
    ghost.Update(map, game_state);

	if (player.HasPowerup()){
        game_state = State::scared;
        scared_timer = chrono::system_clock::to_time_t(chrono::system_clock::now());
	}

	if (ghost.PlayerCollision(player.GetPositionX(), player.GetPositionY(), game_state, map)) {
        game_state = State::death;
        death_music.play();
		sound_delay_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	input.Draw();
    map.Draw(blink);
    ghost.Draw(game_state, blink);
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
