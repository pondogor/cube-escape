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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "SDL.h"
#include "SDL_ttf.h"

#include "main.h"


const float hf_pi = M_PI / 2.0f;


void InitGame( time_t seed )
{
    SDL_PixelFormat *fmt = NULL;
    SDL_Rect rect;
    SDL_Event evt;
    int quad = 0,
        x = 0;

    srand(seed);

    putenv("SDL_VIDEO_WINDOW_POS=center");
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        Quit(1);
    }
    Screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SCREEN_FLAGS);
    if (Screen == NULL)
    {
        fprintf( stderr, "Error initializing framebuffer: %s\n",
                 SDL_GetError() );
        Quit(1);
    }
    fmt = Screen->format;
    SDL_WM_SetCaption(TITLE, TITLE);

    if (TTF_Init())
    {
        fprintf( stderr, "Error initializing framebuffer: %s\n",
                 TTF_GetError() );
        Quit(3);
    }

    white = SDL_MapRGB(fmt, 255, 255, 255);
    grey = SDL_MapRGB(fmt, 127, 127, 127);
    dk_grey = SDL_MapRGB(fmt, 63, 63, 63);
    black = SDL_MapRGB(fmt, 0, 0, 0);
    red = SDL_MapRGB(fmt, 255, 0, 0);
    green = SDL_MapRGB(fmt, 0, 255, 0);
    blue = SDL_MapRGB(fmt, 0, 0, 255);
    med_red = SDL_MapRGB(fmt, 127, 0, 0);
    med_green = SDL_MapRGB(fmt, 0, 127, 0);
    med_blue = SDL_MapRGB(fmt, 0, 0, 127);
    dk_red = SDL_MapRGB(fmt, 63, 0, 0);
    dk_green = SDL_MapRGB(fmt, 0, 63, 0);
    dk_blue = SDL_MapRGB(fmt, 0, 0, 63);
    faded_blue = SDL_MapRGB(fmt, 31, 31, 63);

    SDL_GetRGB(white, fmt, &def_text_fg.r, &def_text_fg.g, &def_text_fg.b);
    SDL_GetRGB(black, fmt, &def_text_bg.r, &def_text_bg.g, &def_text_bg.b);
    SDL_GetRGB(faded_blue, fmt, &hi_bg.r, &hi_bg.g, &hi_bg.b);
    SDL_GetRGB(black, fmt, &no_hi_bg.r, &no_hi_bg.g, &no_hi_bg.b);

    player_sfc = NewSurface(SDL_SWSURFACE, fmt, SPRITE_SIZE, SPRITE_SIZE);
    DrawFilledGradientCircle( player_sfc, SPRITE_SIZE, SPRITE_SIZE / 2,
                              SPRITE_SIZE / 2, green, dk_green );

    exit_up_sfc = SDL_CreateRGBSurface( SDL_SWSURFACE,
                                        SPRITE_SIZE, SPRITE_SIZE, 8,
                                        0, 0, 0, 0 );
    exit_dn_sfc = SDL_CreateRGBSurface( SDL_SWSURFACE,
                                        SPRITE_SIZE, SPRITE_SIZE, 8,
                                        0, 0, 0, 0 );
    exit_final_sfc = SDL_CreateRGBSurface( SDL_SWSURFACE,
                                           SPRITE_SIZE, SPRITE_SIZE, 8,
                                           0, 0, 0, 0 );
    for (x = 0; x < SPRITE_SIZE / 2; ++x)
    {
        rect.x = rect.y = x;
        rect.w = rect.h = SPRITE_SIZE - x * 2;
        SDL_FillRect(exit_up_sfc, &rect, x);
        SDL_FillRect(exit_dn_sfc, &rect, x);
        SDL_FillRect(exit_final_sfc, &rect, x);
    }
    for (x = 0; x < SPRITE_SIZE; ++x)
    {
        if (x < SPRITE_SIZE / 2)
        {
            exit_up_pal[x].r = 64 * 2 * x / SPRITE_SIZE;
            exit_up_pal[x].g = 256 * 2 * x / SPRITE_SIZE;
            exit_up_pal[x].b = 64 * 2 * x / SPRITE_SIZE;
        }
        else
        {
            exit_up_pal[x].r = 127 - 64 * 2 * x / SPRITE_SIZE;
            exit_up_pal[x].g = 255 - 256 * 2 * x / SPRITE_SIZE;
            exit_up_pal[x].b = 127 - 64 * 2 * x / SPRITE_SIZE;
        }

        if (x < SPRITE_SIZE / 2)
        {
            exit_dn_pal[x].r = 64 * 2 * x / SPRITE_SIZE;
            exit_dn_pal[x].g = 64 * 2 * x / SPRITE_SIZE;
            exit_dn_pal[x].b = 256 * 2 * x / SPRITE_SIZE;
        }
        else
        {
            exit_dn_pal[x].r = 127 - 64 * 2 * x / SPRITE_SIZE;
            exit_dn_pal[x].g = 127 - 64 * 2 * x / SPRITE_SIZE;
            exit_dn_pal[x].b = 255 - 256 * 2 * x / SPRITE_SIZE;
        }

        if (x < SPRITE_SIZE / 2)
        {
            exit_final_pal[x].r = 256 * 2 * x / SPRITE_SIZE;
            exit_final_pal[x].g = 64 * 2 * x / SPRITE_SIZE;
            exit_final_pal[x].b = 64 * 2 * x / SPRITE_SIZE;
        }
        else
        {
            exit_final_pal[x].r = 255 - 256 * 2 * x / SPRITE_SIZE;
            exit_final_pal[x].g = 127 - 64 * 2 * x / SPRITE_SIZE;
            exit_final_pal[x].b = 127 - 64 * 2 * x / SPRITE_SIZE;
        }
    }
    SDL_SetColors(exit_up_sfc, exit_up_pal, 0, SPRITE_SIZE);
    SDL_SetColors(exit_dn_sfc, exit_dn_pal, 0, SPRITE_SIZE);
    SDL_SetColors(exit_final_sfc, exit_final_pal, 0, SPRITE_SIZE);

    fade_sfc = SDL_DisplayFormat(Screen);
    SDL_FillRect(fade_sfc, NULL, black);
    SDL_SetAlpha(fade_sfc, SDL_SRCALPHA, 0);

    prev_sfc = SDL_DisplayFormat(Screen);
    SDL_FillRect(prev_sfc, NULL, black);

    show_full_map = 0;

    k_return = 0;
    k_up = 0;
    k_down = 0;
    k_left = 0;
    k_right = 0;
    k_shift = 0;
    k_ctrl = 0;
    k_spc = 0;

    n_frames = 0;
    total_ticks = 0;

    playing = 0;
    total_num_levels = prev_num_levels = 0;

    quid_sfcs = NULL;
    for (quad = 0; quad < NUM_QUADS; ++quad)
    {
        quad_sfcs[quad] = NULL;
        hide_sfcs[quad] = NULL;
        unhide_starts[quad] = NULL;
        connects[quad] = NULL;
    }

    fade_start = 0;
    current_quad = 0;

    exit_choice = DEFAULT_EXIT_DISTANCE_CHOICE;
    total_num_levels = DEFAULT_NUM_LEVELS;
    start_level = DEFAULT_START_LEVEL;
    fast_graphics = DEFAULT_GRAPHICS_CHOICE;
}


