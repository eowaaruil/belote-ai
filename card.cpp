#include "card.hpp"

std::wstring suitToString(int suit)
{
	if (suit == CLUBS)
		return L"clubs";
	else if (suit == HEARTS)
		return L"hearts";
	else if (suit == DIAMOND)
		return L"diamond";
	else if (suit == SPADES)
		return L"spades";
	else
		return L"invalid suit";
}

////////// to be deleted (or not)
//////////
wchar_t card_back = L'\U0001F0A0';
wchar_t card_clubs[13] = {L'\U0001F0D7',L'\U0001F0D8', L'\U0001F0D9', L'\U0001F0DB', L'\U0001F0DD', L'\U0001F0DE', L'\U0001F0DA', L'\U0001F0D1'};
wchar_t card_hearths[13] = {L'\U0001F0B7',L'\U0001F0B8', L'\U0001F0B9', L'\U0001F0BB', L'\U0001F0BD', L'\U0001F0BE', L'\U0001F0BA', L'\U0001F0B1'};
wchar_t card_diamonds[13] = {L'\U0001F0C7',L'\U0001F0C8', L'\U0001F0C9', L'\U0001F0CB', L'\U0001F0CD', L'\U0001F0CE', L'\U0001F0CA', L'\U0001F0C1'};
wchar_t card_spades[13] = {L'\U0001F0A7',L'\U0001F0A8', L'\U0001F0A9', L'\U0001F0AB', L'\U0001F0AD', L'\U0001F0AE', L'\U0001F0AA', L'\U0001F0A1'};

void printCard(std::wostream& s, Card c)
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

