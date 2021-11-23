#pragma once
#include "ShipsDataTypes.h"
#include "Board.h"

int placeShip(
	Board* board, Player* player, 
	int x, int y, int shipId, int direction, 
	int shipTypeId, char shipPartsStates[SHIP_PART_STATES_LENGTH]
);
int shoot(Board* board, Player players[2], int x, int y);
void setFleet(Player* player);