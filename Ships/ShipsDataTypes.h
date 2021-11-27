#pragma once
#include "Getters.h"
#include "string.h"

enum COMMANDS {
	DO_NOTHING, OTHER_PLAYER_TURN, END_TURN, STATE,               // FUNDAMENTAL COMMANDS
	PRINT, SET_FLEET, NEXT_PLAYER, PLACE_SHIP, BASE_SHOOT, CLEAR, // BASIC
	BOARD_SIZE, INIT_POSITION, REEF, SHIP, EXTENDED_SHIPS,		  // EXTENDED
	SET_AI_PLAYER, /*PRINT,*/ SRAND, SAVE, MOVE, EXT_SHOOT		  // --------
};
enum DIRECTIONS { NORTH, EAST, SOUTH, WEST };
enum PLAYERS { ALICE, BOB }; // [A]lice = 0, [B]ob = 1
enum SHIPS { CARRIER, BATTLESHIP, CRUISER, DESTROYER };
enum CELLS { EMPTY_CELLS, OUTSIDE_BOARD, REEF_CELLS, OTHER_SHIP_CELLS };
enum MOVE_DIRECTIONS { LEFT = -1, FORWARD, RIGHT = 1 };

const int NUMBER_OF_PLAYERS = 2;

const int MAX_NUMBER_OF_SHIPS = 10;
const int NUMBER_OF_SHIP_TYPES = 4;

const int CARRIER_LENGTH = 5;
const int BATTLESHIP_LENGTH = 4;
const int CRUISER_LENGTH = 3;
const int DESTROYER_LENGTH = 2;

const int DEFAULT_CARRIER_MOVES = 2;
const int DEFAULT_SHIPS_MOVES = 3;

const int SHIP_TYPE_ABBRV_LENGTH = 2;
const int SHIP_PART_STATES_LENGTH = 6; // 6 because of \0 end character

const int COMMAND_MAX_CHARS = 100;

const char DESTROYED_PART_CHAR_ID = '0';
const char VALID_PART_CHAR_ID = '1';


static const struct Result {
	const char* PLACE_SHIP[5] = {
		"NOT IN STARTING POSITION",            // invalidPosition
		"SHIP ALREADY PRESENT",                // shipAlreadyPresent
		"ALL SHIPS OF THE CLASS ALREADY SET",  // shipsExcess
		"PLACING SHIP ON REEF", 		       // reef
		"PLACING SHIP TOO CLOSE TO OTHER SHIP" // otherShip
	};

	const char* SHOOT[2] = {
		"FIELD DOES NOT EXIST",
		"NOT ALL SHIPS PLACED"
	};

	const char* STATE[1] = {
		"THE OTHER PLAYER EXPECTED"
	};

	const char* MOVE[5] = {
		"SHIP CANNOT MOVE",
		"SHIP MOVED ALREADY",
		"PLACING SHIP ON REEF",
		"SHIP WENT FROM BOARD",
		"PLACING SHIP TOO CLOSE TO OTHER SHIP",
	};

	enum RESULTS {
		undefined = -2, success = -1,								       // COMMON_RESULTS
		invalidPosition = 0, shipAlreadyPresent = 1, shipsExcess = 2,	   // PLACE_SHIP_RESULTS
		reef = 3, otherShip = 4,										   // -------------
		/* invalidPosition = 0, */ notEnoughShips = 1,					   // SHOOT_RESULTS
		otherPlayerExcepted = 0,										   // STATE_RESULTS
		shipBrokenEngine = 0, shipNoMoves = 1, shipOnReef = 2,			   // MOVE_RESULTS
		shipOutsideBoard = 3, shipTooClose = 4							   // -------------
	};


} Result;


struct Ship {
	int x, y, direction;
	int shipTypeId;
	int shipId;
	int movesRemaining = DEFAULT_SHIPS_MOVES;
	int shipLength;
	char partsState[SHIP_PART_STATES_LENGTH] = {};

