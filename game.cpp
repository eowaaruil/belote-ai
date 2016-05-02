#include "game.hpp"

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <dlfcn.h>
#include <locale>

Game::Game()
{
}

Game::Game(char **p)
{
	initGame(p);
}

Game::~Game()
{
	for (auto l = libraries.begin(); l != libraries.end(); l++)
		dlclose(l->second);
	
	// Todo : free players
}

void Game::initGame(char **p)
{
	state.total_score[0] = 0;
	state.total_score[1] = 0;
	dealer = rand() % NB_PLAYERS;
	cheater = INVALID_PLAYER;
	
	libraries.clear();
	for (int i = 0; i < NB_PLAYERS; i++) {
		players[i] = NULL;
		hands[i].clear();
		players[i] = loadPlayer(p[i+1]);
		if (players[i] == NULL) {
			std::cerr << "Cannot open player " << i << "(" << p[i] << ") library" << std::endl;
			exit(EXIT_FAILURE);
		}
		players[i]->setName(p[i+1]);
		players[i]->setNum(i);
	}
	
	for (int c = 0; c < NB_SUITS; c++) {
		for (int v = 0; v < NB_VALUES; v++) {
			deck[c*NB_VALUES+v].suit = c;
			deck[c*NB_VALUES+v].value = v;
		}
	}
}

void Game::startGame()
{
	initNewDeal();
	while (state.total_score[TEAM1] < SCORE_LIMIT && state.total_score[TEAM2] < SCORE_LIMIT)
	{
		if (deal())
		{
			// Fill State structure
			clearTurn();
			createNewRoundStates();
			printBoard();
			
			for (int turn = 0; turn < NB_TURNS; turn++)
			{
				state.turn_number = turn;
				for (int i = 0; i < NB_PLAYERS; i++)
				{
					int player = (state.first_to_play + i) % NB_PLAYERS;
					updateTurnState(player);
					Card c = players[player]->playCard();
					state.turn[player] = c;
					if (!isValidMove(player, c))
					{
						looseByCheating(player);
						turn = NB_TURNS;
						break;
					}
					hands[player].remove(c);
				}
				printBoard();
				
				updateEndTurnStates();
				for (int i = 0; i < NB_PLAYERS; i++)
					players[i]->endRound();
				state.first_to_play = state.playerWinningFold();
				clearTurn();
			}
			
			updateEndRoundStates();
			printRoundScore();
			printGameScore();
			if (cheater != INVALID_PLAYER)
			{
				std::cout << "The player " << cheater << " made his team loose by doing an invalid move." << std::endl;
			}
		}
		
		initNewDeal();
	}
}

void Game::shuffle()
{
	for (int i = 0; i < NB_CARDS; i++)
	{
		int n = rand() % NB_CARDS;
		Card c = deck[i];
		deck[i] = deck[n];
		deck[n] = c;
	}
}

bool Game::deal()
{
	int deck_top = NB_CARDS-1;
	std::cout << "New deal." << std::endl;
	
	// First 3 cards
	for (int i = 0; i < NB_PLAYERS; i++) {
		for (int j = 0; j < DEAL_FIRST_TURN; j++) {
			hands[(dealer+i)%NB_PLAYERS].push_back(deck[deck_top--]);
		}
	}
	
	// Next 2 cards
	for (int i = 0; i < NB_PLAYERS; i++) {
		for (int j = 0; j < DEAL_SECOND_TURN; j++)
			hands[(dealer+i)%NB_PLAYERS].push_back(deck[deck_top--]);
	}
	
	// Bids
	Card extra = deck[deck_top--];
	state.leader = NOBODY;
	createNewRoundStates();
	for (int i = 0; i < NB_PLAYERS; i++)
	{
		if (players[i]->firstBid(extra)) {
			state.trump_suit = extra.suit;
			state.leader = i;
			break;
		}
	}
	
	if (state.leader == NOBODY)
		std::cout << "Nobody took the card during the first round." << std::endl;
	for (int i = 0; i < NB_PLAYERS && state.leader == NOBODY; i++)
	{
		if ((state.trump_suit = players[i]->secondBid(extra)) != INVALID_SUIT) {
			state.leader = i;
			break;
		}
	}
	
	if (state.leader == NOBODY)
	{
		std::cout << "Nobody took the card during the second round." << std::endl;
		return false;
	}
	std::cout << "Player " << state.leader << " takes the card: ";
	printCard(std::cout, extra);
	std::cout << " with " << suitToString(state.trump_suit) << "." << std::endl;
	
	hands[state.leader].push_back(extra);
	
	// Deals the remaining cards
	for (int i = 0; i < NB_PLAYERS; i++) {
		int player = (dealer+i)%NB_PLAYERS;
		for (int j = (player==state.leader?1:0); j < DEAL_THIRD_TURN; j++)
			hands[player].push_back(deck[deck_top--]);
	}
	
	return true;
}