void StartNewMaze()
{
    InitMazeStructure(prev_num_levels, total_num_levels);
    prev_num_levels = total_num_levels;
    ResetObjects();
    NextState = GameLoop;
}


void InitMazeStructure( int old_num_levels, int num_levels )
{
    SfcMazeRoom_t start;
    int num_connects = 0,
        level = 0,
        quad = 0,
        size = 0,
        i = 0,
        x = 0,
        y = 0;

    if (old_num_levels > 0)
    {
        for (level = 0; level < old_num_levels; ++level)
        {
            if (quid_sfcs)
                SDL_FreeSurface(quid_sfcs[level]);
            for (quad = 0; quad < NUM_QUADS; ++quad)
            {
                if (quad_sfcs[quad])
                    SDL_FreeSurface(quad_sfcs[quad][level]);
                if (hide_sfcs[quad])
                    SDL_FreeSurface(hide_sfcs[quad][level]);
                if (unhide_starts[quad])
                    free(unhide_starts[quad][level]);
                if (connects[quad])
                    free(connects[quad][level]);
            }
        }
        if (quid_sfcs)
            free(quid_sfcs);
        for (quad = 0; quad < NUM_QUADS; ++quad)
        {
            if (quad_sfcs[quad])
                free(quad_sfcs[quad]);
            if (hide_sfcs[quad])
                free(hide_sfcs[quad]);
            if (unhide_starts[quad])
                free(unhide_starts[quad]);
            if (connects[quad])
                free(connects[quad]);
        }
    }

    quid_sfcs = (SDL_Surface **)malloc(sizeof(SDL_Surface *) * num_levels);
    for (quad = 0; quad < NUM_QUADS; ++quad)
    {
        quad_sfcs[quad] = (SDL_Surface **)
            malloc(sizeof(SDL_Surface *) * num_levels);
        hide_sfcs[quad] = (SDL_Surface **)
            malloc(sizeof(SDL_Surface *) * num_levels);
        unhide_starts[quad] = (Uint32 **)
            malloc(sizeof(Uint32 *) * num_levels);
        connects[quad] = (SfcMazeConnect_t **)
            malloc(sizeof(SfcMazeConnect_t *) * num_levels);
    }

    for (level = 0; level < num_levels; ++level)
    {
        size = LEVEL_SIZE(level);
        quid_sfcs[level] = NewSurface( SDL_SWSURFACE, Screen->format,
                                       size, size );
        for (quad = 0; quad < NUM_QUADS; ++quad)
        {
            quad_sfcs[quad][level] =
                NewSurface(SDL_SWSURFACE, Screen->format, size, size);
            hide_sfcs[quad][level] =
                NewAlphaSurface(SDL_SWSURFACE, Screen->format, size, size);
            SDL_FillRect( hide_sfcs[quad][level], NULL,
                          HIDE_COLOR | hide_sfcs[quad][level]->format->Amask );
            unhide_starts[quad][level] =
                (Uint32 *)malloc(size * size * sizeof(Uint32));
            for (y = 0; y < size; ++y)
                for (x = 0; x < size; ++x)
                    unhide_starts[quad][level][x + y * size] = 0;
            if (size == 1)
                num_connects = 1;
            else
                num_connects = size / 2 * 4;
            connects[quad][level] = (SfcMazeConnect_t *)
                malloc(sizeof(SfcMazeConnect_t) * num_connects);
        }
    }

    for (quad = 0; quad < 6; ++quad)
        SDL_FillRect(quad_sfcs[quad][0], NULL, WALL_COLOR);
    SDL_FillRect(quad_sfcs[1][0], NULL, HIDE_COLOR);

    for (level = 1; level < num_levels; ++level)
    {
        size = LEVEL_SIZE(level);
        CreateSurfaceMaze(size, level, MAX_PSG_LENGTH);
    }

    player.level = 0;
    player.orient = UP;
    player.x_dir = 0;
    player.y_dir = 0;
    player.traversing = NOT_TRAVERSING;

    follow_player = 0;

    if (exit_up_rooms)
        free(exit_up_rooms);
    if (exit_dn_rooms)
        free(exit_dn_rooms);
    exit_up_rooms = (SfcMazeRoom_t *)
        malloc(sizeof(SfcMazeRoom_t) * num_levels);
    exit_dn_rooms = (SfcMazeRoom_t *)
        malloc(sizeof(SfcMazeRoom_t) * num_levels);
    start.x = 1;
    start.y = 1;
    start.quad = 0;
    CalcExits(&start, num_levels, exit_mult);

    full_zoom = FULL_ZOOM_START;

    player_zoom = SPRITE_SIZE * 8;
    player_zoom_speed = player_zoom / PLAYER_ZOOM_SPEED_DIV;

    points[0].x = -1.0f;  points[0].y = -1.0f;  points[0].z = +5.0f;
    points[1].x = +1.0f;  points[1].y = -1.0f;  points[1].z = +5.0f;
    points[2].x = +1.0f;  points[2].y = +1.0f;  points[2].z = +5.0f;
    points[3].x = -1.0f;  points[3].y = +1.0f;  points[3].z = +5.0f;
    points[4].x = -1.0f;  points[4].y = -1.0f;  points[4].z = +7.0f;
    points[5].x = +1.0f;  points[5].y = -1.0f;  points[5].z = +7.0f;
    points[6].x = +1.0f;  points[6].y = +1.0f;  points[6].z = +7.0f;
    points[7].x = -1.0f;  points[7].y = +1.0f;  points[7].z = +7.0f;
    for (i = 0; i < NUM_POINTS; ++i)
    {
        copy_points[i].x = points[i].x;
        copy_points[i].y = points[i].y;
        copy_points[i].z = points[i].z;
    }

    center.x = 0.0f;  center.y = 0.0f;  center.z = 6.0f;

    quads[0][0] = 0;  quads[0][1] = 1;  quads[0][2] = 2;  quads[0][3] = 3;
    quads[1][0] = 7;  quads[1][1] = 6;  quads[1][2] = 5;  quads[1][3] = 4;
    quads[2][0] = 5;  quads[2][1] = 1;  quads[2][2] = 0;  quads[2][3] = 4;
    quads[3][0] = 7;  quads[3][1] = 3;  quads[3][2] = 2;  quads[3][3] = 6;
    quads[4][0] = 2;  quads[4][1] = 1;  quads[4][2] = 5;  quads[4][3] = 6;
    quads[5][0] = 7;  quads[5][1] = 4;  quads[5][2] = 0;  quads[5][3] = 3;

    player.level = current_level = start_level - 1;
    maze_size = player.level * 4 - 1;
    if (maze_size == -1)
    {
        player.room.x = player.to_room.x = 0;
        player.room.y = player.to_room.y = 0;
        player.room.quad = player.to_room.quad = 1;
        player.orient = UP;
    }
    else
    {
        player.room.x = player.to_room.x = exit_dn_rooms[player.level].x;
        player.room.y = player.to_room.y = exit_dn_rooms[player.level].y;
        player.room.quad = player.to_room.quad = current_quad =
            exit_dn_rooms[player.level].quad;
        for (i = 0; i < NUM_POINTS; ++i)
        {
            switch (player.room.quad)
            {
                case 0:
                    /* No rotation needed. */
                    break;

                case 1:
                    RotatePoints( &points[i].y, &points[i].z,
                                  &copy_points[i].y, &copy_points[i].z,
                                  center.y, center.z, (float)M_PI, 1 );
                    break;

                case 2:
                    RotatePoints( &points[i].y, &points[i].z,
                                  &copy_points[i].y, &copy_points[i].z,
                                  center.y, center.z, (float)-M_PI / 2.0f, 1 );
                    break;

                case 3:
                    RotatePoints( &points[i].y, &points[i].z,
                                  &copy_points[i].y, &copy_points[i].z,
                                  center.y, center.z, (float)M_PI / 2.0f, 1 );
                    break;

                case 4:
                    RotatePoints( &points[i].x, &points[i].z,
                                  &copy_points[i].x, &copy_points[i].z,
                                  center.x, center.z, (float)M_PI / 2.0f, 1 );
                    break;

                case 5:
                    RotatePoints( &points[i].x, &points[i].z,
                                  &copy_points[i].x, &copy_points[i].z,
                                  center.x, center.z, (float)-M_PI / 2.0f, 1 );
                    break;

            }
        }
        switch (player.room.quad)
        {
            case 0:
                player.orient = UP;
                break;

            case 1:
                player.orient = UP;
                break;

            case 2:
                player.orient = LEFT;
                break;

            case 3:
                player.orient = RIGHT;
                break;

            case 4:
                player.orient = RIGHT;
                break;

            case 5:
                player.orient = RIGHT;
                break;

        }
    }

    SetRestorePoints();
}


