#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <cmath>
#include <sstream>
#include <Windows.h>

#if defined(WIN32)
#include "curses.h"
#endif

using namespace std;

// determines the landscape
void ComputeGround(vector<int> & ground)
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
void DrawGround(vector<int> ground)
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

struct Player
{
	void Initialize();
	void Draw(vector<int> ground);
	double angle;
	double power;
	int position;
	int hits;
};

void Player::Initialize()
{
	position = hits = 0;
	angle = 45.0;
	power = 50.0;
}

// draws a player on the landscape
void Player::Draw(vector<int> ground)
{
	move(ground.at(position) - 1, position + 1);
	addch('o');
}

// sets up display of stats
void Display(Player * players, int turn)
{
	// highlights player whose turn it is
	if (turn == 0)
	{
		attron(A_REVERSE);
	}

	move(1, 2);
	addstr("Player 1");

	if (turn == 0)
	{
		attroff(A_REVERSE);
	}

	move(2, 2);
	stringstream ss;
	ss << "Angle: " << players[0].angle;
	addstr(ss.str().c_str());

	move(3, 2);
	ss = stringstream();
	ss << "Power: " << players[0].power;
	addstr(ss.str().c_str());

	move(4, 2);
	ss = stringstream();
	ss << "Hits: " << players[0].hits;
	addstr(ss.str().c_str());

	if (turn == 1)
	{
		attron(A_REVERSE);
	}

	move(1, COLS - 11);
	addstr("Player 2");

	if (turn == 1)
	{
		attroff(A_REVERSE);
	}

	move(2, COLS - 11);
	ss = stringstream();
	ss << "Angle: " << players[1].angle;
	addstr(ss.str().c_str());

	move(3, COLS - 11);
	ss = stringstream();
	ss << "Power: " << players[1].power;
	addstr(ss.str().c_str());

	move(4, COLS - 11);
	ss = stringstream();
	ss << "Hits: " << players[1].hits;
	addstr(ss.str().c_str());
}

// asks if player wants to restart game
void GameOver(bool & keep_going)
{
	bool asking = true;

	while (asking)
	{
		clear();
		move(13, 37);
		addstr("You died.  Do you want to play again? (y/n)");
		char answer = getch();

		if (answer == 'n')
		{
			keep_going = false;
			asking = false;
		}
		else if (answer == 'y')
		{
			asking = false;
		}
	}

	clear();
}

// uses arrow keys to adjust settings
void ProcessKeyboard(Player * players, int turn, int key)
{
	switch (key)
	{
	case KEY_UP:
		players[turn].power++;
		if ((players[turn].power > 90.0))
		{
			players[turn].power = 90.0;
		}
		break;

	case KEY_DOWN:
		players[turn].power--;
		if ((players[turn].power < 0.0))
		{
			players[turn].power = 0.0;
		}
		break;

	case KEY_LEFT:
		players[turn].angle--;
		if ((players[turn].angle < 0.0))
		{
			players[turn].angle = 0.0;
		}
		break;

	case KEY_RIGHT:
		players[turn].angle++;
		if ((players[turn].angle > 90.0))
		{
			players[turn].angle = 90.0;
		}
		break;

	default:
		break;
	}
}

// checks 9 spots around player for hit
void DetectHit(Player * players, double pNx, double pNy, bool & hit)
{
	if ((pNx >= players[0].position - 1) && (pNx <= players[0].position + 1))
	{
		if ((pNy >= players[0].position - 1) && (pNy <= players[0].position + 1))
		{
			players[0].hits++;
			hit = true;
		}
	}

	if ((pNx >= players[1].position - 1) && (pNx <= players[1].position + 1))
	{
		if ((pNy >= players[1].position - 1) && (pNy <= players[1].position + 1))
		{
			players[1].hits++;
			hit = true;
		}
	}
}

// fires at opponent
void Shoot(vector<int> & ground, Player * players, int turn)
{
	bool hit = false;
	// converts degrees to radians
	double angle = players[turn].angle / 180.0 * M_PI;
	// calculates velocities
	double vy = sin(angle) * players[turn].power * 0.2;
	double vx = cos(angle) * players[turn].power * 0.2;
	// sets initial player position
	double p0x = players[turn].position;
	double p0y = ground.at(players[turn].position);
	// higher ground numbers are lower altitudes (0 is first line, etc).
	p0y = LINES - p0y;

	// flips direction for player 2
	if (turn == 1)
	{
		vx = -vx;
	}

	// updates position
	for (int i = 1; i < 5000; i++)
	{
		// loops through time steps
		double step = i / 5.0;
		// curr pos = init pos + (time step * vel) + ((time step^2 + time step) * grav)/2
		double pNx = p0x + (step * vx);
		double pNy = p0y + (step * vy) + ((step * step + step) * -0.98) / 2.0;
		pNy = LINES - pNy;

		DetectHit(players, pNx, pNy, hit);

		// reset landscape and player positions after hits
		if (hit)
		{
			clear();
			ComputeGround(ground);
			players[0].position = rand() % 10 + 10;
			players[1].position = rand() % 10 + COLS - 20;
			break;
		}

		// boundary checking
		if (pNx < 1 || pNx >= COLS - 2)
		{
			clear();
			break;
		}

		// stops when shot lands
		if (pNy >= ground.at((int)pNx))
		{
			clear();
			break;
		}

		// adds stars along trajectory
		move((int)pNy - 1, (int)pNx + 1);

		// only add stars when trajectory is visible
		if (pNy > 2)
		{
			addch('*');
		}

		refresh();
		// shot happens incrementally
		Sleep(30);
	}
}

int main()
{
	// setting up curses
	initscr();
	noecho();
	curs_set(0);
	nodelay(stdscr, 1);
	keypad(stdscr, true);

	// initializing things
	srand((unsigned int)time(nullptr));
	vector<int> ground(COLS - 2);
	ComputeGround(ground);
	Player players[2];
	players[0].Initialize();
	players[1].Initialize();
	players[0].position = rand() % 10 + 10;
	players[1].position = rand() % 10 + COLS - 20;
	int turn = 0;
	bool keep_going = true;

	// gameplay loop
	while (keep_going)
	{
		int key = getch();

		// drawing screen
		border(0, 0, 0, 0, 0, 0, 0, 0);
		DrawGround(ground);
		players[0].Draw(ground);
		players[1].Draw(ground);
		Display(players, turn);

		ProcessKeyboard(players, turn, key);

		// press enter to shoot
		if (key == 10)
		{
			Shoot(ground, players, turn);
			turn = 1 - turn;
		}

		// checks if players are out of lives
		if (players[0].hits > 2 || players[1].hits > 2)
		{
			GameOver(keep_going);

			// resets for new game
			if (keep_going)
			{
				turn = 0;
				ComputeGround(ground);
				players[0].Initialize();
				players[1].Initialize();
				players[0].position = rand() % 10 + 10;
				players[1].position = rand() % 10 + COLS - 20;
			}
		}
	}

	endwin();
	return 0;
}