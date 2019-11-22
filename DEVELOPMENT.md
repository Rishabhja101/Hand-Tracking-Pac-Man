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
