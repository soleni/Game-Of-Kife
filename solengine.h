#pragma once

#include <GL/glut.h>

#include <thread>
#include <vector>
#include <algorithm>

#include <iostream>

namespace solengine 
{
	using dot = std::pair<uint16_t, uint16_t>;
	using dots = std::vector<dot>;

	solengine::dots _active_dots;
	solengine::dots _reserve_dots;

	double d = 1;

	void render_func()
	{
		glBegin(GL_POINTS);
		glColor3f(1.0, 1.0, 1.0); // draw
		glVertex2d(d, d); //use reserve buffer
		glEnd();

		glutSwapBuffers();
		//swap dots buffer here

		glBegin(GL_POINTS);
		glColor3f(0.0, 0.0, 0.0); //clear
		glVertex2d(d - 2, d - 2); // use reserve buffer too
		glEnd();

		for (int i = 0; i < 1000000000; ++i)
			++i;

		d += 2;

		glutPostRedisplay();
	}

	void init_solengine(int argc, char** argv, uint16_t x, uint16_t y)
	{
		std::thread render_thread([&]()
		{
			glutInit(&argc, argv);
			glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
			glutInitWindowSize(x, y);
			glutInitWindowPosition(0, 0);
			glutCreateWindow("GameOfKife");
			glutDisplayFunc(render_func);

			glPointSize(1.0);
			gluOrtho2D(0, x, 0, y);

			glutMainLoop();
		});
		render_thread.join();
	}

	void update_dots()
	{
		std::swap(_reserve_dots, _active_dots);
	}
}