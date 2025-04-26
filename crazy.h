#ifndef CRAZY_H
#define CRAZY_H

#include <iostream>
#include <cstdlib>
#include <cctype>
#include "deck.h"
#include "usll.h"
using namespace std;

//Function used in main file to initiate game
void playCrazyEights();

//Game class with all game info inside
class CrazyEight {
    private:
        deckOfCards *deck;
        card discardPile;
        int playerCount;
        int currentPlayer;
        //Unsorted list used to represent each players current hand
        usll *hands;
        int *scores;
        void zeroScores();
        bool eightHandle();
        
    public:
        CrazyEight(int =2);
        ~CrazyEight();
        void reset();
        bool deal(int =5);
        bool showPlayerHand();
        bool drawCard();
        bool playCard(int);
        void nextPlayer();
        bool win();
        bool displayScore();
        int getCurrentPlayer();
};

//Constructor
CrazyEight::CrazyEight(int players) {
    //Temp variable used to get card out of deck
    string str;
    
    //Check if extra deck needed for large player count
    if (players > 6) {
        deck = new deckOfCards(2);
    }
    else {
        deck = new deckOfCards(1);
    }

    //Shuffle the deck
    deck->shuffleDeck();

    //Initialize first player turn
    currentPlayer = 1;

    //Get the top card in the deck
    str = deck->takeCard();

    //Fill the discard pile card with top card information
    discardPile.face = str[0];
    discardPile.suit = str[1];
    discardPile.next = nullptr;

    //Handle invalid player counts and 2 player games
    if (players <= 2) {
        playerCount = 2;
        hands = new usll[2];
        scores = new int[2];
        zeroScores();
        deal(7);
    }
    else if (players > 8) {
        playerCount = 8;
        hands = new usll[8];
        scores = new int[8];
        zeroScores();
        deal(5);
    }
    else {
        playerCount = players;
        hands = new usll[players];
        scores = new int[players];
        zeroScores();
        deal(5);
    }
}

//Destructor
CrazyEight::~CrazyEight() {
    delete deck;
    delete [] hands;
    delete [] scores;
}

//Initializer function for score keeping
void CrazyEight::zeroScores() {
    for (int i = 0; i < playerCount; i++) {
        scores[i] = 0;
    }
}

//Reset the game and tally scores for next game
void CrazyEight::reset() {
    //Temp card used for scoring
    card temp;
    //Temp variable for getting a card out of the deck
    string str;
    
    //Shuffle the deck
    deck->shuffleDeck();

    //Calculate score for each player
    for (int i = 0; i < playerCount; i++) {
        //Go through each of their cards
        for (int j = 1; j < hands[i].getLength(); j++) {
            hands[i].find(j, temp);

            switch (temp.face) {
                //Face cards and tens 10 point penalty
                case 'T':
                case 'J':
                case 'Q':
                case 'K':
                    scores[i] -= 10;
                    break;
                //Ace is 1 point penalty
                case 'A':
                    scores[i] -= 1;
                    break;
                //Eight is a 50 point penalty
                case '8':
                    scores[i] -= 50;
                    break;
                //All other cards are a penalty equal to the number on them
                default:
                    scores[i] -= temp.face - '0';
            }
        }
        //Clear the hand
        hands[i].makeEmpty();
    }

    //Get new discard pile card
    str = deck->takeCard();

    //Fill the discard pile with the new card
    discardPile.face = str[0];
    discardPile.suit = str[1];

    //Deal new cards
    if (playerCount == 2) deal(7);
    else deal(5);
}

//Helper function to fill each hand at the start of a round
bool CrazyEight::deal(int cardCount) {
    //Temp variable used to get card from deck
    string str;
    //Temp variable filled with card from the deck and added to hand
    card temp;
    //Next on card unused in program
    temp.next = nullptr;

    //Get the number of cards determined by parameter cardCount for each player
    for (int i = 0; i < cardCount; i++) {
        for (int i = 0; i < playerCount; i++) {
            str = deck->takeCard();
            temp.face = str[0];
            temp.suit = str[1];
            hands[i].addNode(temp);
        }
    }
    
    return true;
}

