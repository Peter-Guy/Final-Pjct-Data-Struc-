/*
Program: RollingStone.h
Author: Joseph Reyes
Purpose: This header file contains the logic to play the card game
Rolling Stone using the deck.h header file
*/    
    
#ifndef ROLLINGSTONE_H
#define ROLLINGSTONE_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "deck.h"
#include <thread>
#include <chrono>
using namespace std;

class RollingStone
{
private:
    deckOfCards deck;
    vector<string> playerHands[4];
    vector<string> pile;
    int currentPlayer;
    char currentSuit;

public:
    // Constructor declaration
    RollingStone();

    // Function to play the game
    void playGame();

    // Function to deal cards to players
    void dealCards();

    // Function to display player's hand
    void displayHand(int player);

    // Function to allow player to choose a card
    int chooseCard(int player);

    // Function to ask player to choose from their hand
    int chooseFromHand(int player);

    // Function to pick up cards from the pile
    void pickupPile(int player);

    // Function to move to the next player
    void nextPlayer();

    // Function to check if the round is over
    bool roundOver();

    // Function to compare two cards
    int compareCards(string a, string b);

    // Function to get the value of a card face
    int cardValue(char face);

    // Function to start a new round
    void startNewRound();

    // Free-standing function to be called from the game menu
    void playRollingStone();
};

// Constructor definition
RollingStone::RollingStone()
{
    srand(time(0));  // Randomize the deck
    deck.shuffleDeck();
    dealCards();  // Deal cards to players
    currentPlayer = 0;
}

// Function to play the game
void RollingStone::playGame()
{
    system("cls"); // Clear screen at start (Windows only)
    cout << "=== Rolling Stone Card Game ===" << endl << endl;
    bool winner = false;

    while (!winner)
    {
        if (playerHands[currentPlayer].empty())  // Check if a player is out of cards
        {
            cout << "Player " << currentPlayer + 1 << " wins!" << endl;
            winner = true;
            break;
        }

        cout << "\nPlayer " << currentPlayer + 1 << "'s turn:" << endl;
        displayHand(currentPlayer);  // Show the player's hand

        int cardIndex = chooseCard(currentPlayer);  // Player chooses a card
        if (cardIndex == -1)
        {
            cout << "No matching card. Picking up pile and starting new round.\n";
            pickupPile(currentPlayer);  // Pickup cards from pile
            startNewRound();  // Start a new round
            this_thread::sleep_for(chrono::seconds(1));  // 1-second pause
            system("cls"); // Clear screen after round
            continue;  // Player who couldn't follow suit plays again
        }

        string playedCard = playerHands[currentPlayer][cardIndex];
        cout << "Played: " << playedCard[0] << playedCard[1] << endl;

        pile.push_back(playedCard);  // Add played card to pile
        currentSuit = playedCard[1];  // Set the current suit
        playerHands[currentPlayer].erase(playerHands[currentPlayer].begin() + cardIndex);  // Remove card from player's hand

        if (playerHands[currentPlayer].empty())  // If the player has no more cards after playing
        {
            cout << "Player " << currentPlayer + 1 << " has no cards left! Game Over!" << endl;
            winner = true;
            break;
        }

        if (roundOver())  // Check if the round is over
        {
            cout << "Round over. " << endl;

            // Find the highest card and discard the pile
            int highestIndex = 0;
            for (int i = 1; i < 4; ++i)
            {
                if (compareCards(pile[i], pile[highestIndex]) > 0)
                    highestIndex = i;
            }
            cout << "Player " << highestIndex + 1 << " wins the round and discards the pile." << endl;

            // Discard the pile (the winner takes it)
            pile.clear();
            currentPlayer = highestIndex;  // Move to the player who won the round
            startNewRound();  // Start a new round
            this_thread::sleep_for(chrono::seconds(1));  // 1-second pause
            system("cls"); // Clear screen after round
        }
        else
        {
            nextPlayer();  // Move to the next player
            if (currentPlayer != 0)  // Add pause after computer plays
                this_thread::sleep_for(chrono::seconds(1));  // 1-second pause
        }
    }

    cout << "\n=== Game Over ===" << endl;
    this_thread::sleep_for(chrono::seconds(1));  // 1-second pause
}

// Deal cards to players (only 7-Ace)
void RollingStone::dealCards()
{
    for (int p = 0; p < 4; ++p)
    {
        while (playerHands[p].size() < 8)  // Ensure each player gets 8 cards
        {
            string c = deck.takeCard();
            if (c.empty()) break;  // If the deck runs out, stop
            if (!(c[0] >= '2' && c[0] <= '6'))  // Only accept 7-Ace cards
                playerHands[p].push_back(c);
        }
    }
}

// Display player's hand of cards
void RollingStone::displayHand(int player)
{
    for (int i = 0; i < playerHands[player].size(); ++i)
    {
        cout << i + 1 << ". " << playerHands[player][i][0] << playerHands[player][i][1] << "  ";
    }
    cout << endl;
}

// Allow player to choose a card
int RollingStone::chooseCard(int player)
{
    if (pile.empty())
    {
        if (player == 0)
            return chooseFromHand(player);
        else
            return 0; // Computer plays the first card
    }

    if (player == 0)
    {
        int choice = chooseFromHand(player);
        if (playerHands[player][choice][1] == currentSuit)
            return choice;  // Card matches suit
        else
            return -1;  // No matching card
    }
    else
    {
        for (int i = 0; i < playerHands[player].size(); ++i)
        {
            if (playerHands[player][i][1] == currentSuit)
                return i;  // Computer finds a card with the matching suit
        }
        return -1;  // No matching card
    }
}

// Ask player to choose a card from their hand
int RollingStone::chooseFromHand(int player)
{
    int choice;
    cout << "Select a card to play (1-" << playerHands[player].size() << "): ";
    cin >> choice;
    while (choice < 1 || choice > playerHands[player].size())  // Validate player's choice
    {
        cout << "Invalid choice. Try again: ";
        cin >> choice;
    }
    return choice - 1;
}

// Pick up all cards from the pile
void RollingStone::pickupPile(int player)
{
    for (string c : pile)
    {
        playerHands[player].push_back(c);
    }
    pile.clear();
}

// Move to the next player (circular rotation)
void RollingStone::nextPlayer()
{
    currentPlayer = (currentPlayer + 1) % 4;
}

// Check if the round is over (all players have played their cards)
bool RollingStone::roundOver()
{
    if (pile.size() == 4)
    {
        return true;  // Round is over
    }
    return false;  // Round continues
}

// Compare two cards
int RollingStone::compareCards(string a, string b)
{
    return cardValue(a[0]) - cardValue(b[0]);
}

// Get the value of a card face
int RollingStone::cardValue(char face)
{
    switch (face)
    {
        case '7': return 1;
        case '8': return 2;
        case '9': return 3;
        case 'T': return 4;
        case 'J': return 5;
        case 'Q': return 6;
        case 'K': return 7;
        case 'A': return 8;
        case 'X': return 9;  // Joker is highest
        default: return 0;
    }
}

// Start a new round
void RollingStone::startNewRound()
{
    pile.clear();
    currentSuit = '\0';
}

// Free-standing function to be called from the game menu
void playRollingStone()
{
    RollingStone game;  // Create an instance of the game

    system("cls");  // Clear screen at start
    game.playGame();  // Start the game

    system("cls");  // Clear screen after game
}

#endif
