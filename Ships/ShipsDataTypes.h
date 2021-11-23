#pragma once

enum COMMANDS {
	DO_NOTHING, OTHER_PLAYER_TURN, END_TURN, STATE,          // FUNDAMENTAL COMMANDS
	PRINT, SET_FLEET, NEXT_PLAYER, PLACE_SHIP, SHOOT, CLEAR, // BASIC
	BOARD_SIZE, INIT_POSITION, REEF, SHIP, EXTENDED_SHIPS,   // EXTENDED
	SET_AI_PLAYER, /*PRINT,*/ SRAND, SAVE					 // --------
};
enum DIRECTIONS { NORTH, EAST, SOUTH, WEST };
enum PLAYERS { ALICE, BOB }; // [A]lice = 0, [B]ob = 1
enum SHIPS { CARRIER, BATTLESHIP, CRUISER, DESTROYER };
enum CELLS { EMPTY_CELLS, OUTSIDE_BOARD, REEF_CELLS, OTHER_SHIP_CELLS };

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

	enum RESULTS {
		undefined = -2, success = -1,								       // COMMON_RESULTS
		invalidPosition = 0, shipAlreadyPresent = 1, shipsExcess = 2,	   // PLACE_SHIP_RESULTS
		reef = 3, otherShip = 4,										   // -------------
		/* invalidPosition = 0, */ notEnoughShips = 1,					   // SHOOT_RESULTS
		otherPlayerExcepted = 0											   // STATE_RESULTS
	};


} Result;


struct Ship {
	int x, y, direction;
	int shipTypeId;
	int shipId;
	int movesRemaining = DEFAULT_SHIPS_MOVES;

	int move(int direction) {
		// pass
		return 1;
	}

	int shoot(int x, int y) {
		// pass
		return 1;
	}

	void resetTurn() {
		if (shipTypeId == 1)
			movesRemaining = DEFAULT_CARRIER_MOVES;
		else
			movesRemaining = DEFAULT_SHIPS_MOVES;
	}

	Ship(int n_x, int n_y, int n_direction, int n_shipTypeId, int n_shipId) {
		x = n_x;
		y = n_y;
		direction = n_direction;
		shipTypeId = n_shipTypeId;
		shipId = n_shipId;
		resetTurn();
	}
};


// default ships with their number for each player
// in ships array corresponding ships are on corresponding index (shipId)
struct Fleet {

	// CARRIER, BATTLESHIP, CRUISER, DESTROYER 
	// on corresponding to SHIPS enum indexes
	int shipsNumber[NUMBER_OF_SHIP_TYPES] = { 1, 2, 3, 4 };
	int remainingShips[NUMBER_OF_SHIP_TYPES] = { 1, 2, 3, 4 };
	int shipUsedIds[NUMBER_OF_SHIP_TYPES][MAX_NUMBER_OF_SHIPS] = { };
	Ship* ships[NUMBER_OF_SHIP_TYPES][MAX_NUMBER_OF_SHIPS] = { };

	bool useShip(int x, int y, int direction, int shipTypeId, int shipId) {
		if (remainingShips[shipTypeId] <= 0) return false;
		remainingShips[shipTypeId] -= 1;
		ships[shipTypeId][shipId] = new Ship(x, y, direction, shipTypeId, shipId);
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
