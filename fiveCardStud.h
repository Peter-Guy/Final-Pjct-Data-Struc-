/******************************************************************************
5 Card Stud
******************************************************************************/
#ifndef FIVECARD_H
#define FIVECARD_H

//Imports
#include <iostream>
#include <cstdlib>
#include "deck.h"
#include "usll.h"

using namespace std;

/******************************************************************************
Abstract data type
******************************************************************************/
//Represents the player's data
#ifndef FIVECARDSTUDPLAYER
#define FIVECARDSTUDPLAYER
struct fiveCardStudPlayer
{
    int handScore; //The total score of the player hand
    usll hand = usll(5); //The players hand holds 5 cards
};
#endif
/******************************************************************************
Function Prototypes
******************************************************************************/
//Function used in main file to initiate game
void playFiveCardStud();

/******************************************************************************
Class
******************************************************************************/
class fiveCardStud
{
    public:
        fiveCardStud(int = 2);
        ~fiveCardStud();
        void reset();
        void displayUserHand();
        void displayCPU();
        void displayAll();
        void deal();
        int showdown();
    private:
        void scoreHand(int);
        void multipleKind(int);
        bool isStraight(int);
        bool isFlush(int);
        int playerCount;
        fiveCardStudPlayer* players;
        deckOfCards* deck;
};

/******************************************************************************
Methods
******************************************************************************/
//Constructor
fiveCardStud::fiveCardStud(int numPlayers)
{  
    //Default player count is 2 
    //If the player count is more than that
    if(numPlayers >= 2 && numPlayers <= 8)
    {
        players = new fiveCardStudPlayer[numPlayers];

        //Save number of players
        playerCount = numPlayers;
    }
    else if(numPlayers > 8)
    {
        //Max is 8
        players = new fiveCardStudPlayer[8];

        //Save number of players
        playerCount = 8;
    }
    else
    {
        //Default
        players = new fiveCardStudPlayer[2];

        //Save number of players
        playerCount = 2;
    }

    //Intialize deck of cards
    deck = new deckOfCards(1);

    //Shuffles the card deck
    deck->shuffleDeck();
}

//Deals a card to each player
void fiveCardStud::deal()
{
    string str; //Card information
    card temp; //Card object

    //Prevents assignment of null pointer over and over
    temp.next = nullptr;

    //Draw 1 cards for each player
    for(int i = 0; i < playerCount; i++)
    {
        str = deck->takeCard();
        temp.face = str[0];
        temp.suit = str[1];

        players[i].hand.addNode(temp);
    }
}

//Scores a players hand
void fiveCardStud::scoreHand(int player)
{   
    if(isStraight(player))
    {
        return;
    }
    multipleKind(player);
}

//Checks for a straight and flush of all types
bool fiveCardStud::isStraight(int player)
{
    int counter[13] = {0};
    int highestCard = 0; //Index of the highest card
    bool isRoyal = false;
    bool isStraight = false;
    int cardsNeeded = 5; //Keeps track of number of cards in a row, 1 less
    card temp; //Stores a card
    
    //Loops through each card in hand and tracks the number of times it appears
    for(int i = 1; i <= 5; i++)
    {
        players[player].hand.find(i, temp);

        switch(temp.face)
        {
            case 'A':
                counter[12]++;
                highestCard = 12;
                break;
            case 'T':
                counter[8]++;
                if(8 > highestCard)
                {
                    highestCard = 8;
                }
                break;
            case 'J':
                counter[9]++;
                if(9 > highestCard)
                {
                    highestCard = 9;
                }
                break;
            case 'Q':
                counter[10]++;
                if(10 > highestCard)
                {
                    highestCard = 10;
                }
                break;
            case 'K':
                counter[11]++;
                if(11 > highestCard)
                {
                    highestCard = 11;
                }
                break;
            default:
                counter[temp.face - '2']++;
                if(temp.face - '2' > highestCard)
                {
                    highestCard = temp.face - '2';
                }
        }
    }
    
    //Scores hand
    for(int i = 12; i >= 0; i--) 
    {
        if(cardsNeeded == 0)
        {
            break;
        }
        
        if(counter[i] == 1)
        {
            //Found the start of a potential royal flush
            if(i == 12)
            {
                isRoyal = true;
            }
            //Found the start of a potential straight
            else if(!isStraight)
            {
                isStraight = true;
            }

            cardsNeeded--;
        }
        //If there are more than 1 of the same card, cant be a straight
        else if(counter[i] > 1)
        {
            return false;
        }
        //If the start of a straight found, but no card after
        else if(isRoyal || isStraight)
        {
            //If there is an ace, but no king, check ace low straight
            if(i == 11)
            {
                //Start at card face value 5
                i = 4;
                highestCard = 3;

                continue;
            }
            isRoyal = false;
            isStraight = false;
            break;
        }
    }

    //If it straight starting at ace
    if(isRoyal)
    {
        //If its also a flush
        if(isFlush(player))
        {
            if(highestCard == 12)
            {
                //Royal flush
                players[player].handScore = 1535;
            }
            else
            {
                //Ace low straight flush
                players[player].handScore = 1525 + highestCard - 3;
            }
            return true;
        }
        //Not a flush
        else
        {
            //Straight
            players[player].handScore = 277 + highestCard - 3;
            return true;
        }
    }
    //If it is a straight starting anywhere
    else if(isStraight)
    {
        //If its also a flush
        if(isFlush(player))
        {
            //Straight flush
            players[player].handScore = 1525 + highestCard - 3;
            return true;
        }
        //Not a flush
        else
        {
            //Straight
            players[player].handScore = 277 + highestCard - 3;
            return true;
        }
    }
    else
    {
        //If it is a normal flush
        if(isFlush(player))
        {
            //Flush
            players[player].handScore = 287 + highestCard - 5;
            return true;
        }
        //Not a flush
        else
        {
            //Exit function
            return false;
        }
    }
}

