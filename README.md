# Arduino LED Matrix Game

An Arduino LED matrix game developed using C/C++ for an 8x8 LED display.

This project was developed for the CS12020 Introduction to Programming module at Aberystwyth University.

## Overview

The game runs on an Arduino-based LED matrix. The player is represented as a yellow dot and can move up and down while red obstacles and green power-ups scroll across the display. The aim is to survive as long as possible while avoiding obstacles and collecting power-ups.

## Features

- Start, playing and end game states
- Player movement using input controls
- Red obstacles that scroll across the LED matrix
- Green power-ups that scroll across the LED matrix
- Collision handling between the player, obstacles and power-ups
- Obstacle collisions increase game speed
- Power-ups slow the game down, up to the starting speed limit
- Game difficulty increases gradually over time
- Obstacle count increases as the game progresses
- Game logic separated into input handling, model updates and rendering

## Technologies Used

- Arduino
- C/C++
- LED matrix display
- Arduino IDE

## Game Logic

The program is structured around game states. Each state is handled through separate input, update and render logic. During the playing state, obstacles and power-ups are generated, shifted across the matrix, and checked for collisions with the player.

## Testing

Manual testing was carried out for the main gameplay features, including state transitions, player movement, obstacle and power-up generation, collision behaviour and difficulty scaling.

Some optional/extra features, such as pause functionality, final score display and EEPROM high score storage, were not fully completed.

## Running the Project

1. Clone the repository:

   ```bash
   git clone https://github.com/ArtemDiakov/Arduino-LED-Matrix-Game.git
   
2. Open ard38_cs12020.ino in the Arduino IDE.

3. Connect the Arduino/LED matrix hardware used for the project.

4. Compile and upload the sketch to the board.
   
## Notes

This was an early university programming project focused on C/C++, state-based game logic, hardware interaction and real-time input/render/update behaviour.