void Game::initNewDeal()
{
	dealer = (dealer+1) % NB_PLAYERS;
	state.first_to_play = (dealer+1)%NB_PLAYERS;
	shuffle();
	
	for (int i = 0; i < NB_PLAYERS; i++) {
		players[i]->cleanHand();
		hands[i].clear();
	}
}

void Game::createNewRoundStates()
{
	state.clearRound();
	
	for (int i = 0; i < NB_PLAYERS; i++)
		players[i]->setHand(hands[i]);
}

void Game::updateEndTurnStates()
{
	// Compute score
	int turn_score = 0;
	if (state.turn_number == LAST_TURN)	// Last trick is 10 points
		turn_score = 10;
	for (int i = 0; i < NB_PLAYERS; i++)
		turn_score += state.turn[i].getScore(state.trump_suit);
	if (state.playerWinningFold() % NB_TEAMS == TEAM1)
		state.round_score[TEAM1] += turn_score;
	else
		state.round_score[TEAM2] += turn_score;
	
	// Save played cards
	for (int i = 0; i < NB_PLAYERS; i++)
		state.played[i].push_back(state.turn[i]);
	
	// Update cards for each players
	for (int i = 0; i < NB_PLAYERS; i++)
		updateTurnState(i);
}

void Game::updateTurnState(int player)
{
	players[player]->state = state;
	players[player]->setHand(hands[player]);
}

void Game::updateEndRoundStates()
{
	int leader_team = state.leader % NB_TEAMS;
	int opponent_team = (leader_team==TEAM1?TEAM2:TEAM1);
	if (state.round_score[leader_team] > state.round_score[opponent_team])
	{
		state.total_score[TEAM1] += state.round_score[TEAM1];
		state.total_score[TEAM2] += state.round_score[TEAM2];
	}
	else if (state.round_score[leader_team] == state.round_score[opponent_team])
	{
		// This is not what should happen, half of the points have to be repoted to the next round - to do list
		state.total_score[TEAM1] += state.round_score[TEAM1];
		state.total_score[TEAM2] += state.round_score[TEAM2];
	}
	else
	{
		state.total_score[opponent_team] += state.round_score[TEAM1] + state.round_score[TEAM2];
	}
}

Player* Game::loadPlayer(std::string name)
{
	void *handle;
	typedef Player* (*PlayerFunc)(void);
	PlayerFunc getPlayer;
	
	if (libraries.find(name + ".so") == libraries.end())
	{
		handle = dlopen((name + ".so").c_str(), RTLD_LAZY);
		if (!handle) {
			std::cerr << dlerror() << std::endl;
			exit(EXIT_FAILURE);
		}
		
		libraries[name] = handle;
	}
	else
		handle = libraries[name];
	
	getPlayer = (PlayerFunc)dlsym(handle, ("getPlayer" + name).c_str());
	
	return getPlayer();
}

void Game::printRoundScore()
{
	std::cout << "Round score - Team1: " << state.round_score[TEAM1] << " - Team2: " << state.round_score[TEAM2] << std::endl;
}

void Game::printGameScore()
{
	std::cout << "Game score - Team1: " << state.total_score[TEAM1] << " - Team2: " << state.total_score[TEAM2] << std::endl;
}

