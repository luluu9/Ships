#pragma once
#include <iostream>
#include "ShipsDataTypes.h"

const int DIVIDING_LINE = 10;
const int DEFAULT_BOARD_WIDTH = 10;
const int DEFAULT_BOARD_HEIGHT = 21;

const char SHIP_CHAR = '+';
const char DAMAGED_CHAR = 'x';
const char DIVIDING_LINE_CHAR = ' ';
const char EMPTY_CELL = ' ';
const char REEF_CHAR = '#';


struct Board {
	int BOARD_WIDTH = DEFAULT_BOARD_WIDTH;
	int BOARD_HEIGHT = DEFAULT_BOARD_HEIGHT;
	char* cells = new char[BOARD_WIDTH * BOARD_HEIGHT];
	// array of pointers to ships on corresponding cells
	void** shipCells = new void* [BOARD_WIDTH * BOARD_HEIGHT];
	Player* players;

	// Player's rectangle allows Player to place ships in specified area (inclusive)
	enum rectangleIndexes { minXInd, minYInd, maxXInd, maxYInd };
	int playersRectangle[2][4] = {
		{0, 0, BOARD_WIDTH - 1, DIVIDING_LINE - 1 },
		{ 0, DIVIDING_LINE + 1, BOARD_WIDTH - 1, BOARD_HEIGHT - 1 }
	};


	// BASE METHODS

	bool isPointInside(int x, int y) const {
		if (x < 0 || y < 0) return false;
		if (x >= BOARD_WIDTH || y >= BOARD_HEIGHT) return false;
		return true;
	};

	bool isReef(int x, int y) const {
		if (cells[getIndex(x, y)] == REEF_CHAR)
			return true;
		return false;
	};

	int getIndex(int x, int y, int CUSTOM_BOARD_WIDTH = 0) const {
		if (CUSTOM_BOARD_WIDTH) return x + y * CUSTOM_BOARD_WIDTH;
		return x + y * BOARD_WIDTH;
	};

	char getCell(int x, int y) const {
		if (isPointInside(x, y))
			return cells[getIndex(x, y)];
		printf("BAD INDEX FOR getCell METHOD: (%d, %d)", x, y);
		return 0;
	};

	void setCell(int x, int y, char new_value, Ship* shipPtr = nullptr) {
		if (isPointInside(x, y)) {
			int index = getIndex(x, y);
			cells[index] = new_value;
			shipCells[index] = shipPtr;
		}
		else
			printf("BAD INDEX FOR setCell METHOD: (%d, %d)", x, y);
	};

	void clearBoard() {
		for (int y = 0; y < BOARD_HEIGHT; y++)
			for (int x = 0; x < BOARD_WIDTH; x++) {
				int index = getIndex(x, y);
				cells[index] = ' ';
				shipCells[index] = nullptr;
			}

	};

	void prepareBoard(Player* n_players) {
		clearBoard();
		players = n_players;
	}


	// FOR RESIZING BOARD

	void prepareCustomBoard(char* customCells, void** customShipCells, int CUSTOM_BOARD_HEIGHT, int CUSTOM_BOARD_WIDTH) const {
		for (int y = 0; y < CUSTOM_BOARD_HEIGHT; y++)
			for (int x = 0; x < CUSTOM_BOARD_WIDTH; x++) {
				int index = getIndex(x, y, CUSTOM_BOARD_WIDTH);
				customCells[index] = ' ';
				customShipCells[index] = nullptr;
			}
	};

	void copyCells(char* targetArray, void** targetShipsArray) const {
		for (int y = 0; y < BOARD_HEIGHT; y++)
			for (int x = 0; x < BOARD_WIDTH; x++) {
				int currentIndex = getIndex(x, y);
				targetArray[currentIndex] = cells[currentIndex];
				targetShipsArray[currentIndex] = shipCells[currentIndex];
			}
	};

	void setBoardSize(int newWidth, int newHeight) {
		char* newCells = new char[newWidth * newHeight];
		void** newShipCells = new void* [newWidth * newHeight];
		prepareCustomBoard(newCells, newShipCells, newWidth, newHeight);
		copyCells(newCells, newShipCells);
		delete[] cells;
		delete[] shipCells;
		cells = newCells;
		shipCells = newShipCells;
		BOARD_WIDTH = newWidth;
		BOARD_HEIGHT = newHeight;
	};


	// HELPERS

