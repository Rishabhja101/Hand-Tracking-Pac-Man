#include "ofApp.h"
#include "fstream"
#include "iostream"

Player ofApp::player;
Map ofApp::map;

//--------------------------------------------------------------
void ofApp::setup() {
    ofBackground(0, 0, 0);

	// add 4 ghosts to the game
    for (int i = 0; i < 4; i++) {
        Ghost g;
        ghosts.push_back(g);
        ghosts[i].SetRespawnTime(kStartingTime + i + 1);
    }

    ghosts[0].SetColor(255, 0, 0);      // red
    ghosts[1].SetColor(255, 128, 0);    // pink
    ghosts[2].SetColor(225, 102, 225);  // pink
    ghosts[3].SetColor(0, 255, 255);    // blue

	// load the sound effects for the game
    ofSoundPlayer eat_music;
    eat_music.load(kEatMusicPath);
    game_music.load(kGameMusicPath);
    death_music.load(kDeathMusicPath);
    scared_music.load(kScaredMusicPath);
    background_music.load(kBackgroundMusicPath);
    player.LoadMusic(eat_music);
    scared_music.setLoop(true);
    background_music.setLoop(true);
    background_music.setVolume(0.5);

	// load the fonts for the game
    main_font.load(kFontPath, 20);
    title_font.load(kTitleFontPath, 150);
    secondary_font.load(kSecondFontPath, 75);

	game_state = State::start;

	// load the high score from all previous games
    ifstream reader(kHighScorePath);
    string line;
    getline(reader, line);
    high_score = stoi(line);
    reader.close();
}

