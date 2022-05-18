#ifndef PLAYER_H_
#define PLAYER_H_

#include "Utils.h"
#include "BaseObject.h"

#define GRAVITY_SPEED 0.8 // tốc độ rơi
#define MAX_FALL_SPEED 20 // tốc độ rơi tối đa
#define PLAYER_SPEED 5 // tốc độ của nhân vật
#define PLAYER_JUMP_VAL 10 // độ cao của một lần nhảy

class Player : public BaseObject 
{
public:
    Player();
	~Player();

	enum WalkType {
		WALK_NONE = 0,
		WALK_RIGHT = 1,
		WALK_LEFT = 2,
		PUSH = 3
	};

    bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);
    void HandleInputAction(SDL_Event events, SDL_Renderer* screen, Mix_Chunk* jump_sound); 

    void set_clips();

	void DoPlayer(Map& map_data);

	void CheckToMap(Map& map_data);

	void SetMapXY(const int map_x, const int map_y) {map_x_ = map_x; map_y_ = map_y;};

	void CenterEntityOnMap(Map& map_data);

	void UpdateImagePlayer(SDL_Renderer* des);

	void setCamera(SDL_Rect& camera, Map& map_data);

	void render(SDL_Rect& camera);

	bool temp;

	bool isVictory();
private:
	double x_val_; 
	double y_val_; 

	double x_pos_; 
	double y_pos_; 

	int width_frame_;
	int height_frame_;

	SDL_Rect frame_clip_[8];

	InputMovement input_type_; 

	int frame_;

	int status_; 

	bool on_ground_;

	int map_x_; // Lưu vị trí của bản đồ ( có thể kiểm tra xem khi nhân vật di chuyển, mép mới của bản đồ ở vị trí nào )
	int map_y_; // Lưu vị trí của bản đồ ( có thể kiểm tra xem khi nhân vật di chuyển, mép mới của bản đồ ở vị trí nào )

	double time;

	int unablePush;
};



#endif