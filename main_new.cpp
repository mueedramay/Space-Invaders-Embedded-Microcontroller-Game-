#include "mbed.h"
//#include "wave_player.h"
#include "enemy.h"
#include "player.h"
#include "missile.h"
#include "globals.h"
//#include "rtos.h"
#include <string>
#include "RGBmatrixPanel.h"

/* ==== Navigation Switch ===== */
//class Nav_Switch
//{
//public:
//    Nav_Switch(PinName up,PinName down,PinName left,PinName right,PinName fire);
//    int read();
////boolean functions to test each switch
//    bool up();
//    bool down();
//    bool left();
//    bool right();
//    bool fire();
////automatic read on RHS
//    operator int ();
////index to any switch array style
//    bool operator[](int index) {
//        return _pins[index];
//    };
//private:
//    BusIn _pins;
// 
//};
//Nav_Switch::Nav_Switch (PinName up,PinName down,PinName left,PinName right,PinName fire):
//    _pins(up, down, left, right, fire)
//{
//    _pins.mode(PullUp); //needed if pullups not on board or a bare nav switch is used - delete otherwise
//    wait(0.001); //delays just a bit for pullups to pull inputs high
//}
//inline bool Nav_Switch::up()
//{
//    return !(_pins[0]);
//}
//inline bool Nav_Switch::down()
//{
//    return !(_pins[1]);
//}
//inline bool Nav_Switch::left()
//{
//    return !(_pins[2]);
//}
//inline bool Nav_Switch::right()
//{
//    return !(_pins[3]);
//}
//inline bool Nav_Switch::fire()
//{
//    return !(_pins[4]);
//}
//inline int Nav_Switch::read()
//{
//    return _pins.read();
//}
//inline Nav_Switch::operator int ()
//{
//    return _pins.read();
//}

// Platform initialization
//uLCD_4DGL uLCD(p28,p27,p29); // LCD (serial tx, serial rx, reset pin;)
PinName ub1=D4;
PinName ug1=D3;
PinName ur1=D2;
PinName lb2=D7;
PinName lg2=D6;
PinName lr2=D5;
RGBmatrixPanel matrix(ur1, lr2,ug1,lg2,ub1,lb2,A0, A1, A2, A3, D8,D10, D9, false);
//AnalogOut DACout(p18);      // speaker
//wave_player waver(&DACout); // wav player
//SDFileSystem sd(p5, p6, p7, p8, "sd"); // SD card and filesystem (mosi, miso, sck, cs)
//Nav_Switch myNav(p13, p10, p11, p9, p12); //pin order on Sparkfun breakout
DigitalIn button(SW2); // push button for player misisle fire
DigitalIn button2(SW3);
// Initialize all global enemy objects
enemy_t enemy_1;
enemy_t enemy_2;
enemy_t enemy_3;
enemy_t enemy_4;
enemy_t enemy_5;
enemy_t enemy_6;
enemy_t enemy_7;
enemy_t enemy_8;
enemy_t enemy_9;
enemy_t enemy_10;
enemy_t enemy_11;
enemy_t enemy_12;
enemy_t enemy_13;
enemy_t enemy_14;
enemy_t enemy_15;

// Initialize variables
int numOfEnemies = 0;
int ENEMY_MOVE = 1; 
int MOVE_DOWN = 0;
int DIRECTION = 1;
int firing_col = 0;
int hit_player = 0;
bool lose = false;
int lives = 3;
bool game_menu = false;
bool begin_game = false;
bool gameover = false;

// Initialize global player object
player_t player;

// Intialize global player and enemy missile
missile_t missile; // player missile
missile_t enemy_missile; // enemy missile

// Array of enemy objects
enemy_t * enemyArray[15];

// Function Prototypes
void move_enemy_down();
void playstart(void const *args);

