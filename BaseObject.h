#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_
#include "Utils.h"
#include "Text.h"

class BaseObject 
{
public:
    BaseObject();
	~BaseObject();

	void SetRect(const int& x, const int& y) {rect_.x = x, rect_.y = y;}
	
	SDL_Rect GetRect() const {return rect_;}

	SDL_Texture* GetObject() const {return p_object_;}

	//Function to load image into p_object_
	virtual bool LoadImg(std::string path, SDL_Renderer* screen);

	void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);

	void Free();
protected:
	//Store image
	SDL_Texture* p_object_;
	
	//Store information of image
	SDL_Rect rect_;
};
#endif