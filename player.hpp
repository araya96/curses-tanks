#pragma once
#include "ground.hpp"

struct Player
{
	void Initialize();
	void Draw(vector<int> ground);
	double angle;
	double power;
	int position;
	int hits;
};