// Draws the enemies at the initial starting location    
void draw_enemies_level()
{
    // Initialize local variables
    unsigned int start_x_pos = 12;
    unsigned int start_enemy_y_pos = 20; 
    numOfEnemies = 15;
    
    // First Row of Enemies
    enemy_init(&enemy_1,start_x_pos,start_enemy_y_pos,WHITE); // initialize x-pos and y-pos and color of enemy
    enemy_show(&enemy_1); // displays the enemy on uLCD
    
    enemy_init(&enemy_2,start_x_pos+15,start_enemy_y_pos,WHITE);
    enemy_show(&enemy_2);
    
    enemy_init(&enemy_3,start_x_pos+30,start_enemy_y_pos,WHITE);
    enemy_show(&enemy_3);
    
    enemy_init(&enemy_4,start_x_pos+45,start_enemy_y_pos,WHITE);
    enemy_show(&enemy_4);
    
    enemy_init(&enemy_5,start_x_pos+60,start_enemy_y_pos,WHITE);
    enemy_show(&enemy_5);
    
    // Second Row of Enemies
    enemy_init(&enemy_6,start_x_pos,start_enemy_y_pos+12,WHITE);
    enemy_show(&enemy_6);
    
    enemy_init(&enemy_7,start_x_pos+15,start_enemy_y_pos+12,WHITE);
    enemy_show(&enemy_7);
    
    enemy_init(&enemy_8,start_x_pos+30,start_enemy_y_pos+12,WHITE);
    enemy_show(&enemy_8);
    
    enemy_init(&enemy_9,start_x_pos+45,start_enemy_y_pos+12,WHITE);
    enemy_show(&enemy_9);
    
    enemy_init(&enemy_10,start_x_pos+60,start_enemy_y_pos+12,WHITE);
    enemy_show(&enemy_10);
    
    // Third Row of Enemies
    enemy_init(&enemy_11,start_x_pos,start_enemy_y_pos+24,WHITE);
    enemy_show(&enemy_11);
    
    enemy_init(&enemy_12,start_x_pos+15,start_enemy_y_pos+24,WHITE);
    enemy_show(&enemy_12);
    
    enemy_init(&enemy_13,start_x_pos+30,start_enemy_y_pos+24,WHITE);
    enemy_show(&enemy_13);
    
    enemy_init(&enemy_14,start_x_pos+45,start_enemy_y_pos+24,WHITE);
    enemy_show(&enemy_14);
    
    enemy_init(&enemy_15,start_x_pos+60,start_enemy_y_pos+24,WHITE);
    enemy_show(&enemy_15);
    
    // Put enemy objects into array
    enemyArray[0] = &enemy_1;
    enemyArray[1] = &enemy_2;
    enemyArray[2] = &enemy_3;
    enemyArray[3] = &enemy_4;
    enemyArray[4] = &enemy_5;
    enemyArray[5] = &enemy_6;
    enemyArray[6] = &enemy_7;
    enemyArray[7] = &enemy_8;
    enemyArray[8] = &enemy_9;
    enemyArray[9] = &enemy_10;
    enemyArray[10] = &enemy_11;
    enemyArray[11] = &enemy_12;
    enemyArray[12] = &enemy_13;
    enemyArray[13] = &enemy_14;
    enemyArray[14] = &enemy_15;
}

// Draws the player at the initial starting location
void draw_initial_player()
{
    int start_x_pos = 59;
    int start_y_pos = 110;
    
    player_init(&player,start_x_pos,start_y_pos,WHITE); // intialize x-pos and y-pos and color of player
    player_show(&player); // display player on uLCD
}

// Checks all enemies in row 1
void check_hit_enemy_row1()
{
    int hit_enemy;
    
    // First Row of Enemies
    hit_enemy = check_enemy(&enemy_1, &missile); // checks if the missile hits the enemy and returns 1 if hit, 0 if not hit
    numOfEnemies = numOfEnemies - hit_enemy; // updates the number of enemies left
    hit_enemy = check_enemy(&enemy_2, &missile);
    numOfEnemies = numOfEnemies - hit_enemy;
    hit_enemy = check_enemy(&enemy_3, &missile);
    numOfEnemies = numOfEnemies - hit_enemy;
    hit_enemy = check_enemy(&enemy_4, &missile);
    numOfEnemies = numOfEnemies - hit_enemy;
    hit_enemy = check_enemy(&enemy_5, &missile);
    numOfEnemies = numOfEnemies - hit_enemy;
}

