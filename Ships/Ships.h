#ifndef SHIPS
#define SHIPS

enum COMMANDS { DO_NOTHING, PRINT, SET_FLEET, NEXT_PLAYER, PLACE_SHIP, SHOOT, CLEAR };
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

const int CARRIER_LENGTH = 5;
const int BATTLESHIP_LENGTH = 4;
const int CRUISER_LENGTH = 3;
const int DESTROYER_LENGTH = 2;


// default ships with their number for each player
typedef struct Fleet {

	// CARRIER, BATTLESHIP, CRUISER, DESTROYER 
	// on corresponding to SHIPS enum indexes
	int shipsNumber[4] = { 1, 2, 3, 4 };
	int remainingShips[4] = { 1, 2, 3, 4 };
	int shipUsedIds[4][MAX_NUMBER_OF_SHIPS] = { };

	bool useShip(int shipType) {
		if (remainingShips[shipType] > 0) remainingShips[shipType] -= 1;
		else return false;
		return true;
	}

	bool isShipUsed(int shipType, int shipId) {
		int usedShipsNumber = shipsNumber[shipType] - remainingShips[shipType];
		for (int i = 0; i < usedShipsNumber; i++)
			if (shipUsedIds[shipType][i] == shipId)
				return true;
		return false;
	}
};


typedef struct Player {
	int id = -1;
	Fleet *availableFleet = new Fleet;
};


void prepareBoard(char board[BOARD_HEIGHT][BOARD_WIDTH]);
int countPartsRemaining(int playerId, char board[BOARD_HEIGHT][BOARD_WIDTH]);

bool checkPlace(int playerId, int startX, int endX, int startY, int endY);
int placeShip(char board[BOARD_HEIGHT][BOARD_WIDTH], Player *player, int x, int y, int shipId, int direction, int shipType);
int shoot(char board[BOARD_HEIGHT][BOARD_WIDTH], int x, int y);
void setFleet(Player *player);

int getCommandId(std::string command);
int getPlayerId(char playerInitials);
int getDirectionId(char dirAbbreviation);
int getShipTypeId(std::string shipTypeAbbreviation);
int getShipLength(int shipTypeId);

#endif