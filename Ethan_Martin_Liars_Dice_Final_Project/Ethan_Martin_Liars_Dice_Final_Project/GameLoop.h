#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Players.h"

class Game
{
private:
	std::vector<std::string> playerNames; 
	std::vector<Player> playerCount;

	void ReadRules();
	void GetPlayerCount();
	void GetPlayerNames(); 
	void RollDice();
	void MainGameFunction();
	void DecideWinner(int guessValue, int guessQuantity, std::string currentGuesser, std::string playerWhoCalledLiar);

public:
	void RunGame();
};