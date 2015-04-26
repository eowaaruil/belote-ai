#ifndef _STATE_HPP_
#define _STATE_HPP_

#include <list>

#include "card.hpp"

enum {TEAM1 = 0, TEAM2 = 1, NB_TEAMS = 2};
enum {PLAYER_TEAM = 0, OPPONENT_TEAM = 1};
enum {NOBODY = -1, PLAYER1 = 0, PLAYER2 = 1, PLAYER3 = 2, PLAYER4 = 3, NB_PLAYERS = 4};
#define NUM_PLAYERS         4
#define BID_HAND_NB_CARDS   5

class State
{
public:
	State();
	State(const State &s);
	State& operator=(const State& s);
	void clearRound();
	
	// Useful functions
	int playerWinningFold() const;
	
	int leader;
	int trump_suit;
	int first_to_play;
	int total_score[NB_TEAMS];
	int round_score[NB_TEAMS];
	int turn_number;
	Card turn[NB_PLAYERS];		// Cards played by each player this turn
	std::list<Card> played[NB_PLAYERS]; // All cards played this round
	
private:
};

#endif

