#include "mbed.h"
#include <string>
#include <ctime>
#include "RGBmatrixPanel.h"
#include "joystick.h"
Joystick joystick(A5, A3, D13);

PinName ub1 = D4;
PinName ug1 = D3;
PinName ur1 = D2;
PinName lb2 = D7;
PinName lg2 = D6;
PinName lr2 = D5;
RGBmatrixPanel matrix(ur1, lr2, ug1, lg2, ub1, lb2, A4, A0, A2, A1, D9, D8, D10, false);
DigitalIn button(SW2);

int points = 0; // total points so far
int difficulty = 1; // current difficulty of the game
int space [32][32]; //space array for storing state of board if occupied by aliens or not

//initializing the board to zeros
void spaceInitialize(){
for (int out = 0; out < 32; out = out + 1) {
    for (int in = 0; in < 32; in = in + 1) {
      space[out][in] = 0;
    }
  }
}
//total aliens in the game
int numAliens = 0;

//delay function that also spins on matrix.updateDisplay() that refreshes the display. Helps avoid flickering
void delay(int x) {
  for (int i = 0; i < x; i++) {
    wait_us(10);
    matrix.updateDisplay();
  }
}

//colors array for alien rows
uint16_t colors[] = {
  matrix.Color333(7, 0, 0),
  matrix.Color333(0, 7, 0),
  matrix.Color333(0, 0, 7),
  matrix.Color333(0, 4, 5),
  matrix.Color333(1, 2, 3)
};

//points array for the aliens. Higher row aliens have higher points
int pointsAlien[] = {
  5,
  4,
  3,
  2,
  1
};

//blank is the color to switch off a particular LED 
uint16_t blank = matrix.Color333(0, 0, 0);

//digitPrint helper function prints a number at the specified x y coordinates
void digitPrint(short score, int x, int y) {
  matrix.setCursor(x, y);
  matrix.setTextColor(matrix.Color333(0, 7, 7));
  short temp = score;
  char digits[2] = {
    '0',
    '0'
  };
  for (short i = 1; i >= 0; i--) {
    digits[i] = digits[i] + (temp % 10);
    temp = temp / 10;
  }
  for (short i = 0; i <= 1; i++) {
    matrix.putc(digits[i]);
  }
}

//difficulty_printer is used to print the current difficulty of the game
void diificulty_printer(int difficulty) {
  int x = 18;
  int colors[] = {
    0x4688,
    0xFF83,
    0xF0E3,
  };
  for (int i = difficulty; i > 0; i = i - 1) {
    matrix.fillRect(x, 0, 4, 5, colors[difficulty - 1]);
    x = x + 5;
  }
}
//specifically calls digitprint to print the total points
void score_print(int score) {
  digitPrint(score, 0, 0);
}
//erases the previous displayed score and updates it  with the current score
void update_score() {
  matrix.fillRect(0,0,14,8,blank);
  score_print(points);
}
//updates the difficulty descriptions
void update_difficulty() {
  matrix.fillRect(18,0,14,5,blank);
  diificulty_printer(difficulty);
}

//gameEnd prints out the win and lose screens based on whether you win or lose
void gameEnd(bool win) {
  matrix.fillScreen(blank);
  if (win) {
    matrix.setCursor(7, 0);
    matrix.setTextColor(matrix.Color333(0, 7, 0));
    matrix.putc('W');
    matrix.putc('I');
    matrix.putc('N');
  } else {
    matrix.setCursor(4, 0);
    matrix.setTextColor(matrix.Color333(7, 0, 0));
    matrix.putc('L');
    matrix.putc('O');
    matrix.putc('S');
    matrix.putc('T');
  }
  matrix.setCursor(1, 12);
  matrix.setTextColor(matrix.Color333(7, 0, 7));
  matrix.putc('S');
  matrix.putc('C');
  matrix.putc('O');
  matrix.putc('R');
  matrix.putc('E');
  digitPrint(points, 10, 24);
  while (1) matrix.updateDisplay();
}

