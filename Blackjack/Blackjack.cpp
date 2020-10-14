#include <iostream>
#include <vector>
#include <ctime>
#include <windows.h>
#include <conio.h>

int draw();
int winner(int player, int house);
int gameTable(int playerMoney,int houseMoney, int pot);
void cardBuild(char symbol, int value, char name);


int main()
{
	srand(time(nullptr));
	bool stillPlaying = true;
	int playerMoney = 100;
	int houseMoney = 1000;
	char input = 'x';
	int winner = 0;
	int gamePot = 0;

		do {
			printf("\033c");
			std::cout << "\t[Blackjack]" << std::endl;
			std::cout << "\n\n The house has $" << houseMoney << " left." << std::endl;
			std::cout << " You have $" << playerMoney;
			std::cout << "\n The game has a $10 buy in. Do you want to play? (Y for yes, N for no)";
			input = _getch();
			switch (input) {
			case 'y':case 'Y':
				gamePot = gamePot + 10;
				gamePot = gameTable(playerMoney, houseMoney, gamePot);
				break;
			case'n':case'N':
				stillPlaying = false;
				break;
			default:
				std::cout << "Invalid input";
			}
			playerMoney = playerMoney + gamePot;
			houseMoney = houseMoney - gamePot;
			//if the player or house is out of money after a round, the game ends, as one party can't participate. This also applies if the player can't afford the buy-in.
			if (playerMoney <10) {
				stillPlaying = false;
				std::cout << "\n\n You can't afford to play anymore...";
			}
			if (houseMoney == 0) {
				stillPlaying = false;
				std::wcout << "The house is out of money...";
			}
		} while (stillPlaying == true);
		std::cout << "\n\n\tYou walk away with $" << playerMoney << ", spend it wisely...";
		Sleep(1000);
	}

