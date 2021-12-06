#pragma once
#include "ShipsDataTypes.h"
#include "Board.h"

int placeShip(
	Board* board, Player* player,
	int x, int y, int shipId, int direction,
	int shipTypeId, char shipPartsStates[SHIP_PART_STATES_LENGTH],
	bool gameStart = false
);
int shoot(Board* board, Player players[2], int x, int y, bool extendedShips, Ship* shootingShip = nullptr);
int basicShoot(Board* board, int x, int y);
int extendedShoot(Board* board, Ship* shootingShip, int x, int y);
int spy(Board* board, Ship* sendingPlaneShip, int x, int y);
int move(Board* board, Player* player, Ship* ship, int moveDirection);
void setFleet(Player* player);
void save(Board* board, int nextPlayer, bool extendedShips, int AIPlayerId, int seed);
void AITurn(Board* board, Player players[NUMBER_OF_PLAYERS], int playerId, bool extendedShips, int seed);