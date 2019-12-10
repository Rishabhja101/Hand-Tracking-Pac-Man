#include "ofApp.h"

Player ofApp::player;
Map ofApp::map;

//--------------------------------------------------------------
void ofApp::setup() {
    ofBackground(0, 0, 0);

    for (int i = 0; i < 4; i++) {
        Ghost g;
        ghosts.push_back(g);
        ghosts[i].SetRespawnTime(kStartingTime + i + 1);
    }

    ghosts[0].SetColor(255, 0, 0);      // red
    ghosts[1].SetColor(255, 128, 0);    // pink
    ghosts[2].SetColor(225, 102, 225);  // pink
    ghosts[3].SetColor(0, 255, 255);    // blue

    ofSoundPlayer eat_music;
    eat_music.load(kEatMusicPath);
    player.LoadMusic(eat_music);

	game_state = State::start;

    cout << "start game" << endl;
}

//--------------------------------------------------------------
void ofApp::update() {
    if (game_state == State::start || game_state == State::ended) {
        return;
    }

    if (game_state == State::starting && sound_delay_time == chrono::system_clock::to_time_t(chrono::system_clock::now()) - kStartingTime) {
        game_state = State::regular;
    } else if (game_state == State::death && sound_delay_time == chrono::system_clock::to_time_t(chrono::system_clock::now()) - kDyingTime) {
        game_state = State::starting;
        game_music.play();

        for (int i = 0; i < ghosts.size(); i++) {
            ghosts[i].ResetPosition();
            ghosts[i].SetRespawnTime(kStartingTime + i * 2 + 1);
        }

        player.Kill();
        sound_delay_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
    }

    // get the new direction for the player
    player.ChangeDirection(input.GetDirection());

    // flip the bool for blinking if the blink time has elapsed
    if (blink_timer ==
        chrono::system_clock::to_time_t(chrono::system_clock::now()) -
            kBlinkSpeed) {
        blink_timer =
            chrono::system_clock::to_time_t(chrono::system_clock::now());
        blink = !blink;
    }

    // check if it is time to unscare
    if (game_state == State::scared &&
        scared_timer ==
            chrono::system_clock::to_time_t(chrono::system_clock::now()) -
                kScaredTime) {
        game_state = State::unscaring;
        scared_timer =
            chrono::system_clock::to_time_t(chrono::system_clock::now());
    } else if (game_state == State::unscaring &&
               scared_timer == chrono::system_clock::to_time_t(
                                   chrono::system_clock::now()) -
                                   kUnscaringTime) {
        game_state = State::regular;
    }

    input.Update();
    player.Update(map, game_state);

    for (int i = 0; i < ghosts.size(); i++) {
        ghosts[i].Update(map, game_state);
    }

    if (player.HasPowerup()) {
        game_state = State::scared;
        scared_timer =
            chrono::system_clock::to_time_t(chrono::system_clock::now());
    }

    for (int i = 0; i < ghosts.size(); i++) {
        if (ghosts[i].PlayerCollision(player.GetPositionX(),
                                      player.GetPositionY(), game_state, map)) {
            game_state = State::death;
            death_music.play();
            sound_delay_time =
                chrono::system_clock::to_time_t(chrono::system_clock::now());
        }
    }

    if (player.IsDead()) {
        game_state = State::ended;
        cout << "game over" << endl;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
    if (game_state == State::start || game_state == State::ended) {
        return;
	}

    input.Draw();
    map.Draw(blink);

    for (int i = 0; i < ghosts.size(); i++) {
        ghosts[i].Draw(game_state, blink);
    }

    player.Draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == OF_KEY_RIGHT) {
        player.ChangeDirection(Direction::right);
    } else if (key == OF_KEY_LEFT) {
        player.ChangeDirection(Direction::left);
    } else if (key == OF_KEY_UP) {
        player.ChangeDirection(Direction::up);
    } else if (key == OF_KEY_DOWN) {
        player.ChangeDirection(Direction::down);
    } else if (game_state == State::start) {
        NewGame();
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    input.CalibrateCamera(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {}

void ofApp::NewGame() {
    blink_timer = chrono::system_clock::to_time_t(chrono::system_clock::now());
    sound_delay_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
    game_music.load(kGameMusicPath);
    death_music.load(kDeathMusicPath);
    game_music.play();
    game_state = State::starting;
    blink = false;
}