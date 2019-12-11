#include "Ghost.h"

Ghost::Ghost() {
    ResetPosition();
    current_direction = Direction::none;
    death_music.load(kMusicPath);
    escape = true;
}

void Ghost::Draw(State game_state, bool blink) {
    // if the game mode is regular, death, start, starting, or ended, deisplay the ghost 
	// as its given regular color
	if (game_state == State::regular || game_state == State::death ||
        game_state == State::starting || game_state == State::start ||
        game_state == State::ended) {
        ofSetColor(color_r, color_g, color_b);
    } 
	// if the game mode is scared or unscaring and blink is true, display the ghost as blue
	else if (game_state == State::scared || (game_state == State::unscaring && blink)) {
        ofSetColor(0, 0, 255);
    } 
	// otherwise, if unscaring is true then display the ghost as white
	else if (game_state == State::unscaring) {
        ofSetColor(255, 255, 255);
    }

	// draw the ghost as a circle
    ofDrawCircle(position_x, position_y, kRadius);
}

// Calculate possible directions and the next direction for the ghost and update its position
void Ghost::Update(Map map, State game_state) {
	// if the respawn delay has elapsed since the ghost respawned, set escape to true
	if (respawn_time == chrono::system_clock::to_time_t(chrono::system_clock::now()) - respawn_delay) {
            escape = true;
	}

	// if the game state is starting or death, do nothing
    if (game_state == State::starting || game_state == State::death) {
        return;
	}

	// Check for the possible directions the ghost can move and calculate its next direction
    Collisions(map);
    CalculateNextDirection(map);

	// Move the ghost in the direction that it is facing
    if (current_direction == Direction::up) {
        position_y -= kSpeed;
    } else if (current_direction == Direction::down) {
        position_y += kSpeed;
    } else if (current_direction == Direction::left) {
        position_x -= kSpeed;
    } else if (current_direction == Direction::right) {
        position_x += kSpeed;
    }

	// If the ghost is at a teleport, teleport it
    Teleport(map);
}

// Calculates the directions in which the ghost can move
void Ghost::Collisions(Map map) {
    // initiate all directions to be possible
    possible_directions[Direction::up] = true;
    possible_directions[Direction::down] = true;
    possible_directions[Direction::left] = true;
    possible_directions[Direction::right] = true;

    // if the ghost is not in the center of the path, it cannot turn onto that
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
            current_direction = Direction::none;
        }
    }

    // collide with wall on the right
    temp_x = (position_x - map.kOffsetX + map.kScale / 2) / map.kScale;
    if (map.GetAtPosition(temp_x, y_on_map) == map.kWall) {
        possible_directions[Direction::right] = false;
        if (current_direction == Direction::right) {
            current_direction = Direction::none;
        }
    }

    // collide with wall on the top
    int temp_y = (position_y - map.kOffsetY - map.kScale / 2 - 1) / map.kScale;
    if (map.GetAtPosition(x_on_map, temp_y) == map.kWall) {
        possible_directions[Direction::up] = false;
        if (current_direction == Direction::up) {
            current_direction = Direction::none;
        }
    }

    // collide with wall on the bottom
    temp_y = (position_y - map.kOffsetY + map.kScale / 2) / map.kScale;
    if (map.GetAtPosition(x_on_map, temp_y) == map.kWall) {
        possible_directions[Direction::down] = false;
        if (current_direction == Direction::down) {
            current_direction = Direction::none;
        }
    }
}

// Determines which direction the ghost should move in
void Ghost::CalculateNextDirection(Map map) {
    if (current_direction == Direction::none ||
        ((current_direction == Direction::right || current_direction == Direction::left) &&
         (possible_directions[Direction::up] || possible_directions[Direction::down])) ||
        ((current_direction == Direction::up || current_direction == Direction::down) &&
         (possible_directions[Direction::right] || possible_directions[Direction::left]))) {
        
		// while the currently calculated direction is not a direction the ghost can omve in, 
		// randomly pick a new direction
        int dir = ofRandom(0, 4);
		while (
            !possible_directions[dir] || (dir == current_direction) ||
            (current_direction == Direction::right && dir == Direction::left) ||
            (current_direction == Direction::left && dir == Direction::right) ||
            (current_direction == Direction::up && dir == Direction::down) ||
            (current_direction == Direction::down && dir == Direction::up)) {
            dir = ofRandom(0, 4);
        }
        current_direction = static_cast<Direction>(dir);
    }

	// if the ghost is set to escape, allow it to escape
    if (escape) {
		// if the ghost is at its spawn location, set its direction to up
        if (position_x == kSpawnPositionX &&
            position_y > kSpawnPositionY - map.kScale * 2) {
            current_direction = Direction::up;
        } 
		// Onnce the ghost leaves its cage, set escape to false
		else if (position_y <= kSpawnPositionY - map.kScale * 2) {
            escape = false;
        }
    }
}

// If the ghost is on a teleport spot, move the ghost to the other teleport on that same row
void Ghost::Teleport(Map map) {
    int x_on_map = (position_x - map.kOffsetX) / map.kScale;
    int y_on_map = (position_y - map.kOffsetY) / map.kScale;
    if (map.GetAtPosition(x_on_map, y_on_map) == map.kTeleport) {
        // iterate through each poition on the ghost's row
        for (int i = 0; i < map.GetWidth(); i++) {
			// if the position is a teleport that the ghost is not currently on, move the ghost there
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

// Reset the ghost's position to its spawn location and play its death sound effect
void Ghost::Kill() { 
	death_music.play();
	ResetPosition(); 
}

// Reset the ghost's position to its spawn location and reset the respawn time
void Ghost::ResetPosition() {
    position_x = kSpawnPositionX;
    position_y = kSpawnPositionY;

	respawn_delay = kRespawnTime;
	respawn_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
}

// Returns true if the player died, if the player ate the ghost, kills the ghost
// and returns false, otherwise just returns false
bool Ghost::PlayerCollision(int position_x, int position_y, State game_state, Map map) {
    if ((this->position_x - map.kOffsetX) / map.kScale ==
        (position_x - map.kOffsetX) / map.kScale &&
        (this->position_y - map.kOffsetY) / map.kScale ==
        (position_y - map.kOffsetY) / map.kScale) {
        
		// if the game state is regular, return true that the player died, 
		// otherwise kill the ghost and return false 
		if (game_state == State::regular) {
            return true;
        } else if (game_state == State::unscaring || game_state == State::scared) {
            Kill();
            return false;
        }
    }
    return false;
}

// Set the regular color of the ghost to the given rgb values
void Ghost::SetColor(int r, int g, int b) { 
	color_r = r;
    color_g = g;
    color_b = b;
}

// Set the respawn time of the ghost to the given 
void Ghost::SetRespawnTime(int delay) {
	respawn_delay = delay;
}