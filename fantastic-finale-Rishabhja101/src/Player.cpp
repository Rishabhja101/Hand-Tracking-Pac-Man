#include "Player.h"

Player::Player() {
    score = 0;
    powerup = false;
    lives = kStartingLives;
    
    ResetPosition();
}

void Player::Draw() {
    ofSetColor(255, 255, 0);
    ofDrawCircle(position_x, position_y, kRadius);

	ofSetColor(0, 0, 0);

	if (current_direction != Direction::none) {
        last_direction = current_direction;
	}

	double radius = kRadius * 1.1;

	if (last_direction == Direction::down) {
                    ofDrawTriangle(position_x, position_y,
                                   position_x + radius * sin(mouth_angle),
                                   position_y + radius * cos(mouth_angle),
                                   position_x - radius * sin(mouth_angle),
                                   position_y + radius * cos(mouth_angle));
            ofDrawTriangle(position_x,
                           position_y + 2 * radius * cos(mouth_angle),
                           position_x + radius * sin(mouth_angle),
                           position_y + radius * cos(mouth_angle),
                           position_x - radius * sin(mouth_angle),
                           position_y + radius * cos(mouth_angle));
	} 
	else if (last_direction == Direction::up) {
            ofDrawTriangle(position_x, position_y,
                           position_x + radius * sin(mouth_angle),
                           position_y - radius * cos(mouth_angle),
                           position_x - radius * sin(mouth_angle),
                           position_y - radius * cos(mouth_angle));
            ofDrawTriangle(position_x,
                           position_y - 2 * radius * cos(mouth_angle),
                           position_x + radius * sin(mouth_angle),
                           position_y - radius * cos(mouth_angle),
                           position_x - radius * sin(mouth_angle),
                           position_y - radius * cos(mouth_angle));
	} 
	else if (last_direction == Direction::left) {
            ofDrawTriangle(position_x - 2 * radius * cos(mouth_angle), position_y,
                           position_x - radius * cos(mouth_angle),
                           position_y + radius * sin(mouth_angle),
                           position_x - radius * cos(mouth_angle),
                           position_y - radius * sin(mouth_angle));
            ofDrawTriangle(position_x, position_y,
                           position_x - radius * cos(mouth_angle),
                           position_y + radius * sin(mouth_angle),
                           position_x - radius * cos(mouth_angle),
                           position_y - radius * sin(mouth_angle));
	} 
	else if (last_direction == Direction::right) {
            ofDrawTriangle(position_x + 2 * radius * cos(mouth_angle), position_y,
                           position_x + radius * cos(mouth_angle),
                           position_y + radius * sin(mouth_angle),
                           position_x + radius * cos(mouth_angle),
                           position_y - radius * sin(mouth_angle));
            ofDrawTriangle(position_x, position_y,
                           position_x + radius * cos(mouth_angle),
                           position_y + radius * sin(mouth_angle),
                           position_x + radius * cos(mouth_angle),
                           position_y - radius * sin(mouth_angle));
	}
}

void Player::Update(Map map, State game_state) {
    if (game_state == State::starting || game_state == State::death) {
        return;
    }
	
	Collisions(map);

    if (possible_directions[next_direction]) {
        current_direction = next_direction;
        next_direction = Direction::none;
    } else if (current_direction == Direction::none) {
        next_direction = Direction::none;
    }

    if (current_direction == Direction::up) {
        position_y -= kSpeed;
    } else if (current_direction == Direction::down) {
        position_y += kSpeed;
    } else if (current_direction == Direction::left) {
        position_x -= kSpeed;
    } else if (current_direction == Direction::right) {
        position_x += kSpeed;
    }

    Teleport(map);

	if (current_direction != Direction::none) {
		mouth_angle += mouth_direction;
	}

	if (mouth_angle <= 0 || mouth_angle >= kMaxMouthDegree) {
        mouth_direction *= -1;
	}
}

