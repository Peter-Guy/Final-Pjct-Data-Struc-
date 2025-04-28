/*****************************************************************************
program: ThirtyOne.h
author: Evan Taylor
Purpose: This program allows the user to play a game of thirty one
to play a game call the function playThirtyOne() in main program.
*****************************************************************************/
#ifndef THIRTYONE_H
#define THIRTYONE_H
#include <iostream>
#include "deck.h"
using namespace std;

struct thirtyOnePlayer
{
	string cards[3];
	int score = 0;
	int lives = 4;
	bool alive = true;
	bool dealer = false;
};

class ThirtyOne
{
	private:
		int numPlayers;
		int round;
		thirtyOnePlayer* p;
		deckOfCards* deck;
		string discardPile;
		bool loadGame();
		int cardValue(const char);
		
		
	public:
		ThirtyOne();
		ThirtyOne(int);
		int getPlayers();
		int setDealer(string []);
		int setDealer(int);
		bool isAlive(int);
		bool isDealer(int);
		string getDiscard();
		bool getHand(string[], int);
		int getScore(int);
		int getLives(int);
		string drawCard(int, int);
		string replaceCard(int, int, string);
		bool takeLives(int, int);
		bool gameWinner();
		~ThirtyOne();
};

void playThirtyOne();
void selectDealer(ThirtyOne&);
string cardName(string);
void knock(ThirtyOne&, int);
void drawCard(ThirtyOne&, int);
void displayMenu(ThirtyOne&, int);
void revealCards(ThirtyOne&, int);

int ThirtyOne::getLives(int num)
{
	return p[num].lives;
}

bool ThirtyOne::getHand(string hand[3], int num)
{
	hand[0] = p[num].cards[0];
	hand[1] = p[num].cards[1];
	hand[2] = p[num].cards[2];
	return true;
}

string ThirtyOne::getDiscard()
{
	return discardPile;
}

bool ThirtyOne::isAlive(int num)
{
	if (p[num].alive == true)
		return true;
	else
		return false;
}

bool ThirtyOne::isDealer(int num)
{
	if (p[num].dealer == true)
		return true;
	else
		return false;
}

ThirtyOne::ThirtyOne()
{
	numPlayers = 3; //default 3
	round = 0;
	p = new thirtyOnePlayer[numPlayers];
	deck = new deckOfCards[numPlayers];
	loadGame();
}

ThirtyOne::ThirtyOne(int num)
{
	numPlayers = num;
	round = 0;
	p = new thirtyOnePlayer[numPlayers];
	deck = new deckOfCards[numPlayers];
	loadGame();
}

ThirtyOne::~ThirtyOne()
{
	delete [] p;
	delete [] deck;
	p = nullptr;
	deck = nullptr;
}

int ThirtyOne::getPlayers()
{
	return numPlayers;
}

bool ThirtyOne::loadGame()
{
	deck[round].shuffleDeck();
	//deal 3 cards to each player and get their starting score
	for (int count = 0; count < numPlayers; count++)
	{
		p[count].cards[0] = deck[round].takeCard();
		p[count].cards[1] = deck[round].takeCard();
		p[count].cards[2] = deck[round].takeCard();
		p[count].score = getScore(count);
		if (round == 0)
		{
			p[count].lives = 4;
			p[count].dealer = false;	
		}
	}
	discardPile = deck[round].takeCard();
	return true;
}

int ThirtyOne::setDealer(string pCards[])
{
	int *pValue = new int[numPlayers];
	deckOfCards d;
	d.shuffleDeck();
	for (int count = 0; count < numPlayers; count++)
	{
		string card = d.takeCard();
		pCards[count] = card;
		pValue[count] = cardValue(card.front());
	}
	int lowest = pValue[0];
	int lowestPlayer = 0;
	for (int count = 1; count < numPlayers; count++)
	{
		if (pValue[count] < lowest)
		{
			lowest = pValue[count];
			lowestPlayer = count;
		}
	}
	p[lowestPlayer].dealer = true;
	return lowestPlayer;
}

int ThirtyOne::setDealer(int player)
{
	for (int count = 0; count < numPlayers; count++)
	{
		p[count].dealer = false;
	}
	p[player].dealer = true;
	return player;
}