int gameTable(int playerMoney, int houseMoney, int pot) {
	//I'm using structs to make my cards.
	struct Cards {
		int value;
		char name;
		char symbol;
		bool drawn;
		bool isAce;
	};
	std::vector<Cards>deck;
	std::vector<Cards>playerHand;
	std::vector<Cards>houseHand;
	int symbolTracker = 0;
	int playerScore = 0;
	int houseScore = 0;
	char symbol = 3;
	int drawnCard = 0;
	bool validCard = true;
	char choice = 'x';
	bool playerTurn = true;
	int winningPlayer = 0;
	int houseCash = houseMoney;
	int playerCash = playerMoney-10;
	int gamePot = pot;
	int acesDrawn = 0;
	int bet = 0;
	bool valid = true;

	// I make the deck by making a vector with card objects
	for (int i = 0; i < 52; i++) {
		deck.push_back(Cards());
		deck[i].drawn = false;
		deck[i].value = i % 13 + 1;
		if (deck[i].value == 1) {
			deck[i].isAce = true;
			deck[i].name = 'A';
		}
		else if (deck[i].value == 11) {
			deck[i].value = 10;
			deck[i].name = 'J';
		}
		else if (deck[i].value == 12) {
			deck[i].value = 10;
			deck[i].name = 'Q';
		}
		else if (deck[i].value == 13) {
			deck[i].value = 10;
			deck[i].name = 'K';
		}
		else {
			deck[i].name = 'x';
		}
		deck[i].symbol = symbol;
		symbolTracker++;
		if (symbolTracker == 13) {
			symbol++;
			symbolTracker = 0;
		}
	}

	//draw the first cards for the house and player
	for (int i = 0; i < 2; i++) {
		houseHand.push_back(Cards());
		playerHand.push_back(Cards());
		do {
			drawnCard = draw();
			if (deck[drawnCard].drawn == true) {
				validCard = false;
			}
			else {
				validCard = true;
				deck[drawnCard].drawn = true;
				houseHand[i] = deck[drawnCard];
			}
		} while (validCard==false);
		do {
			drawnCard = draw();
			if (deck[drawnCard].drawn == true) {
				validCard = false;
			}
			else {
				validCard = true;
				deck[drawnCard].drawn = true;
				playerHand[i] = deck[drawnCard];
			}
		} while (validCard == false);
	}
	
	//Now i Draw the "table" and ask for bets
	printf("\033c");
	std::cout << "\t House's hand:\n\t";
	for (int i = 0; i < houseHand.size(); i++) {
		if (i == 0) {
			//the house's first card is always hidden in blackjack, so I only print out ## as the first card.
			std::cout << "## ";
		}
		else {
			std::cout << houseHand[i].symbol;
			if (houseHand[i].name != 'x') {
				std::cout << houseHand[i].name << " ";
			}
			else {
				std::cout << houseHand[i].value << " ";
			}
		}
	}
	std::cout << "\n\n\t   Your hand:\n\t";
	for (int i = 0; i < playerHand.size(); i++) {
		std::cout << playerHand[i].symbol;
		if (playerHand[i].name != 'x') {
			std::cout << playerHand[i].name << " ";
		}
		else {
			std::cout << playerHand[i].value << " ";
		}
	}
	//Here I ask for bets. The house will always match the player, and therefore, the bet can't be more money than what the house has.
	std::cout << "\n\tHow much would you like to bet this round?\n";
	std::cin >> bet;
	while(std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore(32767, '\n');
		std::cout << "invalid input . . ." << std::endl;
		std::cin >> bet;
	}
	while (bet > playerCash || std::cin.fail() || bet > houseCash) {
		if (bet > playerCash) {
			std::cout << "You don't have enough money. You have $" << playerCash << std::endl;
			std::cin >> bet;
			while (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(32767, '\n');
				std::cout << "invalid input . . ." << std::endl;
				std::cin >> bet;
			}
		}
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(32767, '\n');
			std::cout << "invalid input . . ." << std::endl;
			std::cin >> bet;
		}
		if (bet > houseCash) {
			std::cout << "The house doesn't have enough to match you. The house has $" << houseCash << std::endl;
		}
	}
	//now that the bet is placed, I put bet into the pot.
	playerCash = playerCash - bet;
	houseCash = houseCash - bet;
	gamePot = gamePot + bet;

	

	//now, it's the players turn.
	do {
		// I set the playerScore to 0, so I can calculate how much they have in the for loop later.
		playerScore = 0;
		printf("\033c");
		std::cout << "\t    The house holds:\n\t ";
		for (int i = 0; i < houseHand.size(); i++) {
			if (i == 0) {
				//the house's first card is always hidden in blackjack, so I only print out ## as the first card.
				std::cout << "## ";
			}
			else {
				std::cout << houseHand[i].symbol;
				if (houseHand[i].name != 'x') {
					std::cout << houseHand[i].name << " ";
				}
				else {
					std::cout << houseHand[i].value << " ";
				}
			}
		}
		//now I print every card the player has drawn, and add their values together.
		std::cout << "\n\n\t   Your hand:\n\t";
		for (int i = 0; i < playerHand.size(); i++) {
			std::cout << playerHand[i].symbol;
			if (playerHand[i].name != 'x') {
				std::cout << playerHand[i].name<<" ";
			}
			else {
				std::cout << playerHand[i].value << " ";
			}
			playerScore = playerScore + playerHand[i].value;
			if (playerHand[i].isAce == true) {//	  If the player has drawn an ace, they may decide what it's worth. 
				std::cout << "\n\tYou drew an ace! What value do you want it to count as? \n\tA. 1\n\tB: 11\n";
				choice = _getch();
				do {
					switch (choice) {
					case 'a':case'A':case'1':
						playerHand[i].value = 1;
						break;
					case'b':case'B':
						playerHand[i].value = 11;
						break;
					default:
						std::cout << "invalid input . . . counting ace as 1";
						valid = false;
					}
				} while (valid == false);
			}
		}
		//if the player goes above 21, it's an instant loss.
		if (playerScore > 21) {
			winningPlayer=winner(playerScore, houseScore);
			return winningPlayer*gamePot;
		}

		std::cout<<"\n \tYour current score is: "<<playerScore<<"\nWould you like to draw a new card, or hold? (input D for draw or H for hold)" << std::endl;
		choice = _getch();
		switch (choice) {
		case 'd':case 'D':
			playerHand.push_back(Cards());
			do {
				drawnCard = draw();
				if (deck[drawnCard].drawn == true) {
					validCard = false;
				}
				else {
					validCard = true;
					deck[drawnCard].drawn = true;
					playerHand[playerHand.size()-1] = deck[drawnCard];
				}
			} while (validCard == false);
			break;
		case 'h':case'H':
			playerTurn = false;
			break;
		default:
			std::cout << "Invalid input. . ." << std::endl;
		}
	} while (playerTurn == true);

	// Now it's the House's turn
	do {
		printf("\033c");
		houseScore = 0;
		std::cout << "\t    The house holds:\n\t ";
		for (int i = 0; i < houseHand.size(); i++) {
			std::cout << houseHand[i].symbol;
			if (houseHand[i].name != 'x') {
				std::cout << houseHand[i].name << " ";
			}
			else {
				std::cout << houseHand[i].value << " ";
			}
			// the house will decide which value of ace is the best, depending on if it's current score is less than 10
			houseScore = houseScore + houseHand[i].value;
			if (houseHand[i].isAce) {
				if (houseScore<=10) {
					houseHand[i].value = 11;
				}
				else {
					houseHand[i].value = 1;
				}
			}
		}
		std::cout << "\n\n\t   Your hand:\n\t";
		for (int i = 0; i < playerHand.size(); i++) {
			std::cout << playerHand[i].symbol;
			if (playerHand[i].name != 'x') {
				std::cout << playerHand[i].name << " ";
			}
			else {
				std::cout << playerHand[i].value << " ";
			}
		}
		// I added some Sleep functions to make the program seem like it's thinking. It feels more like you're playing against someone, and it's less confusing than
		//having the computer speed through its' turn.
		std::cout << "\n\t The house is thinking...";
		Sleep(1000);
		if (houseScore <= playerScore&&houseScore!=21) {
			std::cout << "\n\tThe house draws a card...";
			Sleep(1000);
			houseHand.push_back(Cards());
			do {
				drawnCard = draw();
				if (deck[drawnCard].drawn == true) {
					validCard = false;
				}
				else {
					validCard = true;
					deck[drawnCard].drawn = true;
					houseHand[houseHand.size() - 1] = deck[drawnCard];
				}
			} while (validCard == false);
		}
		else {
			winningPlayer=winner(playerScore, houseScore);
			playerTurn = true;
		}
		

	} while (playerTurn == false);
	return winningPlayer*gamePot;
	
}

int draw() { // This is simply an RNG function, but with a cool name.
	int Number = 0;
	Number = rand() % 52 + 1;
	return Number;
}

int winner(int player, int house) {//  finally, this function checks who is the winner, and plays a little jingle based on the outcome.
	if (player > house&&player<22||house>21) {
		if (player == 21) {
			std::cout << "\nWinner winner, chicken dinner."<<std::endl;
		}
		std::cout << "Player wins the round";
		
		Beep(600, 200);
		Beep(600, 200);
		Beep(600, 200);
		Beep(800, 500);
		return 1;
	}
	if (house > player && house != 21||player>21) {
		std::cout << "House wins the round";
		Beep(600, 200);
		Beep(400, 200);
		Beep(300, 200);
		Beep(200, 500);
		return -1;
	}
	if (house = player) {
		std::cout << "It's a draw!";
		Beep(500, 100);
		Beep(500, 100);
		Beep(400, 100);
		Beep(500, 100);
		Beep(400, 100);
		Beep(300, 100);
		return 0;
	}
}