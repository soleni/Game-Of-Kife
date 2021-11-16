#include "solengine.h"

#include <iostream>

#include <GL/glut.h>

int main(int argc, char** argv) 
{
	solengine::init_solengine(argc, argv, 500, 500);

	while (true)
	{
		int x, y;
		std::cin >> x >> y;

		solengine::_reserve_dots[0] = { x, y };
		solengine::_reserve_dots[1] = { -1, -1 };
		solengine::update_dots();
	}

	return 0;
}