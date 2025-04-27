#ifndef GOBOOM_H
#define GOBOOM_H

#include <iostream>
#include <string>
#include "deck.h"
#include "QUEUE.h"
#include "STACK.h"
#include "usll.h"
using namespace std;

class GoBoomGame {
private:
    // Game components
    deckOfCards* deck;          // Deck of cards
    Stack discardPile;          // Discard pile using Stack
    queue drawPile;             // Draw pile using Queue
    usll *playerHands;        // Player hands using Unsorted Linked List
    
    int currentPlayer;          // Current player's turn (0, 1, or 2)
    bool gameOver;              // Flag to indicate if the game is over
    char lastSuit;              // Last suit played
    char lastFace;              // Last face played
    
    // Private methods
    void initializeGame();
    void dealCards();
    bool isValidPlay(card& c);
    void displayGameState();
    void displayHand(int playerIndex);
    void displayDiscardTop();
    bool playCard(int playerIndex, int cardPosition);
    bool drawCard(int playerIndex);
    bool checkGameOver();
    void cleanupMemory();

public:
    // Constructor and destructor
    GoBoomGame();
    ~GoBoomGame();
    
    // Public methods
    void startGame();
    void playTurn();
    void switchPlayer();
    void displayRules();
    bool isGameOver();
    int getWinner();
};

// Constructor
GoBoomGame::GoBoomGame() {
    deck = nullptr;
    playerHands = new usll[3];
    currentPlayer = 0;
    gameOver = false;
    lastSuit = '\0';
    lastFace = '\0';
}

// Destructor
GoBoomGame::~GoBoomGame() {
    delete deck;
    delete [] playerHands;
}

// Initialize the game
void GoBoomGame::initializeGame() {
    // Clean up any existing resources
    cleanupMemory();
    
    // Create a new deck and shuffle it
    deck = new deckOfCards(1); // Create a single deck
    deck->shuffleDeck();       // Shuffle the deck
    
    // Clear player hands and piles
    for (int i = 0; i < 3; i++) {
        playerHands[i].makeEmpty();
    }
    
    discardPile.makeEmpty();
    drawPile.makeEmpty();
    
    // Reset game state
    currentPlayer = 0;
    gameOver = false;
    lastSuit = '\0';
    lastFace = '\0';
    
    // Deal cards
    dealCards();
}

// Deal cards to players and initialize draw and discard piles
void GoBoomGame::dealCards() {
    // Deal 7 cards to each player
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 3; j++) {
            string cardStr = deck->takeCard();
            if (!cardStr.empty()) {
                card newCard;
                newCard.face = cardStr[0];
                newCard.suit = cardStr[1];
                newCard.next = nullptr;
                playerHands[j].addNode(newCard);
            }
        }
    }
    
    // Fill the draw pile with remaining cards
    string cardStr;
    while (!(cardStr = deck->takeCard()).empty()) {
        card newCard;
        newCard.face = cardStr[0];
        newCard.suit = cardStr[1];
        newCard.next = nullptr;
        drawPile.enqueue(newCard);
    }
    
    // Place the top card from draw pile to discard pile
    card topCard;
    if (drawPile.dequeue(topCard)) {
        discardPile.push(topCard);
        lastSuit = topCard.suit;
        lastFace = topCard.face;
    }
}

// Check if a card is valid to play
bool GoBoomGame::isValidPlay(card& c) {
    return (c.suit == lastSuit || c.face == lastFace);
}

// Display the game state
void GoBoomGame::displayGameState(){
    cout << "\n========== GAME STATE ==========\n";
    cout << "Player " << (currentPlayer + 1) << "'s turn\n";
    
    // Display discard pile top
    displayDiscardTop();
    
    // Display number of cards in draw pile
    cout << "Draw pile: " << drawPile.getQty()<<" cards remaining\n";
    
    // Display number of cards for each player
    for (int i = 0; i < 3; i++) {
        cout << "Player " << (i + 1) << " has " 
                  << playerHands[i].getLength() << " cards\n";
    }
    cout << "===============================\n";
}

// Display a player's hand
void GoBoomGame::displayHand(int playerIndex) {
    cout << "\nPlayer " << (playerIndex + 1) << "'s hand:\n";
    
    for (int i = 1; i <= playerHands[playerIndex].getLength(); i++) {
        card c;
        if (playerHands[playerIndex].find(i, c)) {
            cout << i << ": " << c.face << c.suit << " ";
        }
    }
    cout << endl;
}

// Display the top card of the discard pile
void GoBoomGame::displayDiscardTop() {
    card topCard;
    if (discardPile.peek(topCard)) {
        cout << "Discard pile top: " << topCard.face 
                  << topCard.suit << endl;
    } else {
        cout << "Discard pile is empty\n";
    }
}

