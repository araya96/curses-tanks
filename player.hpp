#pragma once
#include <vector>
#include "ground.hpp"

struct Player
{
	void Initialize();
	void Draw(Ground & ground);
	double angle;
	double power;
	int position;
	int hits;
};