void Game::printBoard()
{
	// Printing score first
	printRoundScore();
	
	// First line
	std::cout << "  ";
	int i = 0;
	int start = (NB_TURNS - hands[2].size()) / 2;
	for (auto b = hands[2].begin(); b != hands[2].end(); i++)
	{
		if (start - i < 1)
		{
			printCard(std::cout, *b);
			b++;
		}
		else
			std::cout << "  ";
	}
	std::cout << std::endl;
	
	// second to seventh lines
	auto b = hands[1].begin();
	auto c = hands[3].begin();
	int b_start = (NB_TURNS - hands[1].size() + 1) / 2;
	int c_start = (NB_TURNS - hands[3].size()) / 2;
	for (int i = 0; i < NB_TURNS; i++)
	{
		if (b_start < 1 && b != hands[1].end())
		{
			printCard(std::cout, *b);
			b++;
		}
		else
			std::cout << "  ";
		b_start--;
		
		if (i == 1 && state.turn[2].suit != INVALID_SUIT && state.first_to_play == 2) { // player 2 first to play
			std::cout << "|     "; printCard(std::cout, state.turn[2]); std::cout << "       |";
		}
		else if (i == 2 && state.turn[2].suit != INVALID_SUIT && state.first_to_play != 2) { // player 2 not first to play
			std::cout << "|     "; printCard(std::cout, state.turn[2]); std::cout << "       |";
		}
		else if (i == 3 && state.turn[3].suit != INVALID_SUIT) { // player 3 first/not first to play card
			std::cout << "|         " << (state.first_to_play == 3?"  ":""); printCard(std::cout, state.turn[3]); std::cout << (state.first_to_play != 3?"  ":"") << " |";
		}
		else if (i == 4 && state.turn[1].suit != INVALID_SUIT) { // player 1 first/not first to play card
			std::cout << "| " << (state.first_to_play != 1?"  ":""); printCard(std::cout, state.turn[1]); std::cout << (state.first_to_play == 1?"  ":"") << "         |";
		}
		else if (i == 5 && state.turn[0].suit != INVALID_SUIT && state.first_to_play != 0) { // player 0 not first to play
			std::cout << "|       "; printCard(std::cout, state.turn[0]); std::cout << "     |";
		}
		else if (i == 6 && state.turn[0].suit != INVALID_SUIT && state.first_to_play == 0) { // player 0 first to play
			std::cout << "|       "; printCard(std::cout, state.turn[0]); std::cout << "     |";
		}
		else if (i == 0 || i == 7) // first and end line
			std::cout << "----------------";
		else // blank lines
			std::cout << "|              |";
		
		if (c_start < 1 && c != hands[3].end())
		{
			printCard(std::cout, *c);
			c++;
		}
		c_start--;
		
		std::cout << std::endl;
	}
	
	// eighth line
	std::cout << "  ";
	i = 0;
	start = (NB_TURNS - hands[0].size() + 1) / 2;
	for (auto b = hands[0].begin(); b != hands[0].end(); i++)
	{
		if (start - i < 1)
		{
			printCard(std::cout, *b);
			b++;
		}
		else
			std::cout << "  ";
	}
	std::cout << std::endl << std::endl;
}

void Game::clearTurn()
{
	for (int i = 0; i < NB_PLAYERS; i++)
		state.turn[i] = Card(INVALID_SUIT, INVALID_VALUE);
}

bool Game::isValidMove(int player, Card c)
{
	// No constraints for the first card of the turn
	if (player == state.first_to_play)
		return true;
	
	// Cannot play a card which is not the suit asked if the player has a card of the suit in his hand
	if (c.suit != state.turn[state.first_to_play].suit && playerHasSuit(player, state.turn[state.first_to_play].suit)) {
		//~ std::wcout << "must play asked suit" << std::endl;
		return false;
	}
	
	int fold_winner = state.playerWinningFold();
	// If trump, and has a better trump value, need to use that trump
	if (c.suit == state.turn[state.first_to_play].suit && c.suit == state.trump_suit)
	{
		if (c.getTrumpValue() < state.turn[fold_winner].getTrumpValue() && playerHasBetterCardInSuit(player, state.turn[fold_winner])) {
			//std::cout << "have better trump" << std::endl;
			return false;
		}
	}
	
	// If an opponent is winning the fold, the player have to trump, if a trump is played, it must be overtrumped
	if (players[player]->isOpponent(fold_winner) && (!playerHasSuit(player, state.turn[fold_winner].suit) && c.suit != state.turn[fold_winner].suit))
	{
		if (playerHasSuit(player, state.trump_suit))
		{
			if (c.suit != state.trump_suit) {
				//~ std::wcout << "must overtrump" << std::endl;
				return false;
			}
			if (c.getTrumpValue() < state.turn[fold_winner].getTrumpValue() && playerHasBetterCardInSuit(player, state.turn[fold_winner])) {
				//~ std::wcout << "must overtrump " << fold_winner << std::endl;
				return false;
			}
		}
	}
	
	// std::cout << "done" << std::endl;
	return true;
}

void Game::looseByCheating(int player)
{
	cheater = player;
}

bool Game::playerHasSuit(int player, int suit)
{
	for (auto c = hands[player].begin(); c != hands[player].end(); c++)
	{
		if (c->suit == suit)
			return true;
	}
	
	return false;
}

bool Game::playerHasBetterCardInSuit(int player, Card card)
{
	for (auto c = hands[player].begin(); c != hands[player].end(); c++)
	{
		if (c->suit == card.suit)
		{
			if (c->suit == state.trump_suit && c->getTrumpValue() > card.getTrumpValue())
				return true;
			else if (c->suit != state.trump_suit && c->value > card.value)
				return true;
		}
	}
	
	return false;
}

