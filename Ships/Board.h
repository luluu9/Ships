#pragma once
#include "ShipsDataTypes.h"

void prepareBoard(char board[BOARD_HEIGHT][BOARD_WIDTH]);
bool isPointInsideBoard(int x, int y);
bool checkPlace(int playerId, int startX, int endX, int startY, int endY);
void printBoard(char board[BOARD_HEIGHT][BOARD_WIDTH], int printMode);
int countPartsRemaining(int playerId, char board[BOARD_HEIGHT][BOARD_WIDTH]);
int checkWinner(Player players[2], char board[BOARD_HEIGHT][BOARD_WIDTH]);