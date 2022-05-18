#include <stdio.h>
#include "Utils.h"
#include "BaseObject.cpp"
#include "GameMap.cpp"
#include "Player.cpp"
#include "Timer.cpp"
#include "Text.cpp"

BaseObject g_background;
BaseObject g_menu;
TTF_Font* font = NULL;

bool InitData() {
	bool success = true;
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if(ret < 0) return false;
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	g_window = SDL_CreateWindow("Jump King", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if(g_window == NULL) {
		success = false;
	} else {
		g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);

		if(g_screen == NULL) {
			success = false;
		} else {
			SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
			int imgFlags = IMG_INIT_PNG;
			if(!(IMG_Init(imgFlags) && imgFlags)) {
				success = false;
			}
		}
		if(TTF_Init() == -1)
		{
			printf("Failed to initialize TTF");
			success = false;
		}
		font = TTF_OpenFont("font//ChrustyRock-ORLA.ttf", 50);
		if(font == NULL)
		{
			success = false;
		}
	}

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) 
	{
		success = false;
	}
	else 
	{
		g_sound_jump = Mix_LoadWAV("C:/TenGame/audio/cartoon-jump.wav");
		g_music = Mix_LoadMUS("C:/TenGame/audio/Jump-King-OST-04-Dreeg_s-Theme.wav");
	}

	if(g_sound_jump == NULL || g_music == NULL)
	{
		success = false;
	}

	return success;
}

bool LoadBackground() {
	bool ret = g_background.LoadImg("C:/TenGame/img/backgroundFinal.png", g_screen); 
	if(ret == false) 
    {
		printf("%s\n", SDL_GetError());
		return false;
	}
    else
    {
		return true;
	}
}