//--------------------------------------------------------------
void ofApp::update() {
	// update the camera input
    input.Update();

	// after the starting time has passed, set the game state to regular
    if (game_state == State::starting && sound_delay_time == 
			chrono::system_clock::to_time_t(chrono::system_clock::now()) - kStartingTime) {
        game_state = State::regular;
        background_music.play();
    } 
	// after the dying time has passed, set the game state to starting
	else if (game_state == State::death && sound_delay_time == 
			chrono::system_clock::to_time_t(chrono::system_clock::now()) - kDyingTime) {
        game_state = State::starting;
        game_music.play();

		// respawn each ghost
        for (int i = 0; i < ghosts.size(); i++) {
            ghosts[i].ResetPosition();
            ghosts[i].SetRespawnTime(kStartingTime + i * 2 + 1);
        }

        player.Kill();
        sound_delay_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
    }
	// if the game state is start, ended, or ended, don't update anyting
	else if (game_state == State::start || game_state == State::ended || 
		game_state == State::death) {
        return;
	}

    if (kCameraInput) {
		// get the new direction for the player
		player.ChangeDirection(input.GetDirection());
    }

    // flip the bool for blinking if the blink time has elapsed
    if (blink_timer <= chrono::system_clock::to_time_t(chrono::system_clock::now()) - 
			kBlinkSpeed) {
        blink_timer = chrono::system_clock::to_time_t(chrono::system_clock::now());
        blink = !blink;
    }

    // check if it is time to unscare
    if (game_state == State::scared &&
        scared_timer == chrono::system_clock::to_time_t(chrono::system_clock::now()) - 
			kScaredTime) {
        game_state = State::unscaring;
        scared_timer = chrono::system_clock::to_time_t(chrono::system_clock::now());
    } 
	else if (game_state == State::unscaring && scared_timer == 
		chrono::system_clock::to_time_t(chrono::system_clock::now()) - kUnscaringTime) {
        game_state = State::regular;
        scared_music.stop();
        background_music.play();
    }

	// update player and each ghost
    player.Update(map, game_state);
    for (int i = 0; i < ghosts.size(); i++) {
        ghosts[i].Update(map, game_state);
    }

	// if the player has the powerup, set the game state to scared
    if (player.HasPowerup()) {
        game_state = State::scared;
        scared_timer = chrono::system_clock::to_time_t(chrono::system_clock::now());
        background_music.stop();
		scared_music.play();
    }

	// if the player's score is higher than the old high score, set it as the
    // new high score
    if (player.GetScore() > high_score) {
        high_score = player.GetScore();
    }

	// for each ghost, check if the ghost collides with the player when the ghost is not scared
    for (int i = 0; i < ghosts.size(); i++) {
        if (ghosts[i].PlayerCollision(player.GetPositionX(), player.GetPositionY(), 
				game_state, map)) {
            if (game_state != State::scared && game_state != State::unscaring) {
				// if the player has no lives left, set the game state to ended
				if (player.IsDead()) {
					game_state = State::ended;

					// save the high score
					ofstream output(kHighScorePath);
					output << high_score << endl;
					output.close();
				} else {
					// the player has lives left so respawn the player and ghosts
					sound_delay_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
					game_state = State::death;
				}
                background_music.stop();
				death_music.play();
            } 
			else {
                player.EatGhost();
			}
        }
    }

	// if the map is empty then start a new game
    if (map.IsEmpty()) {
        NewGame();
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
	// draw the camera input, map, and player
    input.Draw();
    map.Draw(blink);
    player.Draw();

	// draw each of the ghosts
    for (int i = 0; i < ghosts.size(); i++) {
        ghosts[i].Draw(game_state, blink);
    }

	// format the high score and score as strings
    string score_high = to_string(high_score);
    string score = to_string(player.GetScore());

    string temp;
    for (int i = 0; i < 5 - score.length(); i++) {
        temp += "0";
    }
    temp += score;
    score = temp;

    temp = "";
    for (int i = 0; i < 5 - score_high.length(); i++) {
        temp += "0";
    }
    temp += score_high;
    score_high = temp;

	// display the high score, score, and lives
    ofSetColor(255, 255, 255);
    main_font.drawString("High Score", map.kOffsetX + map.kScale * 3.5, map.kOffsetY);
    main_font.drawString("Score", map.kOffsetX + map.kScale * 9.5, map.kOffsetY);
    main_font.drawString("Lives", map.kOffsetX + map.kScale * 15, map.kOffsetY);
    main_font.drawString(score_high, map.kOffsetX + map.kScale * 4.5, map.kOffsetY + 
		map.kScale * 0.5);
    main_font.drawString(score, map.kOffsetX + map.kScale * 9.5, map.kOffsetY + map.kScale * 0.5);
    main_font.drawString(to_string(player.GetLives()), map.kOffsetX + map.kScale * 15.75, 
		map.kOffsetY + map.kScale * 0.5);

	// if the game state is start, display the start screen
    if (game_state == State::start) {
        ofSetColor(255, 255, 0);
        title_font.drawString("PacMan", 300, 500);
        secondary_font.drawString("PRESS ANY KEY TO START GAME", 270, 950);
    } 
	// otherwise if the game state is ended, display the game over screen
	else if (game_state == State::ended) {
        ofSetColor(255, 255, 0);
        title_font.drawString("Game Over", 170, 500);
        secondary_font.drawString("PRESS ANY KEY TO START A NEW GAME", 170, 950);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	// if the game state is start or ended, reset the player and start a new game
    if (game_state == State::start || game_state == State::ended) {
        player.Reset();
		NewGame();
    }

	// if camer input if false, change the direction of the player to the given directional key
	if (!kCameraInput){
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
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	// calibarate the camera input to the color of the pixel at the given coordinates
    input.CalibrateCamera(x, y);
}

void ofApp::NewGame() {
	// reset the timers
    blink_timer = chrono::system_clock::to_time_t(chrono::system_clock::now());
    sound_delay_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
	
	// play the starting music
    game_music.play();
    
	game_state = State::starting;
    blink = false;

	// load the map and reset the player's location
    map.LoadMap();
    player.ResetPosition();

	// reset the ghosts' locations
    for (int i = 0; i < ghosts.size(); i++) {
        ghosts[i].ResetPosition();
    }
}