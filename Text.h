#ifndef TEXT_H_
#define TEXT_H_
#include "Utils.h"
#include <string>

class Text
{
public:
    Text();
    ~Text();

    enum TextColor {
        BLACK = 2,
        WHITE = 1,
        RED = 0,
    };

    bool LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen);
    void Free();

    void SetColor(Uint8 red, Uint8 green, Uint8 blue);

    void SetColor(int type);

    void RenderText(SDL_Renderer* screen,
                    int xp, int yp,
                    SDL_Rect* clip = NULL,
                    double angle=0.0,
                    SDL_Point* center = NULL,
                    SDL_RendererFlip flip = SDL_FLIP_NONE);

    int getWidth() const { return width_; }
    int getHeight() const { return height_; }

    SDL_Rect GetRect() const {return renderQuad;}
    void SetText(const std::string& text) {textContent = text;}
    std::string GetText() const { return textContent; }

private:
    std::string textContent;
    SDL_Color textColor;

    SDL_Texture* texture_;

    SDL_Rect renderQuad;
    int width_;
    int height_;
};




#endif 