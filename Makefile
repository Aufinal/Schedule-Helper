CC=g++
CFLAGS = -Wall -Werror -std=c++11 -g
LDFLAGS =

EXEC=schedule_helper

SRC = $(wildcard C_source/*.cpp)
OBJ = $(SRC:.cpp=.o)

all: $(EXEC)

schedule_helper: $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	@$(CC) $(CFLAGS) -o $@ -c $< 

.PHONY: clean cleanall

clean:
	@rm -rf C_source/*.o

cleanall: clean
	@rm -rf $(EXEC)
	