void close() {
	g_background.Free();
	g_menu.Free();
	SDL_DestroyRenderer(g_screen);
	g_screen = NULL;

	SDL_DestroyWindow(g_window);
	g_window = NULL;

	Mix_FreeChunk(g_sound_jump);
	Mix_FreeChunk(g_sound_soundtrack);
	g_sound_jump = NULL;
	g_sound_soundtrack = NULL;

	Mix_FreeMusic(g_music);
	g_music = NULL;

	TTF_Quit();	
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool CheckMouseClick(const int& x, const int& y, const SDL_Rect& rect)
{
	if(x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h)
	{
		return true;
	}
	return false;
}

int ShowMenu(TTF_Font* font, SDL_Renderer* screen, SDL_Event p_event);

int ShowControlMenu(TTF_Font* font, SDL_Renderer* screen, SDL_Event p_event)
{
	SDL_Texture* background = IMG_LoadTexture(screen, "C:/TenGame/img/ControlMenu.png");
	SDL_Rect origin = {0,0,SCREEN_HEIGHT,SCREEN_WIDTH};
	Text Back_menu;
	Back_menu.SetText("MENU");
	Back_menu.SetColor(Text::WHITE);
	bool selectedItem;
    int mouse_posx; 
    int mouse_posy;
	SDL_RenderCopy(screen,background,&origin,NULL);
    while (true)
    {
        Back_menu.LoadFromRenderText(font,screen);
        Back_menu.RenderText(screen,150,700);
		while (SDL_PollEvent(&p_event))
            {
                switch (p_event.type)
                {
                    case SDL_QUIT:
						close();
                        return 1;
                    case SDL_MOUSEMOTION:
                        {
                            mouse_posx = p_event.motion.x;
                            mouse_posy = p_event.motion.y;
                            if (CheckMouseClick(mouse_posx, mouse_posy, Back_menu.GetRect()))
                            {
                                if (selectedItem == false)
                                {
                                    selectedItem = 1;
                                    Back_menu.SetColor(Text::RED);
                                }
                            }
                            else
                            {
                                if (selectedItem == true)
                                {
                                    selectedItem = 0;
                                    Back_menu.SetColor(Text::WHITE);
                                }
                             }
                        }
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        {
                            mouse_posx = p_event.button.x;
                            mouse_posy = p_event.button.y;
                            if (CheckMouseClick(mouse_posx, mouse_posy, Back_menu.GetRect()))
                            {   
                                return 3;
                            }
                        }
                        break;
                default:
                    break;
                }
            }
			SDL_RenderPresent(screen);
			if(background != NULL) {
				SDL_DestroyTexture(background);
				background = NULL;
			}
			Back_menu.Free();
	}
}

int ShowMenu(TTF_Font* font, SDL_Renderer* screen, SDL_Event p_event)
{
		SDL_Texture* background = IMG_LoadTexture(screen, "C:/TenGame/img/MenuBackground.png");
		SDL_Rect origin = {0,0,SCREEN_HEIGHT,SCREEN_WIDTH};
        Text Menu_text[3];
        Menu_text[0].SetText("PLAY");
        Menu_text[1].SetText("CONTROLS");
        Menu_text[2].SetText("QUIT");
        for(int i = 0; i < 3; i++)
        {
            Menu_text[i].SetColor(Text::WHITE);
        }
        bool selectedItem[3];
        int mouse_posx; 
        int mouse_posy;
		SDL_RenderCopy(screen,background,&origin,NULL);
        while (true)
        {
            for (int i = 0; i < 3; ++i)
            {   
                Menu_text[i].LoadFromRenderText(font,screen);
                Menu_text[i].RenderText(screen,150,300 + i*100);
            }
            while (SDL_PollEvent(&p_event))
            {
                switch (p_event.type)
                {
                    case SDL_QUIT:
						close();
                        return 1;
                    case SDL_MOUSEMOTION:
                        {
                            mouse_posx = p_event.motion.x;
                            mouse_posy = p_event.motion.y;

                            for (int i = 0; i < 3; i++)
                            {
                                if (CheckMouseClick(mouse_posx, mouse_posy, Menu_text[i].GetRect()))
                                {
                                    if (selectedItem[i] == false)
                                    {
                                        selectedItem[i] = 1;
                                        Menu_text[i].SetColor(Text::RED);
                                    }
                                }
                                else
                                {
                                    if (selectedItem[i] == true)
                                    {
                                        selectedItem[i] = 0;
                                        Menu_text[i].SetColor(Text::WHITE);
                                    }
                                }
                            }
                        }
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        {
                            mouse_posx = p_event.button.x;
                            mouse_posy = p_event.button.y;

                            for (int i = 0; i < 3; i++)
                            {
                                if (CheckMouseClick(mouse_posx, mouse_posy, Menu_text[i].GetRect()))
                                {   
                                	return i-1;
								
                                }
                            }
                        }
                        break;
                    case SDL_KEYDOWN:
                        if (p_event.key.keysym.sym == SDLK_ESCAPE)
                        {
                            return 1;
                        }
                default:
                    break;
                }
            }
            SDL_RenderPresent(screen);
			if(background != NULL) {
				SDL_DestroyTexture(background);
				background = NULL;
			}
			for(int i = 0; i < 3; i++) 
			{
				Menu_text[i].Free();
			}
		}
        return 1;
    }

int ShowVictory(TTF_Font* font, SDL_Renderer* screen, SDL_Event p_event)
{
	SDL_Texture* background = IMG_LoadTexture(screen, "C:/TenGame/img/Victory.png");
	SDL_Rect origin = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
    Text Menu_text[2];
	Menu_text[0].SetText("MENU");
    Menu_text[1].SetText("QUIT");
    Mix_Music* VictoryMusic = Mix_LoadMUS("C:/TenGame/audio/Jump-King-NB-OST-12-Fate.wav");
    Mix_PlayMusic(VictoryMusic, -1);
    for(int i = 0; i < 2;i++)
        {
            Menu_text[i].SetColor(Text::WHITE);
        }
    bool selectedItem[2];
    int mouse_posx; 
    int mouse_posy;
	SDL_RenderCopy(screen,background,&origin,NULL);
	while (true)
    {
        for (int i = 0; i < 2; ++i)
        {   
            Menu_text[i].LoadFromRenderText(font,screen);
            Menu_text[i].RenderText(screen,250,250 + i*100);
        }
        while (SDL_PollEvent(&p_event))
        {
            switch (p_event.type)
            {
                case SDL_QUIT:
					close();
                    return 0;
                case SDL_MOUSEMOTION:
                    {
                        mouse_posx = p_event.motion.x;
                        mouse_posy = p_event.motion.y;

                        for (int i = 0; i < 2; i++)
                        {
                        	if (CheckMouseClick(mouse_posx, mouse_posy, Menu_text[i].GetRect()))
                            {
                                if (selectedItem[i] == false)
                                {
                                    selectedItem[i] = 1;
                                    Menu_text[i].SetColor(Text::RED);
                                }
                            }
                            else
                            {
                                if (selectedItem[i] == true)
                                {
                                    selectedItem[i] = 0;
                                    Menu_text[i].SetColor(Text::WHITE);
                                }
                            }
                        }
                    }
                        break;
                case SDL_MOUSEBUTTONDOWN:
                    {
                        mouse_posx = p_event.button.x;
                        mouse_posy = p_event.button.y;

                        for (int i = 0; i < 2; i++)
                        {
                            if (CheckMouseClick(mouse_posx, mouse_posy, Menu_text[i].GetRect()))
                            {   
                                return i-1;
								
                            }
                        }
                    }
                        break;
                default:
                    break;
                }
        }
        SDL_RenderPresent(screen);
		if(background != NULL) 
		{
			SDL_DestroyTexture(background);
			background = NULL;
		}
		for(int i = 0; i < 2; i++) 
		{
			Menu_text[i].Free();
		}
	}
        Mix_FreeMusic(VictoryMusic);
        VictoryMusic = NULL;
        return 1;
}

int GameLoop()
{
    Timer fps_timer;

    if(InitData() == false) {
		printf("Failed to initialize SDL");
		return -1;
	}

	Mix_Music* gchunk = Mix_LoadMUS("C:/TenGame/audio/Jump-King-OST-01-Menu-Theme.wav");
    Mix_PlayMusic(gchunk,-1);
	bool menu = true;
	while(menu == true)
	{
		switch(ShowMenu(font, g_screen, g_event))
		{
			case 1:
				menu = false;
				return -1;
				break;
			case 0:
				ShowControlMenu(font, g_screen, g_event);
				break;
			case -1:
				menu = false;
			default:
				break;
		}
	}

	Mix_FreeMusic(gchunk);
    gchunk = NULL;

	if(LoadBackground() == false) {
		printf("Failed to load background");
		return -1;
	}

	GameMap game_map;
	game_map.LoadMap("C:/TenGame/map/map.dat");
	game_map.LoadTiles(g_screen);

	Player player_;
	player_.LoadImg("C:/TenGame/img/player_right.png", g_screen);
	player_.set_clips();

    bool is_quit = false;

	Mix_PlayMusic(g_music, -1);
	while(!is_quit) {
		while(SDL_PollEvent(&g_event) != 0) {
			if(g_event.type == SDL_QUIT) {
				return -1;
			}
			player_.HandleInputAction(g_event, g_screen, g_sound_jump);
		}
		SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
		SDL_RenderClear(g_screen); 

		g_background.Render(g_screen, NULL);
		Map map_data = game_map.getMap();

		player_.SetMapXY(map_data.start_x_, map_data.start_y_);
		player_.DoPlayer(map_data);
		if(player_.isVictory() == true)
		{  
            switch(ShowVictory(font, g_screen, g_event))
            {
                case -1:
                    g_background.Free();
                    g_menu.Free();
                    SDL_DestroyRenderer(g_screen);
                    g_screen = NULL;
                    SDL_DestroyWindow(g_window);
                    g_window = NULL;
                    Mix_FreeChunk(g_sound_jump);
                    Mix_FreeChunk(g_sound_soundtrack);
                    g_sound_jump = NULL;
                    g_sound_soundtrack = NULL;
                    Mix_FreeMusic(g_music);
                    g_music = NULL;
                    return 0;
                case 0:
                    return -1;
            }
            
        }	
		player_.Show(g_screen);

		game_map.SetMap(map_data);
		game_map.DrawMap(g_screen);

		SDL_RenderPresent(g_screen);

		int real_time = fps_timer.get_time();
		int time_one_frame = 1000/FRAME_PER_SECOND; 
		if(real_time < time_one_frame) 
		{
			int delay_time = time_one_frame - real_time;
			if(delay_time >= 0 )
			SDL_Delay(delay_time);
			
		}
	}
	close();
    return 0;
}

int main(int argc, char* argv[]) 
{
	while(GameLoop() == 0)
    {
        g_background.Free();
        g_menu.Free();
        SDL_DestroyRenderer(g_screen);
        g_screen = NULL;
        SDL_DestroyWindow(g_window);
        g_window = NULL;
        Mix_FreeChunk(g_sound_jump);
        Mix_FreeChunk(g_sound_soundtrack);
        g_sound_jump = NULL;
        g_sound_soundtrack = NULL;
        Mix_FreeMusic(g_music);
        g_music = NULL;
    }
    close;
	return 0;
}