bool CrazyEight::showPlayerHand() {
    //Clear the console
    system("cls");
    //Temp variable used to pull cards out of hand
    card temp;

    //Output discard pile state
    cout << "Discard: ";

    switch (discardPile.face) {
        case 'A':
            cout << "Ace of ";
            break;
        case 'T':
            cout << "10 of ";
            break;
        case 'J':
            cout << "Jack of ";
            break;
        case 'Q':
            cout << "Queen of ";
            break;
        case 'K':
            cout << "King of ";
            break;
        default:
            cout << discardPile.face << " of ";
    }

    switch (discardPile.suit) {
        case 'C':
            cout << "Clubs\n";
            break;
        case 'D':
            cout << "Diamonds\n";
            break;
        case 'H':
            cout << "Hearts\n";
            break;
        case 'S':
            cout << "Spades\n";
            break;
    }
    
    //Output player cards
    cout << "Player " << currentPlayer << ": \n";

    for (int i = 1; i <= hands[currentPlayer - 1].getLength(); i++) {
        hands[currentPlayer - 1].find(i, temp);
        cout << i << ": ";

        switch (temp.face) {
            case 'A':
                cout << "Ace of ";
                break;
            case 'T':
                cout << "10 of ";
                break;
            case 'J':
                cout << "Jack of ";
                break;
            case 'Q':
                cout << "Queen of ";
                break;
            case 'K':
                cout << "King of ";
                break;
            default:
                cout << temp.face << " of ";
        }

        switch (temp.suit) {
            case 'C':
                cout << "Clubs\n";
                break;
            case 'D':
                cout << "Diamonds\n";
                break;
            case 'H':
                cout << "Hearts\n";
                break;
            case 'S':
                cout << "Spades\n";
                break;
        }
    }
    
    return true;
}

//Draw card function
bool CrazyEight::drawCard() {
    //Temp variable used to take card from deck
    string str;
    //Temp variable used to store card and add to hand
    card temp;

    //Get a card
    str = deck->takeCard();
    //If the deck is empty reshuffle and take a card
    if (str == "") {
        deck->shuffleDeck();
        str = deck->takeCard();
    }

    //Fill temp with card info
    temp.face = str[0];
    temp.suit = str[1];
    //Card next unused in program
    temp.next = nullptr;

    //Add the card to the current player's hand
    hands[currentPlayer - 1].addNode(temp);

    return true;
}

//Helper function to cycle through players
void CrazyEight::nextPlayer() {
    if (currentPlayer == playerCount) currentPlayer = 1;
    else currentPlayer++;
}

//Helper function to allow players to draw cards
bool CrazyEight::playCard(int cardNumber) {
    //Temp variable to get card from hand
    card temp;

    //If there is no card matching the card number return false
    if (!hands[currentPlayer - 1].find(cardNumber, temp)) return false;

    //If the current card is an 8 and the attempted card does not match the 
    //new suit return false
    if (discardPile.face - '0' == 8 && temp.suit != discardPile.suit) {
        return false;
    }

    //If the card is an eight use the eightHandle function to process
    if (temp.face - '0' == 8) {
        eightHandle();

        //Delete card from hand
        hands[currentPlayer - 1].deleteNode(cardNumber, temp);

        return true;
    }
    
    //If the card is not either the same face or the same suit return false
    if (temp.suit != discardPile.suit && temp.face != discardPile.face) {
        return false;
    }

    //Delete the card from the hand
    hands[currentPlayer - 1].deleteNode(cardNumber, temp);

    //Update discard pile
    discardPile.face = temp.face;
    discardPile.suit = temp.suit;

    return true;
}

