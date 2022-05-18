#include <stdio.h>
#include "GameMap.h"
#include <string.h>

//Read file map and setting max coordinate, start position
void GameMap::LoadMap(const char* name)
{
    FILE* fp = NULL;
    fopen_s(&fp, name, "rb");
    if(fp == NULL) 
    {
        printf("Could not open file!\n");
        return;
    }
    game_map_.max_x_ = 0;
	game_map_.max_y_ = 0;
    for(int i = 0; i < MAX_MAP_Y; i++) 
    {
        for(int j = 0; j < MAX_MAP_X; j++)
        {
            fscanf(fp, "%d", &game_map_.tile[i][j]);
            int val = game_map_.tile[i][j];
            if(val > 0)
            {
                if(i > game_map_.max_y_)
                {
                    game_map_.max_y_ = i;
                }
                if(j > game_map_.max_x_)
                {
                    game_map_.max_x_ = j;
                }
            }
        }
    }

    game_map_.max_x_ = (game_map_.max_x_ + 1)*TILE_SIZE;
    game_map_.max_y_ = (game_map_.max_y_ + 1)*TILE_SIZE;

    game_map_.start_x_ = 0;
    game_map_.start_y_ = 20*TILE_SIZE - TILE_SIZE;

    game_map_.file_name = name;
    fclose(fp);
}

//Load tile image
void GameMap::LoadTiles(SDL_Renderer* screen) {
	char file_img[20];
	for(int i = 0; i < MAX_TILES; i++) {
		sprintf_s(file_img, "map/%d.png", i);
		tile_map[i].LoadImg(file_img, screen); 
	}
} 

//Fill the images into the squares
void GameMap::DrawMap(SDL_Renderer* screen)
{
    int x1 = 0;
    int x2 = 0;

    int y1 = 0;
    int y2 = 0;

    int map_x = 0;
    int map_y = 0;

    map_x = game_map_.start_x_ / TILE_SIZE;
    x1 = (game_map_.start_x_ % TILE_SIZE)*-1;
    x2 = x1 + SCREEN_WIDTH + (x1 == 0 ? 0 : TILE_SIZE);

    map_y = game_map_.start_y_ / TILE_SIZE;
    y1 = (game_map_.start_y_ % TILE_SIZE)*-1;
    y2 = y1 + SCREEN_HEIGHT + (y1  == 0 ? 0 : TILE_SIZE);

    for(int i = y1; i < y2; i += TILE_SIZE) 
    {
        map_x = game_map_.start_x_ / TILE_SIZE;
        for(int j = x1; j < x2; j += TILE_SIZE)
        {
            int val = game_map_.tile[map_y][map_x];
            if(val > 0)
            {
                tile_map[val].SetRect(j, i);
                tile_map[val].Render(screen);
            }
            map_x++;
        }
        map_y++;
    }
}