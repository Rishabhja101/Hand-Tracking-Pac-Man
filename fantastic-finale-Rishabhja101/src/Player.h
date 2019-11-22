#include "ofMain.h"
#include "Direction.h"
#include "Map.h"

class Player {
   public:
    Player();
    void Draw();
    void Update(Map map);
    void ChangeDirection(Direction new_direction);

   private:
    const int kSpeed = 5;
    const int kRadius = 20;
    const int kSpawnPositionX = 380;
    const int kSpawnPositionY = 380;

	int position_x;
    int position_y;
    Direction current_direction;
    bool possible_directions[4];
    
	void Collisions(Map map);
};