void ResetObjects()
{
    if (maze_size > -1)
    {
        player_zoom_speed = player_zoom / PLAYER_ZOOM_SPEED_DIV;
        player.to_room.quad = current_quad = player.room.quad;
    }
    player.move_ticks_start = 0;
    player.traversing = NOT_TRAVERSING;

    rotation_start = 0;
    rotating = ROTATE_NONE;

    SearchMaze(&player.room);
}


int PickCurrentQuad()
{
    int total = 0,
        quad = 0,
        i = 0;

    /* Pick quad facing camera as current quad. */
    for (quad = 0; quad < NUM_QUADS; ++quad)
    {
        total = 0;
        for (i = 0; i < 4; ++i)
            if (points[quads[quad][i]].z < center.z)
                total++;
        if (total == 4)
            return quad;
    }
}


void SetRestorePoints()
{
    int i = 0;

    for (i = 0; i < NUM_POINTS; ++i)
    {
        restore_points[i].x = points[i].x;
        restore_points[i].y = points[i].y;
        restore_points[i].z = points[i].z;
    }
}


void RestorePoints()
{
    int i = 0;

    for (i = 0; i < NUM_POINTS; ++i)
    {
        points[i].x = copy_points[i].x = restore_points[i].x;
        points[i].y = copy_points[i].y = restore_points[i].y;
        points[i].z = copy_points[i].z = restore_points[i].z;
    }
    current_quad = player.room.quad;
}


