
# SDL Flags
SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs)

# Compiler settings 
CC = clang++  # either g++ or clang++

ifeq ($(OS), Windows_NT)
	BASE_CXXFLAGS = -std=c++14 -Wall
	CXXFLAGS = -c $(BASE_CXXFLAGS) -I /c/SDL2/SDL2-2.26.1/x86_64-w64-mingw32/include -I ./ $(SDL_CFLAGS)
	LDFLAGS =  -L /c/SDL2/SDL2-2.26.1/x86_64-w64-mingw32/lib -static-libstdc++ $(SDL_LDFLAGS)
	BUILD = build/win32
else
	BASE_CXXFLAGS = -std=c++14 -Wall
	CXXFLAGS = -c $(BASE_CXXFLAGS) $(SDL_CFLAGS)
	LDFLAGS = $(SDL_LDFLAGS)
	BUILD = build/linux
endif	

# Makefile settings 
APPNAME = game
SRC = src
OBJ = obj
TEST = tests


OBJ_LIST = $(OBJ)/game.o $(OBJ_ALL_BUT_MAIN_LIST)
OBJ_ALL_BUT_MAIN_LIST = $(OBJ_root_LIST) $(OBJ_core_LIST) 
OBJ_root_LIST= $(OBJ)/preprocessor.o $(OBJ)/Globals.o 
OBJ_core_LIST= $(OBJ)/Game_Loop.o 


UNIT_TEST_LIST = $(TEST)/test_bitmap_font.cpp

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
debug: BASE_CXXFLAGS += -g
debug: CXXFLAGS += -O0 -fno-inline-functions -D_GLIBCXX_DEBUG
debug: clean  # comment this out after finished with header only BitmapFont library
debug: copy-assets 
debug: $(BUILD)/game
debug: $(TEST)/test
debug: 
	tests/test

.PHONY: clean 
clean:
	$(info )
	$(info ***** Purging object files and profiler data *****)
	rm -f $(OBJ)/*.o 
	rm -f $(BUILD)/game.gif
	rm -f $(BUILD)/*.prof
	rm -f $(BUILD)/*.callgrind
	rm -f $(TEST)/test

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

$(TEST)/test: $(UNIT_TEST_LIST)
	$(info )
	$(info ***** Compiling Test Scripts *****)
	$(CC) $(UNIT_TEST_LIST) $(OBJ_ALL_BUT_MAIN_LIST) -o $(TEST)/test $(LDFLAGS)


$(BUILD)/game: $(OBJ_LIST)
	$(info )
	$(info ***** Linking objects and building target *****)
	$(CC) $(BASE_CXXFLAGS) $(OBJ_LIST) -o $(BUILD)/$(APPNAME) $(LDFLAGS)


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

