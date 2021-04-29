#CC specifies which compiler we're using
CC = g++

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -IC:/MinGW/sdl_libs/include/SDL2

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -LC:/MinGW/sdl_libs/lib

# points to local include folder
LOCAL_INCLUDE_PATH = include

# points to source code folder
SRC_PATH = src

# points to objects folder
OBJ_PATH = $(SRC_PATH)/obj

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2

#COMPILER_FLAGS specifies the additional compilation options we're using
COMPILER_FLAGS = -std=c++17 -Wall -Wextra -pedantic

# compilation args
COMPILATION_ARGS = $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS)

##################################################################################

#DEPS header files
_DEPS = sdl.hpp
DEPS = $(patsubst %,$(LOCAL_INCLUDE_PATH)/%,$(_DEPS))

#OBJS specifies which files to compile as part of the project
_OBJS = main.o sdl.o
OBJS = $(patsubst %,$(OBJ_PATH)/%,$(_OBJS))

##################################################################################

# how to compile object files
# ensures there is an obj folder
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp $(DEPS)
	mkdir -p $(OBJ_PATH); $(CC) -c -o $@ $< $(COMPILATION_ARGS)

#This is the target that compiles our executable
drunk-run : $(OBJS)
	$(CC) $(OBJS) $(COMPILATION_ARGS) $(LINKER_FLAGS) -o $@

##################################################################################

.PHONY: clean

clean:
	rm -f -r $(OBJ_PATH)