SDL_Surface *NewSurface( int flags, SDL_PixelFormat *fmt,
                         Uint32 w, Uint32 h )
{
    SDL_Surface *return_sfc = NULL;

    return_sfc = SDL_CreateRGBSurface( flags, w, h, fmt->BitsPerPixel,
                                       fmt->Rmask, fmt->Gmask, fmt->Bmask, 0 );

    if (return_sfc == NULL)
    {
        fprintf(stderr, "Unable to create new SDL_Surface\n");
        Quit(2);
    }

    return return_sfc;
}


SDL_Surface *NewAlphaSurface( int flags, SDL_PixelFormat *fmt,
                              Uint32 w, Uint32 h )
{
    Uint32 alpha_mask = 0;
    SDL_Surface *return_sfc = NULL;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    alpha_mask = 0x000000ff;
#else
    alpha_mask = 0xff000000;
#endif

    return_sfc = SDL_CreateRGBSurface( flags, w, h, fmt->BitsPerPixel,
                                       fmt->Rmask, fmt->Gmask,
                                       fmt->Bmask, alpha_mask );

    if (return_sfc == NULL)
    {
        fprintf(stderr, "Unable to create new SDL_Surface\n");
        Quit(2);
    }

    return return_sfc;
}


void RotatePoints( float *orig_dim1, float *orig_dim2,
                   float *copy_dim1, float *copy_dim2,
                   float center_dim1, float center_dim2, float angle_delta,
                   int rotate_original )
{
    float a = *orig_dim1 - center_dim1,
          b = *orig_dim2 - center_dim2,
          angle = atan2(*orig_dim1 - center_dim1, *orig_dim2 - center_dim2),
          dist = sqrt(a * a + b * b);

    *copy_dim1 = dist * sin(angle + angle_delta) + center_dim1;
    *copy_dim2 = dist * cos(angle + angle_delta) + center_dim2;
    if (rotate_original)
    {
        *orig_dim1 = *copy_dim1;
        *orig_dim2 = *copy_dim2;
    }
}


