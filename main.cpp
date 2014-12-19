#include <iostream>
#include <ctime>
#include <cstdlib>
#include <locale>

#include "game.hpp"

int main(int argc, char *argv[])
{
	setlocale(LC_CTYPE,"");
	srand(time(NULL));
	
	if (argc != 5) {
		std::cout << "usage: ./prog p1 p2 p3 p4" << std::endl;
		return EXIT_SUCCESS;
	}
	
	Game g(argv);
	
	g.startGame();
	
	return EXIT_SUCCESS;
}