int ThirtyOne::getScore(int player)
{
	int cardValue1 = cardValue(p[player].cards[0].front());
	int cardValue2 = cardValue(p[player].cards[1].front());
	int cardValue3 = cardValue(p[player].cards[2].front());
	
	char cardSuit1 = p[player].cards[0].back();
	char cardSuit2 = p[player].cards[1].back();
	char cardSuit3 = p[player].cards[2].back();

	// all three cards are the same type;
	if (p[player].cards[0].front() == p[player].cards[1].front() && 
		p[player].cards[1].front() == p[player].cards[2].front())
		return 30;
		
	// all three cards are the same suit
	if (cardSuit1 == cardSuit2 && cardSuit2 == cardSuit3)
	{
		int total = (cardValue1 + cardValue2 + cardValue3);
		return total;
	}
	
	// all three cards are a different suit
	if (cardSuit1 != cardSuit2 && cardSuit1 != cardSuit3 
		&& cardSuit2 != cardSuit3)
	{
		int max = cardValue1;
		for (int count = 1; count < 3; count++)
		{
			if (cardValue(p[player].cards[count].front()) > max)
				max = cardValue(p[player].cards[count].front());
		}
		return max;
	}
	
	// 2/3 cards are the same suit
	if (cardSuit1 == cardSuit2)
	{
		if ((cardValue1 + cardValue2) > cardValue3)
			return (cardValue1 + cardValue2);
		else
			return cardValue3;
	}
	else if (cardSuit1 == cardSuit3)
	{
		if ((cardValue1 + cardValue3) > cardValue2)
			return (cardValue1 + cardValue3);
		else
			return cardValue2;
	}
	else if (cardSuit2 == cardSuit3)
	{
		if ((cardValue2 + cardValue3) > cardValue1)
			return (cardValue2 + cardValue3);
		else
			return cardValue1;
	}
	
	return 0;
}

bool ThirtyOne::gameWinner()
{
	round++;
	int numAlive = 0;
	int winner;
	bool draw = false;
	if (round >= numPlayers)
	{
		cout << "\n\nAll decks of cards have been played.";
		cout << "\nHere are the final results:\n";
		for (int count = 0; count < numPlayers; count++)
		{
			cout << "Player " << (count + 1) << " lives:";
			cout << p[count].lives << endl;
		}
		int highest = p[0].lives;
		int highestPlayer = 0;
		for (int count = 1; count < numPlayers; count++)
		{
			if (p[count].lives > highest)
			{
				highest = p[count].lives ;
				highestPlayer = count;
			}
			else if (p[count].lives == highest)
			{
				cout << "Match is a draw!\n";
				return true;
			}
		}
		cout << "Player " << (highestPlayer + 1) << " wins!\n";
		return true;
	}
	
	
	
	for (int count = 0; count < numPlayers; count++)
	{
		if (p[count].alive == true)
			numAlive++;
	}
	
	if (numAlive > 1)
	{
		loadGame();
		return false;
	}
	
	if (numAlive == 1)
	{
		for (int count = 0; count < numPlayers; count++)
		{
			if (p[count].alive == true)
			{
				cout << "Player " << (count + 1) << " wins the game!\n\n";
				return true;
			}
		}
	}
	
	//match can end in a draw if multiple remaining players lose a life
	//at the same time.
	cout << "0 players are remaining. Match is a draw!\n\n";
	return true;
}

string ThirtyOne::drawCard(int player, int pile)
{
	string newCard;
	if (pile == 1)
	{
		newCard = deck[round].takeCard();
		return newCard;
	}
	else if (pile == 2)
	{
		newCard = discardPile;
		return newCard;	
	}
	return "error";
}

string ThirtyOne::replaceCard(int player, int card, string newCard)
{
	string oldCard;
	if (card == 1)
	{
		oldCard = p[player].cards[0];
		discardPile = p[player].cards[0];
		p[player].cards[0] = newCard;
		return oldCard;
	}
	else if (card == 2)
	{
		oldCard = p[player].cards[1];
		discardPile = p[player].cards[1];
		p[player].cards[1] = newCard;
		return oldCard;
	}
	else
	{
		oldCard = p[player].cards[2];
		discardPile = p[player].cards[2];
		p[player].cards[2] = newCard;
		return oldCard;
	}
	return "error";
}

