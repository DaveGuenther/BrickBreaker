
# SDL Flags
SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs)

# Compiler settings 
CC = clang++

ifeq ($(OS), Windows_NT)
	CXXFLAGS = -c -std=c++11 -Wall -I /c/SDL2/SDL2-2.26.1/x86_64-w64-mingw32/include -I ./ $(SDL_CFLAGS)
	LDFLAGS =  -L /c/SDL2/SDL2-2.26.1/x86_64-w64-mingw32/lib -static-libstdc++ $(SDL_LDFLAGS)
	BUILD = build/win32
else
	CXXFLAGS = -c -std=c++11 -Wall $(SDL_CFLAGS)
	LDFLAGS = $(SDL_LDFLAGS)
	BUILD = build/linux
endif	

# Makefile settings 
APPNAME = game
SRC = src
OBJ = obj


OBJ_LIST = $(OBJ)/game.o $(OBJ_root_LIST) $(OBJ_core_LIST) $(OBJ_texture_LIST)
OBJ_root_LIST= $(OBJ)/preprocessor.o $(OBJ)/Globals.o 
OBJ_core_LIST= $(OBJ)/Game_Loop.o 
OBJ_texture_LIST= $(OBJ)/Texture.o

all: release

.PHONY: list
list: 
	$(info Make options:)
	$(info make release			-	purges object files, builds fresh platform specific target files, and copies assets directory to build target)
	$(info make prof			-	compiles with profiler options and executes profiler... for linux only)
	$(info make debug			-	compiles with debug options)
	$(info clean				-	purges object files and removes executable file)
	$(info remove-executables		-	removes executable file)
	$(info copy-assets			-	purges target assets directory and replaces with assets directory at root of repository)

.PHONY: release
release: clean
release: remove-executable
release: $(BUILD)/game
release: copy-assets
	rm -f $(OBJ)/*.o 

.PHONY: prof
prof: LDFLAGS += -lprofiler -ltcmalloc
prof: remove-executable
prof: debug
prof:
	bash profile.sh


.PHONY: debug
debug: CXXFLAGS += -O0 -fno-inline-functions -g -D_GLIBCXX_DEBUG
debug: $(BUILD)/game

.PHONY: clean 
clean:
	$(info )
	$(info ***** Purging object files and profiler data *****)
	rm -f $(OBJ)/*.o 
	rm -f $(BUILD)/game.gif
	rm -f $(BUILD)/*.prof
	rm -f $(BUILD)/*.callgrind

.PHONY: copy-assets
copy-assets:
	$(info )
	$(info ***** Purging assets in $(BUILD)/assets directory *****)
	rm -rf $(BUILD)/assets
	$(info ***** Copying new assets to $(BUILD)/assets directory *****)
	cp -r assets $(BUILD)

.PHONY: remove-executable 
remove-executable:
	rm -f $(BUILD)/game

$(BUILD)/game: $(OBJ_LIST)
	$(info )
	$(info ***** Linking objects and building target *****)
	$(CC) -std=c++11 -Wall -g $(OBJ_LIST) -o $(BUILD)/$(APPNAME) $(LDFLAGS)


$(OBJ)/game.o: $(SRC)/main.cpp $(SRC)/core/Game_Loop.cpp $(SRC)/core/Game_Loop.h  
	$(info )
	$(info ***** Building game.o *****)
	$(CC) $(CXXFLAGS) $(SRC)/main.cpp -o $(OBJ)/game.o


Globals_SRC = $(SRC)/globals.cpp $(SRC)/globals.h
$(OBJ)/Globals.o: $(Globals_SRC)
	$(info )
	$(info ***** Building Globals.o *****)
	$(CC) $(CXXFLAGS) $(SRC)/globals.cpp -o $(OBJ)/Globals.o


Game_Loop_SRC = $(SRC)/core/Game_Loop.cpp $(SRC)/core/Game_Loop.h  
$(OBJ)/Game_Loop.o: $(Game_Loop_SRC)
	$(info )
	$(info ***** Building Game_Loop.o *****)
	$(CC) $(CXXFLAGS) $(SRC)/core/Game_Loop.cpp -o $(OBJ)/Game_Loop.o

$(OBJ)/preprocessor.o: $(SRC)/preprocessor.cpp 
	$(info )
	$(info ***** Building Crossplatform Preprocessor (preprocessor.o) *****)
	$(CC) $(CXXFLAGS) $(SRC)/preprocessor.cpp -o $(OBJ)/preprocessor.o

Texture_SRC = $(SRC)/texture/Texture.cpp $(SRC)/texture/Texture.h
$(OBJ)/Texture.o: $(Texture_SRC)
	$(info )
	$(info ***** Building texture.o *****)
	$(CC) $(CXXFLAGS) $(SRC)/texture/Texture.cpp -o $(OBJ)/Texture.o