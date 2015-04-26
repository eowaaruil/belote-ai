#include "state.hpp"

#include <iostream>

State::State()
{
	total_score[0] = total_score[1] = 0;
	clearRound();
}

State::State(const State &s)
{
	for (int i = 0; i < NB_PLAYERS; i++)
	{
		played[i].clear();
		for (auto c = s.played[i].begin(); c != s.played[i].end(); c++)
			played[i].push_back(*c);
	}
}

State& State::operator=(const State& s)
{
	// for (auto c = s.hand.begin(); c != s.hand.end(); c++)
		// hand.push_back(*c);
	leader = s.leader;
	trump_suit = s.trump_suit;
	first_to_play = s.first_to_play;
	turn_number = s.turn_number;
	
	for (int i = 0; i < NB_TEAMS; i++)
	{
		total_score[i] = s.total_score[i];
		round_score[i] = s.round_score[i];
	}
	for (int i = 0; i < NB_PLAYERS; i++)
	{
		turn[i] = s.turn[i];
		played[i] = s.played[i];
	}
	
	return *this;
}

void State::clearRound()
{
	round_score[0] = round_score[1] = 0;
	turn_number = 0;
	for (int i = 0; i < NB_PLAYERS; i++)
	{
		turn[i] = Card(INVALID_SUIT, INVALID_VALUE);
		played[i].clear();
	}
}

int State::playerWinningFold() const
{
	Card winning_card = turn[first_to_play];
	int winning_player = first_to_play;
	
	for (int i = 1; i < NB_PLAYERS; i++)
	{
		int player = (first_to_play+i)%NB_PLAYERS;
		Card c = turn[player];
		bool new_winner(false);
		
		if (c.suit == INVALID_SUIT)
			return winning_player;
		if (c.suit == winning_card.suit)
		{
			if (trump_suit == c.suit && c.getTrumpValue() > winning_card.getTrumpValue())
				new_winner = true;
			else if (trump_suit != c.suit && c.value > winning_card.value)
				new_winner = true;
		}
		if (c.suit != winning_card.suit && c.suit == trump_suit)
			new_winner = true;
		
		if (new_winner)
		{
			winning_player = player;
			winning_card = c;
		}
	}
	
	return winning_player;
}

