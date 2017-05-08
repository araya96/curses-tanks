#include "curses.h"
#include "ground.hpp"
#include <vector>

void Ground::Compute()
{
	int initHeight = 0;
	initHeight = rand() % 4 + (LINES - 6);

	// loops through ground and adds hills/valleys
	for (size_t i = 0; i < ground.size(); i++)
	{
		int delta = 0;
		int roll = rand() % 100;

		// landscape moves up or down 10% of time
		if (roll < 10)
		{
			delta = 1;
		}
		else if (roll < 20)
		{
			delta = -1;
		}

		// changes added after first int
		if (i == 0)
		{
			ground.at(i) = initHeight + delta;
		}
		else
		{
			ground.at(i) = ground.at(i - 1) + delta;
		}

		// boundaries set
		if (ground.at(i) >= LINES - 1)
		{
			--ground.at(i);
			delta = 0;
		}
		if (ground.at(i) < LINES - 15)
		{
			++ground.at(i);
			delta = 0;
		}
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
