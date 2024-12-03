all:
	g++ -I include -L include/lib -o main src/Button.cpp src/Player.cpp src/main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lole32 -lshell32 -lcomctl32 -loleaut32 -luuid -mwindows