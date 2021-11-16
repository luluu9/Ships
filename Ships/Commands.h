#pragma once
#include "ShipsDataTypes.h"

int placeShip(char board[BOARD_HEIGHT][BOARD_WIDTH], Player* player, int x, int y, int shipId, int direction, int shipType);
int shoot(char board[BOARD_HEIGHT][BOARD_WIDTH], Player players[2], int x, int y);
void setFleet(Player* player);