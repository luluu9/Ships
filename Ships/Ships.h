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


// default ships with their number for each player
typedef struct Fleet {
	int carrier = 1;
	int battleship = 2;
	int cruiser = 3;
	int destroyer = 4;

	bool useShip(int shipType) {
		if (shipType == CARRIER && carrier > 0) carrier -= 1;
		else if (shipType == BATTLESHIP && battleship > 0) battleship -= 1;
		else if (shipType == CRUISER && cruiser > 0) cruiser -= 1;
		else if (shipType == DESTROYER && destroyer > 0) destroyer -= 1;
		else return false;
		return true;
	}
};


typedef struct Player {
	int id = -1;
	Fleet *availableFleet;
};


void prepareBoard(char board[BOARD_HEIGHT][BOARD_WIDTH]);
int countPartsRemaining(int playerId, char board[BOARD_HEIGHT][BOARD_WIDTH]);

bool checkPlace(int playerId, int startX, int endX, int startY, int endY);
int placeShip(char board[BOARD_HEIGHT][BOARD_WIDTH], Player *player, int x, int y, int direction, int shipType);
bool shoot(int x, int y);
void setFleet(Player *player);

int getCommandId(std::string command);
int getPlayerId(char playerInitials);
int getDirectionId(char dirAbbreviation);
int getShipTypeId(std::string shipTypeAbbreviation);
int getShipLength(int shipTypeId);

#endif