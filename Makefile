CFLAGS=-std=c++11 -Wall -Wextra -g
PLAYER_NAME=b

all: ai compil

ai: card.oo state.oo
	g++ $(CFLAGS) -DPLAYER_NAME=$(PLAYER_NAME) -fPIC -shared -o $(PLAYER_NAME).so ai.cpp $^

card.oo: card.cpp card.hpp
	g++ $(CFLAGS) -c -fPIC -o card.oo card.cpp

state.oo: state.cpp state.hpp
	g++ $(CFLAGS) -c -fPIC -o state.oo state.cpp

compil: main.o card.o game.o state.o player.o
	g++ $(CFLAGS) -rdynamic $^ -ldl

state.o: state.cpp state.hpp
	g++ $(CFLAGS) -c $^

card.o: card.cpp card.hpp
	g++ $(CFLAGS) -c $^

game.o: game.hpp game.cpp
	g++ $(CFLAGS) -c $^

player.o: player.hpp player.cpp
	g++ $(CFLAGS) -c $^

main.o: main.cpp
	g++ $(CFLAGS) -c $^

test.o: test.cpp
	g++ $(CFLAGS) -c $^

test: card.o game.o state.o player.o test.o
	g++ $(CFLAGS) $^ -ldl -o tests

clean:
	@rm *~ a.out *.o *.oo tests