// Same as check_hit_enemy_row1, but checks enemies at row 2
void check_hit_enemy_row2()
{
    int hit_enemy;
    
    // Second Row of Enemies
    hit_enemy = check_enemy(&enemy_6, &missile);
    numOfEnemies = numOfEnemies - hit_enemy;
    hit_enemy = check_enemy(&enemy_7, &missile);
    numOfEnemies = numOfEnemies - hit_enemy;
    hit_enemy = check_enemy(&enemy_8, &missile);
    numOfEnemies = numOfEnemies - hit_enemy;
    hit_enemy = check_enemy(&enemy_9, &missile);
    numOfEnemies = numOfEnemies - hit_enemy;
    hit_enemy = check_enemy(&enemy_10, &missile);
    numOfEnemies = numOfEnemies - hit_enemy;
}

// Same as check_hit_enemy_row1, but checks enemies at row 3
void check_hit_enemy_row3()
{
    int hit_enemy;

    // Third Row of Enemies
    hit_enemy = check_enemy(&enemy_11, &missile);
    numOfEnemies = numOfEnemies - hit_enemy;
    hit_enemy = check_enemy(&enemy_12, &missile);
    numOfEnemies = numOfEnemies - hit_enemy;
    hit_enemy = check_enemy(&enemy_13, &missile);
    numOfEnemies = numOfEnemies - hit_enemy;
    hit_enemy = check_enemy(&enemy_14, &missile);
    numOfEnemies = numOfEnemies - hit_enemy;
    hit_enemy = check_enemy(&enemy_15, &missile);
    numOfEnemies = numOfEnemies - hit_enemy;
}

// Checks if player is hit
void check_player_hit()
{    
    hit_player = check_player(&player, &enemy_missile); // checks if the missile hits the player and returns 1 if hit, 0 if not hit
}

// Randomly selects an enemy to fire and updates the position of where the missile will fire from
void random_attack_gen()
{
    firing_col = rand() % 5; // selects a random column
    
    // first checks if the 3rd row closest to the player is alive
    if (enemyArray[firing_col+10]->status == ENEMY_ALIVE)
    {
        // If alive, the enemy missile position is updated to the center of the enemy
        enemy_missile.missile_blk_x = enemyArray[firing_col+10]->enemy_blk_x + (enemyArray[firing_col+10]->enemy_width/2);
        enemy_missile.missile_blk_y = enemyArray[firing_col+10]->enemy_blk_y + enemyArray[firing_col+10]->enemy_height + 1;
        enemy_missile.status = ENEMY_MISSILE_ACTIVE; // sets the enemy missile as active
    }
    // if enemy at 3rd row is dead, checks the enemy in the 2nd row
    else if (enemyArray[firing_col+5]->status == ENEMY_ALIVE)
    {
        // If alive, the enemy missile position is updated to the center of the enemy
        enemy_missile.missile_blk_x = enemyArray[firing_col+5]->enemy_blk_x + (enemyArray[firing_col+5]->enemy_width/2);
        enemy_missile.missile_blk_y = enemyArray[firing_col+5]->enemy_blk_y + enemyArray[firing_col+5]->enemy_height + 1;
        enemy_missile.status = ENEMY_MISSILE_ACTIVE;
    }
    // if enemy at 2nd and 3rd row is dead, checks the enemy in the 1st row
    else if (enemyArray[firing_col]->status == ENEMY_ALIVE)
    {
        // If alive, the enemy missile position is updated to the center of the enemy
        enemy_missile.missile_blk_x = enemyArray[firing_col]->enemy_blk_x + (enemyArray[firing_col]->enemy_width/2);
        enemy_missile.missile_blk_y = enemyArray[firing_col]->enemy_blk_y + enemyArray[firing_col]->enemy_height + 1;
        enemy_missile.status = ENEMY_MISSILE_ACTIVE;
    }
}