int ThirtyOne::cardValue(const char c)
{
	switch (c)
	{
		case 'A':
			return 11;
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 8;
		case '9':
			return 9;
		case 'T':
			return 10;
		case 'J':
			return 10;
		case 'Q':
			return 10;
		case 'K':
			return 10;
		default:
			return -999; //shouldnt reach here
	}
}

bool ThirtyOne::takeLives(int player, int lives)
{
	p[player].lives -= lives;
	if (p[player].lives <= 0)
		p[player].alive = false;
	return true;
}

//call this function in main program to start a game.
void playThirtyOne()
{	
	char choice;
	int numPlayers;

	//Clear screen before game start
	system("cls");

	do
	{
		cout << "How many players are participating? (3-8) ";
		cin >> numPlayers;
		if (numPlayers < 3 || numPlayers > 8)
			cout << "Please enter a valid number (3-8)\n";
	}
	while (numPlayers < 3 || numPlayers > 8);
	
	ThirtyOne game(numPlayers);
	selectDealer(game);
	bool winner = false;
	int turn = 0;
	int round = 0;
	system("pause");
	system("cls");
	do
	{
		if (game.isAlive(turn) && !game.isDealer(turn))
		{
			displayMenu(game, turn);
			cout << "\nA. Draw a card";
			cout << "\nB. Knock on the table\n";
			do
			{
				cout << "\nWhat will you do? (A/B) ";
				cin >> choice;
				if (toupper(choice) != 'A' && toupper(choice) != 'B')
					cout << "Please select a valid option. (A/B)";
			}
			while (toupper(choice) != 'A' && toupper(choice) != 'B');
			
			if (toupper(choice) == 'A')
				drawCard(game, turn);
			if (toupper(choice) == 'B')
			{
				knock(game, turn);
				revealCards(game, turn);
				round++;
				winner = game.gameWinner();
			}
			system("pause");
			system("cls");
		}
		turn++;
		if (turn == numPlayers)
			turn = 0;
	}
	while (!winner);
	
	
	
	return;	
}

void revealCards(ThirtyOne &game, int turn)
{
	system("cls");
	int lowestScore = game.getScore(0);
	int lowestPlayer = 0;
	
	for (int count = 1; count < game.getPlayers(); count++)
	{
		if (game.isAlive(count) && !game.isDealer(count))
		{
			if (game.getScore(count) < lowestScore)
			{
				lowestScore = game.getScore(count);
				lowestPlayer = count;
			}
		}
	}
	
	for (int count = 0; count < game.getPlayers(); count++)
	{
		if (!game.isAlive(count) || game.isDealer(count))
			continue;
			cout << "\n\nPlayer " << (count + 1) << "'s cards:\n";
			string hand[3];
			game.getHand(hand, count);
			cout << cardName(hand[0]) << endl;
			cout << cardName(hand[1]) << endl;
			cout << cardName(hand[2]) << endl;
			cout << "Total Score: " << game.getScore(count) << endl;
	}
	
	for (int count = 0; count < game.getPlayers(); count++)
	{
		if (!game.isAlive(count) || game.isDealer(count))
			continue;
		if (game.getScore(count) == lowestScore)
		{
			if (count == turn)
			{
				cout << "\nPlayer " << (count + 1) << " loses 2 lives!\n";
				game.takeLives(count, 2);
			}
			else
			{
				cout << "\nPlayer " << (count + 1) << " loses 1 life!\n";
				game.takeLives(count, 1);
			}
		}
		cout << endl;
	}
	//pass the role of dealer to the next player
	for (int count = 0; count < game.getPlayers(); count++)
	{
		if (game.isDealer(count))
		{
			if ((count + 1) >= game.getPlayers())
			{
				cout << "Player " << 1 << " is now dealer.\n";
				game.setDealer(1);
				return;
			}
			else
			{
				cout << "Player " << (count + 2) << " is now dealer.\n";
				game.setDealer(count + 1);
				return;
			}
		}
	}
	
	return;
}

