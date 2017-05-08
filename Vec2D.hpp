#pragma once
#include <vector>

class Vec2D
{
public:
	double column;
	double line;

	std::vector<double> vec;
	Vec2D();
	Vec2D(double x, double y);
	Vec2D operator*(const int);
	Vec2D operator*(double d);
	Vec2D operator+(Vec2D & v2);
	Vec2D operator/(int);
};
