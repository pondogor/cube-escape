/* 
 * Copyright (C) 2009  Sean McKean
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MAIN_HDR
#define MAIN_HDR

#include "types.h"
#include "states.h"
#include "draw.h"
#include "maze.h"


/* These defines control debugging characteristics of the game. */
/*#define DEBUG_DIAGNOSTICS*/
/*#define DEBUG_ORIENT_CORNERS*/
/*#define DEBUG_SHOW_MAP*/

#define SCREEN_W  480
#define SCREEN_H  480
#define SCREEN_FLAGS  SDL_SWSURFACE
#define TITLE  "Cube Escape"
#define OPTION_TEXT_DIST_MULT  1.5
#define TARGET_FPS  40

#define FONT_NAME  "data/LiberationMono-Regular.ttf"

#define SPRITE_SIZE  64
#define PLAYER_MOVE_TIME  200

#define MAX_PSG_LENGTH  12
#define NUM_POINTS  8
#define NUM_QUADS  6
#define EXIT_PALETTE_TIME  1200

#define MIN_FULL_ZOOM  50.0f
#define MAX_FULL_ZOOM  1500.0f
#define FULL_ZOOM_START  500.0f
#define FULL_ZOOM_SPEED  20.0f
#define PLAYER_ZOOM_SPEED_DIV  30.0f

#define MAZE_ROTATE_TIME  200

#define ROOM_COLOR  black
#define WALL_COLOR  white
#define HIDE_COLOR  dk_grey
#define MIN_EXIT_SHADE  63
#define UNHIDE_COLOR  0x00000000
#define UNHIDE_TIME  500
#define LEVEL_CHANGE_TIME  500

#define DEFAULT_EXIT_DISTANCE_CHOICE  2
#define DEFAULT_NUM_LEVELS  6
#define DEFAULT_START_LEVEL  1

#define MIN( x, y )  ((x) < (y) ? (x) : (y))
#define MAX( x, y )  ((x) > (y) ? (x) : (y))

#define LEVEL_SIZE( level )  ((level) > 0 ? (level) * 4 - 1 : 1)


/* Globals section */

SDL_Surface *Screen,
            *fade_sfc,
            *prev_sfc,
            *player_sfc,
            *exit_up_sfc,
            *exit_dn_sfc,
            *exit_final_sfc,
            **quid_sfcs,
            **quad_sfcs[NUM_QUADS],
            **hide_sfcs[NUM_QUADS];

SDL_Color exit_up_pal[SPRITE_SIZE],
          exit_dn_pal[SPRITE_SIZE],
          exit_final_pal[SPRITE_SIZE],
          exit_up_pal_cpy[SPRITE_SIZE],
          exit_dn_pal_cpy[SPRITE_SIZE],
          exit_final_pal_cpy[SPRITE_SIZE],
          text_fg,
          text_bg,
          hi_bg,
          no_hi_bg;

Player_t player;

SfcMazeRoom_t *exit_up_rooms,
              *exit_dn_rooms,
              exit_final_room;

Point_t restore_points[NUM_POINTS],
        points[NUM_POINTS],
        copy_points[NUM_POINTS],
        center;

SfcMazeConnect_t **connects[NUM_QUADS];

int maze_size,
    start_ticks,
    n_frames,
    tick_count,
    current_quad,
    exit_choice,
    total_num_levels,
    start_level,
    prev_num_levels,
    current_level,
    rotation,
    show_full_map,
    follow_player,
    playing;

Uint32 white,
       grey,
       dk_grey,
       black,
       red,
       green,
       blue,
       med_red,
       med_green,
       med_blue,
       dk_red,
       dk_green,
       dk_blue,
       faded_blue;

Sint16 quads[6][4];

int k_return,
    k_up,
    k_down,
    k_left,
    k_right,
    k_shift,
    k_ctrl,
    k_spc;

float player_zoom_speed,
      player_zoom,
      full_zoom,
      exit_mult;

Uint32 **unhide_starts[NUM_QUADS],
       rotation_start,
       fade_start,
       n_ticks;

int (*FunctionState)(),
    (*NextState)(),
    (*PreviousState)(),
    (*AffirmState)();

void *state_arg;

/* Function prototypes */

void InitGame( time_t seed );
void StartNewMaze( void );
void InitMazeStructure( int old_num_levels, int num_levels );
void ResetObjects( void );
int PickCurrentQuad( void );
void SetRestorePoints( void );
void RestorePoints( void );
SDL_Surface *NewSurface( int flags, SDL_PixelFormat *fmt,
                         Uint32 w, Uint32 h );
SDL_Surface *NewAlphaSurface( int flags, SDL_PixelFormat *fmt,
                              Uint32 w, Uint32 h );
void RotatePoints( float *orig_dim1, float *orig_dim2,
                   float *copy_dim1, float *copy_dim2,
                   float center_dim1, float center_dim2, float angle_delta,
                   int rotate_original );
int RotateCube( int traversing );
void RotatePalette( SDL_Surface *sfc, SDL_Color *palette, SDL_Color *copy,
                    int size, int io, Uint32 ticks );
void SetUnhideTicks( int quad, int x, int y, Uint32 ticks );
void SearchMaze( SfcMazeRoom_t *from );
void MovePlayer( int dir );
int PlayerOnExit( SfcMazeRoom_t *exit );
void Quit( int code );

#endif  /* MAIN_HDR */