int RotateCube( int traversing )
{
    int rotate_original = 0,
        i = 0;
    float angle = hf_pi * (SDL_GetTicks() - rotation_start) / MAZE_ROTATE_TIME;

    if (traversing != TRAVERSE_FOLLOW)
    {
        if (angle >= hf_pi)
        {
            angle = hf_pi;
            rotate_original = 1;
        }
        else
            rotate_original = 0;
    }
    else
    {
        if ( (float)(SDL_GetTicks() - player.move_ticks_start) /
             PLAYER_MOVE_TIME > 0.5f )
        {
            angle = hf_pi;
            rotate_original = 1;
        }
        else
        {
            angle = 0.0f;
            rotate_original = 0;
        }
    }

    for (i = 0; i < NUM_POINTS; ++i)
        switch (rotating)
        {
            case ROTATE_UP:
                RotatePoints( &points[i].y, &points[i].z,
                              &copy_points[i].y, &copy_points[i].z,
                              center.y, center.z, +angle, rotate_original );
                break;

            case ROTATE_DOWN:
                RotatePoints( &points[i].y, &points[i].z,
                              &copy_points[i].y, &copy_points[i].z,
                              center.y, center.z, -angle, rotate_original );
                break;

            case ROTATE_LEFT:
                RotatePoints( &points[i].x, &points[i].z,
                              &copy_points[i].x, &copy_points[i].z,
                              center.x, center.z, +angle, rotate_original );
                break;

            case ROTATE_RIGHT:
                RotatePoints( &points[i].x, &points[i].z,
                              &copy_points[i].x, &copy_points[i].z,
                              center.x, center.z, -angle, rotate_original );
                break;

            case ROTATE_CCWISE:
                RotatePoints( &points[i].x, &points[i].y,
                              &copy_points[i].x, &copy_points[i].y,
                              center.x, center.y, +angle, rotate_original );
                break;

            case ROTATE_CWISE:
                RotatePoints( &points[i].x, &points[i].y,
                              &copy_points[i].x, &copy_points[i].y,
                              center.x, center.y, -angle, rotate_original );
                break;

            case ROTATE_NONE:
                return -1;

        }

    /* Finish rotation of cube by making faces orthogonal to camera. */
    if (rotate_original)
    {
        for (i = 0; i < NUM_POINTS; ++i)
        {
            if (points[i].x < center.x)
                points[i].x = copy_points[i].x = center.x - 1.0f;
            else
                points[i].x = copy_points[i].x = center.x + 1.0f;

            if (points[i].y < center.y)
                points[i].y = copy_points[i].y = center.y - 1.0f;
            else
                points[i].y = copy_points[i].y = center.y + 1.0f;

            if (points[i].z < center.z)
                points[i].z = copy_points[i].z = center.z - 1.0f;
            else
                points[i].z = copy_points[i].z = center.z + 1.0f;
        }
        current_quad = PickCurrentQuad();
        if (player.to_room.quad == current_quad)
        {
            /* Set player orientation correctly. */
            if ( points[quads[current_quad][0]].y < center.y &&
                 points[quads[current_quad][1]].y < center.y )
                player.orient = UP;
            else if ( points[quads[current_quad][1]].y < center.y &&
                      points[quads[current_quad][2]].y < center.y )
                player.orient = RIGHT;
            else if ( points[quads[current_quad][2]].y < center.y &&
                      points[quads[current_quad][3]].y < center.y )
                player.orient = DOWN;
            else
                player.orient = LEFT;

            SetRestorePoints();
        }

        return 1;
    }

    return 0;
}


