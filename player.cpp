#include "curses.h"
#include "player.hpp"
#include "ground.hpp"

using namespace std;

void Player::Initialize()
{
	position = hits = 0;
	angle = 45.0;
	power = 50.0;
}

// draws a player on the landscape
void Player::Draw(Ground & ground)
{
	mvaddch(ground.ground.at(position) - 1, position + 1, '0');

}