// moves the enemy
void enemy_motion()
{
    // will move the enemy every 6 loops
    if (ENEMY_MOVE % 6 == 0)
    {
        // FIrst Row of Enemies
        MOVE_DOWN = move_enemy(&enemy_1, MOVE_DOWN, DIRECTION); // moves the enemy based on the DIRECTION passed in and also sends global MOVE_DOWN to update in enemy.cpp
        MOVE_DOWN = move_enemy(&enemy_2, MOVE_DOWN, DIRECTION); // MOVE_DOWN will be 1 enemies reach the left or right edge of the screen
        MOVE_DOWN = move_enemy(&enemy_3, MOVE_DOWN, DIRECTION); // MOVE_DOWN will be 2 if enemies reach the player, otherwise 0
        MOVE_DOWN = move_enemy(&enemy_4, MOVE_DOWN, DIRECTION);
        MOVE_DOWN = move_enemy(&enemy_5, MOVE_DOWN, DIRECTION); 
        
        // Second Row of Enemies
        MOVE_DOWN = move_enemy(&enemy_6, MOVE_DOWN, DIRECTION);
        MOVE_DOWN = move_enemy(&enemy_7, MOVE_DOWN, DIRECTION);
        MOVE_DOWN = move_enemy(&enemy_8, MOVE_DOWN, DIRECTION);
        MOVE_DOWN = move_enemy(&enemy_9, MOVE_DOWN, DIRECTION);
        MOVE_DOWN = move_enemy(&enemy_10, MOVE_DOWN, DIRECTION);
        
        // Third Row of Enemies
        MOVE_DOWN = move_enemy(&enemy_11, MOVE_DOWN, DIRECTION);
        MOVE_DOWN = move_enemy(&enemy_12, MOVE_DOWN, DIRECTION);
        MOVE_DOWN = move_enemy(&enemy_13, MOVE_DOWN, DIRECTION);
        MOVE_DOWN = move_enemy(&enemy_14, MOVE_DOWN, DIRECTION);
        MOVE_DOWN = move_enemy(&enemy_15, MOVE_DOWN, DIRECTION);
        
        // if MOVE_DOWN is 2, then the enemies have reached the player
        if (MOVE_DOWN == 2)
        {
            lose = true; // set global lose to true to go to gameover screen
        }
        
        // if MOVE_DOWN is 1, update the y-pos of the enemies
        if (MOVE_DOWN == 1)
        {
            move_enemy_down(); // updates the y-pos of the enemies
            
            // Flips the direction when the enemy moves down
            if (DIRECTION == 1)
            {
                DIRECTION = 2;
            }
            else
            {
                DIRECTION = 1;
            }
            MOVE_DOWN = 0; // sets MOVE_DOWN back to 0 to stop down movement until 
        }  
        
        ENEMY_MOVE += 1;
    }
    else
    {
        ENEMY_MOVE += 1;
    }
}

// moves all the enemies down in the y-direction
void move_enemy_down()
{
    // First Row of Enemies
    enemy_erase(&enemy_1);
    enemy_erase(&enemy_2);
    enemy_erase(&enemy_3);
    enemy_erase(&enemy_4);
    enemy_erase(&enemy_5);
    
    enemy_1.enemy_blk_y += enemy_1.enemy_height+4;
    enemy_2.enemy_blk_y += enemy_2.enemy_height+4;
    enemy_3.enemy_blk_y += enemy_3.enemy_height+4;
    enemy_4.enemy_blk_y += enemy_4.enemy_height+4;
    enemy_5.enemy_blk_y += enemy_5.enemy_height+4;
    
    // Second Row of Enemies
    enemy_erase(&enemy_6);
    enemy_erase(&enemy_7);
    enemy_erase(&enemy_8);
    enemy_erase(&enemy_9);
    enemy_erase(&enemy_10);
    
    enemy_6.enemy_blk_y += enemy_6.enemy_height+4;
    enemy_7.enemy_blk_y += enemy_7.enemy_height+4;
    enemy_8.enemy_blk_y += enemy_8.enemy_height+4;
    enemy_9.enemy_blk_y += enemy_9.enemy_height+4;
    enemy_10.enemy_blk_y += enemy_10.enemy_height+4;
    
    // Third Row of Enemies
    enemy_erase(&enemy_11);
    enemy_erase(&enemy_12);
    enemy_erase(&enemy_13);
    enemy_erase(&enemy_14);
    enemy_erase(&enemy_15);
    
    enemy_11.enemy_blk_y += enemy_11.enemy_height+4;
    enemy_12.enemy_blk_y += enemy_12.enemy_height+4;
    enemy_13.enemy_blk_y += enemy_13.enemy_height+4;
    enemy_14.enemy_blk_y += enemy_14.enemy_height+4;
    enemy_15.enemy_blk_y += enemy_15.enemy_height+4;
}

