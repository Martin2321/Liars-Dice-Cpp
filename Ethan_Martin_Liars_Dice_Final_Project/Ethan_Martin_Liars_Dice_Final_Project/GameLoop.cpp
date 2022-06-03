#include"GameLoop.h"
#include "Players.h"

#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <algorithm>
#include <iterator>

using namespace std;

int numOfPlayers;

// Rolls the value of all 5 dice in a player's hand.
void Player::RollDice()
{
	int numOfDice = 5;
	for (int i = 1; i <= numOfDice; ++i)
	{
		m_playerHand[i] = (int)(1 + rand() % 6);
	}
}

// Prints each user's hand (dice) to the screen.
void Player::ShowHand()
{
	int numOfDice = 5;
	for (int i = 1; i <= numOfDice; ++i)
	{
		cout << m_playerHand[i] << endl;
	}

	cout << endl;
}

// Checks how many times a dice value is found in a player's hand.
int Player::CheckForValueInHand(int guessValue) 
{
	int myCount;

	myCount = count(begin(m_playerHand), end(m_playerHand) + 2, guessValue);

	return myCount;
}

// Reads in the rules from the 'Rules.txt' file.
void Game::ReadRules()
{
	string line;

	ifstream readMyFile;

	readMyFile.open("Rules.txt");

	if (readMyFile.is_open())
	{
		while (getline(readMyFile, line))
		{
			cout << line << '\n';
		}
	}
	readMyFile.close();
}

// Gets the number of players that are playing that round.
void Game::GetPlayerCount()
{
	cout << "Enter the number of players" << endl;
	cin >> numOfPlayers;
	cout << endl;

	// Does not allow the game to continue with less than 2 players.
	while (numOfPlayers < 2)
	{
		cout << endl <<  "This game requires 2 or more players to play." << endl << endl;
		cout << "Please add more players" << endl;
		cin >> numOfPlayers;
	}

	playerNames.resize(numOfPlayers);
	playerCount.resize(numOfPlayers);
}

// Gets a name input for each player.
void Game::GetPlayerNames()
{
	for (int i = 0; i < playerNames.size(); ++i)
	{
		static int x = 1;
		cout << "Enter name of player " << x << ": ";
		cin >> playerNames.at(i);
		++x;
	}
}

// Rolls the dice for every player in the game.
void Game::RollDice()
{
	cout << endl << "Press enter to roll the dice for each player." << endl;
	cin.ignore();
	cin.get();

	cout << endl;
	cout << "Rolling Dice..." << endl;
	for (int i = 0; i < numOfPlayers; ++i)
	{
		playerCount.at(i).RollDice();
	}

	Sleep(3000);
}

// Uses the players' information in order to decide who won the round.
void Game::DecideWinner(int guessValue, int guessQuantity, string currentGuesser, string playerWhoCalledLiar)
{
	int totalAppareancesOfGuessedValue = 0;
	bool isGuessCorrect = false;

	// Output each player's dice to the screen.
	cout << "Revealing All Dice..." << endl << endl;
	for (int i = 0; i < numOfPlayers; ++i)
	{
		cout << playerNames.at(i) << "'s dice" << endl;
		playerCount.at(i).ShowHand();
		cout << endl;
		Sleep(1000);
	}

	// Counts how many times the player's guessed value shows up in each players' hand.
	for (int i = 0; i < numOfPlayers; ++i)
	{
		totalAppareancesOfGuessedValue += playerCount.at(i).CheckForValueInHand(guessValue);
	}

	// Checks if the player's guess is correct or incorrect.
	if (totalAppareancesOfGuessedValue >= guessQuantity)
	{
		isGuessCorrect = true;
	}

	cout << currentGuesser << "'s guess is:" << endl;
	cout << guessQuantity << " dice that show a value of " << guessValue << endl << endl;

	cout << playerWhoCalledLiar << " called that out as a lie." << endl << endl;

	cout << "There are a total of " << totalAppareancesOfGuessedValue << " dice that showed a value of " << guessValue << endl << endl;

	// If the player's guess is correct, that player wins.
	// If the player's guess is incorrect, the player who called them out wins.
	if (isGuessCorrect == true)
	{
		cout << currentGuesser << "'s guess is correct" << endl << endl;
		cout << currentGuesser << " wins!" << endl << endl;
	}
	else
	{
		cout << currentGuesser << "'s guess is incorrect" << endl << endl;
		cout << playerWhoCalledLiar << " wins!" << endl << endl;
	}
}

