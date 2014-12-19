#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include <list>

#include "state.hpp"

#define INVALID_PLAYER -1

class Player
{
public:
	virtual void init() = 0;
	virtual bool firstBid(Card c) = 0;
	virtual int secondBid(Card c) = 0;
	virtual void endRound() = 0;
	virtual Card playCard() = 0;
	virtual ~Player();
	
	bool isFirstToPlay() const;
	bool isOpponent(int player) const;
	bool isAlly(int player) const;
	State getState() const;
	void setName(std::string n);
	void setNum(int num);
	void setHand(std::list<Card>& h);
	std::list<Card> getHand() const;	// should not be used
	void cleanHand();
	bool hasSuit(int suit) const;
	
	State state;
protected:
	std::list<Card> hand;
	
protected:
	
	int player_num;
	std::string name;
};

#endif