//Help function to handle and eight being played
bool CrazyEight::eightHandle() {
    //Variable used to accept user input of card suit
    char userSuit;
    //Flag to determine if the choice was valid or not
    bool valid = false;
    
    discardPile.face = '8';

    do {
        cout << "What suit would you like to set the pile to? (H, D, S, C): ";
        cin >> userSuit;
        switch (toupper(userSuit)) {
            case 'H':
            case 'D':
            case 'S':
            case 'C':
                discardPile.suit = toupper(userSuit);
                valid = true;
                break;
            default:
                cout << "Invalid suit entered.\n";
        }
    } while (!valid);

    return true;
}

//Boolean function that returns if the current player won
bool CrazyEight::win() {
    return hands[currentPlayer - 1].getLength() == 0;
}

//Int function that returns the current player
int CrazyEight::getCurrentPlayer() {
    return currentPlayer;
}

//Display each players current score
bool CrazyEight::displayScore() {
    for (int i = 0; i < playerCount; i++) {
        cout << "Player " << i + 1 << " score: " << scores[i] << endl;
    }

    return true;
}

//Function called in main file to run crazy eights
void playCrazyEights() {
    //Input from user for player count
    int players;
    //Input from user for choosing what card to play
    int cardNumber;
    //Flag to determine if card played is valid
    bool valid;
    //Flag for quitting the game after a round
    bool quit = false;
    //Char used to determine quit flag
    char continueGame;
    //Crazy eight point used to create the game object
    CrazyEight *game;

    //Clear the console
    system("cls");

    //Get number of players not allowing less than 2 or more than 8
    do {
        cout << "How many players will be playing? (Max 8): ";
        cin >> players;
        if (players > 8 || players < 2) {
            cout << "Invalid player count.\n";
        }
    } while (players > 8 || players < 2);

    //Create the game object with the number of players given by user
    game = new CrazyEight(players);

    //Clear input
    cin.ignore();

    //Main game loop
    do {
        //Clear console
        system("cls");

        //Allow each player to keep their cards hidden
        cout << "Press enter to start turn.";
        cin.ignore();
        
        //Show current hand and discard pile
        game->showPlayerHand();

        //Loop until a valid card is chosen to be played
        do {
            valid = true;
            
            //Ask user which card to play
            cout << "Which card would you like to play? (0 to draw): ";
            cin >> cardNumber;

            //Draw card
            if (cardNumber == 0) {
                game->drawCard();
                break;
            }
            
            //If the card can't be played it is invalid
            if (!game->playCard(cardNumber)) {
                valid = false;
                cout << "Invalid card.\n";
            }
        } while (!valid);

        //Check if the current player won after playing their card
        if (game->win()) {
            //Output who won
            cout << "Player " << game->getCurrentPlayer() << " wins.\n";
            //Prompt for another round
            do {
                cout << "Would you like to play another round? (Y/N): ";
                cin >> continueGame;
                if (toupper(continueGame) != 'Y' && 
                    toupper(continueGame) != 'N') {
                    cout << "Invalid choice.\n";
                }
            } while (toupper(continueGame) != 'Y' && 
                toupper(continueGame) != 'N');

            if (toupper(continueGame) == 'Y') {
                quit = false;
            }
            else {
                quit = true;
            }

            //Reset the game to get the scores ready
            game->reset();

            //Clear the console
            system("cls");

            //Ouput the scores
            game->displayScore();

            //Allow user to see scores before returning to main or
            //playing again
            cin.ignore();
            if (!quit) {
                cout << "Press enter to start next round.";
            }
            else {
                cout << "Press enter to quit the game.";
            }
            cin.ignore();
        }
        else {
            //Show the new state of the player's hand
            game->showPlayerHand();

            //Allow each player to see their current hand before ending their
            //turn
            cin.ignore();
            cout << "Press enter to end turn.";
            cin.ignore();
        }

        //Move to next player
        game->nextPlayer();
    } while (!quit);
}
#endif