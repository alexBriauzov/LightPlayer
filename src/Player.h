#pragma once
#define NTDDI_VERSION 0x0A000006 //NTDDI_WIN10_RS5
#define _WIN32_WINNT 0x0A00 // _WIN32_WINNT_WIN10, the _WIN32_WINNT macro must also be defined when defining NTDDI_VERSION
#include "SDL2/SDL_video.h"
#include "SDL2/SDL_mouse.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "Button.h"
#include <string>

class Player
{
public:

    inline static bool isRunning;
    inline static bool isPlaying;
    inline static bool needsRerender;

    Player();
    virtual ~Player();

    void update();
	void render();

    inline static void clickPlay();

    inline static void clickBrowse();

    inline static std::wstring OpenFileDialog();

private:

    Mix_Music* gMusic = NULL;

    inline static Button* playButton;
    inline static Button* dirButton;

    Button* selectedButton;
    Button* selectedClick;

    SDL_Rect timeline1;
    SDL_Rect timeline2;

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event ev;

    inline static SDL_Texture* texPlay = NULL;
    inline static SDL_Texture* texPause = NULL;
    SDL_Texture* texDir = NULL;
    SDL_Texture* texBlank = NULL;

    void initWindow();

    Button* getButtonAt(int x, int y);

    bool isButtonAt(int x, int y, Button* button);

    bool isMouseOverPlaycontrol(int x, int y);

    double getMousePlaycontrolPos(int x, int y);

    void updateTimeline();

    int currentPlayPos;
    int maxPlayPos;
    double playPosRatio;

    bool mouseDown;
    bool isSettingplayPosRatio;

};