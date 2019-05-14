#include "player.h"
#include "missile.h"
#include "mbed.h"
#include "RGBmatrixPanel.h"

PinName ub1p=D4;
PinName ug1p=D3;
PinName ur1p=D2;
PinName lb2p=D7;
PinName lg2p=D6;
PinName lr2p=D5;
RGBmatrixPanel matrixp(ur1p, lr2p,ug1p,lg2p,ub1p,lb2p,A0, A1, A2, A3, D8,D10, D9, false);

void draw_player_object(int blk_x, int blk_y, int player_color, int p_width, int p_height)
{
    matrixp.fillRect(blk_x,blk_y,blk_x+p_width,blk_y+p_height,player_color);
}

void erase_player(int blk_x, int blk_y, int p_width, int p_height)
{
    matrixp.fillRect(blk_x,blk_y,blk_x+p_width,blk_y+p_height,BACKGROUND_COLOR);
}

void player_init(player_t * g, int blk_x, int blk_y, int color)
{
    g->player_blk_x = blk_x;
    g->player_blk_y = blk_y;
    g->player_color = 0x00FF00;
    g->player_height = 8;
    g->player_width = 8;
    g->status = PLAYER_ALIVE;
}

void player_show(player_t * g)
{
    draw_player_object(g->player_blk_x, g->player_blk_y, g->player_color, g->player_width, g->player_height);
}

void player_erase(player_t *g)
{
    erase_player(g->player_blk_x, g->player_blk_y, g->player_width, g->player_height);
}

int check_player(player_t * g, missile_t * h)
{
    int player_died = 0;
    if (g->status == PLAYER_ALIVE 
    && ((h->missile_blk_x >= g->player_blk_x) && (h->missile_blk_x <= (g->player_blk_x + g->player_width)))) 
    {
        player_erase(g);
        g->status = PLAYER_DEAD;
        player_died = 1;
        h->status = ENEMY_MISSILE_EXPLODED;                         
    }  
    
    return player_died;
}