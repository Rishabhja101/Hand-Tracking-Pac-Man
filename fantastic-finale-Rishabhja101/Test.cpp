#define CATCH_CONFIG_MAIN
#include "src/Ghost.h"
#include "src/Map.h"
#include "src/Player.h"
#include "src/catch.hpp"

Map GetMap() {
    Map map;
    map.LoadMap();
    return map;
}

//-------------------------------------------------------------------------------------------------
// Map Tests
//-------------------------------------------------------------------------------------------------

TEST_CASE("GetAtPositon(int x, int y) correctly returns the item at the given coordinate") {
    Map map = GetMap();
    REQUIRE(map.GetAtPosition(1, 1) == map.kWall);
}

TEST_CASE("GetWidth() returns the width of the map") {
    Map map = GetMap();
    REQUIRE(map.GetWidth() == 21);
}

TEST_CASE("GetHeight() returns the height of the map") {
    Map map = GetMap();
    REQUIRE(map.GetHeight() == 23);
}

TEST_CASE("CollectCoin(int x, int y) true when there is a coin and removes it from the map") {
    Map map = GetMap();
    REQUIRE(map.CollectCoin(2, 2));
    REQUIRE(map.GetAtPosition(2, 2) == map.kEmpty);
}

TEST_CASE("CollectCoin(int x, int y) false when there is no coin and doesnt change the map") {
    Map map = GetMap();
    REQUIRE(map.CollectCoin(1, 1));
    REQUIRE(map.GetAtPosition(1, 1) == map.kWall);
}

TEST_CASE("GetPowerup(int x, int y) true when there is a powerup and removes it from the map") {
    Map map = GetMap();
    REQUIRE(map.CollectCoin(2, 3));
    REQUIRE(map.GetAtPosition(2, 3) == map.kEmpty);
}

TEST_CASE("GetPowerup(int x, int y) false when there is no GetPowerup and doesnt change the map") {
    Map map = GetMap();
    REQUIRE(map.GetPowerup(1, 1));
    REQUIRE(map.GetAtPosition(1, 1) == map.kWall);
}

TEST_CASE("IsEmpty() returns false when the map is not empty") {
    Map map = GetMap();
    REQUIRE(!map.IsEmpty());
}

//-------------------------------------------------------------------------------------------------
// Ghost Tests
//-------------------------------------------------------------------------------------------------

TEST_CASE("PlayerCollision returns true when it collides with the player") {
    Map map = GetMap();
    Ghost ghost;
    REQUIRE(ghost.PlayerCollision(680, 680, State::regular, map));
}

TEST_CASE("PlayerCollision returns false when it doesn't collides with the player") {
    Map map = GetMap();
    Ghost ghost;
    REQUIRE(ghost.PlayerCollision(0, 0, State::regular, map));
}

//-------------------------------------------------------------------------------------------------
// Player Tests
//-------------------------------------------------------------------------------------------------

TEST_CASE("ChangeDirection() changes player's direction and Update() updates player's position") {
    Map map = GetMap();
    Player player;
    player.ChangeDirection(Direction::right);
    player.Update(map, State::regular);
    REQUIRE(player.GetPositionX() == 674);
}

TEST_CASE("GetLives() returns the number of lives the player has left") {
    Player player;
    REQUIRE(player.GetLives() == 3);
}

TEST_CASE("GetScore() returns the score of the player") {
    Player player;
    REQUIRE(player.GetScore() == 0);
}

TEST_CASE("IsDead() returns false if the player has more than 0 lives left") {
    Player player;
    REQUIRE(!player.IsDead());
}

TEST_CASE("ResetPosition() returns the player to its spawn location") {
    Map map = GetMap();
    Player player;
    player.ChangeDirection(Direction::right);
    player.Update(map, State::regular);
    player.ResetPosition();
    REQUIRE(player.GetPositionX() == 680);
}

TEST_CASE("Reset() returns the player to its spawn location and gives player back its lives") {
    Map map = GetMap();
    Player player;
    player.ChangeDirection(Direction::right);
    player.Update(map, State::regular);
    player.Kill();
    player.Reset();
    REQUIRE(player.GetPositionX() == 680);
    REQUIRE(player.GetLives() == 3);
}

TEST_CASE("Kill() removes one life from the player") {
    Player player;
    player.Kill();
    REQUIRE(player.GetLives() == 2);
}

TEST_CASE("GetPositionX() returns the x coordinate of the player's position") {
    Player player;
    REQUIRE(player.GetPositionX() == 680);
}

TEST_CASE("GetPositionY() returns the y coordinate of the player's position") {
    Player player;
    REQUIRE(player.GetPositionY() == 1040);
}

TEST_CASE("EatGhost() increases player's score") {
    Player player;
    player.EatGhost();
    REQUIRE(player.GetScore() == 50);
}