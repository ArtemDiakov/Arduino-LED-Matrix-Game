//including AberLED library
#include <AberLED.h>
#include <TFT_ST7735.h>
#include <User_Setup.h>
#include <TFT_FastPin.h>

//debuging
//#define DEBUG

//defining states for each of the game stages
#define S_INVALID -1
#define S_START 0
#define S_PLAYING 1
#define S_END 2
int state = S_INVALID;
unsigned long stateStartTime;

unsigned long gameTime;

/////////////////////////////////////////////////////////////////////////////////////////////
//moving to the different state
void goToState(int s) {
  //changing state value
  state = s;
  //saving the time when state has started
  stateStartTime = millis();
}

//returns the duration of the state up until now
unsigned long getStateTime() {
  return millis() - stateStartTime;
}
/////////////////////////////////////////////////////////////////////////////////////////////

//render Starting menu

unsigned long FLASHINTERVAL = 1000;
unsigned long flashTimer;

// rendering starting menu that flashes each interval
void renderStartMenu() {
  // Draw a green arrow pointing right
  AberLED.set(0, 3, YELLOW);
  AberLED.set(0, 4, YELLOW);
  AberLED.set(1, 3, YELLOW);
  AberLED.set(1, 4, YELLOW);
  AberLED.set(2, 3, YELLOW);
  AberLED.set(2, 4, YELLOW);
  AberLED.set(3, 1, YELLOW);
  AberLED.set(3, 2, YELLOW);
  AberLED.set(3, 3, YELLOW);
  AberLED.set(3, 4, YELLOW);
  AberLED.set(3, 5, YELLOW);
  AberLED.set(3, 6, YELLOW);
  AberLED.set(4, 2, YELLOW);
  AberLED.set(4, 3, YELLOW);
  AberLED.set(4, 4, YELLOW);
  AberLED.set(4, 5, YELLOW);
  AberLED.set(5, 3, YELLOW);
  AberLED.set(5, 4, YELLOW);
  //draw red obsticles
  AberLED.set(5, 0, RED);
  AberLED.set(5, 7, RED);
  AberLED.set(6, 0, RED);
  AberLED.set(6, 1, RED);
  AberLED.set(6, 6, RED);
  AberLED.set(6, 7, RED);
  AberLED.set(7, 0, RED);
  AberLED.set(7, 1, RED);
  AberLED.set(7, 2, RED);
  AberLED.set(7, 5, RED);
  AberLED.set(7, 6, RED);
  AberLED.set(7, 7, RED);

  if (millis() > flashTimer) {
    flashTimer = millis() + FLASHINTERVAL;
    for (int x = 0; x < 8; x++) {
      for (int y = 0; y < 8; y++) {
        AberLED.set(x, y, BLACK);
      }
    }
  }
  delay(200);
}

/////////////////////////////////////////////////////////////////////////////////////////////

//rendering player

int playerPosition;
int currentPosition;

//setting players initial position
void initPlayer() {
  playerPosition = 4;
  currentPosition = 4;
}

//player goes down and remembers current poisition
void movePlayerDown() {
  currentPosition = playerPosition;
  playerPosition++;
}

//player goes up and remembers current poisition
void movePlayerUp() {
  currentPosition = playerPosition;
  playerPosition--;
}

//renders player
void renderPlayer() {
  AberLED.set(0, playerPosition, YELLOW);
}
/////////////////////////////////////////////////////////////////////////////////////////////

//render blocks

//initialise 2D array as representation of the screen
int blocks[8][8];

unsigned long blockTimer;
unsigned long BLOCKINTERVAL = 250;

//probabilities of obstacles/boosters to appear
// (the smaller the number, the bigger is chance)
int obstacleProb = 8;
int boostProb = obstacleProb * 4;

//setting all blocks to be 0 (black)
void initBlocks() {
  //increase blockTimer for the first time after initialisation
  blockTimer = millis() + BLOCKINTERVAL;
  //set all screen to 0
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      blocks[x][y] = 0;
    }
  }
}

