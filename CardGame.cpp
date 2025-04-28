#include <iostream>
#include "deck.h"
#include "crazy.h"
#include "ThirtyOne.h"
#include "98.h"
#include "fiveCardStud.h"
#include "playGoBoom.h"
#include "RollingStone.h"
using namespace std;

int main() {
	int gameChoice;

	cout << "Welcome to the card games.\n";
	
	do {
		cout << "1: War\n"
			<< "2: Black Jack\n"
			<< "3: Go Boom\n"
			<< "4: Crazy Eights\n"
			<< "5: 5 Card Stud\n"
			<< "6: Rolling Stone\n"
			<< "7: 98\n"
			<< "8: Thirty One\n"
			<< "9: Quit\n"
			<< "Which game would you like to play: ";
		cin >> gameChoice;
		switch (gameChoice) {
			case 1:
				//George
				//playWar();
				break;
			case 2:
				//Stephen
				//playBlackJack();
				break;
			case 3:
				//Peter
				playGoBoom();
				break;
			case 4:
				//Matthew
				playCrazyEights();
				break;
			case 5:
				//Jeremy
				playFiveCardStud();
				break;
			case 6:
				//Joseph
				playRollingStone();
				break;
			case 7:
				//Colin
				playNinetyEight();
				break;
			case 8:
				//Evan
				playThirtyOne();
				break;
			case 9:
				break;
			default:
				cout << "Invalid game choice.\n";
		}
	} while (gameChoice != 9);

	return 0;
}
