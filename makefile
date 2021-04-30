#CC specifies which compiler we're using
CC = g++

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = $(shell sdl2-config --cflags)

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = $(shell sdl2-config --libs) 

# points to local include folder
LOCAL_INCLUDE_PATH = include

# points to source code folder
SRC_PATH = src

# points to objects folder
OBJ_PATH = $(SRC_PATH)/obj

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
	$(CC) $(OBJS) $(COMPILATION_ARGS) -o $@

##################################################################################

.PHONY: clean

clean:
	rm -f -r $(OBJ_PATH)