//setting states to blocks of top wall
void setBlockState(int x, int y, bool unbreakable, bool boost) {
  //based on the boolean condition determine the state of the block
  //created on top wall (unbreakable/booster/nothing)
  if (unbreakable) {
    blocks[x][y] = -1;
  } else if (boost) {
    blocks[x][y] = 1;
  } else {
    blocks[x][y] = 0;
  }
}

//creating the first/top row to appear
void createTopWall() {
  //based on probability set boolean value to obstacle/boost to appear
  for (int y = 0; y < 8; y++) {
    //if random number out of obstacleProb/boostProb equal to 0 then
    //it passes true, else false
    setBlockState(7, y, random(obstacleProb) == 0, random(boostProb) == 0);
  }
}

//moving blocks by rows
void updateBlocks() {
  //every interval blocks are moved by one row
  if (millis() > blockTimer) {
    //reset blockTimer for it to wait until its time to move one more row
    blockTimer = millis() + BLOCKINTERVAL;
    //starting at the one row above bottom copy its value to the bottom one
    //and move one row up, then repeat
    for (int x = 1; x < 8; x++) {
      for (int y = 0; y < 8; y++) {
        blocks[x - 1][y] = blocks[x][y];
      }
    }
    //every interval creates a new row on the top
    createTopWall();
  }
}

