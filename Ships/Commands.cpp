#include <iostream>
#include "Getters.h"
#include "ShipsDataTypes.h"
#include "Board.h"


// places ship on the board if it is possible
// x, y is the bow of the ship
// czy i-th ship (shipId u mnie) trzeba sprawdzaæ? 
// po co w ogóle to jest, jeœli wiemy ile statków 
// mo¿e byæ maks (mo¿emy zliczaæ ile by³o dotychczas)?
int placeShip(Board *board, Player* player, int x, int y, int shipId, int direction, int shipType) {
	int shipLength = getShipLength(shipType);
	int startX = x, endX = x, startY = y, endY = y;

	// -1 because startY takes one cell
	if (direction == NORTH) endY = y + shipLength - 1;
	else if (direction == SOUTH) endY = y - shipLength + 1;
	else if (direction == EAST) endX = x - shipLength + 1;
	else if (direction == WEST) endX = x + shipLength - 1;
	else return Result.undefined;

	if (!board->checkPlace(player->id, startX, endX, startY, endY)) {
		// invalid position for ship
		return Result.invalidPosition;
	}

	bool shipAlreadyUsed = player->availableFleet->isShipUsed(shipType, shipId);
	if (shipAlreadyUsed) return Result.shipAlreadyPresent;

	bool enoughShips = player->availableFleet->useShip(shipType);
	if (!enoughShips) return Result.shipsExcess;

	for (int i = 0; i < shipLength; i++) {
		if (direction == NORTH) board->setCell(startX, startY + i, SHIP_CHAR);
		else if (direction == SOUTH) board->setCell(startX, startY - i, SHIP_CHAR);
		else if (direction == EAST) board->setCell(startX - i, startY, SHIP_CHAR);
		else if (direction == WEST) board->setCell(startX + i, startY, SHIP_CHAR);
	}

	return Result.success;
}


// returns if shot hits
// the shoot is at a position in the board (FIELD DOES NOT EXIST),
// and that all ships that should be placed were already placed (NOT ALL SHIPS PLACED)
int shoot(Board *board, Player players[2], int x, int y) {
	if (!board->isPointInside(x, y)) return Result.invalidPosition;
	if (!players[ALICE].availableFleet->areAllShipsPlaced()) return Result.notEnoughShips;
	if (!players[BOB].availableFleet->areAllShipsPlaced()) return Result.notEnoughShips;

	if (board->getCell(x, y) == SHIP_CHAR)
		board->setCell(x, y, DAMAGED_CHAR);

	return Result.success;
}


// set number of each ship for player
void setFleet(Player* player) {
	int quantity;
	for (int shipType = CARRIER; shipType <= DESTROYER; shipType++) {
		std::cin >> quantity;
		int actualShipsNumber = player->availableFleet->shipsNumber[shipType];
		int actualShipsRemaining = player->availableFleet->shipsNumber[shipType];
		int usedShipsNumber = actualShipsNumber - actualShipsRemaining;

		player->availableFleet->shipsNumber[shipType] = quantity;
		player->availableFleet->remainingShips[shipType] = quantity - usedShipsNumber;
	}
}