// thread that plays sounds during game
// void playstart(void const *args)//Th
// {   //Depending on the state of the game,
//     //queue either screen music or play sound effect upon fire
//     // while(true) {
//     //     //FILE *wave_file;
        
//     //     // plays intro music during menu screen
//     //     // while(game_menu)
//     //     // {
//     //     //     wave_file=fopen("/sd/wavfiles/spacey_sound.wav","r");
                
//     //     //     waver.play(wave_file);
//     //     //     fclose(wave_file);
//     //     // }
        
//     //     // Checks in game sound conditions
//     //     while(begin_game) 
//     //     {
//     //         // play firing sound when the player fires
//     //         // if(!pb && missile.status == PLAYER_MISSILE_INACTIVE) {
 
//     //         //     wave_file=fopen("/sd/wavfiles/4.wav","r");
 
 
//     //         //     waver.play(wave_file);
//     //         //     fclose(wave_file);
//     //         // }
            
//     //         // if player hit, play hit sound
//     //         // if (hit_player)
//     //         // {
//     //         //     wave_file=fopen("/sd/wavfiles/1.wav","r");
                
//     //         //     waver.play(wave_file);
//     //         //     fclose(wave_file);
//     //         // }
//     //     }
        
//         // players gameover voice if player loses
//         // while(gameover)
//         // {
//         //     wave_file=fopen("/sd/wavfiles/game_over.wav","r");
            
//         //     waver.play(wave_file);
//         //     fclose(wave_file);
//         // }
//    // }
// }


void delay(int x){
    for(int i=0;i<x;i++){
        wait_us(10);
        matrix.updateDisplay();
    }
}

