#include "Player.h"

Player::Player() {
    position_x = kSpawnPositionX;
    position_y = kSpawnPositionY;

    current_direction = Direction::none;
    next_direction = Direction::none;
}

void Player::Draw() {
    ofSetColor(255, 255, 0);
    ofDrawCircle(position_x, position_y, kRadius);
}

void Player::Update(Map map) {
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
}

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

void Player::ChangeDirection(Direction new_direction) {
    if (possible_directions[new_direction]) {
        current_direction = new_direction;
    } else if (current_direction != new_direction) {
        next_direction = new_direction;
    }
}