#include <iostream>
#include "ShipsDataTypes.h"


int getCommandId(char* command) {
	if (strcmp(command, "PRINT") == 0) return PRINT;
	if (strcmp(command, "SET_FLEET") == 0) return SET_FLEET;
	if (strcmp(command, "NEXT_PLAYER") == 0) return NEXT_PLAYER;
	if (strcmp(command, "PLACE_SHIP") == 0) return PLACE_SHIP;
	if (strcmp(command, "SHOOT") == 0) return BASE_SHOOT;
	if (strcmp(command, "CLEAR") == 0) return CLEAR;
	if (strcmp(command, "BOARD_SIZE") == 0) return BOARD_SIZE;
	if (strcmp(command, "INIT_POSITION") == 0) return INIT_POSITION;
	if (strcmp(command, "REEF") == 0) return REEF;
	if (strcmp(command, "SHIP") == 0) return SHIP;
	if (strcmp(command, "EXTENDED_SHIPS") == 0) return EXTENDED_SHIPS;
	if (strcmp(command, "SET_AI_PLAYER") == 0) return SET_AI_PLAYER;
	if (strcmp(command, "SRAND") == 0) return SRAND;
	if (strcmp(command, "SAVE") == 0) return SAVE;
	if (strcmp(command, "MOVE") == 0) return MOVE;
	std::cout << "GOT BAD COMMAND IN getCommandId FUNCTION" << std::endl;
	return -1;
}


int getPlayerId(char playerInitials) {
	if (playerInitials == 'A') return ALICE;
	if (playerInitials == 'B') return BOB;
	std::cout << "GOT BAD PLAYER INITIALS IN getPlayerId FUNCTION" << std::endl;
	return -1;
}


int getDirectionId(char dirAbbreviation) {
	if (dirAbbreviation == 'N') return NORTH;
	if (dirAbbreviation == 'E') return EAST;
	if (dirAbbreviation == 'S') return SOUTH;
	if (dirAbbreviation == 'W') return WEST;
	std::cout << "GOT BAD DIRECTION ABBREVIATION IN getDirectionId FUNCTION" << std::endl;
	return -1;
}


int getShipTypeId(char* shipTypeAbbreviation) {
	if (strcmp(shipTypeAbbreviation, "CAR") == 0) return CARRIER;
	if (strcmp(shipTypeAbbreviation, "BAT") == 0) return BATTLESHIP;
	if (strcmp(shipTypeAbbreviation, "CRU") == 0) return CRUISER;
	if (strcmp(shipTypeAbbreviation, "DES") == 0) return DESTROYER;
	std::cout << "GOT BAD SHIP TYPE ABBREVIATION IN getShipTypeId FUNCTION" << std::endl;
	return -1;
}


int getShipLength(int shipTypeId) {
	if (shipTypeId == CARRIER) return CARRIER_LENGTH;
	if (shipTypeId == BATTLESHIP) return BATTLESHIP_LENGTH;
	if (shipTypeId == CRUISER) return CRUISER_LENGTH;
	if (shipTypeId == DESTROYER) return DESTROYER_LENGTH;
	std::cout << "GOT BAD SHIP TYPE ID IN getShipLength FUNCTION" << std::endl;
	return -1;
}

int getMoveDirectionId(char moveChar) {
	if (moveChar == 'F') return FORWARD;
	if (moveChar == 'L') return LEFT;
	if (moveChar == 'R') return RIGHT;
	std::cout << "GOT BAD MOVE CHAR IN getMoveDirectionId FUNCTION" << std::endl;
	return -1;
}