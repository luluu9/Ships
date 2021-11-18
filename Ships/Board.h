#pragma once
#include <iostream>
#include "ShipsDataTypes.h"

const int DIVIDING_LINE = 10;
const int DEFAULT_BOARD_WIDTH = 10;
const int DEFAULT_BOARD_HEIGHT = 21;

const char SHIP_CHAR = '+';
const char DAMAGED_CHAR = 'x';
const char DIVIDING_LINE_CHAR = ' ';
const char REEF_CHAR = '#';


struct Board {
	int BOARD_WIDTH = DEFAULT_BOARD_WIDTH;
	int BOARD_HEIGHT = DEFAULT_BOARD_HEIGHT;
	char* cells = new char[BOARD_WIDTH * BOARD_HEIGHT];

	// Player's rectangle allows Player to place ships in specified area (inclusive)
	enum rectangleIndexes { minXInd, minYInd, maxXInd, maxYInd };
	int playersRectangle[2][4] = {
		{0, 0, BOARD_WIDTH - 1, DIVIDING_LINE - 1 },
		{ 0, DIVIDING_LINE + 1, BOARD_WIDTH - 1, BOARD_HEIGHT - 1 }
	};


	// BASE METHODS

	bool isPointInside(int x, int y) {
		if (x < 0 || y < 0) return false;
		if (x >= BOARD_WIDTH || y >= BOARD_HEIGHT) return false;
		return true;
	};

	bool isReef(int x, int y) {
		if (cells[getIndex(x, y)] == REEF_CHAR)
			return true;
		return false;
	};

	int getIndex(int x, int y, int CUSTOM_BOARD_WIDTH=0) {
		if (CUSTOM_BOARD_WIDTH) return x + y * CUSTOM_BOARD_WIDTH;
		return x + y * BOARD_WIDTH;
	};

	char getCell(int x, int y) {
		if (isPointInside(x, y))
			return cells[getIndex(x, y)];
		printf("BAD INDEX FOR getCell METHOD: (%d, %d)", x, y);
		return 0;
	};

	void setCell(int x, int y, char new_value) {
		if (isPointInside(x, y))
			cells[getIndex(x, y)] = new_value;
		else
			printf("BAD INDEX FOR setCell METHOD: (%d, %d)", x, y);
	};

	void prepareBoard() {
		for (int y = 0; y < BOARD_HEIGHT; y++)
			for (int x = 0; x < BOARD_WIDTH; x++)
				cells[getIndex(x, y)] = ' ';
	};


	// FOR RESIZING BOARD

	void prepareCustomBoard(char* customCells, int CUSTOM_BOARD_HEIGHT, int CUSTOM_BOARD_WIDTH) {
		for (int y = 0; y < CUSTOM_BOARD_HEIGHT; y++)
			for (int x = 0; x < CUSTOM_BOARD_WIDTH; x++)
				customCells[getIndex(x, y, CUSTOM_BOARD_WIDTH)] = ' ';
	};

	void copyCells(char* targetArray) {
		for (int y = 0; y < BOARD_HEIGHT; y++)
			for (int x = 0; x < BOARD_WIDTH; x++) {
				int currentIndex = getIndex(x, y);
				targetArray[currentIndex] = cells[currentIndex];
			}
	};

	void setBoardSize(int newWidth, int newHeight) {
		char* newCells = new char[newWidth * newHeight];
		prepareCustomBoard(newCells, newWidth, newHeight);
		copyCells(newCells);
		delete[] cells;
		cells = newCells;
		BOARD_WIDTH = newWidth;
		BOARD_HEIGHT = newHeight;
	};


	// HELPERS

	int countPartsRemaining(int playerId) {
		int partsRemaining = 0;
		int startY = playersRectangle[playerId][minYInd];
		int endY = playersRectangle[playerId][maxYInd];
		int startX = playersRectangle[playerId][minXInd];
		int endX = playersRectangle[playerId][maxXInd];
		for (int y = startY; y <= endY; y++) {
			for (int x = startX; x <= endX; x++) {
				if (getCell(x, y) == SHIP_CHAR)
					partsRemaining++;
			}
		}
		return partsRemaining;
	};

	bool isOccupiedByShip(int x, int y) {
		if (!isPointInside(x, y)) return false;
		char cellContent = getCell(x, y);
		if (cellContent == SHIP_CHAR || cellContent == DAMAGED_CHAR)
			return true;
		return false;
	}


	// COMMANDS

	void printBoard(int printMode) {
		int partsRemainingPlayerA = countPartsRemaining(ALICE);
		int partsRemainingPlayerB = countPartsRemaining(BOB);
		if (partsRemainingPlayerA == 0 && partsRemainingPlayerB == 0)
			return;

		for (int y = 0; y < BOARD_HEIGHT; y++) {
			for (int x = 0; x < BOARD_WIDTH; x++) {
				std::cout << getCell(x, y);
			}
			std::cout << std::endl;
		}
		printf("PARTS REMAINING:: A : %d B : %d\n", partsRemainingPlayerA, partsRemainingPlayerB);
	};

	// check if player can put ship in specified place
	int checkPlace(int playerId, int startX, int endX, int startY, int endY) {
		// check if its player's part of board
		if (startX < playersRectangle[playerId][minXInd] || endX > playersRectangle[playerId][maxXInd])
			return OUTSIDE_BOARD;
		if (startY < playersRectangle[playerId][minYInd] || endY > playersRectangle[playerId][maxYInd])
			return OUTSIDE_BOARD;

		// check borders
		if (!isPointInside(startX, startY)) return OUTSIDE_BOARD;
		if (!isPointInside(endX, endY)) return OUTSIDE_BOARD;

		// check reefs
		for (int x = startX; x <= endX; x++)
			for (int y = startY; y <= endY; y++)
				if (isReef(x, y))
					return REEF_CELLS;

		// check other ships
		const int cellsAround = 1;
		for (int x = startX - cellsAround; x <= endX + cellsAround; x++)
			for (int y = startY - cellsAround; y <= endY + cellsAround; y++)
				if (isOccupiedByShip(x, y))
					return OTHER_SHIP_CELLS;

		return EMPTY_CELLS;
	};

	// returns id of winning Player
	// if there is no winner, returns -1
	int checkWinner(Player players[2]) {
		// all ships have to be placed
		bool playerAplacedAll = players[ALICE].availableFleet->areAllShipsPlaced();
		bool playerBplacedAll = players[BOB].availableFleet->areAllShipsPlaced();
		if (!(playerAplacedAll && playerBplacedAll))
			return -1;

		int playerAPartsRemaining = countPartsRemaining(ALICE);
		int playerBPartsRemaining = countPartsRemaining(BOB);
		if (playerBPartsRemaining == 0)
			return ALICE;
		else if (playerAPartsRemaining == 0)
			return BOB;
		return -1; // nobody wins
	};

	void limitPosition(int playerId, int minX, int minY, int maxX, int maxY) {
		playersRectangle[playerId][minXInd] = minX;
		playersRectangle[playerId][minYInd] = minY;
		playersRectangle[playerId][maxXInd] = maxX;
		playersRectangle[playerId][maxYInd] = maxY;
	};

	void setReef(int x, int y) {
		setCell(x, y, REEF_CHAR);
	};


	// DESTROYER

	~Board() {
		delete[] cells;
	}

};