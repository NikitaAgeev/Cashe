VERSION 	= 1.00
CC			= g++
DEBUG 		= -g
CFLAGS		= -Wall -Wextra -Wpedantic
SOURCES	 	=$(wildcard ./Code/Src/*.cpp)
INCLUDES  	= 
LIB_NAMES 	=
LIB_PATH 	= -lm
OBJ			=$(patsubst %.cpp, %.o, $(SOURCES))
TARGET		= kesh_

#links
$(TARGET):$(OBJ)
	@mkdir -p progect
	$(CC) $(OBJ) $(LIB_PATH) $(LIB_NAMES) -o ./progect/$(TARGET)$(VERSION)
	@rm -rf $(OBJ)
	
#compile
%.o: %.cpp
	$(CC) $(DEBUG) -c $(CFLAGS) $< -o $@ $(INCLUDES)

.PHONY:clean
clean:
	@echo "Remove linked and compiled files......"
	rm -rf $(OBJ) $(TARGET)$(VERSION) progect

