#pragma once
#include <vector>

struct Vec2D
{
	double column;
	double line;

	std::vector<double> vec;

	Vec2D(double x, double y);
	void IntTimesVec2D(Vec2D & v, int s);
	void DoubleTimesVec2D(Vec2D & v, double d);
	void Vec2DPlusVec2D(Vec2D & v, Vec2D & v2);
};
