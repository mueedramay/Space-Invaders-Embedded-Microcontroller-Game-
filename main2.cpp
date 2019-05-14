#include "mbed.h"
#include "RGBmatrixPanel.h" // Hardware-specific library
#include <cstdlib>
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
 
DigitalIn button2(SW3);

 
enum Color {Red, Orange, Yellow, Green, DBlue, LBlue, Pink};
 
uint16_t getColor(Color c);

 
class Playerboard{
    private:
        int x,y;
        int filled;
        uint16_t color;  
         
    public:
        Playerboard() {
            x = 0;
            y = 0;
            filled = 0;
            color = matrix.Color333(0,0,0);  
        }
     
        Playerboard(int a, int b, uint16_t c) {
            x = a;
            y = b;
            color = c;
            filled = 1; 
        }
         
        void draw() {
            if(filled) {
                matrix.drawPixel(x,y,color);
            }
        }
         void rise(){
             x--;
         }
        void fall() {
            x++;  
        }
         
        int atTop(){
            if(x==0){
                return 1;
            }
            return 0;
        }
        int atGround() {
            if( x == 31) {
                return 1;  
            }
            return 0;
        }
         
        int getX() {
            return x;
        }
         
        int getY() {
            return y;  
        }
        int full() {
            return filled;  
        }
        void fill() {
            filled = 1;  
        }
        void clear() {
            filled = 0;  
        }
        void shift(int d) {
            y = (y+d+16)%16;  
        }
    };
 Playerboard board[32][32];///
 
void clearline() {
    int clear = 0;
    for(int i = 1; i < 32; i ++) {
        clear = 1;
        for(int j = 0; j < 32; j ++) {
            if(!board[i][j].full()) {
                clear = 0;
            }  
        } 
        if(clear) {
            for(int j = 0; j < 32; j ++) {
                board[i][j].clear();
                for(int k = i; k > 0; k --) {
                    board[k][j] = board[k-1][j]; 
                    board[k][j].fall();
                }
            } 
        }
    }  
}

class missile{
public:

Playerboard blocks0[1];
missile(){
    blocks0[0]=Playerboard(2,2,Red);
}
void drawm(){
            blocks0[0].draw();
 }
    
    int up(){

        Playerboard * b = &(blocks0[0]);
                int x0 = b->getX();
                int y0 = b->getY();
                if(b->atTop()) {
                    return 0;  
                }

            blocks0[0].rise();
                return 1;
    }

    int xcord(){
        Playerboard * b = &(blocks0[0]);
                int x = b->getX();
                return x;
    }


};

class player : public missile{
public:
    Playerboard blocks1[6];

    player(){
        blocks1[0]=Playerboard(2,2,Red);
        blocks1[1]=Playerboard(3,2,Red);
        blocks1[2]=Playerboard(2,3,Red);
        blocks1[3]=Playerboard(3,3,Red);
        blocks1[4]=Playerboard(2,4,Red);
        blocks1[5]=Playerboard(3,4,Red);

    }

    void drawp(){
        for(int i=0; i<6; i++){
            blocks1[i].draw();
        }
    matrix.updateDisplay();
    }


    void shift(int d) {
            for(int i = 0; i < 6; i ++) {
                Playerboard * b = &(blocks1[i]);
                int x1 = b->getX();
                int y1 = b->getY();
                if((board[x1][(y1+d+32)%32].full())) {
                    return;
                }
            }
            for(int i = 0; i < 6; i ++) {
                blocks1[i].shift(d);  
            }
        }

};

 
class Enemy : public player {
    private:
        Playerboard blocks[4];
        Enemy(){}
     
    public:
       
        Enemy(Color c) {
        switch(c) {
            case Red:
                blocks[0] = Playerboard(2,2,getColor(c));
                blocks[1] = Playerboard(3,2,getColor(c));
                blocks[2] = Playerboard(2,3,getColor(c));
                blocks[3] = Playerboard(3,3,getColor(c));
                break;
            case Orange:
           blocks[0] = Playerboard(2,2,getColor(c));
                blocks[1] = Playerboard(3,2,getColor(c));
                blocks[2] = Playerboard(2,3,getColor(c));
                blocks[3] = Playerboard(3,3,getColor(c));
                break;
            case Yellow:
               blocks[0] = Playerboard(2,2,getColor(c));
                blocks[1] = Playerboard(3,2,getColor(c));
                blocks[2] = Playerboard(2,3,getColor(c));
                blocks[3] = Playerboard(3,3,getColor(c));
                break;
            case Green:
               blocks[0] = Playerboard(2,2,getColor(c));
                blocks[1] = Playerboard(3,2,getColor(c));
                blocks[2] = Playerboard(2,3,getColor(c));
                blocks[3] = Playerboard(3,3,getColor(c));
                break;
            case LBlue: 
               blocks[0] = Playerboard(2,2,getColor(c));
                blocks[1] = Playerboard(3,2,getColor(c));
                blocks[2] = Playerboard(2,3,getColor(c));
                blocks[3] = Playerboard(3,3,getColor(c));
                break;
            case DBlue:
              blocks[0] = Playerboard(2,2,getColor(c));
                blocks[1] = Playerboard(3,2,getColor(c));
                blocks[2] = Playerboard(2,3,getColor(c));
                blocks[3] = Playerboard(3,3,getColor(c));
                break;
            case Pink:
               blocks[0] = Playerboard(2,2,getColor(c));
                blocks[1] = Playerboard(3,2,getColor(c));
                blocks[2] = Playerboard(2,3,getColor(c));
                blocks[3] = Playerboard(3,3,getColor(c));
                break;
            }
        }