//class alien that represents the enemies
class alien {
  public:
    int X; // x coordinate
  int Y; // y coordinate
  int alive; // whether the alien is alive or not
  uint16_t color; // the alien's color
  short moves; // used to track how far the alien has moved in the row
  int point; // points awarded for killing this alien
  bool move_right; // true if the alien is moving to the right, false otherwise
  int identifier; // the unique numerical identifier for this alien. An integer from 1 to Total Number  of Aliens
  alien() {} //empty constructor
  void alien_init(short x, short y, short color_index) { //initializes the aliens with their x,y, sets alive to 1, moves to 0, assigns colors and points and updates the matrix
    X = x;
    Y = y;
    alive = 1;
    point = pointsAlien[color_index];
    moves = 0;
    numAliens = numAliens + 1;
    identifier = numAliens;
    move_right = true;
    color = colors[color_index];
    matrix.fillRect(X, Y, 2, 2, color);
//    clearCurrentSpace();
    setNewSpace(); // updates the space array with the alien's identifier in the four indices to represent the 2x2 pixel grid the alien occupies
    
  }
  alien(int x, int y) {
    if (alive == 1 && x != 30 && y != 30)
      matrix.fillRect(x, y, 2, 2, matrix.Color333(7, 0, 0));
  }
  void go_left() { //moves the alien two spaces to the left and updates the space array to reflect its new position in the board
    clearCurrentSpace();
    X = X - 2;
    moves = moves - 2;
    matrix.fillRect(X, Y, 2, 2, color);
    setNewSpace();
  }
  void go_right() { //moves the alien two spaces to the right and updates the space array to reflect its new position in the board
    clearCurrentSpace();
    X = X + 2;
    moves = moves + 2;
    matrix.fillRect(X, Y, 2, 2, color);
    setNewSpace();
  }
  void go_down() { //moves the alien two spaces down and updates the space array to reflect its new position in the board
    clearCurrentSpace();
    Y = Y + 2;
    if (Y >= 30) { //if an alien enters the player's rows, then the game ends by calling gameEnd with arg false
      gameEnd(0);
    }
    matrix.fillRect(X, Y, 2, 2, color);
    setNewSpace();
  }
  
  void clearCurrentSpace(){ // used to set the previously occupied space of the alien to 0 to indicate the alien is no longer there
    space[Y][X] = 0; // set 4 indices to 0 as alien occupies a 2x2 grid
    space[Y][X+1] = 0;
    space[Y+1][X] = 0;
    space[Y+1][X+1] = 0;
    }

void setNewSpace(){ // used to set the 4 indices in the space array that capture the current 2x2 grid occupied by the alien to store the alien's unique identifier
    space[Y][X] = identifier;
    space[Y][X+1] = identifier;
    space[Y+1][X] = identifier;
    space[Y+1][X+1] = identifier;
    }
  void erase() { //erases the current position of an alive alien
    if (alive == 1) { 
    matrix.fillRect(X, Y, 2, 2, blank);  
    }
  }
  void delet(){ //called if the alien is shot.
 if (alive == 1){
 matrix.fillRect(X, Y, 2, 2, blank); //switch off LEDs
 alive = 0; //alien is dead
 points = points + point;
 clearCurrentSpace(); //remove alien's identifiers from space Array
 update_score(); // increment  the scores
 numAliens = numAliens - 1; // decrement total number of aliens
 if (numAliens >= 8 && numAliens <=15){ //update difficulty based on remaining aliens
     difficulty = 2;
     update_difficulty();
     }
 else if (numAliens >= 0 && numAliens < 8){ //update difficulty based on remaining aliens
     difficulty = 3;
     update_difficulty();  
     }
 }
}
  void move() { //move function for each alien
    if (alive == 1) {
      if (moves != 18 && moves != 0) {
        if (move_right) { // if the alien is halfway through the row, move it right if it was moving right
          go_right();
        } else { // else move the alien left. This if and else block is preemptive and is incorporated for efficiency purposes as this if else is likely to trigger first.
          go_left();
        }
      } else if (moves == 18 && move_right) { // if the alien is as far right as it is allowed to go, move down
        go_down();
        move_right = false;
      } else if (moves == 0 && !move_right) { // // if the alien is as far left as it is allowed to go, move down
        go_down();
        move_right = true;
      } else {
        if (move_right) { // otherwise move right or move left as normal
          go_right();
        } else {
          go_left();
        }
      }
    }

  }
};

int alien_rows = 4; //total alien rows
int alien_columns = 5; // total alien columns
alien aliens[4][5]; // global alien array. Avoided dynamic arrays to reduce memory footprint

//Call this to move all aliens  in the entire array
void moveAll() {
  for (int y = 0; y < alien_rows; y = y + 1) {
    for (int x = 0; x < alien_columns; x = x + 1) {
      aliens[y][x].erase(); //iterating overthe entire array and erasing the alien's current position
    }
  }
  for (int y = 0; y < alien_rows; y = y + 1) {
    for (int x = 0; x < alien_columns; x = x + 1) {
      aliens[y][x].move(); // iterate over the array again and move the aliens. Necessary to do it like this to ensure proper display without alien's overlapping
    }
  }

}
//class player that stores the player's x and y coordinates
class Player{
 public:
 int x;
 int y;
};

//global Player variable
Player player;