int main() {
     
     // Initialization of Setup variables
     int blk_x, blk_y;
     //pb.mode(PullUp);
     
     //Thread thread(playstart); // intializes the thread to play sound
     
     //uLCD.baudrate(500000); // set to 500000 to increase smooth gameplay
     delay (10000);
     // Initialization of Game Menu variables
    const int title_x_pos = 2; // initial x-pos of title
    const int title_y_pos = 3; // initial y-pos of title
    int start_label_x_pos = 7; // start label x-pos
    int start_label_y_pos = 7; // start label y-pos
    int level_cursor_x_pos = 5; // level cursor x-position
    int level_cursor_y_pos = 7; // level cursor y-position
    int gameover_x_pos = 5; // gameover label x-position
    int gameover_y_pos = 5; // gameover label y-position
    int win_x_pos = 2; // congratulations label x-position
    int win_y_pos = 5; // congratulations label y-position
    int startover_x_pos = 3; // startover label x-position
    int startover_y_pos = 8; // startover label y-position
    
    // intialize temporary score and current score
    int temp = 0;
    int score = 0;
     
    // Begin game loop
    while(1)
    {
        // initialize all starting conditions for the beginning of the game
        game_menu = true; // defaults to display menu screen
        ENEMY_MOVE = true; // defaults to move enemy
        DIRECTION = 1; // default to move right
        hit_player = 0; // default to not player hit
        MOVE_DOWN = 0; // default to not move down
        lose = false; // default to not lose
        lives = 3; // defaults to 3 lives
        score = 0; // default to score of 0

        //uLCD.cls();
        
        // Implementation of Game Menu
        //while(game_menu) 
//        {
//            uLCD.locate(level_cursor_x_pos,level_cursor_y_pos); // draws cursor next to "START" label
//            uLCD.printf("->");
//            
//            uLCD.locate(title_x_pos,title_y_pos); // "SPACE INVADERS" title position
//            uLCD.printf("SPACE INVADERS"); // Title
//            
//            uLCD.locate(start_label_x_pos,start_label_y_pos); // "START" label position
//            uLCD.printf("START");
//
//            // if pushbutton is pressed, game menu is exited and game begins
//            if(!pb) 
//            { 
//                game_menu = false;
//                wait(0.5);
//            }
//        }
        
        begin_game = true; // defaults begin_game to true
        
        //uLCD.cls();
        matrix.fillScreen(BACKGROUND_COLOR);

        // Draw the enemies
        draw_enemies_level();

        // Draw the player
        draw_initial_player();
        
        // sets the initial position of player missile and enemy missile (later updated)
        blk_x = player.player_blk_x+(player.player_width/2);
        blk_y = player.player_blk_y;
        missile_init(&missile, blk_x, blk_y, WHITE);
        int e_blk_x = 0;
        int e_blk_y = 2;
        enemy_missile_init(&enemy_missile, e_blk_x, e_blk_y, WHITE);
        
        // prints lives
        matrix.setCursor(0,0);
        //uLCD.printf("Lives:%i", lives);
        matrix.drawChar(1,0,(char)lives,matrix.Color333(2, 2, 2), matrix.Color333(0, 0, 0), 1);
        // prints score
        matrix.setCursor(9,0);
         matrix.drawChar(1,0,(char)score,matrix.Color333(2, 2, 2), matrix.Color333(0, 0, 0), 1);
        
        // game play loop
        while(begin_game) 
        {
            // updates score
            temp = score;
            score = (15-numOfEnemies)*15;
            
            // prints score if score changes
            if (score != temp)
            {
                matrix.setCursor(9,0);
         matrix.drawChar(1,0,(char)score,matrix.Color333(2, 2, 2), matrix.Color333(0, 0, 0), 1);
            }
            
            // move enemy
            enemy_motion();

            // checks if player missile passes y-pos of row1
            if (missile.missile_blk_y+1-missile.missile_height <= enemy_1.enemy_blk_y
                    && missile.missile_blk_y+1-missile.missile_height >= enemy_1.enemy_blk_y-enemy_1.enemy_height) 
            {
                check_hit_enemy_row1();
            }

            // checks if player missile passes y-pos of row2
            if (missile.missile_blk_y+1-missile.missile_height <= enemy_6.enemy_blk_y
                    && missile.missile_blk_y+1-missile.missile_height >= enemy_6.enemy_blk_y-enemy_6.enemy_height) 
            {
                check_hit_enemy_row2();
            }

            // checks if player missile passes y-pos of row3
            if (missile.missile_blk_y+1-missile.missile_height <= enemy_11.enemy_blk_y
                    && missile.missile_blk_y+1-missile.missile_height >= enemy_11.enemy_blk_y-enemy_11.enemy_height) 
            {
                check_hit_enemy_row3();
            }
            
            // Random Enemy Fire
            if (enemy_missile.status == ENEMY_MISSILE_INACTIVE) 
            {
                random_attack_gen();
            }
            
            // checks if enemy missile passes y-pos of player
            if (enemy_missile.missile_blk_y >= player.player_blk_y
                    && enemy_missile.missile_blk_y <= player.player_blk_y+player.player_height)
            {
                check_player_hit();
            }

            update_missile_pos(&missile); // updates player missile position
            update_enemy_missile_pos(&enemy_missile); // updates enemy missile position

            // Player Movement checked with navigation switch
            if (button==0 && ((player.player_blk_x-3) > 0)) 
            {
                player_erase(&player);
                player.player_blk_x -= 3;
                player_show(&player);
            } 
            else if (button2==0 && ((player.player_blk_x+3) < (128-player.player_width))) 
            {
                player_erase(&player);
                player.player_blk_x += 3;
                player_show(&player);
            }

            // Player Fire
            if ( missile.status == PLAYER_MISSILE_INACTIVE) 
            {
                missile.missile_blk_x = player.player_blk_x+(player.player_width/2);
                missile.missile_blk_y = player.player_blk_y;
                missile.status = PLAYER_MISSILE_ACTIVE;
            }
            
            // checks if player destroyed all enemies
            if (numOfEnemies == 0)
            {
                matrix.fillScreen(matrix.Color333(0,0,0));
                
                bool win = true; // sets win to true, for win screen
                begin_game = false;
                
                // displays video clip
                // uLCD.cls();
                // uLCD.media_init();
                // uLCD.set_sector_address(0x00, 0x00);
                // uLCD.display_video(0,0);
                // wait(1);
                
                // uLCD.cls();
                
                // prints "Congratulations" on uLCD    
                matrix.setCursor(win_x_pos,win_y_pos);
                //uLCD.printf("CONGRATULATIONS!");
                 matrix.drawChar(7, 0, 'w', matrix.Color333(2, 2, 2), matrix.Color333(0, 0, 0), 1);
    matrix.drawChar(13,0, 'o', matrix.Color333(2, 2, 2), matrix.Color333(0, 0, 0), 1);
    matrix.drawChar(19, 0, 'n', matrix.Color333(2, 2, 2), matrix.Color333(0, 0, 0), 1);
                
                // prints "Play Again?" and "Press pb..."
                // uLCD.locate(startover_x_pos, startover_y_pos);
                // uLCD.printf("Play Again?");
                // uLCD.locate(startover_x_pos, startover_y_pos+1);
                // uLCD.printf("Press pb...");
                
                // waits at win screen until pushbutton is pressed
                // while (win)
                // {
                //     // if pb is pressed, reset game to start menu
                //     if (!pb)
                //     {
                //         win = false;
                //         wait(0.5);
                //     }
                // }
                
            }
            
            // checks if player was hit
            if (hit_player)
            {
                // updates lives
                lives -= 1;
                wait(0.5);
                hit_player = 0;
                player_show(&player);
                player.status = PLAYER_ALIVE;
                
                // prints updated lives number
               matrix.setCursor(0,0);
        //uLCD.printf("Lives:%i", lives);
        matrix.drawChar(1,0,(char)lives,matrix.Color333(2, 2, 2), matrix.Color333(0, 0, 0), 1);
            }   
            
            // if player loses all lives or enemy reaches the player
            if (lose || lives == 0)
            {    
                begin_game = false; // set to false to end game
                matrix.fillScreen(matrix.Color333(0,0,0));
                
                
                gameover = true; // set to go to display gameover screen
                
                // prints "GAMEOVER" to uLCD
                matrix.setCursor(gameover_x_pos, gameover_y_pos);
                 matrix.drawChar(7, 0, 'l', matrix.Color333(2, 2, 2), matrix.Color333(0, 0, 0), 1);
             matrix.drawChar(13,0, 'o', matrix.Color333(2, 2, 2), matrix.Color333(0, 0, 0), 1);
        matrix.drawChar(19, 0, 's', matrix.Color333(2, 2, 2), matrix.Color333(0, 0, 0), 1);
        matrix.drawChar(25, 0, 't', matrix.Color333(2, 2, 2), matrix.Color333(0, 0, 0), 1);
                wait(1);
                
                // prints "Play Again?" and "Press pb..."
                // uLCD.locate(startover_x_pos, startover_y_pos);
                // uLCD.printf("Play Again?");
                // uLCD.locate(startover_x_pos, startover_y_pos+1);
                // uLCD.printf("Press pb...");
                
                // stays in gameover screen until pb is pressed
                // while (gameover)
                // {
                //     // if pb is pressed, game is reset to the game menu screen
                //     if (!pb)
                //     {
                //         gameover = false;
                //         game_menu = true;
                //         wait(0.5);
                //     }
                // }
            }

        }
    }
}