//draw blocks based on assigned state
void renderBlocks() {
  //go through every block on the screen
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      //based on the state assign colour
      int col;
      if (blocks[x][y] == 0) {
        col = BLACK;
      } else if (blocks[x][y] == 1) {
        col = GREEN;
      } else if (blocks[x][y] == -1) {
        col = RED;
      }
      //based on the assigned colour draw one by one
      AberLED.set(x, y, col);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

//hiting obstacles/boosts

//returns true if player hits the green square
bool hitBoost() {
  //check if at the position where player is exists green aquare
  if (blocks[0][playerPosition] == 1) {
    //sets green square to black
    blocks[0][playerPosition] = 0;
    return true;
  } else {
    //no green square
    return false;
  }
}

//decreases interval/ slows down the game
void slowGameDown() {
  //limits minimum game speed
  if (BLOCKINTERVAL <= 225) {
    //slows game down
    BLOCKINTERVAL += 25;
  }
}

//returns true if player hits the red square
bool hitObstacle() {
  //check if at the position where player is exists red aquare
  if (blocks[0][playerPosition] == -1) {
    //sets red square to black
    blocks[0][playerPosition] = 0;
    return true;
  } else {
    return false;
  }
}

//increases interval/speeds game up
void speedGameUp() {
  //limits the maximum game speed
  if (BLOCKINTERVAL >= 50) {
    BLOCKINTERVAL -= 50;
  }
  //if game speed drops to zero
  if (BLOCKINTERVAL <= 0) {
    //records time spent playing at the playing state
    gameTime = getStateTime();
#ifdef DEBUG
    Serial.println(gameTime);
#endif
    //moving to the end state
    goToState(S_END);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////
//gradyually speeding game up

unsigned long gameSpeedUpTimer;
unsigned long GAMESPEEDUPINTERVAL = 20000;

//every interval speeds game up and increases number of obsticles
void addGameComplexity() {
  //interval update
  if (millis() > gameSpeedUpTimer) {
    gameSpeedUpTimer = millis() + GAMESPEEDUPINTERVAL;
    //speed game up
    speedGameUp();
    //does not allow to create too many obsticles
    if (obstacleProb > 4) {
      //increase chance of obsticles to appear
      obstacleProb--;
    }
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////
//display result

bool resultDisplayed = false;
int score;

//miliseconds in a second
int MILLSINSEC = 1000;

//render score on a scale 0-8
void renderGameScore() {
  //only draws if spent more then 10 seconds playing
  if (score > 10) {
    //comes to a scale 1-99
    score /= 10;
    //comes to a scale 1-8
    if (score > 8) {
      score = 8;
    }
    //draws the result 1-8
    for (int x = 0; x < score; x++) {
      AberLED.set(x, 3, GREEN);
      delay(200);
    }
  }

////====REPORT===////
//(reading following after report 2.4 (paragraph 3) might make more sense)
//I think I have figured the problem(but I have already wrote the report regarding an issue with score (green squares)
//that are not displayed) I swap it only after I draw the red screen thats why I couldn't see them
//I think if I add the swap inside the for loop for "draws the result 1-8" comment that would be a quick solution.
//if fix it properly then devide that 1 function in 3: update model(for if statements), render score, render red screen.
//I appologize, I have no idea how I couldn't see that before, (remains as FAIL in testing table)
//however, that does not solve the issue with retrieving score time

  delay(300);
  //if result 0 or if the result has already been shown
  //draw red screen
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      AberLED.set(x, y, RED);
    }
  }
}

//display time played in serial screen
void displayResult() {
  //if the result hasnt been shown yet
  if (!resultDisplayed) {
#ifdef DEBUG
    Serial.println(gameTime);
#endif
    //go from miliseconds to seconds
    score = gameTime / MILLSINSEC;
    Serial.println();
    Serial.print("Your time is: ");
    Serial.print(score);
    Serial.println(" seconds");
    //change to "result has been shown"
    resultDisplayed = true;
    // displaying how good player performed on the scale 0-8
    renderGameScore();
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////

void handleInput() {
  switch (state) {
    case S_START:
      //if fire button was pressed change to playing state
      if (AberLED.getButtonDown(FIRE)) {
        goToState(S_PLAYING);
      }
      break;
    case S_PLAYING:
      //if button down/up was pressed move him
      if (AberLED.getButtonDown(DOWN)) {
        movePlayerDown();
      } else if (AberLED.getButtonDown(UP)) {
        movePlayerUp();
      }
      //if button fire was pressed change state and record time spent palying
      if (AberLED.getButtonDown(FIRE)) {
        gameTime = getStateTime();
#ifdef DEBUG
        Serial.println(gameTime);
#endif
        goToState(S_END);
      }
      break;
    case S_END:
      break;
    default:
      Serial.print("Bad state");
      break;
  }
}

void updateModel() {
  switch (state) {
    case S_START:
      break;
    case S_PLAYING:
      //screen border restrictions for player
      if (playerPosition >= 8 || playerPosition <= -1) {
        playerPosition = currentPosition;
      }
      //slow gam edown if boost hit
      if (hitBoost()) {
        slowGameDown();
      }
      //speed game up if obstacl hit
      if (hitObstacle()) {
        speedGameUp();
      }
      //move the rows down
      updateBlocks();
      //speed game up and create more obstacles
      addGameComplexity();
      break;
    case S_END:
      break;
    default:
      Serial.print("Bad state");
      break;
  }
}

void render() {
  switch (state) {
    case S_START:
      //draw starting menu
      renderStartMenu();
      break;
    case S_PLAYING:
      //draw blocks
      renderBlocks();
      //draw player after so that he is not under the blocks
      renderPlayer();
      break;
    case S_END:
      //draw the result bar
      displayResult();
      break;
    default:
      Serial.print("Bad state");
      break;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  AberLED.begin();
  //go to the first state
  goToState(S_START);
  //increasing all of the intervals
  blockTimer = millis() + BLOCKINTERVAL;
  flashTimer = millis() + FLASHINTERVAL;
  gameSpeedUpTimer = millis() + GAMESPEEDUPINTERVAL;
  //initialising blocks and player
  initPlayer();
  initBlocks();
}

void loop() {
  handleInput();
  updateModel();
  AberLED.clear();
  AberLED.clearText();
  render();
  AberLED.swap();
}
