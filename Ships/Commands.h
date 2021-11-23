#pragma once
#include "ShipsDataTypes.h"
#include "Board.h"

int placeShip(
	Board* board, Player* player,
	int x, int y, int shipId, int direction,
	int shipTypeId, char shipPartsStates[SHIP_PART_STATES_LENGTH],
	bool gameStart = false
);
int shoot(Board* board, Player players[2], int x, int y);
int move(Board* board, Player* player, Ship* ship, int moveDirection);
void setFleet(Player* player);
