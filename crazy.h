#ifndef CRAZY_H
#define CRAZY_H

#include <iostream>
#include <cstdlib>
#include <cctype>
#include "deck.h"
#include "usll.h"
using namespace std;

void playCrazyEight();

class CrazyEight {
    private:
        deckOfCards *deck;
        card discardPile;
        int playerCount;
        int currentPlayer;
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

CrazyEight::CrazyEight(int players) {
    string str;
    if (players > 6) {
        deck = new deckOfCards(2);
    }
    else {
        deck = new deckOfCards(1);
    }

    deck->shuffleDeck();

    currentPlayer = 1;

    str = deck->takeCard();

    discardPile.face = str[0];
    discardPile.suit = str[1];
    discardPile.next = nullptr;

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

CrazyEight::~CrazyEight() {
    delete deck;
    delete [] hands;
    delete [] scores;
}

void CrazyEight::zeroScores() {
    for (int i = 0; i < playerCount; i++) {
        scores[i] = 0;
    }
}

void CrazyEight::reset() {
    card temp;
    string str;
    
    deck->shuffleDeck();

    for (int i = 0; i < playerCount; i++) {
        for (int j = 1; j < hands[i].getLength(); j++) {
            hands[i].find(j, temp);

            switch (temp.face) {
                case 'T':
                case 'J':
                case 'Q':
                case 'K':
                    scores[i] -= 10;
                    break;
                case 'A':
                    scores[i] -= 1;
                    break;
                case '8':
                    scores[i] -= 50;
                    break;
                default:
                    scores[i] -= temp.face - '0';
            }
        }
        hands[i].makeEmpty();
    }

    str = deck->takeCard();

    discardPile.face = str[0];
    discardPile.suit = str[1];

    if (playerCount == 2) deal(7);
    else deal(5);
}

bool CrazyEight::deal(int cardCount) {
    string str;
    card temp;
    temp.next = nullptr;
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
    system("cls");
    card temp;

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

bool CrazyEight::drawCard() {
    string str;
    card temp;

    str = deck->takeCard();
    if (str == "") {
        deck->shuffleDeck();
        str = deck->takeCard();
    }

    temp.face = str[0];
    temp.suit = str[1];
    temp.next = nullptr;

    hands[currentPlayer - 1].addNode(temp);

    return true;
}

void CrazyEight::nextPlayer() {
    if (currentPlayer == playerCount) currentPlayer = 1;
    else currentPlayer++;
}

bool CrazyEight::playCard(int cardNumber) {
    card temp;
    if (!hands[currentPlayer - 1].find(cardNumber, temp)) return false;

    if (temp.face - '0' == 8) {
        eightHandle();

        hands[currentPlayer - 1].deleteNode(cardNumber, temp);

        return true;
    }
    
    if (temp.suit != discardPile.suit && temp.face != discardPile.face) {
        return false;
    }

    if (discardPile.face - '0' == 8 && temp.suit != discardPile.suit) {
        return false;
    }

    hands[currentPlayer - 1].deleteNode(cardNumber, temp);

    discardPile.face = temp.face;
    discardPile.suit = temp.suit;

    return true;
}

bool CrazyEight::eightHandle() {
    char userSuit;
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

bool CrazyEight::win() {
    return hands[currentPlayer - 1].getLength() == 0;
}

int CrazyEight::getCurrentPlayer() {
    return currentPlayer;
}

bool CrazyEight::displayScore() {
    for (int i = 0; i < playerCount; i++) {
        cout << "Player " << i + 1 << " score: " << scores[i] << endl;
    }

    return true;
}

void playCrazyEight() {
    int players;
    int cardNumber;
    bool valid;
    bool quit = false;
    char continueGame;
    CrazyEight *game;

    system("cls");

    do {
        cout << "How many players will be playing? (Max 8): ";
        cin >> players;
        if (players > 8 || players < 2) {
            cout << "Invalid player count.\n";
        }
    } while (players > 8 || players < 2);

    game = new CrazyEight(players);

    do {
        game->showPlayerHand();

        do {
            valid = true;
            
            cout << "Which card would you like to play? (0 to draw): ";
            
            cin >> cardNumber;

            if (cardNumber == 0) {
                game->drawCard();
                break;
            }
            
            if (!game->playCard(cardNumber)) {
                valid = false;
                cout << "Invalid card.\n";
            }
        } while (!valid);

        if (game->win()) {
            cout << "Player " << game->getCurrentPlayer() << " wins.\n";
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

            game->reset();

            game->displayScore();
        }

        game->nextPlayer();
    } while (!quit);
}
#endif