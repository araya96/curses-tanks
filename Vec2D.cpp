#include <cmath>
#include <iostream>
#include "Vec2D.hpp"
#include "curses.h"

using namespace std;

Vec2D::Vec2D(double x, double y)
{
	line = y;
	column = x;

}

void Vec2D::IntTimesVec2D(Vec2D & v, int s)
{
	v.vec.at(0) = (int)(v.vec.at(0) * s);
	v.vec.at(1) = (int)(v.column * s);
 }
void Vec2D::DoubleTimesVec2D(Vec2D & v, double d) 
{

}
void Vec2D::Vec2DPlusVec2D(Vec2D & v, Vec2D & v2)
{

}