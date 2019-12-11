#include "Ghost.h"

Ghost::Ghost() {
    ResetPosition();
    current_direction = Direction::none;
    death_music.load(kMusicPath);
    escape = true;
}

void Ghost::Draw(State game_state, bool blink) {
    if (game_state == State::regular || game_state == State::death ||
        game_state == State::starting || game_state == State::start ||
        game_state == State::ended) {
        ofSetColor(color_r, color_g, color_b);
    } else if (game_state == State::scared ||
               (game_state == State::unscaring && blink)) {
        ofSetColor(0, 0, 255);
    } else if (game_state == State::unscaring) {
        ofSetColor(255, 255, 255);
    }

    ofDrawCircle(position_x, position_y, kRadius);
}

void Ghost::Update(Map map, State game_state) {
	if (respawn_time == chrono::system_clock::to_time_t(chrono::system_clock::now()) - respawn_delay) {
            escape = true;
	}

    if (game_state == State::starting || game_state == State::death) {
        return;
	}

    Collisions(map);
    CalculateNextDirection(map);

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
}

void Ghost::Collisions(Map map) {
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

void Ghost::CalculateNextDirection(Map map) {
    if (current_direction == Direction::none ||
        ((current_direction == Direction::right ||
          current_direction == Direction::left) &&
         (possible_directions[Direction::up] ||
          possible_directions[Direction::down])) ||
        ((current_direction == Direction::up ||
          current_direction == Direction::down) &&
         (possible_directions[Direction::right] ||
          possible_directions[Direction::left]))) {
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

    if (escape) {
        if (position_x == kSpawnPositionX &&
            position_y > kSpawnPositionY - map.kScale * 2) {
            current_direction = Direction::up;
        } else if (position_y <= kSpawnPositionY - map.kScale * 2) {
            escape = false;
        }
    }
}

void Ghost::Teleport(Map map) {
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

void Ghost::Kill() { 
	death_music.play();
	ResetPosition(); 
}

void Ghost::ResetPosition() {
    position_x = kSpawnPositionX;
    position_y = kSpawnPositionY;

	respawn_delay = kRespawnTime;
	respawn_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
}

// Returns true if the player died, if the player ate the ghost, kills the ghost
// and returns false, otherwise just returns false
bool Ghost::PlayerCollision(int position_x, int position_y, State game_state,
                            Map map) {
    if ((this->position_x - map.kOffsetX) / map.kScale ==
            (position_x - map.kOffsetX) / map.kScale &&
        (this->position_y - map.kOffsetY) / map.kScale ==
            (position_y - map.kOffsetY) / map.kScale) {
        if (game_state == State::regular) {
            return true;
        } else if (game_state == State::unscaring ||
                   game_state == State::scared) {
            Kill();
            return false;
        }
    }
    return false;
}

void Ghost::SetColor(int r, int g, int b) { 
	color_r = r;
    color_g = g;
    color_b = b;
}

void Ghost::SetRespawnTime(int delay) {
	respawn_delay = delay;
}