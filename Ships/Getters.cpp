#include <iostream>
#include "ships.h"


int getCommandId(char* command) {
	if (strcmp(command, "PRINT") == 0) return PRINT;
	if (strcmp(command, "SET_FLEET") == 0) return SET_FLEET;
	if (strcmp(command, "NEXT_PLAYER") == 0) return NEXT_PLAYER;
	if (strcmp(command, "PLACE_SHIP") == 0) return PLACE_SHIP;
	if (strcmp(command, "SHOOT") == 0) return SHOOT;
	if (strcmp(command, "CLEAR") == 0) return CLEAR;
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