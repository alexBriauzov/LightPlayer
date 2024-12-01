#pragma once
#include "SDL2/SDL_image.h"

class Button
{
public:
    Button(SDL_Texture* texture, int xPos, int yPos, int width, int height);
    virtual ~Button();

    void setTexture(SDL_Texture* texture);

    void onEnter();

    void onExit();

    void (*onClick)();

    SDL_Texture* texture = NULL;

    SDL_Rect buttonFrame;

private:

};