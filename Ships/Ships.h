#pragma once

enum COMMANDS { DO_NOTHING, OTHER_PLAYER_TURN, END_TURN, STATE, PRINT, SET_FLEET, NEXT_PLAYER, PLACE_SHIP, SHOOT, CLEAR };
enum DIRECTIONS { NORTH, EAST, SOUTH, WEST };
enum PLAYERS { ALICE, BOB }; // [A]lice = 0, [B]ob = 1
enum SHIPS { CARRIER, BATTLESHIP, CRUISER, DESTROYER };

const int DIVIDING_LINE = 10;
const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 21;

const char SHIP_CHAR = '+';
const char DAMAGED_CHAR = 'x';
const char DIVIDING_LINE_CHAR = ' ';

const int MAX_NUMBER_OF_SHIPS = 10;
const int NUMBER_OF_SHIP_TYPES = 4;

const int CARRIER_LENGTH = 5;
const int BATTLESHIP_LENGTH = 4;
const int CRUISER_LENGTH = 3;
const int DESTROYER_LENGTH = 2;


static const struct Result {
	const char* PLACE_SHIP[3] = {
		"NOT IN STARTING POSITION",          // invalidPosition
		"SHIP ALREADY PRESENT",              // shipAlreadyPresent
		"ALL SHIPS OF THE CLASS ALREADY SET" // shipsExcess
	};

	const char* SHOOT[2] = {
		"FIELD DOES NOT EXIST",
		"NOT ALL SHIPS PLACED"
	};

	const char* STATE[1] = {
		"THE OTHER PLAYER EXPECTED"
	};

	enum RESULTS {
		undefined = -2, success = -1,								  // COMMON_RESULTS
		invalidPosition = 0, shipAlreadyPresent = 1, shipsExcess = 2, // PLACE_SHIP_RESULTS
		/* invalidPosition = 0, */ notEnoughShips = 1,                // SHOOT_RESULTS
		otherPlayerExcepted = 0										  // STATE_RESULTS
	};


} Result;


// default ships with their number for each player
struct Fleet {

	// CARRIER, BATTLESHIP, CRUISER, DESTROYER 
	// on corresponding to SHIPS enum indexes
	int shipsNumber[NUMBER_OF_SHIP_TYPES] = { 1, 2, 3, 4 };
	int remainingShips[NUMBER_OF_SHIP_TYPES] = { 1, 2, 3, 4 };
	int shipUsedIds[NUMBER_OF_SHIP_TYPES][MAX_NUMBER_OF_SHIPS] = { };

	bool useShip(int shipType) {
		if (remainingShips[shipType] > 0) remainingShips[shipType] -= 1;
		else return false;
		return true;
	}

	bool isShipUsed(int shipType, int shipId) const {
		int usedShipsNumber = shipsNumber[shipType] - remainingShips[shipType];
		for (int i = 0; i < usedShipsNumber; i++)
			if (shipUsedIds[shipType][i] == shipId)
				return true;
		return false;
	}

	bool areAllShipsPlaced() const {
		for (int i = 0; i < NUMBER_OF_SHIP_TYPES; i++)
			if (remainingShips[i] > 0) return false;
		return true;
	}
};


struct Player {
	int id = -1;
	Fleet* availableFleet = new Fleet;
};


void printProblem(char* commandText, const char* problemText);
bool switchPlayerTurn(int currentCommandPlayerId, bool* playerCommandsToChange, int* currentStatePlayer, int* previousStatePlayer);
int handleInput(char* command, int* currentStatePlayer, int* previousStatePlayer);
void handleResult(int commandId, int resultId, char* commandText);