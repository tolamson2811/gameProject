#include <stdio.h>
#include "Player.h"

Player::Player() 
{
    frame_ = 0;
	x_pos_ = SCREEN_WIDTH / 2 - TILE_SIZE / 2;
	y_pos_ = 6200;
	x_val_ = 0;
	y_val_ = 0;
	width_frame_ = 0;
	height_frame_ = 0;
	status_ = WALK_NONE;
	input_type_.left_ = 0;
	input_type_.right_ = 0;
	input_type_.jump_ = 0;
	input_type_.down_ = 0;
	input_type_.up_ = 0;
	on_ground_ = false;
	map_x_ = 0;
	map_y_ = 100*TILE_SIZE - TILE_SIZE;
	time = 0;
	unablePush = 0;
	temp = false;
}

Player::~Player()
{

}

bool Player::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool res = BaseObject::LoadImg(path, screen);
    if(res == true)
    {   
        width_frame_ = 40; 
        height_frame_ = 60;
    }
    return res;
}

void Player::set_clips() 
{
    if(width_frame_ > 0 && height_frame_ > 0)
    {
        frame_clip_[0].x = 0;
		frame_clip_[0].y = 0;
		frame_clip_[0].w = width_frame_;
		frame_clip_[0].h = height_frame_;

		frame_clip_[1].x = width_frame_;
		frame_clip_[1].y = 0;
		frame_clip_[1].w = width_frame_;
		frame_clip_[1].h = height_frame_;

		frame_clip_[2].x = 2*width_frame_;
		frame_clip_[2].y = 0;
		frame_clip_[2].w = width_frame_;
		frame_clip_[2].h = height_frame_;

		frame_clip_[3].x = 3*width_frame_;
		frame_clip_[3].y = 0;
		frame_clip_[3].w = width_frame_;
		frame_clip_[3].h = height_frame_;

		frame_clip_[4].x = 4*width_frame_;
		frame_clip_[4].y = 0;
		frame_clip_[4].w = width_frame_;
		frame_clip_[4].h = height_frame_;

		frame_clip_[5].x = 5*width_frame_;
		frame_clip_[5].y = 0;
		frame_clip_[5].w = width_frame_;
		frame_clip_[5].h = height_frame_;

		frame_clip_[6].x = 6*width_frame_;
		frame_clip_[6].y = 0;
		frame_clip_[6].w = width_frame_;
		frame_clip_[6].h = height_frame_;

    }
}

void Player::Show(SDL_Renderer* des) {
	UpdateImagePlayer(des);
	if(input_type_.left_ == 1 || input_type_.right_ == 1) {
		frame_++;
	} else 
	{
		frame_ = 0;
	}


	if(frame_ >= 6) {
		frame_ = 0;
	}
	rect_.x = x_pos_ - map_x_;
	rect_.y = y_pos_ - map_y_;

	SDL_Rect* current_clip = &frame_clip_[frame_];

	SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame_, height_frame_};

	SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
	
}

void Player::HandleInputAction(SDL_Event events, SDL_Renderer* screen, Mix_Chunk* jump_sound) {
	if(events.type == SDL_KEYDOWN) {
		switch (events.key.keysym.sym) 
		{
		case SDLK_RIGHT:
			{
				if(status_ != PUSH) {
					status_ = WALK_RIGHT; 
					input_type_.right_ = 1;
					input_type_.left_ = 0;
					UpdateImagePlayer(screen);
				}
			}
			break;
		case SDLK_LEFT:
			{
				if(status_ != PUSH) {
					status_ = WALK_LEFT;
					input_type_.left_ = 1;
					input_type_.right_ = 0;
					UpdateImagePlayer(screen);
				}
			}
			break;
		case SDLK_SPACE:
			if(on_ground_ == TRUE && unablePush == 0)
			{	
				status_ = PUSH;
				input_type_.jump_ = 0;
				if(time < 10) time += 10;
				if(time >= 10 && time < 13) time += 2;
				if(time >= 13 && time < 15) time += 1;
				if(time >= 15 && time < 18) time += 0.5;
				if(time >= 18) time = 18;
				UpdateImagePlayer(screen);
			}
			break;
		default:
			break;
		}
	}else if(events.type == SDL_KEYUP) {
		switch (events.key.keysym.sym) 
		{
		case SDLK_RIGHT:
			{
				input_type_.right_ = 0;
			}
			break;
		case SDLK_LEFT:
			{
				input_type_.left_ = 0;
			}
			break;
		case SDLK_SPACE:
			{
				if(status_ == PUSH) 
				{
					Mix_PlayChannel(-1, jump_sound, 0);
				}
				input_type_.jump_ = 1;
				status_ = WALK_NONE;
				UpdateImagePlayer(screen);
			}
			break;
		default:
			break;
		}
	}
	
}

