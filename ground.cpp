#include "curses.h"
#include "ground.hpp"
#include <vector>
#include <cstdlib>


int base_height_divisor = 8;
int max_height_divisor = 2;

void Ground::Compute()
{
	int current_height = LINES - LINES / base_height_divisor;
	//int maximum_height = lines / max_height_divisor;

	for (int i = 1; i < COLS - 1; i++) {
		int h = current_height;
		int r = rand() % 10000;
		if (r < 800) {
			current_height++;
			if (current_height >= LINES - 2)
				current_height = LINES - 2;
		}
		else if (r < 1600) {
			current_height--;
			if (current_height < 1)
				current_height = 1;
		}
		ground.push_back(h);
	}
	
}

// prints landscape with line drawing characters
void Ground::Draw()
{
	for (size_t i = 0; i < ground.size(); i++)
		{
			move(ground.at(i), (int)i + 1);

			// first int dealt with separately
			if (i == 0)
			{
				addch(ACS_HLINE);
			}
			// adds corners where appropriate
			else
			{
				if (ground.at(i) < ground.at(i - 1))
				{
					addch(ACS_ULCORNER);
				}
				else if (ground.at(i) > ground.at(i - 1))
				{
					addch(ACS_LLCORNER);
				}
				else
				{
					addch(ACS_HLINE);
				}
			}
		}
}