void displayMenu(ThirtyOne &game, int turn)
{
	string hand[3];
	game.getHand(hand, turn);
	cout << "Player " << (turn + 1) << "'s" << " turn:\n\n";
	cout << "1. Top of stock pile: ???\n";
	cout << "2. Top of discard pile: " << cardName(game.getDiscard());
		
	cout << "\n\nYour cards:\n";
	cout << "1. " << cardName(hand[0]);
	cout << "\n2. " << cardName(hand[1]);
	cout << "\n3. " << cardName(hand[2]);
	cout << "\n\nCurrent score: " << game.getScore(turn);
	cout << "\nCurrent lives: " << game.getLives(turn) << endl;
	return;
}

void knock(ThirtyOne &game, int turn)
{
	int turn2 = (turn + 1);
	char choice;
	system("cls");
	for (int count = 0; count < (game.getPlayers() - 1); count++)
	{
		if (turn2 == game.getPlayers())
			turn2 = 0;
		if (game.isAlive(turn2) && !game.isDealer(turn2))
		{
			system("cls");
			cout << "Player " << (turn + 1) << " knocks on the table, ";
			cout << "all other players get one more turn.\n\n";
			displayMenu(game, turn2);
			do
			{
				cout << "\nWould you like to draw a card? (Y/N) ";
				cin >> choice;
				if (toupper(choice) != 'Y' && toupper(choice) != 'N')
					cout << "Please select a valid option (Y/N)";
			}
			while (toupper(choice) != 'Y' && toupper(choice) != 'N');
					
			if (toupper(choice) == 'Y')
				drawCard(game, turn2);
			system("pause");
		}
		turn2++;
	}
}

void drawCard(ThirtyOne &game, int turn)
{
	int c;
	do
	{
		cout << "\nWhich pile would you like to draw your card from? (1-2) ";
		cin >> c;
		if (c < 1 || c > 2)
			cout << "Please select a valid option. (1-2)";
	}
	while (c < 1 || c > 2);
	string newCard = game.drawCard(turn, c);
	cout << "\nYou draw a " << cardName(newCard) << " from the";
	if (c == 1)
		cout << " stock pile";
	else
		cout << " discard pile";
	do
	{
		cout << "\nWhich card do you want to discard? (1-3) ";
		cin >> c;
		if (c < 1 || c > 3)
			cout << "Please select a valid option. (1-3)";
	}
	while (c < 1 || c > 3);
	string oldCard = game.replaceCard(turn, c, newCard);
	cout << "You replace your " << cardName(oldCard) << " with a ";
	cout << cardName(newCard) << endl;
	return;
}

void selectDealer(ThirtyOne &game)
{
	string *pCards = new string[game.getPlayers()];
	int dealer = game.setDealer(pCards);
	cout << "\nEach player will draw a card from the deck, ";
	cout << "the lowest card will be dealer first.\n\n";
	for (int count = 0; count < game.getPlayers(); count++)
	{
		cout << "Player " << (count + 1) << " draws a ";
		cout << cardName(pCards[count]) << endl;
	}
	cout << "\nPlayer " << (dealer + 1) << " is the dealer.\n";
	return;
}

//converts the short version of a cards name to its proper name.
string cardName(string card)
{
	string str1, str2;
	str1.assign(card, 0, 1);
	str2.assign(card, 1, 1);	
	
	if (str1 == "A")
		str1 = "Ace";
	else if (str1 == "2")
		str1 = "Two";
	else if (str1 == "3")
		str1 = "Three";
	else if (str1 == "4")
		str1 = "Four";
	else if (str1 == "5")
		str1 = "Five";
	else if (str1 == "6")
		str1 = "Six";
	else if (str1 == "7")
		str1 = "Seven";
	else if (str1 == "8")
		str1 = "Eight";
	else if (str1 == "9")
		str1 = "Nine";
	else if (str1 == "T")
		str1 = "Ten";
	else if (str1 == "J")
		str1 = "Jack";
	else if (str1 == "Q")
		str1 = "Queen";
	else if (str1 == "K")
		str1 = "King";
	else
		return "error";
		
	if (str2 == "C")
		str2 = "Clubs";
	else if (str2 == "S")
		str2 = "Spades";
	else if (str2 == "D")
		str2 = "Diamonds";
	else if (str2 == "H")
		str2 = "Hearts";
	else
		return "error";
		
	string str3 = " of ";
	card.clear();
	card.insert(0, str1);
	card.insert(card.length(), str3);
	card.insert(card.length(), str2);
	return card;
}

#endif