#pragma once
#include <vector>
#include "ground.hpp"

struct Player
{
	void Initialize();
	void Draw(Ground & ground, char c);
	double angle;
	double power;
	int position;
	int hits;
};