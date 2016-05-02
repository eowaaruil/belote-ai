#ifndef _CARTE_HPP_
#define _CARTE_HPP_

#include <iostream>

enum {INVALID_SUIT = -1, CLUBS = 0, HEARTS = 1, DIAMOND = 2, SPADES = 3, NB_SUITS = 4};
#define INVALID_VALUE -1
enum {T7, T8, TQ, TK, T10, TA, T9, TJ, NB_TRUMPS};		// Trump
const int trump_score[] = {0, 0, 3, 4, 10, 11, 14, 20};
enum {C7, C8, C9, CJ, CQ, CK, C10, CA, NB_VALUES};		// Normal suits
const int suit_score[] = {0, 0, 0, 2, 3, 4, 10, 11};

std::string suitToString(int suit);

#define NB_CARDS		32

class Card
{
public:
	Card(){}
	Card(int suit, int value);
	Card(const Card &c);
	Card& operator=(const Card& c);
	bool operator==(Card const& c);
	
	int getScore(int trump) const;
	int getTrumpValue() const;
	
public:
	int suit;
	int value;
	
private:
	
};

void printCard(std::ostream& s, Card c);

#endif
