#ifndef _GAME_HPP_
#define _GAME_HPP_

#include <list>
#include <string>
#include <map>

#include "card.hpp"
#include "player.hpp"

#define SCORE_LIMIT			1000

#define NB_TURNS			8
#define LAST_TURN			7

#define DEAL_FIRST_TURN		3
#define DEAL_SECOND_TURN	2
#define DEAL_THIRD_TURN		3

// Cards unicode characters
extern std::string card_back;
extern std::string card_clubs[13];
extern std::string card_hearths[13];
extern std::string card_diamond[13];
extern std::string card_spades[13];

class Game
{
public:
	Game();
	Game(char **p);
	~Game();
	void initGame(char **p);
	void startGame();		// start a game
	void shuffle();			// shuffle cards
	bool deal();			// deals cards to players return false if nobody took the exposed card XXX need to finish first and second bid XXX
	void initNewDeal();		// initialize game for a new round - do not call deal()
	void createNewRoundStates();		// update state for new round
	void updateEndTurnStates();			// update states for the end of turn
	void updateTurnState(int player);	// update state of the current turn for a player to make him play a card
	void updateEndRoundStates();		// update score at the end of the round
	Player* loadPlayer(std::string name);
	void clearTurn(); // Clears the variable current_turn
	bool isValidMove(int player, Card c);
	void looseByCheating(int player);
	bool playerHasSuit(int player, int suit);
	bool playerHasBetterCardInSuit(int player, Card c);
	
	// Display functions
	void printBoard();
	void printRoundScore();
	void printGameScore();
	
protected:
	std::map<std::string, void*> libraries;
	Player* players[NB_PLAYERS];
	int dealer;
	int cheater;			// If one of the played didn't follow the rules its different from INVALID_PLAYER
	Card deck[NB_CARDS];	// The 32 cards used to draw
	std::list<Card> hands[NB_PLAYERS];
	
	State state;	// The game state, all players know it
};

#endif
