#pragma once
#include "Getters.h"
#include "string.h"

enum COMMANDS {
	DO_NOTHING, OTHER_PLAYER_TURN, END_TURN, STATE,               // FUNDAMENTAL COMMANDS
	PRINT, SET_FLEET, NEXT_PLAYER, PLACE_SHIP, BASE_SHOOT, CLEAR, // BASIC
	BOARD_SIZE, INIT_POSITION, REEF, SHIP, EXTENDED_SHIPS,		  // EXTENDED
	SET_AI_PLAYER, /*PRINT,*/ SRAND, SAVE, MOVE, EXT_SHOOT,		  // --------
	PRINT_STATE, SPY												  // --------
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

	const char* SHOOT[5] = {
		"FIELD DOES NOT EXIST",
		"NOT ALL SHIPS PLACED",
		"SHOOTING TOO FAR",
		"SHIP CANNOT SHOOT",
		"TOO MANY SHOOTS"
	};

	const char* STATE[1] = {
		"THE OTHER PLAYER EXPECTED"
	};

	const char* MOVE[5] = {
		"SHIP CANNOT MOVE",
		"SHIP MOVED ALREADY",
		"PLACING SHIP ON REEF",
		"SHIP WENT FROM BOARD",
		"PLACING SHIP TOO CLOSE TO OTHER SHIP"
	};

	const char* PLANE[2] = {
		"CANNOT SEND PLANE",
		"ALL PLANES SENT",
	};

	enum RESULTS {
		undefined = -2, success = -1,								       // COMMON_RESULTS
		invalidPosition = 0, shipAlreadyPresent = 1, shipsExcess = 2,	   // PLACE_SHIP_RESULTS
		reef = 3, otherShip = 4,										   // -------------
		/* invalidPosition = 0, */ notEnoughShips = 1, shootTooFar = 2,	   // SHOOT_RESULTS
		cannonDestroyed = 3, tooManyShoots = 4,							   // -------------
		otherPlayerExcepted = 0,										   // STATE_RESULTS
		shipBrokenEngine = 0, shipNoMoves = 1, shipOnReef = 2,			   // MOVE_RESULTS
		shipOutsideBoard = 3, shipTooClose = 4,							   // -------------
		cannonPlaneDestroyed = 0, tooManyPlanes = 1						   // PLANE_RESULTS
	};


} Result;


struct Ship {
	int x, y, direction;
	int shipTypeId;
	int shipId;
	int movesRemaining = DEFAULT_SHIPS_MOVES;
	int shootsRemaining;
	int planesRemaining;
	int shipLength;
	char partsState[SHIP_PART_STATES_LENGTH] = {};

	const int CANNON_INDEX = 1;
	const int RADAR_INDEX = 1;
	int ENGINE_INDEX = 1;

	int planesCoords[CARRIER_LENGTH][2];
	int planesSend = 0;

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
		if (shipTypeId == CARRIER)
			movesRemaining = DEFAULT_CARRIER_MOVES;
		else
			movesRemaining = DEFAULT_SHIPS_MOVES;
		shootsRemaining = shipLength;
		planesSend = 0;
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

	bool engineWorks() const {
		if (partsState[ENGINE_INDEX] == DESTROYED_PART_CHAR_ID)
			return false;
		return true;
	}

	bool radarWorks() const {
		if (partsState[RADAR_INDEX] == DESTROYED_PART_CHAR_ID)
			return false;
		return true;
	}

	double calcDistance(double x1, double x2, double y1, double y2) const {
		return sqrt(pow(abs(y1 - y2), 2) + pow(abs(x1 - x2), 2));
	}

	int prepareShooting(int enemyX, int enemyY) {
		if (partsState[CANNON_INDEX] == DESTROYED_PART_CHAR_ID)
			return Result.cannonDestroyed;
		if (shootsRemaining <= 0)
			return Result.tooManyShoots;

		int cannonX = x, cannonY = y;
		if (direction == NORTH)
			cannonY = y + 1;
		else if (direction == EAST)
			cannonX = x - 1;
		else if (direction == SOUTH)
			cannonY = y - 1;
		else
			cannonX = x + 1;

		double distance = calcDistance(cannonX, enemyX, cannonY, enemyY);
		if (shipTypeId == CARRIER || distance <= shipLength) {
			shootsRemaining--;
			return Result.success;
		}

		return Result.shootTooFar;
	}

	int prepareSendingPlane(int planeX, int planeY) {
		if (partsState[CANNON_INDEX] == DESTROYED_PART_CHAR_ID)
			return Result.cannonPlaneDestroyed;
		if (shootsRemaining <= 0)
			return Result.tooManyPlanes;
		shootsRemaining--;

		planesCoords[planesSend][0] = planeX;
		planesCoords[planesSend][1] = planeY;
		planesSend++;
		return Result.success;
	}

	Ship(int n_x, int n_y, int n_direction, int n_shipTypeId, int n_shipId, char* n_partsStates) {
		x = n_x;
		y = n_y;
		direction = n_direction;
		shipTypeId = n_shipTypeId;
		shipId = n_shipId;
		shipLength = getShipLength(shipTypeId);
		ENGINE_INDEX = shipLength - 1;
		strncpy_s(partsState, sizeof(char) * (SHIP_PART_STATES_LENGTH), n_partsStates, shipLength + 1);
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

	bool useShip(int x, int y, int direction, int shipTypeId, int shipId, char* shipPartsStates) {
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

	Fleet() {
		for (int i = 0; i < NUMBER_OF_SHIP_TYPES; i++)
			for (int j = 0; j < MAX_NUMBER_OF_SHIPS; j++)
				ships[i][j] = nullptr;
	}
};


struct Player {
	int id = -1;
	Fleet* availableFleet = new Fleet;
};