void Player::DoPlayer(Map& map_data) {
		x_val_ = 0;
		y_val_ += GRAVITY_SPEED; // Tốc độ rơi

		if(y_val_ >= MAX_FALL_SPEED) {
			y_val_ = MAX_FALL_SPEED;
		}

		if(input_type_.left_ == 1) {
			x_val_ -= PLAYER_SPEED;
		}

		else if(input_type_.right_ == 1) {
			x_val_ += PLAYER_SPEED;
		}

		if(input_type_.jump_ == 1) 
		{	
			if(on_ground_ == true) 
			{
				y_val_ = -time;
			}
			time = 0;
			on_ground_ = false;
			input_type_.jump_ = 0;
		}
		CheckToMap(map_data);
		CenterEntityOnMap(map_data);
}

void Player::CenterEntityOnMap(Map& map_data) 
{
	

	map_data.start_y_ = y_pos_ - (SCREEN_HEIGHT / 2);
	if(map_data.start_y_ < 0) 
	{
		map_data.start_y_ = 0;
	}
	else if(map_data.start_y_ + SCREEN_HEIGHT >= map_data.max_y_)
	{
		map_data.start_y_ = map_data.max_y_ - SCREEN_HEIGHT;
	}
}

void Player::CheckToMap(Map& map_data) {
	int x1 = 0; // Giới hạn kiểm tra theo chiều x
	int x2 = 0; // Giới hạn kiểm tra theo chiều x
	 
	int y1 = 0; // Giới hạn kiểm tra theo chiều y
	int y2 = 0; // Giới hạn kiểm tra theo chiều y

	//Check theo chieu ngang
	x1 = (x_pos_ + x_val_) / TILE_SIZE; 
	x2 = (x_pos_ + x_val_ + width_frame_ - 1) / TILE_SIZE; 

	y1 = y_pos_ / TILE_SIZE;
	y2 = (y_pos_ + height_frame_ - 1) / TILE_SIZE;	

	if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
		if(x_val_ > 0) //dang tien ve ben phai
		{
			if(map_data.tile[y1][x2] == 1 || map_data.tile[y2][x2] == 1 || map_data.tile[y1][x2] == 2 || map_data.tile[y2][x2] == 2 ||
				map_data.tile[y1][x2] == 6 || map_data.tile[y2][x2] == 6 || map_data.tile[y1][x2] == 7 || map_data.tile[y2][x2] == 7 ||
				map_data.tile[y1][x2] == 8 || map_data.tile[y2][x2] == 8 || map_data.tile[y1][x2] == 9 || map_data.tile[y2][x2] == 9 ||
				map_data.tile[y1][x2] == 10 || map_data.tile[y2][x2] == 10 ||map_data.tile[y1][x2] == 23 || map_data.tile[y2][x2] == 23 ||
				map_data.tile[y1][x2] == 28 || map_data.tile[y2][x2] == 28 || map_data.tile[y1][x2] == 29 || map_data.tile[y2][x2] == 29 ||
				map_data.tile[y1][x2] == 32 || map_data.tile[y2][x2] == 32 || map_data.tile[y1][x2] == 33 || map_data.tile[y2][x2] == 33 ||
				map_data.tile[y1][x2] == 25 || map_data.tile[y2][x2] == 25) 
			{
				x_pos_ = x2 * TILE_SIZE;
				x_pos_ -= width_frame_ + 1;
				x_val_ = 0; 
			}
			else if(map_data.tile[y1][x2] == 53 || map_data.tile[y2][x2] == 53)
			{
				x_pos_ = x2 * TILE_SIZE;
				x_pos_ -= width_frame_ + 1;
				x_val_ = 0;
				temp = true;
			}
			else if(map_data.tile[y1][x2] == 3)
			{
				if((y_pos_ + height_frame_ + y_val_ >= y_pos_ - TILE_SIZE + TILE_SIZE*3/4 ) && on_ground_ == false) 
				{
					x_pos_ = x2 * TILE_SIZE;
					x_pos_ -= width_frame_ + 1;
					x_val_ = 0; 
				}
			}
			else if(map_data.tile[y1][x2] == 21)
			{
				if((y_pos_ + height_frame_ + y_val_ >= y_pos_ - TILE_SIZE + TILE_SIZE*2/3 ) && on_ground_ == false) 
				{
					x_pos_ = x2 * TILE_SIZE;
					x_pos_ -= width_frame_ + 1;
					x_val_ = 0; 
				}
			}
			else if(map_data.tile[y1][x2] == 24 || map_data.tile[y2][x2] == 24) 
			{
				if((x_pos_ + width_frame_ < x2*TILE_SIZE + TILE_SIZE*2/3) && (x_pos_ + x_val_ + width_frame_>= x2*TILE_SIZE + TILE_SIZE*2/3))
				{
					x_pos_ = x2*TILE_SIZE + TILE_SIZE*2/3 - 8;
					x_pos_ -= width_frame_ + 1;
					x_val_ = 0;
				}
			}
			else if(map_data.tile[y1][x2] == 22 || map_data.tile[y2][x2] == 22 || map_data.tile[y1][x2] == 34 || map_data.tile[y2][x2] == 34 ||
					map_data.tile[y1][x2] == 35 || map_data.tile[y2][x2] == 35)
			{
				if((x_pos_ + width_frame_ < x2*TILE_SIZE) && (x_pos_ + width_frame_ + x_val_ >= x2*TILE_SIZE))
				{
					x_pos_ = x2 * TILE_SIZE;
					x_pos_ -= width_frame_ + 1;
					x_val_ = 0;
				}
			}
			else if(map_data.tile[y1][x1] == 30 || map_data.tile[y2][x2] == 30 || map_data.tile[y1][x2] == 31 || map_data.tile[y2][x2] == 31)
			{
				unablePush = 1;
				x_val_ = 0;
			}else {
				unablePush = 0;
			}
		}
		else if(x_val_ < 0) // Đang đi về bên trái 
		{
			if(map_data.tile[y1][x1] == 1 || map_data.tile[y2][x1] == 1 || map_data.tile[y1][x1] == 2 || map_data.tile[y2][x1] == 2 ||
				map_data.tile[y1][x1] == 6 || map_data.tile[y2][x1] == 6 || map_data.tile[y1][x1] == 7 || map_data.tile[y2][x1] == 7 ||
				map_data.tile[y1][x1] == 8 || map_data.tile[y2][x1] == 8 || map_data.tile[y1][x1] == 9 || map_data.tile[y2][x1] == 9 ||
				map_data.tile[y1][x1] == 10 || map_data.tile[y2][x1] == 10 || map_data.tile[y1][x1] == 23 || map_data.tile[y2][x1] == 23 ||
				map_data.tile[y1][x1] == 28 || map_data.tile[y2][x1] == 28 || map_data.tile[y1][x1] == 29 || map_data.tile[y2][x1] == 29 ||
				map_data.tile[y1][x1] == 32 || map_data.tile[y2][x1] == 32 || map_data.tile[y1][x1] == 33 || map_data.tile[y2][x1] == 33 ||
				map_data.tile[y1][x1] == 25 || map_data.tile[y2][x1] == 25) 
			{
				x_pos_ = (x1 + 1) * TILE_SIZE + 1;
				x_val_ = 0;
			}
			else if(map_data.tile[y1][x1] == 3) 
			{
				if((y_pos_ + height_frame_ + y_val_ >= y_pos_ - TILE_SIZE + TILE_SIZE*3/4 ) && on_ground_ == false) 
				{
					x_pos_ = (x1 + 1) * TILE_SIZE + 1;
					x_val_ = 0; 
				}
			}
			else if(map_data.tile[y1][x1] == 21)
			{
				if((y_pos_ + height_frame_ + y_val_ >= y_pos_ - TILE_SIZE + TILE_SIZE*2/3 ) && on_ground_ == false) 
				{
					x_pos_ = (x1 + 1) * TILE_SIZE + 1;
					x_val_ = 0; 
				}
			}
			else if(map_data.tile[y1][x1] == 24 || map_data.tile[y2][x1] == 24)
			{
				if((x_pos_ > (x1+1)*TILE_SIZE) && (x_pos_ + x_val_ <= (x1+1)*TILE_SIZE)) 
				{
					x_pos_ = (x1+1)*TILE_SIZE + 1;
					x_val_ = 0; 
				}
			}
			else if(map_data.tile[y1][x1] == 22 || map_data.tile[y2][x1] == 22 || map_data.tile[y1][x1] == 34 || map_data.tile[y2][x1] == 34 ||
					map_data.tile[y1][x1] == 35 || map_data.tile[y2][x1] == 35) 
			{
				if((x_pos_ > x1*TILE_SIZE + TILE_SIZE*1/3) && (x_pos_ + x_val_ <= x1*TILE_SIZE + TILE_SIZE*1/3))
				{
					x_pos_ = x1*TILE_SIZE + TILE_SIZE*1/3 + 5;
					x_val_ = 0;
				}
			}
			else if(map_data.tile[y1][x1] == 30 || map_data.tile[y2][x1] == 30 || map_data.tile[y1][x1] == 31 || map_data.tile[y2][x1] == 31) 
			{
				unablePush = 1;
				x_val_ = 0;
			}
			else {
				unablePush = 0;
			}
		}
	}

	//Check theo chieu doc
	x1 = (x_pos_) / TILE_SIZE;
	x2 = (x_pos_ + width_frame_) / TILE_SIZE;
	
	y1 = (y_pos_ + y_val_) / TILE_SIZE;
	y2 = (y_pos_ + y_val_ + height_frame_-1) / TILE_SIZE;

	if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
	{
		if(y_val_ > 0) 
		{
			if(map_data.tile[y2][x1] == 1 || map_data.tile[y2][x2] == 1 || map_data.tile[y2][x1] == 2 || map_data.tile[y2][x2] == 2 ||
				map_data.tile[y2][x1] == 6 || map_data.tile[y2][x2] == 6 || map_data.tile[y2][x1] == 7 || map_data.tile[y2][x2] == 7 ||
				map_data.tile[y2][x1] == 8 || map_data.tile[y2][x2] == 8 || map_data.tile[y2][x1] == 9 || map_data.tile[y2][x2] == 9 ||
				map_data.tile[y2][x1] == 10 || map_data.tile[y2][x2] == 10 || map_data.tile[y2][x1] == 23 || map_data.tile[y2][x2] == 23 ||
				map_data.tile[y2][x1] == 28 || map_data.tile[y2][x2] == 28 || map_data.tile[y2][x1] == 29 || map_data.tile[y2][x2] == 29 ||
				map_data.tile[y2][x1] == 32 || map_data.tile[y2][x2] == 32 || map_data.tile[y2][x1] == 33 || map_data.tile[y2][x2] == 33 ||
				map_data.tile[y2][x1] == 25 || map_data.tile[y2][x2] == 25) 
			{
				y_pos_ = y2 * TILE_SIZE;
				y_pos_ -= (height_frame_ + 1);
				y_val_ = 0;
				on_ground_ = true;
				if(status_ == WALK_NONE)
				{
					status_ = WALK_RIGHT;
				}
			}
			else if(map_data.tile[y2][x1] == 3 || map_data.tile[y2][x2] == 3)
			{
				double toptile = y2*TILE_SIZE +TILE_SIZE*3/4;
				
				if(y_pos_ + height_frame_ + y_val_ >= toptile)
				{
					y_pos_ = toptile;
					y_pos_ -= (height_frame_ - 1);
					y_val_ = 0;
					on_ground_ = true;
					
					if(status_ == WALK_NONE)
					{
						status_ = WALK_RIGHT;
					}
				}
			}	
			else if(map_data.tile[y2][x1] == 21 || map_data.tile[y2][x2] == 21)
			{
				double toptile = y2*TILE_SIZE + TILE_SIZE*2/3 - 5;
				
				if(y_pos_ + height_frame_ + y_val_ >= toptile)
				{
					y_pos_ = toptile;
					y_pos_ -= (height_frame_ - 1);
					y_val_ = 0;
					on_ground_ = true;
					
					if(status_ == WALK_NONE)
					{
						status_ = WALK_RIGHT;
					}
				}
			}
			else if(map_data.tile[y2][x1] == 24 || map_data.tile[y2][x2] == 24)
			{
				if(map_data.tile[y2][x1] != 24 && map_data.tile[y2][x2] == 24)
				{
					if((x_pos_ + width_frame_ >= x2*TILE_SIZE + TILE_SIZE*2/3) && (x_pos_ <= (x2+1)*TILE_SIZE)) 
					{
						y_pos_ = y2*TILE_SIZE;
						y_pos_ -= (height_frame_ - 1);
						y_val_ = 0;
						on_ground_ = true;
						
						if(status_ == WALK_NONE)
						{
							status_ = WALK_RIGHT;
						}
					}
				}
				else if(map_data.tile[y2][x1] == 24)
				{
					if((x_pos_ + width_frame_ >= x1*TILE_SIZE + TILE_SIZE*2/3) && (x_pos_ <= (x1+1)*TILE_SIZE)) 
					{
						y_pos_ = y2*TILE_SIZE;
						y_pos_ -= (height_frame_ - 1);
						y_val_ = 0;
						on_ground_ = true;
						
						if(status_ == WALK_NONE)
						{
							status_ = WALK_RIGHT;
						}
					}
				}

			}
			else if( map_data.tile[y2][x1] == 22 || map_data.tile[y2][x2] == 22 || map_data.tile[y2][x1] == 34 || map_data.tile[y2][x2] == 34 ||
					 map_data.tile[y2][x1] == 35 || map_data.tile[y2][x2] == 35)
			{
				if((map_data.tile[y2][x1] != 22 && map_data.tile[y2][x2] == 22) || (map_data.tile[y2][x1] != 34 && map_data.tile[y2][x2] == 34) ||
					(map_data.tile[y2][x1] != 35 && map_data.tile[y2][x2] == 35)
					) 
				{
					if((x_pos_ + width_frame_ >= x2*TILE_SIZE) && (x_pos_ <= x2*TILE_SIZE + TILE_SIZE*1/3))
					{
						y_pos_ = y2*TILE_SIZE;
						y_pos_ -= (height_frame_ - 1);
						y_val_ = 0;
						on_ground_ = true;
						
						if(status_ == WALK_NONE)
						{
							status_ = WALK_RIGHT;
						}
					}
				}
				else if(map_data.tile[y2][x1] == 22 || map_data.tile[y2][x1] == 34 || map_data.tile[y2][x1] == 35) 
				{
					if((x_pos_ + width_frame_ >= x1*TILE_SIZE) && (x_pos_ <= x1*TILE_SIZE + TILE_SIZE*1/3)) 
					{
						y_pos_ = y2*TILE_SIZE;
						y_pos_ -= (height_frame_ - 1);
						y_val_ = 0;
						on_ground_ = true;
						
						if(status_ == WALK_NONE)
						{
							status_ = WALK_RIGHT;
						}
					}
				}
			}
			else if(map_data.tile[y2][x1] == 30)
			{
				double mark = (x1 + 1)*TILE_SIZE;
				double x = TILE_SIZE - (mark - x_pos_);
				double y = x;
				double toptile = y2*TILE_SIZE + y;
				if(x == 0 || y == 0) 
				{
					x = TILE_SIZE;
					y = 1;
				}
				if(y_pos_ + height_frame_ + y_val_ >= toptile)
				{
					y_pos_ = toptile;
					y_pos_ -= (height_frame_ - 1);
					y_val_ = 0;
					x_val_ = 0;
					on_ground_ = true;
					
					if(status_ == WALK_NONE)
					{
						status_ = WALK_RIGHT;
					}
					x_pos_ += 3;
				}
				unablePush = 1;
			}
			else if(map_data.tile[y2][x2] == 31)
			{
				double mark = x2*TILE_SIZE;
				double x = TILE_SIZE - (mark - x_pos_);
				double y = 64 - x;
				double toptile = y2*TILE_SIZE + y + 23;
				if(x == 0) 
				{
					x = TILE_SIZE;
					y = 1;
				}
				if(y_pos_ + height_frame_ + y_val_ >= toptile)
				{
					y_pos_ = toptile;
					y_pos_ -= (height_frame_ - 1);
					y_val_ = 0;
					x_val_ = 0;
					on_ground_ = true;
					
					if(status_ == WALK_NONE)
					{
						status_ = WALK_RIGHT;
					}
					x_pos_ -= 3;
				}
				unablePush = 1;
			}
			else 
			{
				unablePush = 0;
			}
		}
		else if(y_val_ < 0) 
		{
			if(map_data.tile[y1][x1] == 24 || map_data.tile[y1][x2] == 24)
			{
				if(map_data.tile[y1][x1] != 24 && map_data.tile[y1][x2] == 24)
				{
					if(x_pos_ + width_frame_ >= x2*TILE_SIZE + TILE_SIZE*2/3)
					{
						y_pos_ = (y1 + 1) * TILE_SIZE;
						y_val_ = 0;
					}
				}
				else if(map_data.tile[y1][x1] == 24)
				{
					if(x_pos_ + width_frame_ >= x1*TILE_SIZE + TILE_SIZE*2/3)
					{
						y_pos_ = (y1 + 1) * TILE_SIZE;
						y_val_ = 0;
					}
				}
				
			}
			else if(map_data.tile[y1][x1] == 22 || map_data.tile[y1][x2] == 22 || map_data.tile[y1][x1] == 34 || map_data.tile[y1][x2] == 34 || 
					map_data.tile[y1][x1] == 35 || map_data.tile[y1][x2] == 35)
			{
				if((map_data.tile[y1][x2] == 22 && map_data.tile[y1][x1] != 22) || (map_data.tile[y1][x2] == 34 && map_data.tile[y1][x1] != 34) ||
					(map_data.tile[y1][x2] == 35 && map_data.tile[y1][x1] != 35)
				)
				{
					if((x_pos_ + width_frame_ >= x2*TILE_SIZE) && (x_pos_ <= x2*TILE_SIZE + TILE_SIZE*1/3))
					{
						y_pos_ = (y1 + 1) * TILE_SIZE;
						y_val_ = 0;
					}
				}
				else if(map_data.tile[y1][x1] == 22 || map_data.tile[y1][x1] == 34 || map_data.tile[y1][x1] == 35)
				{
					if((x_pos_ + width_frame_ >= x1*TILE_SIZE) && (x_pos_ <= x1*TILE_SIZE + TILE_SIZE*1/3))
					{
						y_pos_ = (y1 + 1) * TILE_SIZE;
						y_val_ = 0;
					}
				}
			}
			else if(map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y1][x2] != BLANK_TILE) 
			{
				y_pos_ = (y1 + 1) * TILE_SIZE;
				y_val_ = 0;
			}
		}
	}

	x_pos_ += x_val_;
	y_pos_ += y_val_;

	if(x_pos_ < 0) 
 	{
	 	x_pos_ = 0;
	}
	 else if(x_pos_ + width_frame_ > map_data.max_x_) 
	{
		x_pos_ = map_data.max_x_ - width_frame_ - 1;

	}
	
}

void Player::UpdateImagePlayer(SDL_Renderer* des)
{
	if(on_ground_ == true) 
	{
		if(status_ == PUSH) {
			LoadImg("C:/TenGame/img/push.png", des);
		}
	 	else if(status_ == WALK_LEFT) 
		{
			LoadImg("C:/TenGame/img/player_left.png", des);
		}
		else
		{
			LoadImg("C:/TenGame/img/player_right.png", des);
		}
	}
	else 
	{
		if(status_ == WALK_LEFT)
		{
			LoadImg("C:/TenGame/img/jump_left.png", des);
		}
		else 
		{
			LoadImg("C:/TenGame/img/jump_right.png", des);
		}
	}
}

bool Player::isVictory()
{
	if(temp == false)
	{
		return false;
	}
	else 
	{
		return true;
	}
}
