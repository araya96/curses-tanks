#pragma once
#include <vector>

struct Player
{
	void Initialize();
	void Draw(std::vector<int> ground);
	double angle;
	double power;
	int position;
	int hits;
};