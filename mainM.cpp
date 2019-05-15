#include "mbed.h"
 //#include "wave_player.h"
#include "enemy.h"
 //#include "player.h"
//#include "missile.h"
#include "globals.h"
 //#include "rtos.h"
#include <string>

#include "RGBmatrixPanel.h"


PinName ub1 = D4;
PinName ug1 = D3;
PinName ur1 = D2;
PinName lb2 = D7;
PinName lg2 = D6;
PinName lr2 = D5;
RGBmatrixPanel matrix(ur1, lr2, ug1, lg2, ub1, lb2, A4, A0, A2, A1, D9, D8, D10, false);
//AnalogOut DACout(p18);      // speaker
//wave_player waver(&DACout); // wav player
//SDFileSystem sd(p5, p6, p7, p8, "sd"); // SD card and filesystem (mosi, miso, sck, cs)
//Nav_Switch myNav(p13, p10, p11, p9, p12); //pin order on Sparkfun breakout
//
DigitalIn button(SW2);
DigitalIn button2(SW3);

// 

int score = 0;
int lives = 3;

void delay(int x) {
  for (int i = 0; i < x; i++) {
    wait_us(10);
    matrix.updateDisplay();
  }
}
uint16_t colors[] = {
  matrix.Color333(7, 0, 0),
  matrix.Color333(0, 7, 0),
  matrix.Color333(0, 0, 7),
  matrix.Color333(0, 4, 5),
  matrix.Color333(1, 2, 3)
};
short points[] = {
  1,
  2,
  3,
  4,
  5
};

uint16_t blank = matrix.Color333(0, 0, 0);
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
    //matrix.putc(digit);
  }
  for (short i = 0; i <= 1; i++) {
    matrix.putc(digits[i]);
  }
}

void life_printer(int life) {
  int x = 18;
  int colors[] = {
    0xF0E3,
    0xFF83,
    0x4688
  };
  for (int i = life; i > 0; i = i - 1) {
    matrix.fillRect(x, 0, 4, 5, colors[life - 1]);
    x = x + 5;
  }
}
void score_print(int score) {
  digitPrint(score, 0, 0);
}
void update_score() {
  score_print(score);
}
void update_life() {
  life_printer(lives);
}

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
  digitPrint(score, 10, 24);
  while (1) matrix.updateDisplay();
}

class alien {
  public:
    int X;
  int Y;
  int alive;
  uint16_t color;
  short moves;
  short point;
  bool move_right;
  alien() {}
  void alien_init(short x, short y, short color_index) {
    X = x;
    Y = y;
    alive = 1;
    point = points[color_index];
    moves = 0;
    move_right = true;
    color = colors[color_index];
    matrix.fillRect(X, Y, 2, 2, color);
  }
  alien(int x, int y) {
    if (alive == 1 && x != 30 && y != 30)
      matrix.fillRect(x, y, 2, 2, matrix.Color333(7, 0, 0));
  }
  void go_left() {
    X = X - 2;
    moves = moves - 2;
    matrix.fillRect(X, Y, 2, 2, color);
  }
  void go_right() {
    X = X + 2;
    moves = moves + 2;
    matrix.fillRect(X, Y, 2, 2, color);
  }
  void go_down() {
    Y = Y + 2;
    if (Y >= 30) {
      gameEnd(0);
    }
    matrix.fillRect(X, Y, 2, 2, color);
  }

  void erase() {
    if (alive = 1) { 
    matrix.fillRect(X, Y, 2, 2, blank);  
    }
  }

  void move() {
    if (alive == 1) {
      if (moves != 18 && moves != 0) {
        if (move_right) {
          go_right();
        } else {
          go_left();
        }
      } else if (moves == 18 && move_right) {
        go_down();
        move_right = false;
      } else if (moves == 0 && !move_right) {
        go_down();
        move_right = true;
      } else {
        if (move_right) {
          go_right();
        } else {
          go_left();
        }
      }
    }

  }
};

int alien_rows = 4;
int alien_columns = 5;
alien aliens[4][5];

//Call this to moveall aliens  in theentire array
void moveAll() {
  for (int y = 0; y < alien_rows; y = y + 1) {
    for (int x = 0; x < alien_columns; x = x + 1) {
      aliens[y][x].erase();
    }
  }
  for (int y = 0; y < alien_rows; y = y + 1) {
    for (int x = 0; x < alien_columns; x = x + 1) {
      aliens[y][x].move();
    }
  }

}

class Player{
 public:
 int x;
 int y;
};

Player player;

void playerPrinter (int x, int y){
    matrix.fillRect(x-1,y,3,2, matrix.Color333(7,3,0));
    }

int main() {
  matrix.begin();
  update_score();
  update_life();
  for (int y = 0; y < alien_rows; y = y + 1) {
    for (int x = 0; x < alien_columns; x = x + 1) {
      aliens[y][x].alien_init(3 * x, 3 * y + 9, y);
    }
  }
  while (1) {
    moveAll();
    playerPrinter (16,30);
    delay(5000);
  }
}