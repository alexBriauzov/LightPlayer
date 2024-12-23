#include "Player.h"
#include <iostream>

int main(int argv, char** args) {

	SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
	Player player;

	while(!player.isRunning){
		SDL_Delay(10);
	}
	player.render();
	player.render();
	while (player.isRunning) {
		SDL_Delay(16);
		player.update();
		if(player.needsRerender){
			player.render();
		}
	}
	SDL_Quit();	
	return 0;
}