        int xcord(){
            Playerboard * b = &(blocks[3]);
            int x=b->getX();
            return x;
        }
        void disappear(){
               for(int i = 0; i < 4; i++) {
                Playerboard * b = &(blocks[i]);
                int x = b->getX();
                int y = b->getY();
                board[x][y].clear();
        }  
        
        }
        void draw() {
            for (int i = 0; i < 4; i ++) {
                blocks[i].draw();
            } 
            matrix.updateDisplay();
        }
         
        int fall() {
            for (int i = 0; i < 4; i ++) {
                Playerboard * b = &(blocks[i]);
                int x = b->getX();
                int y = b->getY();
                if(b->atGround()) {
                    return 0;  
                } 
                
        
                // if(board[x+1][y].full()) {
                //     fix();
                //     return 0; 
                // }   
            }  
            for (int i = 0; i < 4; i ++) {
                blocks[i].fall();  
            }
            return 1;
        }
         
        void fix() {
            for(int i = 0; i < 4; i++) {
                Playerboard * b = &(blocks[i]);
                int x = b->getX();
                int y = b->getY();
                board[x][y] = *b;
            }
            clearline();     
        }
         
        int conflicts() {
            for(int i = 0; i < 4; i++) {
                Playerboard * b = &(blocks[i]);
                int x = b->getX();
                int y = b->getY();
                if(board[x][y].full()) {
                    return 1;  
                }  
            }
            return 0;
        }  
        
    };
 
uint16_t getColor(Color c) {
    switch (c) {
        case Red:
            return matrix.Color333(7,0,0);
        case Orange:
            return matrix.Color333(7,1,0);
        case Yellow:
            return matrix.Color333(7,3,0);
        case Green:
            return matrix.Color333(0,7,0);
        case LBlue: 
            return matrix.Color333(0,7,7);
        case DBlue:
            return matrix.Color333(0,0,7);
        case Pink:
            return matrix.Color333(7,0,7);
    }  
    return NULL;
}
 
void delay(int x){
    for(int i=0;i<x;i++){
        wait_us(10);
        matrix.updateDisplay();
    }
}
 
/*lose function
freezes screen to show collision
flashes score
goes to lose screen (red square)*/
void lose(){
     
    //clear screen for lose screen
    delay(10000);
    matrix.fillRect(0, 0, 32, 32, matrix.Color333(0, 0, 0));
     
    matrix.fillRect(14,14, 4,4, matrix.Color333(3,0,0));//6 for 2nd position
    while(1) matrix.updateDisplay();
}
 
int main(){
    int score;
    Timer timer;
    timer.start();
    int fallTime = 100; //fall speed in milliseconds
    int pastTime = timer.read_ms();
    Color colors[7] = {Red, Orange, Yellow, Green, LBlue, DBlue, Pink};
    matrix.fillScreen(matrix.Color333(0,0,0));
    srand(time(0));
    Enemy e = Enemy(colors[rand()%7]);
    player p;
    e.draw();
    //p.shoot();

    missile m;
    m.drawm();
    m.up();
    while (1) {
        int currTime = timer.read_ms();
        if (currTime - pastTime > fallTime) {
            pastTime = currTime;
            // if(!e.fall()) {
            //     e = Enemy(colors[rand()%7]);
            //     if(e.conflicts()) {
            //         break;  
            //     }
            //     matrix.fillScreen(matrix.Color333(0,0,0));
                 
            // } else {
            //     matrix.fillScreen(matrix.Color333(0,0,0));
            //     e.draw();
            // } 
            // //draw board
            // for(int i = 0; i < 32; i ++) {
            //     for(int j = 0; j < 32; j ++) {
            //         //if(&(board[i][j]) != NULL) {
            //             board[i][j].draw();
            //         //}
            //     }  
            // }
        if(m.xcord()>=e.xcord()){  
         score++;
        e.disappear();
        }

            
            

        } else {
            delay(1);
        }
        for(int i = 0; i < 32; i ++) {
                for(int j = 0; j < 32; j ++) {
                    if(&(board[i][j]) != NULL) {
                        board[i][j].draw();
                    }
                }  
            }
        
        if(button == 0) {
            p.shift(-1); 
            delay(500);
            matrix.fillScreen(matrix.Color333(0,0,0));
            p.drawm();
            //draw board
            for(int i = 0; i < 32; i ++) {
                for(int j = 0; j < 32; j ++) {
                    //if(&(board[i][j]) != NULL) {
                        board[i][j].draw();
                    //}
                }  
            }
        }
        if(button2 == 0) {
            p.shift(1); 
            delay(500);
            matrix.fillScreen(matrix.Color333(0,0,0));
            p.drawm();
            //draw board
            for(int i = 0; i < 32; i ++) {
                for(int j = 0; j < 32; j ++) {
                    //if(&(board[i][j]) != NULL) {
                        board[i][j].draw();
                    //}
                }  
            }
        }
    }
    lose();
}