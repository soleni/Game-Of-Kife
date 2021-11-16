#include <GL/glut.h>

#include "solengine.h"

int main(int argc, char** argv) 
{
	solengine::init_solengine(argc, argv, 500, 500);

	return 0;
}