void RotatePalette( SDL_Surface *sfc, SDL_Color *palette, SDL_Color *copy,
                    int size, int io, Uint32 ticks )
{
    size_t sz_color = sizeof(SDL_Color);
    int offset = 0,
        end = 0;

    offset = (ticks % EXIT_PALETTE_TIME) * size / EXIT_PALETTE_TIME;
    end = size - offset - 1;

    if (io == INWARD)
    {
        memcpy(copy, palette + end, sz_color * offset);
        memcpy(copy + offset, palette, sz_color * end);
    }
    else  /* io == OUTWARD */
    {
        memcpy(copy, palette + offset, sz_color * end);
        memcpy(copy + end, palette, sz_color * offset);
    }

    SDL_SetColors(sfc, copy, 0, SPRITE_SIZE / 2);
}


void SetUnhideTicks( int quad, int x, int y, Uint32 ticks )
{
    Uint32 *set = &unhide_starts[quad][current_level][x + y * maze_size];
    if (*set == 0)
        *set = ticks;
}


void SearchMaze( SfcMazeRoom_t *from )
{
    int finished = 0,
        quad = from->quad,
        x = 0,
        y = 0;
    Uint32 ticks = SDL_GetTicks();

    if (ticks == 0)
        ticks = 1;

    finished = 0;
    for (x = from->x, y = from->y; x < maze_size && !finished; ++x)
    {
        if (GetPixel_32(quad_sfcs[quad][player.level], x, y) == WALL_COLOR)
            finished = 1;
        SetUnhideTicks(quad, x, y, ticks);
        if (y > 0)
            SetUnhideTicks(quad, x, y - 1, ticks);
        if (y < maze_size - 1)
            SetUnhideTicks(quad, x, y + 1, ticks);
    }

    finished = 0;
    for (x = from->x, y = from->y; x >= 0 && !finished; --x)
    {
        if (GetPixel_32(quad_sfcs[quad][player.level], x, y) == WALL_COLOR)
            finished = 1;
        SetUnhideTicks(quad, x, y, ticks);
        if (y > 0)
            SetUnhideTicks(quad, x, y - 1, ticks);
        if (y < maze_size - 1)
            SetUnhideTicks(quad, x, y + 1, ticks);
    }

    finished = 0;
    for (x = from->x, y = from->y; y < maze_size && !finished; ++y)
    {
        if (GetPixel_32(quad_sfcs[quad][player.level], x, y) == WALL_COLOR)
            finished = 1;
        SetUnhideTicks(quad, x, y, ticks);
        if (x > 0)
            SetUnhideTicks(quad, x - 1, y, ticks);
        if (x < maze_size - 1)
            SetUnhideTicks(quad, x + 1, y, ticks);
    }

    finished = 0;
    for (x = from->x, y = from->y; y >= 0 && !finished; --y)
    {
        if (GetPixel_32(quad_sfcs[quad][player.level], x, y) == WALL_COLOR)
            finished = 1;
        SetUnhideTicks(quad, x, y, ticks);
        if (x > 0)
            SetUnhideTicks(quad, x - 1, y, ticks);
        if (x < maze_size - 1)
            SetUnhideTicks(quad, x + 1, y, ticks);
    }
}


