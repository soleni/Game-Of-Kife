#pragma once

#include <GL/glut.h>

#include <thread>
#include <vector>
#include <algorithm>
#include <atomic>

namespace solengine 
{
	using dot = std::pair<int16_t, int16_t>;
	using dots = std::vector<dot>;

	solengine::dots _active_dots;
	solengine::dots _reserve_dots;

	double d = 1;

	void render_func()
	{
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_POINTS);
		#pragma omp for 
			for (int i = 0 ; i < _reserve_dots.size() && _reserve_dots[i].first >= 0; ++i)
				glVertex2d(_reserve_dots[i].first, _reserve_dots[i].second);
		glEnd();

		glutSwapBuffers();

		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_POINTS);
		#pragma omp for 
			for (int i = 0; i < _active_dots.size() && _active_dots[i].first >= 0; ++i)
				glVertex2d(_active_dots[i].first, _active_dots[i].second);
		glEnd();

		glutPostRedisplay();
	}

	void init_solengine(int argc, char** argv, int16_t x, int16_t y)
	{
		_active_dots.resize(x * y);
		_reserve_dots.resize(x * y);

		std::atomic<bool> glut_inited{ false };
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
			
			glut_inited.store(true, std::memory_order_relaxed);

			glutMainLoop();
		});
		while (!glut_inited.load(std::memory_order_relaxed));
		render_thread.detach();
	}

	void update_dots()
	{
		std::swap(_reserve_dots, _active_dots);
	}
}