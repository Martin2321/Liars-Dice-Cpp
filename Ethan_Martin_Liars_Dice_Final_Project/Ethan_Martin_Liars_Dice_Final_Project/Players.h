#pragma once
#include <vector>
#include <string>
#include "GameLoop.h"
#include <iostream>

class Player
{
private:
	int m_playerHand[4];

	// Unsure as to why, but removing this string line creates an error after running the program, despite it not being used
	std::string m_playerName; 
	
public:
	void RollDice();
	void ShowHand();
	int CheckForValueInHand(int guessValue); 
};