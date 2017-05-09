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

#include "player.hpp"
#include "ground.hpp"
#include "Vec2D.hpp"


using namespace std;

double ComputeWind()
{
	double wind = rand() % 100;
	wind = wind / 100;
	return wind;
}


// sets up display of stats
void Display(Player * players, int turn, double & wind)
{
	attron(COLOR_PAIR(1));
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

	move(1, COLS / 2);
	ss = stringstream();
	if (wind < 0)
	{
		ss << "Wind: " << wind << " m/s West";
		addstr(ss.str().c_str());
	}
	if (wind > 0)
	{
		ss << "Wind: " << wind << " m/s East";
		addstr(ss.str().c_str());
	}

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
void GameOver(bool & keep_going, int turn)
{
	bool asking = true;
	turn = turn + 1;
	stringstream ss;
	ss << "Player " << turn << " died.  Do you want to play again? (y/n)";
	while (asking)
	{
		clear();
		move(13, 37);
		addstr(ss.str().c_str());
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
	case 'r':
		clear();
		players[0].position = rand() % 50 + 1;;
		break;
	case 'l':
		clear();
		players[1].position = rand() % 50 + COLS - 53;
		break;

	default:
		break;
	}
}

// checks 9 spots around player for hit
void DetectHit(Player * players, Ground ground,  double pNx, double pNy, bool & hit, int i, double & wind)
{
	if (i > 1)
	{
		if ((pNx >= players[0].position - 1) && (pNx <= players[0].position + 1))
		{
			if ((pNy >= ground.ground.at(players[0].position) - 1) && (pNy <= ground.ground.at(players[0].position) + 1))
			{
				players[0].hits++;
				hit = true;
				wind = ComputeWind();
			}
		}

		if ((pNx >= players[1].position - 1) && (pNx <= players[1].position + 1))
		{
			if ((pNy >= ground.ground.at(players[1].position) - 1) && (pNy <= ground.ground.at(players[1].position) + 1))
			{
				players[1].hits++;
				wind = ComputeWind();
				wind = -wind;
				hit = true;
			}
		}
	}
}


// fires at opponent
void Shoot(Ground & ground, Player * players, int turn, double & wind)
{
	bool hit = false;

	// converts degrees to radians
	double angle = players[turn].angle / 180.0 * M_PI;

	// calculates velocities
	Vec2D v(cos(angle) * players[turn].power * 0.2, sin(angle) * players[turn].power * 0.2);
	
	// sets initial player position
	Vec2D p0(players[turn].position, ground.ground.at(players[turn].position+1));
	p0.line = LINES - p0.line;

	// flips direction for player 2
	if (turn == 1)
	{
		v.column = -v.column;
	}

	// updates position
	for (int i = 1; i < 5000; i++)
	{
		// loops through time steps
		double step = i / 5.0;

		Vec2D a(wind, -.98);
		Vec2D pN = p0 + (v * step) + (a * (step * step + step)) / 2;
		pN.line = LINES - pN.line;

		DetectHit(players, ground, pN.column, pN.line, hit, i, wind);

		// reset landscape and player positions after hits
		if (hit == true)
		{
			erase();
			ground.ground.resize(0);
			ground.Compute();
			players[0].position = rand() % 50 + 1;;
			players[1].position = rand() % 50 + COLS - 53;
			break;
		}

		// boundary checking
		if (pN.column < 1 || pN.column >= COLS - 2)
		{
			clear();
			break;
		}
		
		
		// stops when shot lands
		if (pN.line >= ground.ground.at((int)pN.column))
		{
			clear();
			break;
		}

		// adds stars along trajectory
		move((int)pN.line - 1, (int)pN.column + 1);

		// only add stars when trajectory is visible
		if (pN.line > 2 && turn == 0)
		{
			attron(COLOR_PAIR(3));
			addch('*');
		}
		if (pN.line > 2 && turn == 1)
		{
			attron(COLOR_PAIR(4));
			addch('*');
		}
		if (pN.line < 2)
		{
			Sleep(30);
			continue;
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
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_GREEN, COLOR_BLACK);
	// initializing things
	srand((unsigned int)time(nullptr));
	Ground ground;
	ground.Compute();
	Player players[2];
	players[0].Initialize();
	players[1].Initialize();
	players[0].position = rand() % 50 + 1;;
	players[1].position = rand() % 50 + COLS - 53;
	
	int turn = 0;
	bool keep_going = true;
	double wind = ComputeWind();
	
	// gameplay loop
	while (keep_going)
	{
		// drawing screen
		border(0, 0, 0, 0, 0, 0, 0, 0);
		ground.Draw();
		attron(COLOR_PAIR(3));
		players[0].Draw(ground, '#');
		attron(COLOR_PAIR(4));
		players[1].Draw(ground, 'O');
		Display(players, turn, wind);

		int key = getch();

		ProcessKeyboard(players, turn, key);

		// switch turn of person who used last resort key
		if (key == 'l')
		{
			turn = 1 - turn;
			continue;
		}
		if (key == 'r')
		{
			turn = 1 - turn;
			continue;
		}
		// press enter to shoot
		if (key == 10)
		{
			Shoot(ground, players, turn, wind);
			turn = 1 - turn;
		}

		// checks if players are out of lives
		if (players[0].hits > 2)
		{
			GameOver(keep_going, 0);

			// resets for new game
			if (keep_going)
			{
				turn = 0;
				erase();
				ground.ground.resize(0);
				ground.Compute();
				wind = ComputeWind();
				players[0].Initialize();
				players[1].Initialize();
				players[0].position = rand() % 50 + 1;
				players[1].position = rand() % 50 + COLS - 53;
			}
		}
		if (players[1].hits > 2)
		{
			GameOver(keep_going, 1);

			// resets for new game
			if (keep_going)
			{
				turn = 0;
				erase();
				ground.ground.resize(0);
				ground.Compute();
				wind = -ComputeWind();
				players[0].Initialize();
				players[1].Initialize();
				players[0].position = rand() % 50 + 1;;
				players[1].position = rand() % 50 + COLS - 53;
			}
		}
		refresh();
	}
	erase();
	addstr("Hit any key to exit");
	refresh();
	getch();
	echo();
	endwin();
	return 0;
}