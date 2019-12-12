#include "Player.h"

// Resets the score, lives, and position of the player
Player::Player() {
    score = 0;
    powerup = false;
    lives = kStartingLives;
    ResetPosition();
}

// Draws the player as a yellow circle with a black cutout for its mouth
void Player::Draw() {
    ofSetColor(255, 255, 0);
    ofDrawCircle(position_x, position_y, kRadius);

    ofSetColor(0, 0, 0);
    if (current_direction != Direction::none) {
        last_direction = current_direction;
    }

    double radius = kRadius * 1.1;

    if (last_direction == Direction::down) {
		// if the player is facing down, draw the mouth pointed down
        ofDrawTriangle(position_x, position_y,
                       position_x + radius * sin(mouth_angle),
                       position_y + radius * cos(mouth_angle),
                       position_x - radius * sin(mouth_angle),
                       position_y + radius * cos(mouth_angle));
        ofDrawTriangle(position_x, position_y + 2 * radius * cos(mouth_angle),
                       position_x + radius * sin(mouth_angle),
                       position_y + radius * cos(mouth_angle),
                       position_x - radius * sin(mouth_angle),
                       position_y + radius * cos(mouth_angle));
    } else if (last_direction == Direction::up) {
        // if the player is facing up, draw the mouth pointed up
        ofDrawTriangle(position_x, position_y,
                       position_x + radius * sin(mouth_angle),
                       position_y - radius * cos(mouth_angle),
                       position_x - radius * sin(mouth_angle),
                       position_y - radius * cos(mouth_angle));
        ofDrawTriangle(position_x, position_y - 2 * radius * cos(mouth_angle),
                       position_x + radius * sin(mouth_angle),
                       position_y - radius * cos(mouth_angle),
                       position_x - radius * sin(mouth_angle),
                       position_y - radius * cos(mouth_angle));
    } else if (last_direction == Direction::left) {
        // if the player is facing left, draw the mouth pointed left
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
    } else if (last_direction == Direction::right) {
        // if the player is facing right, draw the mouth pointed right
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

// Checks possible moves for the player, updates its current direction, and moves the player
// updates the angle of player's mouth
// teleports the player if they are at a teleport
void Player::Update(Map map, State game_state) {
	// if the game state is starting or death, dont do anything
	if (game_state == State::starting || game_state == State::death) {
        return;
    }

	// check which directions the player can move in
    Collisions(map);

	// if the next direction the player wanted to move in is a legal move, move in that direction
    if (possible_directions[next_direction]) {
        current_direction = next_direction;
        next_direction = Direction::none;
    } else if (current_direction == Direction::none) {
        next_direction = Direction::none;
    }

	// move in the current direction that the player is facing
    if (current_direction == Direction::up) {
        position_y -= kSpeed;
    } else if (current_direction == Direction::down) {
        position_y += kSpeed;
    } else if (current_direction == Direction::left) {
        position_x -= kSpeed;
    } else if (current_direction == Direction::right) {
        position_x += kSpeed;
    }

	// if the player is at a teleport, teleport the player
    Teleport(map);

    // incriment the angle of the player's mouth
	if (current_direction != Direction::none) {
        mouth_angle += mouth_direction;
    }

	// if the mouth is completely open or closed, switch the direction of the mouth's movement
    if (mouth_angle <= 0 || mouth_angle >= kMaxMouthDegree) {
        mouth_direction *= -1;
    }
}

// Determines the directions in which the player can move
void Player::Collisions(Map map) {
	// initiate all directions to be possible
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
    }

    // get powerup
    if (map.GetPowerup(x_on_map, y_on_map)) {
        score += kPowerupValue;
        powerup = true;
    }
}

// If the given direction is available, set that to the current direction of the player, 
// otherwise set it as the player's next direction
void Player::ChangeDirection(Direction new_direction) {
	// if the given direction is valid, set that as the current direction
    if (possible_directions[new_direction]) {
        current_direction = new_direction;
    } 
	// otherwise set that as the next direction
	else if (current_direction != new_direction) {
        next_direction = new_direction;
    }
}

// If the player is on a teleport spot, move the player to the other teleport on that same row
void Player::Teleport(Map map) {
    int x_on_map = (position_x - map.kOffsetX) / map.kScale;
    int y_on_map = (position_y - map.kOffsetY) / map.kScale;
    if (map.GetAtPosition(x_on_map, y_on_map) == map.kTeleport) {
		// iterate through each poition on the player's row
        for (int i = 0; i < map.GetWidth(); i++) {
			// if the position is a teleport that the player is not currently on, move the player there
            if (map.GetAtPosition(i, y_on_map) == map.kTeleport && i != x_on_map) {
                int direction = 1;
                if (i > map.GetWidth() / 2) {
                    direction = -1;
                }
                position_x = map.kOffsetX + i * map.kScale + map.kScale / 2 + direction * map.kScale / 2;
                return;
            }
        }
    }
}

// if the player had a powerup, set the powerup to false and return true, otherwise just return false
bool Player::HasPowerup() {
    bool temp = powerup;
    powerup = false;
    return temp;
}

// Reset the position of the player to its spawn location and decrease its lives cout
void Player::Kill() {
    ResetPosition();
    lives--;
}

// Reset the player to its spawn location, reset its mouth angle, set its current and next 
// direction to none, and set its last direction to right
void Player::ResetPosition() {
    position_x = kSpawnPositionX;
    position_y = kSpawnPositionY;

    mouth_angle = kMaxMouthDegree;
    mouth_direction = -kMouthSpeed;
    current_direction = Direction::none;
    next_direction = Direction::none;
    last_direction = Direction::right;
}

// Return the x position of the player
int Player::GetPositionX() { 
	return position_x; 
}


// Return the y position of the player
int Player::GetPositionY() { 
	return position_y; 
}

// Set the eat_music to the ofSoundPlayer object given
void Player::LoadMusic(ofSoundPlayer eat_music) { 
	this->eat_music = eat_music; 
}


// If the player has 0 lives, return true, otherwise return false
bool Player::IsDead() { 
	return lives == 0; 
}

// Return the number of lives the player has left
int Player::GetLives() { 
	return lives; 
}

// Return the player's score
int Player::GetScore() { 
	return score; 
}

// Reset the player's lives and score for a new game
void Player::Reset() { 
	lives = kStartingLives;
    score = 0;
}

// Called when player eats a ghost, increases score accordingly
void Player::EatGhost() { 
	score += kGhostValue; 
}