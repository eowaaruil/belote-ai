#include "card.hpp"

std::string suitToString(int suit)
{
	if (suit == CLUBS)
		return "clubs";
	else if (suit == HEARTS)
		return "hearts";
	else if (suit == DIAMOND)
		return "diamond";
	else if (suit == SPADES)
		return "spades";
	else
		return "invalid suit";
}

////////// to be deleted (or not)
//////////
std::string card_back = "\U0001F0A0";
std::string card_clubs[13] = {"\U0001F0D7", "\U0001F0D8", "\U0001F0D9", "\U0001F0DB", "\U0001F0DD", "\U0001F0DE", "\U0001F0DA", "\U0001F0D1"};
std::string card_hearths[13] = {"\U0001F0B7", "\U0001F0B8", "\U0001F0B9", "\U0001F0BB", "\U0001F0BD", "\U0001F0BE", "\U0001F0BA", "\U0001F0B1"};
std::string card_diamonds[13] = {"\U0001F0C7", "\U0001F0C8", "\U0001F0C9", "\U0001F0CB", "\U0001F0CD", "\U0001F0CE", "\U0001F0CA", "\U0001F0C1"};
std::string card_spades[13] = {"\U0001F0A7", "\U0001F0A8", "\U0001F0A9", "\U0001F0AB", "\U0001F0AD", "\U0001F0AE", "\U0001F0AA", "\U0001F0A1"};

void printCard(std::ostream& s, Card c)
{
	if (c.suit == CLUBS)
		s << card_clubs[c.value] << ' ';
	else if (c.suit == HEARTS)
		s << card_hearths[c.value] << ' ';
	else if (c.suit == DIAMOND)
		s << card_diamonds[c.value] << ' ';
	else if (c.suit == SPADES)
		s << card_spades[c.value] << ' ';
	else
		s << card_back << ' ';
}
//////////
//////////

Card::Card(int suit, int value)
{
	this->suit = suit;
	this->value = value;
}

Card::Card(const Card &c)
{
	*this = c;
}

Card& Card::operator=(const Card &c)
{
	suit = c.suit;
	value = c.value;
	return *this;
}

bool Card::operator==(Card const& c)
{
	return value == c.value && suit == c.suit;
}

int Card::getScore(int trump) const
{
	if (suit == trump)
		return trump_score[getTrumpValue()];
	else
		return suit_score[value];
}

int Card::getTrumpValue() const
{
	if (value == CJ)
		return TJ;
	else if (value == C9)
		return T9;
	else if (value == CA)
		return TA;
	else if (value == C10)
		return T10;
	else if (value == CK)
		return TK;
	else if (value == CQ)
		return TQ;
	else if (value == C8)
		return T8;
	else if (value == C7)
		return T7;
	else
		return INVALID_VALUE;
}

