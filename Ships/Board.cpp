#include <iostream>
#include "ShipsDataTypes.h"


void prepareBoard(char board[BOARD_HEIGHT][BOARD_WIDTH]) {
	for (int y = 0; y < BOARD_HEIGHT; y++) {
		for (int x = 0; x < BOARD_WIDTH; x++) {
			if (y == DIVIDING_LINE) board[y][x] = DIVIDING_LINE_CHAR; // to visualize border
			else board[y][x] = ' ';
		}
	}
}


bool isPointInsideBoard(int x, int y) {
	if (x < 0 || y < 0) return false;
	if (x >= BOARD_WIDTH || y >= BOARD_HEIGHT) return false;
	return true;
}


// check if player can put ship in specified place
bool checkPlace(int playerId, int startX, int endX, int startY, int endY) {
	// check if its player's part of board
	if (playerId == 0) {
		if (startY >= DIVIDING_LINE || endY >= DIVIDING_LINE) // ship exceded forbidden line
			return false;
	}
	else {
		if (startY <= DIVIDING_LINE || endY <= DIVIDING_LINE) // ship exceded forbidden line
			return false;
	}

	// check borders
	if (!isPointInsideBoard(startX, startY)) return false;
	if (!isPointInsideBoard(endX, endY)) return false;

	// check collisions?

	return true;
}


int countPartsRemaining(int playerId, char board[BOARD_HEIGHT][BOARD_WIDTH]) {
	int startY = playerId == ALICE ? 0 : DIVIDING_LINE + 1;
	int partsRemaining = 0;
	for (int y = startY; y < startY + BOARD_HEIGHT / 2; y++) {
		for (int x = 0; x < BOARD_WIDTH; x++) {
			if (board[y][x] == SHIP_CHAR)
				partsRemaining++;
		}
	}
	return partsRemaining;
}


void printBoard(char board[BOARD_HEIGHT][BOARD_WIDTH], int printMode) {
	int partsRemainingPlayerA = countPartsRemaining(ALICE, board);
	int partsRemainingPlayerB = countPartsRemaining(BOB, board);
	if (partsRemainingPlayerA == 0 && partsRemainingPlayerB == 0)
		return;

	for (int y = 0; y < BOARD_HEIGHT; y++) {
		for (int x = 0; x < BOARD_WIDTH; x++) {
			std::cout << board[y][x];
		}
		std::cout << std::endl;
	}
	printf("PARTS REMAINING:: A : %d B : %d\n", partsRemainingPlayerA, partsRemainingPlayerB);
}


int checkWinner(Player players[2], char board[BOARD_HEIGHT][BOARD_WIDTH]) {
	// all ships have to be placed
	bool playerAplacedAll = players[ALICE].availableFleet->areAllShipsPlaced();
	bool playerBplacedAll = players[BOB].availableFleet->areAllShipsPlaced();
	if (!(playerAplacedAll && playerBplacedAll))
		return -1;

	int playerAPartsRemaining = countPartsRemaining(ALICE, board);
	int playerBPartsRemaining = countPartsRemaining(BOB, board);
	if (playerBPartsRemaining == 0)
		return ALICE;
	else if (playerAPartsRemaining == 0)
		return BOB;
	return -1; // nobody wins
}