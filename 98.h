/*
Program name: Ninety Eight
Author: Colin Rosier
Assingment: Final Project 
Date: 4/23/25
Description: This header file contains the logic to play the game ninety eight
using the deck.h header and the queue.h header file 
*/

#ifndef GAME98_H
#define GAME98_H

#include <ctime>
#include <iostream>
#include <cstdlib>
#include <limits>
#include "deck.h"
#include "queue.h"

using namespace std;

void playNinetyEight();

//Class for the game 98
class game98 
{
private:
    deckOfCards stock;
    queue* players;
    int numPlayers;
    int pile;

    void dealCards();
    void playTurn(int playerNum);
    void drawCard(int playerNum);
    card convertStringToCard(string s);
    void displayHand(int playerNum);
    void clearScreen();
    bool askPlayAgain();

public:
    game98();
    ~game98();
    void startGame();
};

//Constructor
game98::game98() 
{
    pile = 0;
    numPlayers = 0;
    players = NULL;
}

//Clear the screen
void game98::clearScreen() 
{
    system("cls");
}

//Convert string to card struct
card game98::convertStringToCard(string s) 
{
    card temp;
    
    if (s.length() >= 2) 
	{
        temp.face = s[0];
        temp.suit = s[1];
    } else 
	{
        temp.face = '0';
        temp.suit = '0';
    }
    temp.next = NULL;
    return temp;
}

//Deals 4 cards to each player
void game98::dealCards() 
{
    for (int i = 0; i < numPlayers; i++)
	{
        for (int j = 0; j < 4; j++) 
		{
            string s = stock.takeCard();
            card c = convertStringToCard(s);
            players[i].enqueue(c);
        }
    }
}

//Draw a card for the player after playing
void game98::drawCard(int playerNum) 
{
    string s = stock.takeCard();
    if (s != "") 
	{
        card c = convertStringToCard(s);
        players[playerNum].enqueue(c);
    }
}

//Display player's hand
void game98::displayHand(int playerNum) 
{
    cout << "Your cards:\n";
    queue temp;
    card tempCard;
    int count = 1;

    while (!players[playerNum].isEmpty()) 
	{
        players[playerNum].dequeue(tempCard);
        cout << count << ") " << tempCard.face << tempCard.suit << endl;
        temp.enqueue(tempCard);
        count++;
    }

    while (!temp.isEmpty()) 
	{
        temp.dequeue(tempCard);
        players[playerNum].enqueue(tempCard);
    }
}
//Handles the turn logic for the players
void game98::playTurn(int playerNum) 
{
    cout << "Current pile value: " << pile << endl;
    cout << "Player " << (playerNum + 1) << "'s turn." << endl;

    //Copy player's hand to array temporarily to show choices
    card hand[4];
    int handSize = players[playerNum].getQty();

    for (int i = 0; i < handSize; i++) {
        players[playerNum].dequeue(hand[i]);
    }

    //Display the cards
    cout << "Your cards:" << endl;
    for (int i = 0; i < handSize; i++) 
	{
        cout << (i + 1) << ") " << hand[i].face << hand[i].suit << endl;
    }

    //Let player pick
    int choice;
    do 
	{
        cout << "Select a card to play (1-" << handSize << "): ";
        cin >> choice;
        if (cin.fail() || choice < 1 || choice > handSize) 
		{
            cin.clear();
            cin.ignore();
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice < 1 || choice > handSize);

    card played = hand[choice - 1];

    //Apply the effect
    if (played.face >= '2' && played.face <= '9')
        pile += (played.face - '0');
    else if (played.face == 'A')
        pile += 1;
    else if (played.face == 'T') 
	{
    if (pile <= 10)
        pile = 0;
    else
        pile -= 10;
	}
    else if (played.face == 'J' || played.face == 'Q')
        pile = pile;
    else if (played.face == 'K')
        pile = 98;

    cout << "You played: " << played.face << played.suit << endl;
    cout << "New pile value: " << pile << endl;

    //Put unplayed cards back into queue
    for (int i = 0; i < handSize; i++) {
        if (i != (choice - 1))
            players[playerNum].enqueue(hand[i]);
    }

    //Draw new card
    drawCard(playerNum);

    cout << "Press enter to end your turn.";
    cin.ignore();
    cin.get();
    clearScreen();
}

//Ask if players want to play again
bool game98::askPlayAgain() 
{
    char answer;
    cout << "Would you like to play again? (y/n): ";
    cin >> answer;
    return (answer == 'y' || answer == 'Y');
}

void game98::startGame() 
{
	//shuffles the cards each time the game launchs
    srand(time(0)); 

    do 
	{
        pile = 0;
        cout << "Enter number of players: ";
        cin >> numPlayers;

        if (numPlayers < 2)
            numPlayers = 2;

        players = new queue[numPlayers];

        stock.shuffleDeck();
        dealCards();

        int currentPlayer = 0;
        bool gameOver = false;

        while (!gameOver) 
		{
            playTurn(currentPlayer);

            if (pile > 98) 
			{
                cout << "Player " << (currentPlayer + 1) << " loses!" << endl;
                gameOver = true;
            } 
			
			else 
			{
                currentPlayer++;
                if (currentPlayer == numPlayers)
                    currentPlayer = 0;
            }
        }

        delete[] players;
        players = NULL;

    } while (askPlayAgain());
}

//Destructor
game98::~game98() 
{
    if (players != NULL)
        delete [] players;
}


//Function to be called from the game menu
void playNinetyEight()
{
    game98 g;

    system("cls");

    g.startGame();

    //Clear Screen after game
    system("cls");
}

#endif
