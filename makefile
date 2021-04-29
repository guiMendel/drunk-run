#CC specifies which compiler we're using
CC = g++

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -IC:\MinGW\sdl_libs\include\SDL2

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -LC:\MinGW\sdl_libs\lib

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2

#COMPILER_FLAGS specifies the additional compilation options we're using
COMPILER_FLAGS = -std=c++17 -Wall -Wextra -pedantic

# compilation args
COMPILATION_ARGS = $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS)

##################################################################################

#DEPS header files
DEPS = sdl.hpp

#OBJS specifies which files to compile as part of the project
OBJS = main.o sdl.o

##################################################################################

# how to compile object files
%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(COMPILATION_ARGS)

#This is the target that compiles our executable
drunk-run : $(OBJS)
	$(CC) $(OBJS) $(COMPILATION_ARGS) -o $@