#include <iostream>
#include "goboom.h"
using namespace std;

// Function prototypes
void displayMenu();
void playGame();

int playGoBoom() {
    //"your brother gets 5 big booms"
    // Seed random number generator
    srand(time(0));
    
    int choice;
    bool exit = false;
    
    // Main menu loop
    while (!exit) {
        displayMenu();
        cin >> choice;
        
        switch (choice) {
            case 1:
                playGame();
                break;
            case 2:
                cout << "Thanks for playing! Goodbye.\n";
                exit = true;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    }
    
    return 0;
}

// Display the main menu
void displayMenu() {
    cout << "\n=== GO BOOM CARD GAME ===\n";
    cout << "1. Start New Game\n";
    cout << "2. Exit\n";
    cout << "Enter your choice: ";
}

// Play the Go Boom game
void playGame() {
    GoBoomGame game;
    game.startGame();
    
    cout << "\nGame Over!\n";
    
    // Wait for user input before returning to main menu
    cout << "\nPress Enter to continue...";
    cin.ignore(10000, '\n');
    cin.get();
}