//Checks for a flush
bool fiveCardStud::isFlush(int player)
{
    char suit; //Stores the card suit
    card temp; //Stores a card

    //Get a card
    players[player].hand.find(1, temp);
    suit = temp.suit;

    //Check each card, see they all match suit
    for(int i = 2; i <= 5; i++)
    {  
        players[player].hand.find(i, temp);

        if(suit != temp.suit)
        {
            //If one dosent match
            return false;
        }
    }

    return true;
}

//Checks for pair and of a kind card matches
void fiveCardStud::multipleKind(int player)
{
    int counter[13] = {0};

    int numPair = -1; //Index of lowest pair
    int threeKind = -1; //Index of 3 kind
    int highCard; //Index of high card

    //Stores a card
    card temp;

    //Loops through each card in hand and tracks the number of times it appears
    for(int i = 1; i <= 5; i++)
    {
        players[player].hand.find(i, temp);

        switch(temp.face)
        {
            case 'A':
                counter[12]++;
                break;
            case 'T':
                counter[8]++;
                break;
            case 'J':
                counter[9]++;
                break;
            case 'Q':
                counter[10]++;
                break;
            case 'K':
                counter[11]++;
                break;
            default:
                counter[temp.face - '2']++;
        }
    }

    //Scores the Hand
    for(int i = 0; i < 13; i++) 
    {
        //Pair checks
        if(counter[i] == 2) 
        {
            //If another pair already found
            if(numPair != -1) 
            {
                //Two pair
                players[player].handScore = i * 21 + numPair;
                return;
            }
            //If a three of a kind already found
            else if(threeKind != -1) 
            {
                //Full house
                players[player].handScore = (threeKind + 3) * 100 + i;
                return;
            }
            //You found an instance of a pair
            else 
            {
                //Indexs the lowest 2 pair
                numPair = i;
            }
        }
        //3 of a kind check
        else if(counter[i] == 3) 
        {
            //There is a pair as well
            if (numPair != -1) 
            {
                //Full house
                players[player].handScore = (i + 3) * 100 + numPair;
                return;
            }
            else 
            {
                //Indexs the three kind
                threeKind = i;
            }
        }
        //Found Four of a kind
        else if (counter[i] == 4) 
        {
            //Four of a kind
            players[player].handScore = 1512 + i;
            return;
        }
        //Stores highest card
        else if (counter[i] == 1) 
        {
            highCard = i;
        }
    }
    
    //You have only 1 pair
    if (numPair != -1) 
    {
        //One pair
        players[player].handScore = 8 + numPair;
    }
    //You have a three of a kind
    else if (threeKind != -1) 
    {
        //Three kind
        players[player].handScore = 264 + threeKind;
    }
    //You just have a high card
    else 
    {
        //High card
        players[player].handScore = highCard - 6;
    }
}

//Decides the winning player, based on their hand
int fiveCardStud::showdown()
{
    int highest = 0; //Index of the highest player

    for(int i = 0; i < playerCount; i++)
    {
        //Scores each player hand
        scoreHand(i);

        if(players[i].handScore > players[highest].handScore)
        {
            highest = i;
        }
    }

    //Returns the index of the highest scored player
    return highest;
}

