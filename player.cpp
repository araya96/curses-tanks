#include "curses.h"
#include "player.hpp"

using namespace std;

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