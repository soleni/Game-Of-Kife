#include "solengine.h"

#include <iostream>
#include <unordered_set>
#include <thread>
#include <chrono>

#include <GL/glut.h>

const uint8_t map_size = 100;

struct pair_hash 
{
	inline std::size_t operator()(const std::pair<int, int>& v) const 
	{
		return v.first * 1000 + v.second;
	}
};

int main(int argc, char** argv) 
{
	solengine::init_solengine(argc, argv, 100, 100);

	std::unordered_set<std::pair<int, int>, pair_hash> kife;
	std::unordered_set<std::pair<int, int>, pair_hash> reserve_kife;

	auto play_kife = [&]()
	{
		reserve_kife.clear();
		#pragma omp for
		for (auto const & [x,y] : kife)
		{
			auto resurrect = [&kife](int x, int y)
			{
				int neighbours = 0;
				for (int i = -1; i <= 1; ++i)
					for (int j = -1; j <= 1; ++j)
						if (kife.find({ x + i, y + j }) != kife.end())
							neighbours++;
				return neighbours == 3;
			};

			int neighbours = -1;
			for (int i = -1; i <= 1; ++i)
				for (int j = -1; j <= 1; ++j)
					if (kife.find({ x + i, y + j }) != kife.end()) //TODO negative values
						neighbours++;
					else if (resurrect(x + i, y + j))
						reserve_kife.emplace(x + i, y + j);


			if (neighbours == 2 || neighbours == 3)
				reserve_kife.emplace(x, y);
		}
		std::swap(reserve_kife, kife);
	};
	auto render_kife = [&]()
	{
		int i = 0;
		#pragma omp for
		for (auto& [x, y] : kife)
			solengine::_reserve_dots[i++] = { x, y };
		solengine::_reserve_dots[i++] = { -1, -1 };
		solengine::update_dots();
	};

	/* glider */
	kife.emplace(15, 15);
	kife.emplace(16, 16);
	kife.emplace(16, 17);
	kife.emplace(15, 17);
	kife.emplace(14, 17);
	

	/* blinker */
	kife.emplace(6, 7);
	kife.emplace(7, 7);
	kife.emplace(8, 7);
	

	/* beacon */
	kife.emplace(10, 10);
	kife.emplace(11, 10);
	kife.emplace(10, 11);
	kife.emplace(13, 13);
	kife.emplace(13, 12);
	kife.emplace(12, 13);
	

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		play_kife();
		render_kife();
	}

	return 0;
}