//Display all the cpus cards visible to the player
void fiveCardStud::displayCPU()
{
    int counter = 2; //Skips the hole card
    card temp; //Stores a card

    //Loop through each cpu, skipping 0 which is the player
    for(int i = 1; i < playerCount; i++)
    {
        cout << "CPU " << i << ": " << endl;
        //Loops through and displays each card
        while(players[i].hand.find(counter, temp))
        {
            cout << temp.face << ": " << temp.suit << endl;

            counter++;
        }

        counter = 2;
        cout << "\n\n";
    }
}

//Displays player hand
void fiveCardStud::displayUserHand()
{
    int counter = 1;
    card temp; //Stores a card

    cout << "Player Hand: " << endl;
    //Loops through and displays each card
    while(players[0].hand.find(counter, temp))
    {
        cout << temp.face << ": " << temp.suit << endl;

        counter++;
    }

    cout << "\n\n";
}

//Display all card data from every participant to the screen
void fiveCardStud::displayAll()
{
    int counter = 1;
    card temp; //Stores a card

    //Loop through each participant including the player
    for(int i = 0; i < playerCount; i++)
    {
        if(i == 0)
        {
            cout << "Player Hand: " << endl;
        }
        else
        {
            cout << "CPU " << i << ": " << endl;
        }
        
        //Loops through and displays each card
        while(players[i].hand.find(counter, temp))
        {
            cout << temp.face << ": " << temp.suit << endl;

            counter++;
        }

        counter = 1;
        cout << "\n\n";
    }
}

//Reset the game
void fiveCardStud::reset()
{
    //Go through each player set there score to zero, and clear hand
    for(int i = 0; i < playerCount; i++)
    {
        players[i].handScore = 0;
        players[i].hand.makeEmpty();
    }

    //Shuffle deck to reset
    deck->shuffleDeck();
}

//Destructor
fiveCardStud::~fiveCardStud()
{
    //Delete memory objects
    delete []players;
    delete deck;
}

/******************************************************************************
Game Logic
******************************************************************************/
//Function called in main to run five card stud
void playFiveCardStud()
{
    int numOfPlayers;
    bool endGame = false;
    char userInput;
    int winningPlayer = 0;//The index of the winning player

    //Get number of players to have
    do
    {
        cout << "How many players will play? (Max 8, Min 2): ";
        cin >> numOfPlayers;

        if(numOfPlayers > 8 || numOfPlayers < 2)
        {
            cout << "Invalid input, try again." << endl;
        }
    }while (numOfPlayers > 8 || numOfPlayers < 2);
    cin.ignore();

    //Create game object
    fiveCardStud mainGame = fiveCardStud(numOfPlayers);

    //Game loop
    do
    {
        //Clear Screen
        system("cls");

        //Start draws two cards 1 face down 1 face up
        mainGame.deal();
        mainGame.deal();
        mainGame.displayUserHand();
        mainGame.displayCPU();

        //Pauses game for user to read cards
        cout << "Press enter to continue: ";
        cin.ignore();
        
        
        //Run for 3 rounds
        for(int i = 0; i < 3; i++)
        {
            //Clear Screen
            system("cls");

            cout << "Round " << (i + 1) << ": ";

            //Deal a card
            mainGame.deal();
            cout << "A card was dealt to every player\n";

            //Display the user hands and the cards they can see
            mainGame.displayUserHand();
            mainGame.displayCPU();
            
            //Pauses game for user to read cards
            cout << "Press enter to continue: ";
            cin.ignore();
            
        }

        //Clear Screen
        system("cls");

        cout << "The final hand of all players is: " << endl;

        //Display all cards
        mainGame.displayAll(); 

        //Showdown
        winningPlayer = mainGame.showdown();

        //Display who won
        if(winningPlayer == 0)
        {
            cout << "You won!" << "\n\n";
        }
        else
        {
            cout << "CPU " << winningPlayer << ": Won" << "\n\n";
        }

        //Prompt the user if they want to play another round
        do
        {
            cout << "Would you like to play another round (y/n): ";
            cin >> userInput;

            if(tolower(userInput) != 'y' && tolower(userInput) != 'n')
            {
                cout << "Invalid Input, try again\n";
            }

        }while(tolower(userInput) != 'y' && tolower(userInput) != 'n');
        cin.ignore();

        //Handles whether to quit or not
        if(towlower(userInput) == 'y')
        {
            endGame = false;
            //Reset the game
            mainGame.reset();
        }
        else
        {
            endGame = true;

            //Clear screen after game
            system("cls");
        }
    }while(!endGame);
}
#endif