// Play a card from a player's hand
bool GoBoomGame::playCard(int playerIndex, int cardPosition) {
if (cardPosition < 1 || cardPosition > playerHands[playerIndex].getLength()){
        return false;
    }
    
    // Get the card
    card selectedCard;
    if (!playerHands[playerIndex].find(cardPosition, selectedCard)) {
        return false;
    }
    
    // Check if the play is valid
    if (!isValidPlay(selectedCard)) {
        cout << "Invalid move! Card must ";
        cout<< "match suit or face value of top card.\n";
        return false;
    }
    
    // Remove card from player's hand and add to discard pile
    card removedCard;
    if (playerHands[playerIndex].deleteNode(cardPosition, removedCard)) {
        discardPile.push(removedCard);
        lastSuit = removedCard.suit;
        lastFace = removedCard.face;
        
        cout << "Played " << removedCard.face << removedCard.suit 
                  << endl;
        return true;
    }
    
    return false;
}

// Draw a card from the draw pile
bool GoBoomGame::drawCard(int playerIndex) {
    // If draw pile is empty, reset it using cards from discard pile
    if (drawPile.isEmpty()) {
        // Keep the top card of discard pile
        card topCard;
        if (discardPile.peek(topCard)) {
            // Move all cards except top card from discard to draw pile
            card temp;
            discardPile.pop(temp); // Remove top card temporarily
            
            while (!discardPile.isEmpty()) {
                card c;
                discardPile.pop(c);
                drawPile.enqueue(c);
            }
            
            // Put top card back to discard pile
            discardPile.push(temp);
        }
        
        // If draw pile is still empty, return false
        if (drawPile.isEmpty()) {
            cout << "No cards left to draw!\n";
            return false;
        }
    }
    
    // Draw a card from the draw pile
    card drawnCard;
    if (drawPile.dequeue(drawnCard)) {
        playerHands[playerIndex].addNode(drawnCard);
        cout << "Drew " << drawnCard.face << drawnCard.suit << endl;
        return true;
    }
    
    return false;
}

// Check if the game is over
bool GoBoomGame::checkGameOver() {
    for (int i = 0; i < 3; i++) {
        if (playerHands[i].isEmpty()) {
            gameOver = true;
            return true;
        }
    }
    return false;
}

// Clean up memory
void GoBoomGame::cleanupMemory() {
    // Delete deck
    if (deck != nullptr) {
        delete deck;
        deck = nullptr;
    }
    
    // Clear player hands
    for (int i = 0; i < 3; i++) {
        playerHands[i].makeEmpty();
    }
    
    // Clear piles
    discardPile.makeEmpty();
    drawPile.makeEmpty();
}

// Start the game
void GoBoomGame::startGame() {
    // Display welcome message and rules
    cout << "=== GO BOOM CARD GAME ===\n";
    displayRules();
    
    // Initialize the game
    initializeGame();
    
    // Game loop
    while (!gameOver) {
        // Display game state and play turn
        displayGameState();
        playTurn();
        
        // Check if game is over
        if (checkGameOver()) {
            break;
        }
        
        // Switch to next player
        switchPlayer();
    }
    
    // Display winner
    int winner = getWinner();
    if (winner != -1) {
        cout << "\n*** PLAYER " << (winner + 1) << " WINS! ***\n";
    }
}

// Play a turn
void GoBoomGame::playTurn() {
    // Display current player's hand
    displayHand(currentPlayer);
    
    // Get player action
    int choice;
    bool validAction = false;
    
    while (!validAction) {
        cout << "\nPlayer " << (currentPlayer + 1) 
                  << ", choose action:\n";
        cout << "1. Play a card\n";
        cout << "2. Draw a card\n";
        cout << "Choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1: {
                int cardPosition;
                cout << "Enter card position to play (1-" 
                          << playerHands[currentPlayer].getLength() << "): ";
                cin >> cardPosition;
                
                validAction = playCard(currentPlayer, cardPosition);
                if (!validAction) {
                    cout << "Invalid card selection. Try again.\n";
                }
                break;
            }
            case 2: {
                validAction = drawCard(currentPlayer);
                break;
            }
            default:
                cout << "Invalid choice. Try again.\n";
                break;
        }
    }
    
    // Check if the player has won
    if (playerHands[currentPlayer].isEmpty()) {
        gameOver = true;
    }
}

// Switch to next player
void GoBoomGame::switchPlayer() {
    currentPlayer = (currentPlayer + 1) % 3;
}

// Display game rules
void GoBoomGame::displayRules() {
    cout << "\n=== RULES ===\n";
    cout << "1. Each player starts with 7 cards.\n";
    cout << "2. Players take turns playing cards that match either";
    cout<< "the suit or face value of the top card the discard pile.\n";
    cout << "3. If a player cannot play, they must draw a card.\n";
    cout << "4. The first player to get rid of all their cards wins.\n";
    cout << "5. You will play as all three players.\n\n";
}

// Check if the game is over
bool GoBoomGame::isGameOver() {
    return gameOver;
}

// Get the winner of the game
int GoBoomGame::getWinner() {
    for (int i = 0; i < 3; i++) {
        if (playerHands[i].isEmpty()) {
            return i;
        }
    }
    return -1; // No winner yet
}

#endif // GOBOOM_H