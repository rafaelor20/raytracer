NOME_PROJ=teste
CC=g++
FLAGS= -g -pg -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++20

SRC=$(wildcard *.cpp include/*.cpp)
OBJ_DIR=obj
OBJ=$(patsubst %.cpp,$(OBJ_DIR)/%.o,$(notdir $(SRC)))

all: $(NOME_PROJ)

$(NOME_PROJ): $(OBJ)
	$(CC) $(FLAGS) -o $(NOME_PROJ) $(OBJ) 

$(OBJ_DIR)/%.o: %.cpp
	$(CC) -c $< -o $@

$(OBJ_DIR)/%.o: include/%.cpp
	$(CC) -c $< -o $@

obj/olcPixelGameEngine.h.ghc: include/olcPixelGameEngine.h
	$(CC)  include/olcPixelGameEngine.h -o obj/olcPixelGameEngine.h.ghc

run: all
	./$(NOME_PROJ)