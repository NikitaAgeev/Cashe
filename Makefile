VERSION 	= 1.00
CXX			= g++
DEBUG 		= -g
CXXFLAGS	= -Wall -Wextra -Wpedantic -O2
SOURCES	 	=$(wildcard ./Code/Src/*.cpp)
INCLUDES  	= -I ./Code/Inc
LIB_NAMES 	=
LIB_PATH 	= -lm
OBJ			=$(patsubst %.cpp, %.o, $(SOURCES))
TARGET		= ceshe_

#links
$(TARGET):$(OBJ)
	@mkdir -p progect
	$(CXX) $(OBJ) $(LIB_PATH) $(LIB_NAMES) -o ./progect/$(TARGET)$(VERSION)
	@rm -rf $(OBJ)
	
#compile
%.o: %.cpp
	$(CXX) $(DEBUG) -c $(CXXFLAGS) $< -o $@ $(INCLUDES)

.PHONY:clean
clean:
	@echo "Remove linked and compiled files......"
	rm -rf $(OBJ) $(TARGET)$(VERSION) progect

