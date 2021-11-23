#include <iostream>
#include "Getters.h"
#include "ShipsDataTypes.h"
#include "Board.h"


// places ship on the board if it is possible
// x, y is the bow of the ship
int placeShip(Board* board, Player* player, int x, int y, int shipId, int direction, int shipTypeId, char shipPartsStates[SHIP_PART_STATES_LENGTH], bool gameStart=false) {
	int shipLength = getShipLength(shipTypeId);
	int startX = x, endX = x, startY = y, endY = y;

	// -1 because startY takes one cell
	if (direction == NORTH) endY = y + shipLength - 1;
	else if (direction == SOUTH) endY = y - shipLength + 1;
	else if (direction == EAST) endX = x - shipLength + 1;
	else if (direction == WEST) endX = x + shipLength - 1;
	else return Result.undefined;

	// make sure that startPos is smaller or equal to endPos
	if (startX > endX) std::swap(startX, endX);
	if (startY > endY) std::swap(startY, endY);

	int cellsContent = board->checkPlace(player->id, startX, endX, startY, endY, gameStart);
	if (cellsContent == OUTSIDE_BOARD)
		return Result.invalidPosition;
	else if (cellsContent == REEF_CELLS)
		return Result.reef;
	else if (cellsContent == OTHER_SHIP_CELLS)
		return Result.otherShip;

	bool shipAlreadyUsed = player->availableFleet->isShipUsed(shipTypeId, shipId);
	if (shipAlreadyUsed) return Result.shipAlreadyPresent;

	bool enoughShips = player->availableFleet->useShip(x, y, direction, shipTypeId, shipId);
	if (!enoughShips) return Result.shipsExcess;

	int currentShipPart = 0;
	char current_char;
	for (int currentX = startX; currentX <= endX; currentX++)
		for (int currentY = startY; currentY <= endY; currentY++) {
			if (direction == NORTH || direction == WEST || direction == EAST)
				current_char = shipPartsStates[currentShipPart] == '1' ? SHIP_CHAR : DAMAGED_CHAR;
			else // for SOUTH
				current_char = shipPartsStates[shipLength - 1 - currentShipPart] == '1' ? SHIP_CHAR : DAMAGED_CHAR;

			board->setCell(currentX, currentY, current_char);
			currentShipPart++;
		}

	// board->printBoard(0); // debug

	return Result.success;
}


// returns if shot hits
// the shoot is at a position in the board (FIELD DOES NOT EXIST),
// and that all ships that should be placed were already placed (NOT ALL SHIPS PLACED)
int shoot(Board* board, Player players[2], int x, int y) {
	if (!board->isPointInside(x, y)) return Result.invalidPosition;
	if (!players[ALICE].availableFleet->areAllShipsPlaced()) return Result.notEnoughShips;
	if (!players[BOB].availableFleet->areAllShipsPlaced()) return Result.notEnoughShips;

	if (board->getCell(x, y) == SHIP_CHAR)
		board->setCell(x, y, DAMAGED_CHAR);

	return Result.success;
}


int move(Board* board, Player* player, Ship* ship, int moveDirection) {
	// TODO:
	// engine check
	// too close check
	if (ship->movesRemaining <= 0) return Result.shipNoMoves;

	int shipLength = getShipLength(ship->shipTypeId);
	int startX = ship->x, endX = ship->x, startY = ship->y, endY = ship->y;

	// -1 because startY takes one cell
	if (ship->direction == NORTH) endY = ship->y + shipLength - 1;
	else if (ship->direction == SOUTH) endY = ship->y - shipLength + 1;
	else if (ship->direction == EAST) endX = ship->x - shipLength + 1;
	else if (ship->direction == WEST) endX = ship->x + shipLength - 1;
	else return Result.undefined;

	// make sure that startPos is smaller or equal to endPos
	if (startX > endX) std::swap(startX, endX);
	if (startY > endY) std::swap(startY, endY);

	int newStartX = startX, newEndX = endX,
		newStartY = startY, newEndY = endY;

	if (moveDirection == FORWARD) {
		if (ship->direction == NORTH) {
			newStartY = startY - 1;
			newEndY = endY - 1;
		}
		else if (ship->direction == SOUTH) {
			newStartY = startY + 1;
			newEndY = endY + 1;
		}
		else if (ship->direction == EAST) {
			newStartX = startX + 1;
			newEndX = endX + 1;
		}
		else if (ship->direction == WEST) {
			newStartX = startX - 1;
			newEndX = endX - 1;
		}
	}

	// firstly, we have to clear previous ship cells
	// and read states of the ship parts
	int currentShipPart = 0;
	int* shipPartsChars = new int[shipLength];
	for (int currentX = startX; currentX <= endX; currentX++)
		for (int currentY = startY; currentY <= endY; currentY++) {
			shipPartsChars[currentShipPart] = board->getCell(currentX, currentY);
			currentShipPart++;
			board->setCell(currentX, currentY, EMPTY_CELL);
		}

	// checkPlace doesnt work, because it detected old position of ship
	int cellsContent = board->checkPlace(player->id, newStartX, newEndX, newStartY, newEndY);
	if (cellsContent == OUTSIDE_BOARD)
		return Result.shipOutsideBoard;
	else if (cellsContent == REEF_CELLS)
		return Result.shipOnReef;
	else if (cellsContent == OTHER_SHIP_CELLS)
		return Result.shipTooClose;

	
	// everything OK, lets move

	currentShipPart = 0;
	// place the ship in new position
	for (int currentX = newStartX; currentX <= newEndX; currentX++)
		for (int currentY = newStartY; currentY <= newEndY; currentY++) {
			char current_char = shipPartsChars[currentShipPart];
			board->setCell(currentX, currentY, current_char);
			currentShipPart++;
		}

	// update ship values
	if (ship->direction == NORTH) {
		ship->x = newStartX;
		ship->y = newStartY;
	}
	else if (ship->direction == SOUTH) {
		ship->x = newEndX;
		ship->y = newEndY;
	}
	else if (ship->direction == EAST) {
		ship->x = newEndX;
		ship->y = newEndY;
	}
	else if (ship->direction == WEST) {
		ship->x = newStartX;
		ship->y = newStartY;
	}

	// board->printBoard(0); // debug
	delete shipPartsChars;
	return 0;
}


// set number of each ship for player
void setFleet(Player* player) {
	int quantity;
	for (int shipTypeId = CARRIER; shipTypeId <= DESTROYER; shipTypeId++) {
		std::cin >> quantity;
		int actualShipsNumber = player->availableFleet->shipsNumber[shipTypeId];
		int actualShipsRemaining = player->availableFleet->shipsNumber[shipTypeId];
		int usedShipsNumber = actualShipsNumber - actualShipsRemaining;

		player->availableFleet->shipsNumber[shipTypeId] = quantity;
		player->availableFleet->remainingShips[shipTypeId] = quantity - usedShipsNumber;
	}
}

