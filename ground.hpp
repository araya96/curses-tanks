#pragma once
#include <vector>

struct Ground
{
	Ground();
	void Compute();
	void Draw();
	std::vector<int> ground;
};

Ground::Ground()
{
	vector<int> ground(COLS - 2);
}