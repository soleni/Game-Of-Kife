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

	uint8_t SCALE = 10;

	void render_func()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_POINTS);
		#pragma omp for 
			for (int i = 0 ; i < _active_dots.size() && _active_dots[i].first >= 0; ++i)
				glVertex3d(_active_dots[i].first, _active_dots[i].second, 0.0);
		glEnd();

		glutSwapBuffers();

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
			glutInitWindowSize(x * SCALE, y * SCALE);
			glutInitWindowPosition(0, 0);
			glColor3f(1.0, 1.0, 1.0);
			glutCreateWindow("GameOfKife");
			glutDisplayFunc(render_func);

			glPointSize(SCALE);
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