// Returns true if the player hit a powerup otherwise returns false
void Player::Collisions(Map map) {
    possible_directions[Direction::up] = true;
    possible_directions[Direction::down] = true;
    possible_directions[Direction::left] = true;
    possible_directions[Direction::right] = true;

    // if the player is not in the center of the path, it cannot turn onto that
    // path
    if ((position_x - map.kOffsetX) % map.kScale != map.kScale / 2) {
        possible_directions[Direction::up] = false;
        possible_directions[Direction::down] = false;
    }
    if ((position_y - map.kOffsetY) % map.kScale != map.kScale / 2) {
        possible_directions[Direction::left] = false;
        possible_directions[Direction::right] = false;
    }

    int x_on_map = (position_x - map.kOffsetX) / map.kScale;
    int y_on_map = (position_y - map.kOffsetY) / map.kScale;

    // collide with wall on the left
    int temp_x = (position_x - map.kOffsetX - map.kScale / 2 - 1) / map.kScale;
    if (map.GetAtPosition(temp_x, y_on_map) == map.kWall) {
        possible_directions[Direction::left] = false;
        if (current_direction == Direction::left) {
            last_direction = current_direction;
            current_direction = Direction::none;
        }
    }

    // collide with wall on the right
    temp_x = (position_x - map.kOffsetX + map.kScale / 2) / map.kScale;
    if (map.GetAtPosition(temp_x, y_on_map) == map.kWall) {
        possible_directions[Direction::right] = false;
        if (current_direction == Direction::right) {
            last_direction = current_direction;
            current_direction = Direction::none;
        }
    }

    // collide with wall on the top
    int temp_y = (position_y - map.kOffsetY - map.kScale / 2 - 1) / map.kScale;
    if (map.GetAtPosition(x_on_map, temp_y) == map.kWall) {
        possible_directions[Direction::up] = false;
        if (current_direction == Direction::up) {
            last_direction = current_direction;
            current_direction = Direction::none;
        }
    }

    // collide with wall on the bottom
    temp_y = (position_y - map.kOffsetY + map.kScale / 2) / map.kScale;
    if (map.GetAtPosition(x_on_map, temp_y) == map.kWall) {
        possible_directions[Direction::down] = false;
        if (current_direction == Direction::down) {
            last_direction = current_direction;
            current_direction = Direction::none;
        }
    }

    // collect coin
    if (map.CollectCoin(x_on_map, y_on_map)) {
        score += kCoinValue;
        eat_music.play();
        cout << "score: " << score << endl;
    }

    // get powerup
    if (map.GetPowerup(x_on_map, y_on_map)) {
        powerup = true;
    }
}

void Player::ChangeDirection(Direction new_direction) {
    if (possible_directions[new_direction]) {
        current_direction = new_direction;
    } else if (current_direction != new_direction) {
        next_direction = new_direction;
    }
}

void Player::Teleport(Map map) {
    int x_on_map = (position_x - map.kOffsetX) / map.kScale;
    int y_on_map = (position_y - map.kOffsetY) / map.kScale;
    if (map.GetAtPosition(x_on_map, y_on_map) == map.kTeleport) {
        for (int i = 0; i < map.GetWidth(); i++) {
            if (map.GetAtPosition(i, y_on_map) == map.kTeleport &&
                i != x_on_map) {
                int direction = 1;
                if (i > map.GetWidth() / 2) {
                    direction = -1;
                }
                position_x = map.kOffsetX + i * map.kScale + map.kScale / 2 +
                             direction * map.kScale / 2;
                return;
            }
        }
    }
}

bool Player::HasPowerup() {
    bool temp = powerup;
    powerup = false;
    return temp;
}

void Player::Kill() {
    ResetPosition();
    lives--;
    cout << "lives: " << lives << endl;
}

void Player::ResetPosition() {
    position_x = kSpawnPositionX;
    position_y = kSpawnPositionY;

	mouth_angle = kMaxMouthDegree;
    mouth_direction = -kMouthSpeed;
    current_direction = Direction::none;
    next_direction = Direction::none;
    last_direction = Direction::right;
}

int Player::GetPositionX() { 
	return position_x; 
}

int Player::GetPositionY() {
	return position_y;
}

void Player::LoadMusic(ofSoundPlayer eat_music) {
    this->eat_music = eat_music;
}

bool Player::IsDead() {
	return lives == 0;
}

int Player::GetLives() {
	return lives;
}

int Player::GetScore() {
	return score;
}