void MovePlayer( int dir )
{
    int over_edge = 0,
        hf_sz = maze_size / 2,
        i = 0,
        x = 0,
        y = 0;

    player.x_dir = 0;
    player.y_dir = 0;

    switch (dir)
    {
        case UP:
            if (player.orient == UP)
                player.y_dir = -1;
            else if (player.orient == RIGHT)
                player.x_dir = +1;
            else if (player.orient == DOWN)
                player.y_dir = +1;
            else if (player.orient == LEFT)
                player.x_dir = -1;
            break;

        case RIGHT:
            if (player.orient == UP)
                player.x_dir = +1;
            else if (player.orient == RIGHT)
                player.y_dir = +1;
            else if (player.orient == DOWN)
                player.x_dir = -1;
            else if (player.orient == LEFT)
                player.y_dir = -1;
            break;

        case DOWN:
            if (player.orient == UP)
                player.y_dir = +1;
            else if (player.orient == RIGHT)
                player.x_dir = -1;
            else if (player.orient == DOWN)
                player.y_dir = -1;
            else if (player.orient == LEFT)
                player.x_dir = +1;
            break;

        case LEFT:
            if (player.orient == UP)
                player.x_dir = -1;
            else if (player.orient == RIGHT)
                player.y_dir = -1;
            else if (player.orient == DOWN)
                player.x_dir = +1;
            else if (player.orient == LEFT)
                player.y_dir = +1;
            break;

    }

    if (player.room.x == 0 && player.x_dir == -1)
        over_edge = LEFT;
    else if (player.room.x == maze_size - 1 && player.x_dir == +1)
        over_edge = RIGHT;
    else if (player.room.y == 0 && player.y_dir == -1)
        over_edge = UP;
    else if (player.room.y == maze_size - 1 && player.y_dir == +1)
        over_edge = DOWN;
    else
    {
        /* Move does not traverse an edge of the cube. */
        player.traversing = NOT_TRAVERSING;
        if ( GetPixel_32( quad_sfcs[player.room.quad][player.level],
                          player.room.x + player.x_dir,
                          player.room.y + player.y_dir ) != WALL_COLOR )

        {
            player.move_ticks_start = SDL_GetTicks();
            player.to_room.quad = player.room.quad;
            player.to_room.x = player.room.x + player.x_dir;
            player.to_room.y = player.room.y + player.y_dir;
        }
        else
        {
            player.x_dir = 0;
            player.y_dir = 0;
        }
        SearchMaze(&player.to_room);
        return;
    }

    for (i = 0; i < hf_sz * 4; ++i)
    {
        x = connects[current_quad][player.level][i].fr_x;
        y = connects[current_quad][player.level][i].fr_y;
        if (player.room.x == x && player.room.y == y)
            break;
    }

    /* Traverse the edge. */
    switch (over_edge)
    {
        case UP:
            switch (player.orient)
            {
                case UP:
                    rotating = ROTATE_DOWN;
                    break;

                case RIGHT:
                    rotating = ROTATE_RIGHT;
                    break;

                case DOWN:
                    rotating = ROTATE_UP;
                    break;

                case LEFT:
                    rotating = ROTATE_LEFT;
                    break;

            }
            break;

        case RIGHT:
            switch (player.orient)
            {
                case UP:
                    rotating = ROTATE_LEFT;
                    break;

                case RIGHT:
                    rotating = ROTATE_DOWN;
                    break;

                case DOWN:
                    rotating = ROTATE_RIGHT;
                    break;

                case LEFT:
                    rotating = ROTATE_UP;
                    break;

            }
            break;

        case DOWN:
            switch (player.orient)
            {
                case UP:
                    rotating = ROTATE_UP;
                    break;

                case RIGHT:
                    rotating = ROTATE_LEFT;
                    break;

                case DOWN:
                    rotating = ROTATE_DOWN;
                    break;

                case LEFT:
                    rotating = ROTATE_RIGHT;
                    break;

            }
            break;

        case LEFT:
            switch (player.orient)
            {
                case UP:
                    rotating = ROTATE_RIGHT;
                    break;

                case RIGHT:
                    rotating = ROTATE_UP;
                    break;

                case DOWN:
                    rotating = ROTATE_LEFT;
                    break;

                case LEFT:
                    rotating = ROTATE_DOWN;
                    break;

            }
            break;

    }

    player.to_room.x = connects[current_quad][player.level][i].to_x;
    player.to_room.y = connects[current_quad][player.level][i].to_y;
    player.to_room.quad = connects[current_quad][player.level][i].to_qd;
    player.move_ticks_start = SDL_GetTicks();
    if (follow_player)
        player.traversing = TRAVERSE_FOLLOW;
    else
        player.traversing = TRAVERSE_CENTER;
    rotation_start = SDL_GetTicks();
    SearchMaze(&player.to_room);
}


