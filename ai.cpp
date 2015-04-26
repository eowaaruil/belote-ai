#include <iostream>

#include "state.hpp"
#include "player.hpp"

/**
 * Rules
 * 1: An AI that play an invalid card is disqualified
 * 2: You have n(5-10?) seconds to compute your turn, after, you're disqualified (clock ticks ? another mesurement unit ?)
 * 3: No parallel computing
 * 4: 
 * 5: The use of standard I/O is forbidden
 */

class PLAYER_NAME : public Player
{
public:
	void init();
	bool firstBid(Card c);
	int secondBid(Card c);
	Card playCard();
	void endRound();
	virtual ~PLAYER_NAME();
	
private:
};

#ifdef __cplusplus
extern "C"
{
#endif
	// Double indirection needed to generate a function name
	#define MAKE_PLAYER_FUNCTION(x) Player* getPlayer ## x ()
	#define PLAYER_FUNCTION(x) MAKE_PLAYER_FUNCTION(x)
	
	PLAYER_FUNCTION(PLAYER_NAME)
	{
		return new PLAYER_NAME();
	}
#ifdef __cplusplus
}
#endif

void PLAYER_NAME::init()
{
	// This function is called once at the program start, use it to initialize your variables and structures
}

bool PLAYER_NAME::firstBid(Card c)
{
	// Test if the player has J and 9
	bool n(false), j(false);
	if (c.value == CJ)
		j = true;
	if (c.value == C9)
		n = true;
	
	for (auto h = hand.begin(); h != hand.end(); h++)
	{
		if (h->suit == c.suit)
		{
			if (h->value == CJ)
				j = true;
			if (h->value == C9)
				n = true;
		}
	}
	
	return j && n;
}

int PLAYER_NAME::secondBid(Card c)
{
	bool n(false), j(false);
	int suit = INVALID_SUIT;
	
	for (int i = 0; i < NB_SUITS; i++)
	{
		n = j = false;
		if (i != c.suit)
		{
			for (auto h = hand.begin(); h != hand.end(); h++)
			{
				if (h->suit == i && h->value == CJ)
					j = true;
				if (h->suit == i && h->value == C9)
					n = true;
			}
		}
		if (n && j)
			suit = i;
	}
	
	return suit;
}

Card PLAYER_NAME::playCard()
{
	Card c(INVALID_SUIT, INVALID_VALUE);
	
	if (isFirstToPlay())
		return hand.front();
	
	int asked_suit = state.turn[state.first_to_play].suit;
	int winning_player = state.playerWinningFold();
	
	// if (isOpponent(winning_player))
		// asked_suit = state.turn[winning_player].suit;
	
	if (asked_suit == state.trump_suit)
	{
		for (auto i = hand.begin(); i != hand.end(); i++)
		{
			if (i->suit == asked_suit)
			{
				if (i->getTrumpValue() > state.turn[winning_player].getTrumpValue())
					return *i;
			}
		}
	}
	for (auto i = hand.begin(); i != hand.end(); i++)
	{
		if (i->suit == asked_suit)
			return *i;
	}
	if (isOpponent(state.playerWinningFold())) {
		for (auto i = hand.begin(); i != hand.end(); i++) {
			if (i->suit == state.trump_suit)
				return *i;
		}
	}
	
	return hand.front();
}

void PLAYER_NAME::endRound()
{
	// Can do additional calculation when the turn ended
}

PLAYER_NAME::~PLAYER_NAME()
{
}