	//// should these commands be there or in Commands.cpp file?
	//int placeShip() {
	//	// pass
	//}

	//int move(int n_direction) {
	//	// pass
	//	return 1;
	//}

	//int shoot(int n_x, int n_y) {
	//	// pass
	//	return 1;
	//}

	void resetTurn() {
		if (shipTypeId == 1)
			movesRemaining = DEFAULT_CARRIER_MOVES;
		else
			movesRemaining = DEFAULT_SHIPS_MOVES;
	}

	int getDestroyedPartsAmount() const {
		int destroyedParts = 0;
		for (int i = 0; i < shipLength; i++)
			if (partsState[i] == DESTROYED_PART_CHAR_ID)
				destroyedParts++;
		return destroyedParts;
	}

	int getValidPartsRemaining() const {
		return (shipLength - getDestroyedPartsAmount());
	}

	void gotShot(int impactX, int impactY) {
		int destroyedPartId = -1;
		// translate impact coordinates with corresponding directions
		// to get destroyed id in partsStates
		if (direction == NORTH || direction == SOUTH)
			destroyedPartId = abs(impactY - y);
		else
			destroyedPartId = abs(x - impactX);	
		partsState[destroyedPartId] = DESTROYED_PART_CHAR_ID;
	}

	Ship(int n_x, int n_y, int n_direction, int n_shipTypeId, int n_shipId, char* n_partsStates) {
		x = n_x;
		y = n_y;
		direction = n_direction;
		shipTypeId = n_shipTypeId;
		shipId = n_shipId;
		shipLength = getShipLength(shipTypeId);
		strncpy_s(partsState, sizeof(char)*(SHIP_PART_STATES_LENGTH), n_partsStates, shipLength+1);
		resetTurn();
	}

	~Ship() {
	}
};


// default ships with their number for each player
// in ships array corresponding ships are on corresponding index (shipId)
struct Fleet {

	// CARRIER, BATTLESHIP, CRUISER, DESTROYER 
	// on corresponding to SHIPS enum indexes
	int shipsNumber[NUMBER_OF_SHIP_TYPES] = { 1, 2, 3, 4 };
	int remainingShips[NUMBER_OF_SHIP_TYPES] = { 1, 2, 3, 4 };
	// if id of ship is used, corresponding cell equals to 1
	int shipUsedIds[NUMBER_OF_SHIP_TYPES][MAX_NUMBER_OF_SHIPS] = { };
	Ship* ships[NUMBER_OF_SHIP_TYPES][MAX_NUMBER_OF_SHIPS] = { };

	bool useShip(int x, int y, int direction, int shipTypeId, int shipId, char *shipPartsStates) {
		if (remainingShips[shipTypeId] <= 0) return false;
		remainingShips[shipTypeId] -= 1;
		shipUsedIds[shipTypeId][shipId] = 1;

		ships[shipTypeId][shipId] = new Ship(x, y, direction, shipTypeId, shipId, shipPartsStates);
		return true;
	}

	bool isShipUsed(int shipType, int shipId) const {
		if (shipUsedIds[shipType][shipId] == 1) return true;
		return false;
	}

	bool areAllShipsPlaced() const {
		for (int i = 0; i < NUMBER_OF_SHIP_TYPES; i++)
			if (remainingShips[i] > 0) return false;
		return true;
	}

	Ship* getShip(int shipId, int shipTypeId) const {
		return ships[shipTypeId][shipId];
	}

	void resetMoves() {
		for (int i = 0; i < NUMBER_OF_SHIP_TYPES; i++)
			for (int j = 0; j < MAX_NUMBER_OF_SHIPS; j++)
				if (ships[i][j] != nullptr)
					ships[i][j]->resetTurn();
	}
};


struct Player {
	int id = -1;
	Fleet* availableFleet = new Fleet;
};