//playerPrinter prints the player's current position on the screen
void playerPrinter (){
    matrix.fillRect(player.x-1,player.y,3,2, matrix.Color333(7,3,0));
    matrix.drawPixel(player.x,player.y,blank);
    }

//playerPrinter erases the player from the screen  
void playerErase (){
    matrix.fillRect(player.x-1,player.y,3,2, blank);
    }
    
//class Missile to represent the missile  fired by the player
class Missile{
    public:
    int y_cor; // y coordinate of missile
    int x_cor; //  x coordinate of missile
    int hit; // whether a hit has been made
    bool cont; // whether the missile can continue traveling
    void fire(){ // reinitializes the missle at the player location
        y_cor = player.y;
        x_cor = player.x;
        hit = 0;
        cont = true;
        matrix.drawPixel(x_cor,y_cor,matrix.Color333(7, 0, 0));
        }
  void move(){ //moves the missile
    int identifier = (space [y_cor][x_cor]) ; // index space array to detect collision
           if (identifier !=0){ //check if an alien is in the next spot
               matrix.drawPixel(x_cor,y_cor,blank); //erase the laser
               identifier = identifier - 1; 
               hit = 1;
               cont = false;
               int row = identifier / 5; 
                   int column = identifier % 5;
                   aliens[row][column].delet(); //if so delete that alien by using its identifier to index the alien two dimensional array
               }
    else if (y_cor - 1 == 7){ //stop motion if the pixel has reached the section of the screen beyond which we display score and difficulty
    matrix.drawPixel(x_cor,y_cor,blank);
    cont = false;
    }
    else if (cont == true){ //if no collision and laser has not travelled fully, move the missile one step upwards
    matrix.drawPixel(x_cor,y_cor,blank);
    y_cor = y_cor - 1;
    matrix.drawPixel(x_cor,y_cor,matrix.Color333(7, 0, 0));
    }
    }
};
 
Missile m1; // global missile object. We work with only one to reduce memory footprint
    
int main() {
  joystick.init();
  spaceInitialize();
  Timer timerMove; //timer to track when the aliens should move
  Timer timerFire; // timer to track when the firing can happen
  timerFire.start();; // start the fire timer 
  timerMove.start(); // start the move  tier
  int fireTime = 150; // this is the interval in ms that must elapse before the missile is allowed to fire
  int lastTime = timerMove.read_ms(); //get the current times for moving
  int lastFire= timerFire.read_ms(); // get the current times for firing
  int moveTime; // movetime is initialized but updated later to account for difficulty
  matrix.begin(); // initialize LED board instance
  update_score(); // print score
  update_difficulty(); // print difficulty
  for (int y = 0; y < alien_rows; y = y + 1) { //nested for loops to initialize all aliens in their positions
    for (int x = 0; x < alien_columns; x = x + 1) {
      aliens[y][x].alien_init(3 * x, 3 * y + 9, y);
    }
  }
   player.x = 16; //setting initial x and y coordinates  for the player
    player.y = 30;
    int currentTime; //tracks the currentTime for moving
    int currFireTime; // tracks the fireTime for firing
            playerPrinter (); // print out the player
                m1.fire(); //initialize the missile
  while (1) {
      moveTime = 1000 + (numAliens) * 100; //compute movetime based on remaining aliens
      if (numAliens == 0){ // if all aliens killed, display win screen
          gameEnd(true);
      }
    currentTime = timerMove.read_ms(); // get currentTime for the moveTimer
    currFireTime = timerFire.read_ms(); // get time for the fireTimer
    if(joystick.GetXValue()<0.2){
           if (player.x!=1){ //if the player is not at the extreme left 
            playerErase();
           player.x=player.x-1; // move the player to the left
                   playerPrinter (); // display the player
            delay (500); // delay to ensure movement is not too fast for the player
               }
    }
    else if(joystick.GetXValue()>0.7){
    if (player.x != 30){ //if the player is not at the extreme right
    playerErase(); // erase the current position of the player
    player.x = player.x + 1; // move the player to the right
    playerPrinter (); // print out the player's position
    delay (500); // delay to ensure movement is not too fast for the player
    }
    }
    if (button ==0 && (!m1.cont || m1.hit == 1)){
        m1.fire();
        }
    if (currFireTime - lastFire > fireTime){ //if enough time has elapsed for the missile to travel
         lastFire = currFireTime; //update lastFire time for next check
         m1.move(); //move the missile
         }
    if (currentTime - lastTime > moveTime ) {// if sufficient time has elapsed, move the aliens
        lastTime = currentTime; // //update lastTime time for next check
        moveAll();
        }
    delay(50); //delay slightly. Helps avoid flickering in the display
    }
}