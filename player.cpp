#include "player.hpp"

bool Player::isFirstToPlay() const
{
	return player_num == state.first_to_play;
}

bool Player::isOpponent(int player) const
{
	return (player_num + player) % 2;
}

bool Player::isAlly(int player) const
{
	return !isOpponent(player);
}

State Player::getState() const
{
	return state;
}

void Player::setName(std::string n)
{
	name = n;
}

void Player::setNum(int num)
{
	player_num = num;
}

void Player::setHand(std::list<Card>& h)
{
	hand = h;
}

std::list<Card> Player::getHand() const
{
	std::list<Card> c;
	return c;
}

void Player::cleanHand()
{
	hand.clear();
}

bool Player::hasSuit(int suit) const
{
	for (auto c = hand.begin(); c != hand.end(); c++)
	{
		if (c->suit == suit)
			return true;
	}
	
	return false;
}

Player::~Player()
{
}

