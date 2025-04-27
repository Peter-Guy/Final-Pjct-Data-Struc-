#include <iostream>
#include "goboom.h"

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
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                playGame();
                break;
            case 2:
                std::cout << "Thanks for playing! Goodbye.\n";
                exit = true;
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
        }
    }
    
    return 0;
}

// Display the main menu
void displayMenu() {
    std::cout << "\n=== GO BOOM CARD GAME ===\n";
    std::cout << "1. Start New Game\n";
    std::cout << "2. Exit\n";
    std::cout << "Enter your choice: ";
}

// Play the Go Boom game
void playGame() {
    GoBoomGame game;
    game.startGame();
    
    std::cout << "\nGame Over!\n";
    
    // Wait for user input before returning to main menu
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(10000, '\n');
    std::cin.get();
}