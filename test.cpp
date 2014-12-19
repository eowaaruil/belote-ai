#include <iostream>

#include "game.hpp"

class TestPlayer : public Player
{
public:
	TestPlayer(int num) {player_num = num;}
	void init() {}
	bool firstBid(Card c) {return c.suit==0;}
	int secondBid(Card c) {return c.suit;}
	void endRound() {}
	Card playCard() {return Card(0,0);}
	~TestPlayer(){}
private:
	
};

class TestGame : public Game
{
public:
	TestGame() {}
	void set(State &s, std::list<Card> a, std::list<Card> b, std::list<Card> c, std::list<Card> d){
		state = s;
		hands[0] = a;
		hands[1] = b;
		hands[2] = c;
		hands[3] = d;
	}
	void setPlayers(Player *p1, Player *p2, Player *p3, Player *p4) {players[0] = p1;players[1] = p2;players[2] = p3;players[3] = p4;}
};

int assertion(bool assert, std::string description)
{
	if (assert)
		return 0;
	
	std::cout << description << std::endl;
	return 1;
}

int statePlayerWinningFold()
{
	int ret = 0;
	State s;
	s.leader = 0;	s.trump_suit = CLUBS;	s.first_to_play = 0;
	s.turn[0] = Card(CLUBS, CJ);	s.turn[1] = Card(HEARTS, CJ);	s.turn[2] = Card(DIAMOND, CJ);	s.turn[3] = Card(SPADES, CJ);
	ret += assertion(s.playerWinningFold() == 0, "First player trump others different suits, sames values, first player winner");
	
	s.turn[0] = Card(HEARTS, CJ);	s.turn[1] = Card(CLUBS, CJ);	s.turn[2] = Card(DIAMOND, CJ);	s.turn[3] = Card(SPADES, CJ);
	ret += assertion(s.playerWinningFold() == 1, "First player trump others different suits, sames values, not first player winner");
	
	s.turn[0] = Card(CLUBS, CJ);	s.turn[1] = Card(CLUBS, CA);	s.turn[2] = Card(DIAMOND, C8);	s.turn[3] = Card(SPADES, CK);
	ret += assertion(s.playerWinningFold() == 0, "All trump, different values, first player winner");
	
	s.turn[0] = Card(CLUBS, CA);	s.turn[1] = Card(CLUBS, CJ);	s.turn[2] = Card(DIAMOND, C8);	s.turn[3] = Card(SPADES, CK);
	ret += assertion(s.playerWinningFold() == 1, "All trump, different values, not first player winner");
	
	s.turn[0] = Card(DIAMOND, C7);	s.turn[1] = Card(DIAMOND, C9);	s.turn[2] = Card(DIAMOND, CK);	s.turn[3] = Card(DIAMOND, C10);
	ret += assertion(s.playerWinningFold() == 3, "All card better than the previous, same suit, not trump");
	
	s.turn[0] = Card(CLUBS, C7);	s.turn[1] = Card(CLUBS, CK);	s.turn[2] = Card(CLUBS, CA);	s.turn[3] = Card(CLUBS, C9);
	ret += assertion(s.playerWinningFold() == 3, "All card better than the previous, same suit, trump");
	
	return ret;
}

int gameIsValidMove()
{
	int ret = 0;// to be done => créer des faussaires pour les joueurs
	State s;
	TestPlayer p1(0), p2(1), p3(2), p4(3);
	TestGame game;
	game.setPlayers(&p1, &p2, &p3, &p4);
	s.leader = 0;	s.trump_suit = CLUBS;	s.first_to_play = 0;
	
	s.turn[0] = Card(HEARTS, CQ);s.turn[1] = Card(INVALID_SUIT, INVALID_VALUE);s.turn[2] = Card(INVALID_SUIT, INVALID_VALUE);s.turn[3] = Card(INVALID_SUIT, INVALID_VALUE);
	game.set(s, std::list<Card>{Card(CLUBS, C8)}, std::list<Card>{Card(CLUBS, C9), Card(HEARTS, C10)},
		std::list<Card>{Card(HEARTS, CA), Card(SPADES, CQ)}, std::list<Card>{Card(DIAMOND, C8), Card(DIAMOND, C7)});
	ret += assertion(game.isValidMove(1, Card(HEARTS, C7)), "have asked suit, play same suit, not trump, playedcard.value < winningcard.value");
	
	s.turn[0] = Card(HEARTS, CQ);s.turn[1] = Card(INVALID_SUIT, INVALID_VALUE);s.turn[2] = Card(INVALID_SUIT, INVALID_VALUE);s.turn[3] = Card(INVALID_SUIT, INVALID_VALUE);
	game.set(s, std::list<Card>{Card(CLUBS, C8)}, std::list<Card>{Card(CLUBS, C9), Card(SPADES, C10)},
		std::list<Card>{Card(HEARTS, CA), Card(SPADES, CQ)}, std::list<Card>{Card(DIAMOND, C8), Card(DIAMOND, C7)});
	ret += assertion(game.isValidMove(1, Card(HEARTS, CA)), "have asked suit, play same suit, not trump, playedcard.value > winningcard.value");
	
	s.turn[0] = Card(HEARTS, CQ);s.turn[1] = Card(INVALID_SUIT, INVALID_VALUE);s.turn[2] = Card(INVALID_SUIT, INVALID_VALUE);s.turn[3] = Card(INVALID_SUIT, INVALID_VALUE);
        game.set(s, std::list<Card>{Card(CLUBS, C8)}, std::list<Card>{Card(CLUBS, C9), Card(SPADES, C10)},
		 std::list<Card>{Card(HEARTS, CA), Card(SPADES, CQ)}, std::list<Card>{Card(DIAMOND, C8), Card(DIAMOND, C7)});
        ret += assertion(!game.isValidMove(1, Card(DIAMOND, CA)), "don't have asked suit, don't trump, have trump, not trump, playedcard.value > winningcard.value");
	
	s.turn[0] = Card(HEARTS, CQ);s.turn[1] = Card(INVALID_SUIT, INVALID_VALUE);s.turn[2] = Card(INVALID_SUIT, INVALID_VALUE);s.turn[3] = Card(INVALID_SUIT, INVALID_VALUE);
        game.set(s, std::list<Card>{Card(CLUBS, C8)}, std::list<Card>{Card(CLUBS, C9), Card(SPADES, C10)},
		 std::list<Card>{Card(HEARTS, CA), Card(SPADES, CQ)}, std::list<Card>{Card(DIAMOND, C8), Card(DIAMOND, C7)});
        ret += assertion(game.isValidMove(1, Card(CLUBS, CA)), "have asked suit, trump, have trump, play same suit, not trump, playedcard.value > winningcard.value");
	
	return ret;
}

void testResult(std::string name, int ret)
{
	if (ret == 0)
		std::cout << name << " ==> ok" << std::endl;
	else
		std::cout << name << " ==> " << ret << " test(s) failed" << std::endl;
}

int main(int argc, char *argv[])
{
	(void) argc; (void)argv;
	
	testResult("gameIsValidMove", gameIsValidMove());
	testResult("statePlayerWinningFold", statePlayerWinningFold());
	
	return EXIT_SUCCESS;
}
