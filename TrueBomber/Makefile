#OBJS specifies which files to compile as part of the project 
OBJS =*.cpp #main.cpp Map.cpp Player.cpp Texture.cpp Tile.cpp Bomb.cpp Game.cpp TextureTTF.cpp InputHandler.cpp MoveDown.cpp MoveUp.cpp MoveRight.cpp MoveLeft.cpp DropBomb.cpp Sound.cpp SetTrap.cpp
#CC specifies which compiler we're using 
CC = g++ 
#COMPILER_FLAGS specifies the additional compilation options we're using 
# -w suppresses all warnings 
COMPILER_FLAGS = -std=c++0x -W -Wall -g 
#LINKER_FLAGS specifies the libraries we're linking against 
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lmagic
#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = TrueBomber
#This is the target that compiles our executable 
all : $(OBJS) 
	  $(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
