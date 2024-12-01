#include "Button.h"

Button::Button(SDL_Texture* tex, int x, int y, int w, int h) {
    texture = tex;
    buttonFrame.x = x;
	buttonFrame.y = y;
	buttonFrame.w = w;
	buttonFrame.h = h;
    SDL_SetTextureColorMod(texture, 0, 0, 0);
}

Button::~Button() {
}

void Button::setTexture(SDL_Texture* tex) {
    SDL_SetTextureColorMod(texture, 0, 0, 0);
    texture = tex;
    SDL_SetTextureColorMod(texture, 0, 0, 0);
}

void Button::onEnter(){
    SDL_SetTextureColorMod(texture, 140, 140, 140);
}

void Button::onExit(){
    SDL_SetTextureColorMod(texture, 0, 0, 0);
}
