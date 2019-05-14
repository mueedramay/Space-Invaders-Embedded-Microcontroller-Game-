#include "mbed.h"
#include "RGBmatrixPanel.h" // Hardware-specific library
 
//pin setups
//PinName ub1=D13;
//PinName ug1=D14;
//PinName ur1=D15;
//PinName lb2=D10;
//PinName lg2=D11;
//PinName lr2=D12;
PinName ub1=D4;
PinName ug1=D3;
PinName ur1=D2;
PinName lb2=D7;
PinName lg2=D6;
PinName lr2=D5;
RGBmatrixPanel matrix(ur1, lr2,ug1,lg2,ub1,lb2,A0, A1, A2, A3, D8,D10, D9, false);
//RGBmatrixPanel matrix(ur1,ug1,ub1,lr2,lg2,lb2,A0,A1,A2,D9,A3,D8,false);
//RGBmatrixPanel(r1, g1, b1, r2, g2, b2, a, b, c, sclk, latch, oe, enable double_buffer);
//RGBmatrixPanel(r1,r2,g1, g2, b1, b2, a,b, c, d, sclk, latch, oe,bool dbuf)
//input button
DigitalIn button(SW2);
 
int x = 2;  // x coordinate of player
int y = 9;  // y coordinate of player
int l = 2;  // length of player
int h = 5;  // width of player
int speed = 50;  // speed of game: lower = faster
int score = 0;
int ox = 50; // x coordinate of obstacle
int oy = 10; // y coordinate of obstacle
int ol = 3;  // length of obstacle
int oh = 4;  // height of obstacle
 
Timer timer;  // timer for interrupt
 
/*basic wait function
calls updateDisplay() frequently enough for smooth display
x=4000 ~ 1 second*/
void delay1(int x){
    for(int i=0;i<x;i++){
        wait_us(10);
        matrix.updateDisplay();
    }
}
 
void displayScore(){
     
}
 
/*lose function
freezes screen to show collision
flashes score
goes to lose screen (red square)*/
void lose(){
    //freeze screen briefly
    delay1(4000);
     
    //clear screen for lose screen
    matrix.fillRect(0, 0, 32, 16, matrix.Color333(0, 0, 0));
    matrix.drawChar(1, 0, (char)50, matrix.Color333(2, 2, 2), matrix.Color333(0, 0, 0), 1);
    matrix.drawChar(7, 0, 'c', matrix.Color333(2, 2, 2), matrix.Color333(0, 0, 0), 1);
    matrix.drawChar(13,0, 'o', matrix.Color333(2, 2, 2), matrix.Color333(0, 0, 0), 1);
    matrix.drawChar(19, 0, 'r', matrix.Color333(2, 2, 2), matrix.Color333(0, 0, 0), 1);
    matrix.drawChar(25, 0, 'e', matrix.Color333(2, 2, 2), matrix.Color333(0, 0, 0), 1);
     
    matrix.fillRect(14,6, 4,4, matrix.Color333(3,0,0));
  while(1) matrix.updateDisplay();
}
 
/*interrupt function
called whenever timer is above speed
moves the obstacle forward
creates new randomized obstacle when off screen*/
void advance(){
    //clear screen
    matrix.fillRect(ox,oy,ol,oh,matrix.Color333(0,0,0));
    //increment ox and score
    ox --;
    score++;
    // move obstacle
    matrix.fillRect(ox,oy,ol,oh,matrix.Color333(3,0,0));
    // if obstacle off screen, generate random new one
    if (ox<-5){
        ol = (rand()%3)+1;
        ox=32+(rand()%15);
        oh= (rand()%4)+2;
        oy=14-oh;
    }
    return;
}
/*modified delay function
checks interrupts and lose condition*/
void delay(int x){
    for(int i=0;i<x;i++){
        //wait_us(10);
        matrix.updateDisplay();
        //check to see if timer has reached speed
        int t = timer.read_ms();
        if(t >speed) {
            timer.reset();
            advance();
        }
         
        //check collision for lose condition
        if(ox<4 && ox>1 && oy<y+h) lose();
        }
}
/*player jump function
called whenever button is pressed*/
void jump(){
    matrix.fillRect(x,y,l,h,matrix.Color333(0,0,0));
    //every time player jumps, the game gets faster
    speed = speed - 2;
    //jump up and down from y = 9 to 1
    for (int i = 9 ; i>0 ; i--){
        matrix.fillRect(x,i,l,h,matrix.Color333(0,3,0));
        //include effects of gravity
        delay(400-25*i);
        matrix.fillRect(x,i,l,h,matrix.Color333(0,0,0));
        //update player y coordinate
        y = i;
    }
    for (int i = 1 ; i<10 ; i++){
        matrix.fillRect(x,i,l,h,matrix.Color333(0,3,0));
        delay(400-30*i);
        matrix.fillRect(x,i,l,h,matrix.Color333(0,0,0));
        y=i;
    }
    return;
}
 
 
/* main game function
*/
int main(){
    timer.start();
     
    while(1){
        matrix.drawLine(0,14,31,14,matrix.Color333(0,0,3));  //grass
        matrix.fillRect(x,y,l,h,matrix.Color333(0,3,0));  // player sprite 
        matrix.updateDisplay();
        delay(10);
        // check button to jump
        if(button == 0) jump();
         
    }
}