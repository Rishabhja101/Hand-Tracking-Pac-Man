#include "Player.h"

Player::Player() {
    position_x = kSpawnPositionX;
    position_y = kSpawnPositionY;

    current_direction = Direction::none;
}

void Player::Draw() {
    ofSetColor(255, 255, 0);
    ofDrawCircle(position_x, position_y, kRadius);
}

void Player::Update(Map map) {
    Collisions(map);

    if (current_direction == Direction::up) {
        position_y -= kSpeed;
    } else if (current_direction == Direction::down) {
        position_y += kSpeed;
    } else if (current_direction == Direction::left) {
        position_x -= kSpeed;
    } else if (current_direction == Direction::right) {
        position_x += kSpeed;
    }
}

void Player::Collisions(Map map) {
    possible_directions[Direction::up] = true;
    possible_directions[Direction::down] = true;
    possible_directions[Direction::left] = true;
    possible_directions[Direction::right] = true;

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

	int temp_x = (position_x - map.kOffsetX - map.kScale / 2 - 1) / map.kScale;
    if (map.GetAtPosition(temp_x, y_on_map) == map.kWall) {
        possible_directions[Direction::left] = false;
        if (current_direction == Direction::left) {
            current_direction = Direction::none;
        }
    }

	temp_x = (position_x - map.kOffsetX + map.kScale / 2) / map.kScale;
    if (map.GetAtPosition(temp_x, y_on_map) == map.kWall) {
        possible_directions[Direction::right] = false;
        if (current_direction == Direction::right) {
            current_direction = Direction::none;
        }
    }

	int temp_y = (position_y - map.kOffsetY - map.kScale / 2 - 1) / map.kScale;
    if (map.GetAtPosition(x_on_map, temp_y) == map.kWall) {
        possible_directions[Direction::up] = false;
        if (current_direction == Direction::up) {
            current_direction = Direction::none;
        }
    }

	temp_y = (position_y - map.kOffsetY + map.kScale / 2) / map.kScale;
    if (map.GetAtPosition(x_on_map, temp_y) == map.kWall) {
        possible_directions[Direction::down] = false;
        if (current_direction == Direction::down) {
            current_direction = Direction::none;
        }
    }

	cout << position_x << " " << position_y << " " << possible_directions[Direction::up] << endl;
}

void Player::ChangeDirection(Direction new_direction) { 
	if (possible_directions[new_direction]) {
        current_direction = new_direction;
    }
}