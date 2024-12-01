#include "Player.h"
#include <iostream>
#include <windows.h>
#include <iostream>
#include <shobjidl.h> 

Player::Player() {
    isRunning = false;
    isPlaying = false;
    mouseDown = false;
    isSettingplayPosRatio = false;
    needsRerender = true;
    playPosRatio = 0;
    selectedButton = NULL;
	this->initWindow();
}

Player::~Player() {
}

void Player::initWindow(){
    window = SDL_CreateWindow( "LightPlayer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 400, 50, SDL_WINDOW_ALLOW_HIGHDPI);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if(window == NULL || renderer == NULL){
        SDL_Quit();	
    }

    texPlay =  IMG_LoadTexture(renderer, "assets/play.png");
    texPause = IMG_LoadTexture(renderer, "assets/pause.png");
    texDir = IMG_LoadTexture(renderer, "assets/folder.png");
    texBlank = IMG_LoadTexture(renderer, "assets/blank.png");

    if(texPlay== NULL || texPause== NULL || texBlank == NULL || texDir == NULL){
        SDL_Quit();
    }

    playButton = new Button(texPlay, 10, 10, 30, 30);
    playButton->onClick = clickPlay;
    dirButton = new Button(texDir, 50, 10, 30, 30);
    dirButton->onClick = clickBrowse;

    timeline1 = {90, 10, 0, 30};
    timeline2 = {90, 10, 300, 30};

    currentPlayPos = 0;
    isRunning = true;
}

void Player::update(){

    int newMouseX; int newMouseY;
	SDL_GetMouseState(&newMouseX,&newMouseY);

    Button* btn = getButtonAt(newMouseX, newMouseY);

    //mouse is no longer over the selectedButton
    if(selectedButton != NULL && selectedButton != btn) {
        selectedButton->onExit();
        selectedButton = NULL;
        selectedClick = NULL;
        needsRerender = true;
    }

    //mouse has moved to some new button
    if(btn != NULL && btn != selectedButton) {
        btn->onEnter();
        selectedButton = btn;
        needsRerender = true;
    }

	while (SDL_PollEvent(&ev))
		switch (ev.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;

		case SDL_MOUSEBUTTONUP:
            mouseDown = false;
            isSettingplayPosRatio = false;
            if(selectedClick != NULL){
                selectedClick->onClick();
            }
			break;

        case SDL_MOUSEBUTTONDOWN:
            mouseDown = true;
            if(isMouseOverPlaycontrol(newMouseX, newMouseY)){
                isPlaying = false;
                playButton->setTexture(texPlay);
                isSettingplayPosRatio = true;
            }
            if(selectedButton != NULL){
                selectedClick = selectedButton;
            }
			break;

		default:
			break;
	}

    if(isSettingplayPosRatio){
        double newRatio = getMousePlaycontrolPos(newMouseX, newMouseY);
        if(newRatio != playPosRatio){
            needsRerender = true;
            playPosRatio = newRatio;
            updateTimeline();
        }
    }

    //playPosRatio = ((double) currentPlayPos) / maxPlayPos;

}

void Player::render(){
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texBlank, NULL, NULL);

    SDL_RenderCopy(renderer, playButton->texture, NULL, &playButton->buttonFrame);

    SDL_RenderCopy(renderer, dirButton->texture, NULL, &dirButton->buttonFrame);

    SDL_SetTextureColorMod(texBlank, 140, 140, 140);
    SDL_RenderCopy(renderer, texBlank, NULL, &timeline1);
    SDL_SetTextureColorMod(texBlank, 0, 0, 0);
    SDL_RenderCopy(renderer, texBlank, NULL, &timeline2);
    SDL_SetTextureColorMod(texBlank, 255, 255, 255);

    SDL_RenderPresent(renderer);
    needsRerender = false;
}

Button* Player::getButtonAt(int x, int y){
    if(isButtonAt(x, y, playButton)){
        return playButton;
    }
    if(isButtonAt(x, y, dirButton)){
        return dirButton;
    }
    return NULL;
}


bool Player::isButtonAt(int x, int y, Button* button){
    return  button->buttonFrame.x < x && button->buttonFrame.x + button->buttonFrame.w >= x &&
            button->buttonFrame.y < y && button->buttonFrame.y + button->buttonFrame.h >= y;
}

bool Player::isMouseOverPlaycontrol(int x, int y){
    return  timeline1.x < x && timeline1.x + 300 >= x &&
            timeline1.y < y && timeline1.y + timeline1.h >= y;
}

double Player::getMousePlaycontrolPos(int x, int y){
    if(x > timeline1.x + 300)
        return 1;
    if(x < timeline1.x)
        return 0;
    return ((double)x - timeline1.x)/ 300;
}

void Player::updateTimeline(){
    timeline1.w = 300 * playPosRatio;
    timeline2.x = timeline1.x + timeline1.w;
    timeline2.w = 300 - timeline1.w;
}

void Player::clickPlay(){
    if(isPlaying){
        playButton->setTexture(texPlay);
    } else {
        playButton->setTexture(texPause);
    }
    isPlaying = !isPlaying;
    playButton->onEnter();
    needsRerender = true;
}

void Player::clickBrowse(){
     std::wstring filePath = OpenFileDialog();
}

std::wstring Player::OpenFileDialog()
{
    // Initialize COM Library
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr)) {
        std::wcerr << L"Failed to initialize COM library." << std::endl;
        return L"";
    }

    IFileOpenDialog* pFileOpen = nullptr;

    // Create the FileOpenDialog object.
    hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileOpen));
    if (FAILED(hr)) {
        std::wcerr << L"Failed to create FileOpenDialog instance." << std::endl;
        CoUninitialize();
        return L"";
    }

    // Show the Open dialog box.
    hr = pFileOpen->Show(NULL);
    if (SUCCEEDED(hr)) {
        // Get the file name from the dialog box.
        IShellItem* pItem = nullptr;
        hr = pFileOpen->GetResult(&pItem);
        if (SUCCEEDED(hr)) {
            PWSTR pszFilePath = nullptr;
            hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

            // Get the file path as a wstring and free the allocated memory.
            if (SUCCEEDED(hr)) {
                std::wstring filePath = pszFilePath;
                CoTaskMemFree(pszFilePath);
                pItem->Release();
                pFileOpen->Release();
                CoUninitialize();
                return filePath;
            }

            pItem->Release();
        }
    }

    // Clean up
    if (pFileOpen) pFileOpen->Release();
    CoUninitialize();

    return L"";
}