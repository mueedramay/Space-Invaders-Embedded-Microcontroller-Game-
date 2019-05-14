#ifndef PLAYER_H
#define PLAYER_H

#include "mbed.h"
#include "player.h"
#include "missile.h"
#include "globals.h"

//extern uLCD_4DGL uLCD;

typedef enum {
    PLAYER_DEAD = 0,
    PLAYER_ALIVE = 1
} PLAYER_STATUS;

/// This struct contains the status of a player
typedef struct
{
    int player_blk_x;   ///< horizontal position in the grid
    int player_blk_y;   ///< vertical position in the grid
    unsigned int player_old_blk_x; ///< old horizontal position in the grid
    unsigned int player_old_blk_y; ///< old vertical position in the grid
    int player_width;
    int player_height;
    int player_color;   ///< color of the player  
    PLAYER_STATUS status;
} player_t;

void player_init(player_t * g, int blk_x, int blk_y, int color);
void player_show(player_t *g);
void player_erase(player_t *g);
int check_player(player_t *g, missile_t *h);
#endif //PLAYER_H