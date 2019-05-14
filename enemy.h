#ifndef ENEMY_H
#define ENEMY_H

#include "mbed.h"
#include "enemy.h"
#include "missile.h"
#include "globals.h"

//extern uLCD_4DGL uLCD;

// Enemy Status
typedef enum {
    ENEMY_DEAD = 0,
    ENEMY_ALIVE = 1
} ENEMY_STATUS;

/// This struct contains the status of a enemy
typedef struct
{
    int enemy_blk_x;   ///< horizontal position in the grid
    int enemy_blk_y;   ///< vertical position in the grid
    int enemy_old_blk_x; ///< old horizontal position in the grid
    int enemy_old_blk_y; ///< old vertical position in the grid
    int enemy_width;
    int enemy_height;
    int enemy_color;   ///< color of the enemy
    ENEMY_STATUS status;
} enemy_t;

void enemy_init(enemy_t * g, unsigned int blk_x, unsigned int blk_y, unsigned int color);
void enemy_show(enemy_t *g);
void enemy_erase(enemy_t *g);
int check_enemy(enemy_t *g, missile_t *h);
int move_enemy(enemy_t *g, int MOVE_DOWN, int DIRECTION);

#endif //ENEMY_H