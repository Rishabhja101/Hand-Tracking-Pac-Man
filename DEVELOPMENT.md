- **11/15/19** Spent all day playing around with OpenCV and following examples for hand tracking and color tracking
  - None of them worked correctly
  - Will look for more examples with OpenCV tomorrow, if that doesn't work then might need to modify project proposal

- **11/16/19** Found an example on https://sites.google.com/site/ofauckland/examples/10-testing for color tracking with OpenCV
  - Used example to create class for camera input with a method that returns up/down/left/right depending on position of hand in the frame

- **11/18/19** Added ability to move the position where the camera frame is displayed on the screen
  - Added enum for direction instead of using a string

- **11/21/19** Created Map class which loads a map from a file and displays the maze, coins, and powerups
  - Implemented use of constants for items on map

- **11/22/19** Created Player class
  - Player is now able to move around the board without traveling through the walls
  - Moved constants in Map to public so they can be accessed by Player
  - Player can now set their next move for the direction the player will turn once it reaches a wall
  - Spend a couple hours trying to make it possible for the player to pick up coins from the map with no success
  - Will come back to coin collecting next week

- **12/01/19** Created Ghost class
  - Ghost moves around the map randomly
  - Ghost does not travel through walls, chooses a random direction to move from available directions
  - Added Player teleportation from one end of the map to the other
  - Added Ghost teleportation from one end of the map to the other

- **12/02/19** Added ability for player to pick up coins
  - Coins get removed from the Map
  - The player's score increases accordingly
  - Powerups blink

- **12/03/19** Player can pickup Powerups
  - Ghosts enter a scared state when player has a powerup
  - Powerup's effects only last a specified amount of time
  - Ghost's color changes during scared mode
  - Ghost's color starts blinking when scared mode is about to end
  - Ghosts now respawn when eaten by the player
  - Player now respawns upon colliding with the ghost
  - Player now loses lives when it dies
  - Player and Ghost respawn at their correct positions and Ghost can escape the ghost cage

- **12/04/19** Added sounds for start game, collecting coin, dying, and eating ghosts
  - Added delays to game to match music
  - Added respawn time for ghost
  - Player's mouth opens and closes
  - Player can now be controlled through the camera

- **12/08/19** Added multiple ghosts

- **12/10/19** Gave each ghost a unique color
  - Added different respawn delays for each ghost
  - Added empty menu and game over screens
  - Added display for score, high score, and lives
  - Game now keeps track of high score between multiple games
  - Map is reloaded after player collects all coins and powerups
  - Created opening title screen and closing screen
  - Added nice looking title and closing screens
  - At game over, user can choose to start a new game

- **12/11/19** Added some comments and changed small things in the code
  - Window size displays entire map when program starts
  - Wrote README
  - Score now increases when player gets powerups and eats ghosts
  - Map data now leads from relative path rather than absolute path
  - Removed some unnecessary files
  - Added Tests
  - Fixed formatting of code to ensure no lines are longer than 100 characters
  - Added background music for scared state
  - Added background music for regular game state
  - Fixed bug preventing ghosts from escaping cage one at a time when game starts
  - Background music stops playing when map is reloaded