	int countPartsRemaining(int playerId) const {
		int partsRemaining = 0;
		Player player = players[playerId];
		for (int i = 0; i < NUMBER_OF_SHIP_TYPES; i++) {
			for (int j = 0; j < MAX_NUMBER_OF_SHIPS; j++) {
				Ship* currentShip = player.availableFleet->ships[i][j];
				if (currentShip != nullptr)
					partsRemaining += currentShip->getValidPartsRemaining();
			}
		}
		return partsRemaining;
	};

	bool isOccupiedByShip(int x, int y) const {
		if (!isPointInside(x, y)) return false;
		char cellContent = getCell(x, y);
		if (cellContent == SHIP_CHAR || cellContent == DAMAGED_CHAR)
			return true;
		return false;
	}

	Ship* getShipPtr(int x, int y) const {
		return static_cast<Ship*>(shipCells[getIndex(x, y)]);
	}


	// COMMANDS

	void printBoard(int printMode) const {
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

	void printPlayerBoard(int printMode, Player* player) {
		//printf("%s", player->id == 0 ? "ALICE" : "BOB");

		bool* visibleCells = new bool[BOARD_WIDTH * BOARD_HEIGHT];
		for (int y = 0; y < BOARD_HEIGHT; y++)
			for (int x = 0; x < BOARD_WIDTH; x++)
				visibleCells[getIndex(x, y)] = false;

		Ship* (*ships)[NUMBER_OF_SHIP_TYPES][MAX_NUMBER_OF_SHIPS] = &player->availableFleet->ships;
		for (int i = 0; i < NUMBER_OF_SHIP_TYPES; i++)
			for (int j = 0; j < MAX_NUMBER_OF_SHIPS; j++)
				if (player->availableFleet->ships[i][j] != nullptr) {
					Ship* ship = player->availableFleet->ships[i][j];
					int shipLength = ship->shipLength;
					int shipX = ship->x;
					int shipY = ship->y;
					int radarCells = shipLength;
					int planesSend = ship->planesSend;
					if (!ship->radarWorks()) {
						int direction = ship->direction;
						int startX = shipX, endX = shipX, startY = shipY, endY = shipY;
						// make this ship visible 
						if (direction == NORTH) endY = startY + shipLength - 1;
						else if (direction == SOUTH) endY = startY - shipLength + 1;
						else if (direction == EAST) endX = startX - shipLength + 1;
						else if (direction == WEST) endX = startX + shipLength - 1;

						if (startX > endX) std::swap(startX, endX);
						if (startY > endY) std::swap(startY, endY);

						for (int currentX = startX; currentX <= endX; currentX++)
							for (int currentY = startY; currentY <= endY; currentY++)
								visibleCells[getIndex(currentX, currentY)] = true;

						radarCells = 1;
					}

					// make visible cells in range of radar
					for (int x = shipX - radarCells; x <= shipX + radarCells; x++)
						for (int y = shipY - radarCells; y <= shipY + radarCells; y++) {
							if (!isPointInside(x, y))
								continue;
							double distance = ship->calcDistance(shipX, x, shipY, y);
							if (distance <= radarCells)
								visibleCells[getIndex(x, y)] = true;
						}

					// make visible cells in range of planes
					for (int i = 0; i < planesSend; i++) {
						int planeX = ship->planesCoords[i][0];
						int planeY = ship->planesCoords[i][1];
						for (int x = planeX - 1; x <= planeX + 1; x++)
							for (int y = planeY - 1; y <= planeY + 1; y++) {
								if (!isPointInside(x, y))
									continue;
								visibleCells[getIndex(x, y)] = true;
							}
					}
				}


		for (int y = 0; y < BOARD_HEIGHT; y++) {
			for (int x = 0; x < BOARD_WIDTH; x++) {
				if (visibleCells[getIndex(x, y)] == true)
					std::cout << getCell(x, y);
				else
					std::cout << '?';
			}
			std::cout << std::endl;
		}

		delete[] visibleCells;
	}

	// check if player can put ship in specified place
	int checkPlace(int playerId, int startX, int endX, int startY, int endY, bool starting = false) const {
		// check this only if players are deploying ships on game start
		if (starting) {
			// check if its player's part of board
			if (startX < playersRectangle[playerId][minXInd] || endX > playersRectangle[playerId][maxXInd])
				return OUTSIDE_BOARD;
			if (startY < playersRectangle[playerId][minYInd] || endY > playersRectangle[playerId][maxYInd])
				return OUTSIDE_BOARD;
		}


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
	int checkWinner() const {
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


	// DESTRUCTOR

	~Board() {
		delete[] cells;
	}

};