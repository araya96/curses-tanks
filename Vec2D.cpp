#include <cmath>
#include <iostream>
#include "Vec2D.hpp"
#include "curses.h"

using namespace std;

Vec2D::Vec2D()
{
	line = 0.0;
	column = 0.0;
}

Vec2D::Vec2D(double x, double y)
{
	line = y;
	column = x;
}

Vec2D Vec2D::operator*(const int s)  
{
	return Vec2D(column * s, line * s);
 }
Vec2D Vec2D::operator*(double d) 
{
	return Vec2D(column * d, line * d);
}
Vec2D Vec2D::operator+(Vec2D & v2)
{
	return Vec2D(column + v2.column, line + v2.line);
}
Vec2D Vec2D:: operator/(int s)
{
	return Vec2D(column / s, line / s);
}