// Allows each player to view their own dice and enter a guess.
// The other players either call out that guess as a lie or move onto the next player to guess.
void Game::MainGameFunction()
{
	bool liarCalled = false;

	string playerResponses;
	string currentGuesser;
	string playerWhoCalledLiar;


	int guessQuantity;
	int guessValue;
	int previousGuessQuantity = 0;
	int numOfPlayerWhoCalledLiar;

	// Gameplay continues until someone calls liar.
	while (liarCalled == false)
	{
		// Cycles through and gives each player a turn to enter a guess.
		for (int i = 0; i < numOfPlayers; ++i)
		{
			currentGuesser = playerNames.at(i);

			cout << playerNames.at(i) << ". When you are ready, press enter to start your turn";
			
			// A slight bug is caused by this cin.ignore() line. When it is here, the first player must press enter twice to start their first turn.
			// However, without it, no other player gets the chance to press enter at all revealing their dice too early.
			cin.ignore(); 
			cin.get();
			cout << endl;

			cout << "Your numbers" << endl;
			playerCount.at(i).ShowHand();

			cout << "Enter an integer for your first value (Quantity)" << endl;
			cin >> guessQuantity;

			// Ensures that the current player enters a first value that is higher than the previous player.
			while (guessQuantity <= previousGuessQuantity)
			{
				cout << "Your quantity must be larger than the previous guess" << endl << endl;
				cout << "Enter a value larger than " << previousGuessQuantity << endl;
				cin >> guessQuantity;
			}

			previousGuessQuantity = guessQuantity;

			cout << "Enter an integer for your second value (Dice Value)" << endl;
			cin >> guessValue;

			// Hides current player's dice from other players, by creating new lines.
			for (int i = 0; i <= 20; ++i)
			{
				cout << endl;
			}

			cout << playerNames.at(i) << ", your guess is that there are:" << endl;
			cout << guessQuantity << " dice that show a value of " << guessValue << endl << endl;
			
			cout << "*Does anybody believe that " << playerNames.at(i) << " is a liar?*" << endl << endl;
			cout << "Enter a 'Yes' or 'No' response" << endl;
			cin >> playerResponses;

			// If no player calls liar, the game moves onto the next player to guess.
			if (playerResponses == "No" || playerResponses == "no" || playerResponses == "NO")
			{
				cout << endl <<  "Moving on to the next player" << endl;
				
				Sleep(2000);
				
				for (int i = 0; i <= 25; ++i)
				{
					cout << endl;
				}

			}
			// If a player calls liar, they are asked to enter their player number and break the loop.
			else if (playerResponses == "Yes" || playerResponses == "yes" || playerResponses == "YES")
			{
				cout << endl << "Who believes " << playerNames.at(i) << " to be a liar?" << endl << endl;
				
				cout << "Enter your player number" << endl;
				cin >> numOfPlayerWhoCalledLiar;

				playerWhoCalledLiar = playerNames.at(numOfPlayerWhoCalledLiar - 1);

				cout << endl;

				liarCalled = true;

				break;
			}

			// Resets the turn order back to Player 1
			if (i > numOfPlayers)
			{
				i = 0;
			}
		}

	}

	Game::DecideWinner(guessValue, guessQuantity, currentGuesser, playerWhoCalledLiar);
}

// Simulates the gameplay of Liar's Dice by calling other functions.
void Game::RunGame()
{
	srand(time(NULL));
	
	cout << "Welcome to Liar's Dice!" << endl << endl;

	cout << "Press enter to see the game rules" << endl;
	cin.get();

	Game::ReadRules();

	cout << "When you are ready to start the game, press enter" << endl;
	cin.get();

	Game::GetPlayerCount();

	Game::GetPlayerNames();

	Game::RollDice();

	cout << endl;

	Game::MainGameFunction();

	cout << "Thanks for Playing!" << endl;
}