int PlayerOnExit( SfcMazeRoom_t *exit )
{
    if ( player.room.quad == exit->quad &&
         player.room.x == exit->x && player.room.y == exit->y )
        return 1;

    return 0;
}


void Quit( int code )
{
    int level = 0,
        quad = 0;

    for (level = 0; level < total_num_levels; ++level)
    {
        if (quid_sfcs)
            SDL_FreeSurface(quid_sfcs[level]);
        for (quad = 0; quad < NUM_QUADS; ++quad)
        {
            if (quad_sfcs[quad])
                SDL_FreeSurface(quad_sfcs[quad][level]);
            if (hide_sfcs[quad])
                SDL_FreeSurface(hide_sfcs[quad][level]);
            if (unhide_starts[quad])
                free(unhide_starts[quad][level]);
            if (connects[quad])
                free(connects[quad][level]);
        }
    }
    if (quid_sfcs)
        free(quid_sfcs);
    for (quad = 0; quad < NUM_QUADS; ++quad)
    {
        if (quad_sfcs[quad])
            free(quad_sfcs[quad]);
        if (hide_sfcs[quad])
            free(hide_sfcs[quad]);
        if (unhide_starts[quad])
            free(unhide_starts[quad]);
        if (connects[quad])
            free(connects[quad]);
    }
    SDL_FreeSurface(player_sfc);
    SDL_FreeSurface(exit_up_sfc);
    SDL_FreeSurface(exit_dn_sfc);
    SDL_FreeSurface(exit_final_sfc);
    SDL_FreeSurface(fade_sfc);
    SDL_FreeSurface(prev_sfc);
    if (exit_up_rooms)
        free(exit_up_rooms);
    if (exit_dn_rooms)
        free(exit_dn_rooms);
    TTF_Quit();
    SDL_Quit();

#ifdef DEBUG_DIAGNOSTICS
    printf("Exit status:  %s\n", (code == 0) ? "Normal" : "Fatal");
#endif
    exit(code);
}


int main( int argc, char *argv[] )
{
    int finished = 0;

    InitGame(time(NULL));
    NextState = IntroScreen;
    state_arg = NULL;
    while (!finished)
    {
        FunctionState = NextState;
        finished = FunctionState();
    }
#ifdef DEBUG_DIAGNOSTICS
    if (total_ticks > 0)
        printf("Average FPS: %.2f\n", (float)n_frames * 1000.0f / total_ticks);
#endif
    Quit(0);
}
