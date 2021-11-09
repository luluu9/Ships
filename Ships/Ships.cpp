#include <iostream>
#define DIVIDING_LINE 10
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 21
using namespace std;

enum COMMANDS { PRINT, SET_FLEET, NEXT_PLAYER, PLACE_SHIP, SHOOT };
enum DIRECTIONS { NORTH, EAST, SOUTH, WEST };
enum PLAYERS { ALICE, BOB }; // [A]lice = 0, [B]ob = 1
enum SHIPS { CARRIER, BATTLESHIP, CRUISER, DESTROYER };

const char SHIP_CHAR = '+';
const char DAMAGED_CHAR = 'x';


// default ships with their number for each player
typedef struct Fleet {
	int carrier = 1;
	int battleship = 2;
	int cruiser = 3;
	int destroyer = 4;
};


typedef struct Player {
	int id;
	Fleet availableFleet;
};


// check if player can put ship in specified place
bool checkPlace(int playerId, int startX, int endX, int startY, int endY) {
	
	// check if its player's part of board
	if (playerId == 0) {
		if (startY >= DIVIDING_LINE || endY >= DIVIDING_LINE) // ship exceded forbidden line
			return false;
	}
	else {
		if (startY <= DIVIDING_LINE || endY <= DIVIDING_LINE) // ship exceded forbidden line
			return false;
	}

	// check borders
	if (startX < 0 || endX < 0 || startY < 0 || endY < 0) return false;
	if (startX >= BOARD_WIDTH || endX >= BOARD_WIDTH ||
		startY >= BOARD_HEIGHT || endY >= BOARD_HEIGHT) return false;

	// check collisions?

	return true;
}


// returns if shot hits
bool shoot(int x, int y) {
	return true;
}


void printBoard(char board[21][10]) {
	for (int y = 0; y < BOARD_HEIGHT ; y++) {
		for (int x = 0; x < BOARD_WIDTH; x++) {
			cout << board[y][x];
		}
		cout << endl;
	}
}


// set number of each ship for player
void setFleet(Player player) {
	int quantity;
	for (int i = 0; i < 4; i++) {
		cin >> quantity;
		switch (i) {
		case 0:
			player.availableFleet.carrier = quantity;
			break;
		case 1:
			player.availableFleet.battleship = quantity;
			break;
		case 2:
			player.availableFleet.cruiser = quantity;
			break;
		case 3:
			player.availableFleet.destroyer = quantity;
			break;
		default:
			cout << "BAD INPUT FOR SET_FLEET COMMAND" << endl;
		}
	}
}


// GETTERS - mo�e da si� to ujednolici� do jednej kr�tkiej funkcji?
int getCommandId(string command) {
	if (command == "PRINT") return PRINT;
	if (command == "SET_FLEET") return SET_FLEET;
	if (command == "NEXT_PLAYER") return NEXT_PLAYER;
	if (command == "PRINT") return PRINT;
	if (command == "PLACE_SHIP") return PLACE_SHIP;
	if (command == "SHOOT") return SHOOT;
	cout << "GOT BAD COMMAND IN getCommandId FUNCTION" << endl;
	return -1;
}

int getPlayerId(char playerInitials) {
	if (playerInitials == 'A') return ALICE;
	if (playerInitials == 'B') return BOB;
	cout << "GOT BAD PLAYER INITIALS IN getPlayerId FUNCTION" << endl;
	return -1;
}

int getDirectionId(char dirAbbreviation) {
	if (dirAbbreviation == 'N') return NORTH;
	if (dirAbbreviation == 'E') return EAST;
	if (dirAbbreviation == 'S') return SOUTH;
	if (dirAbbreviation == 'W') return WEST;
	cout << "GOT BAD DIRECTION ABBREVIATION IN getDirectionId FUNCTION" << endl;
	return -1;
}

int getShipTypeId(string shipTypeAbbreviation) {
	if (shipTypeAbbreviation == "CAR") return CARRIER;
	if (shipTypeAbbreviation == "BAT") return BATTLESHIP;
	if (shipTypeAbbreviation == "CRU") return CRUISER;
	if (shipTypeAbbreviation == "DES") return DESTROYER;
	cout << "GOT BAD SHIP TYPE ABBREVIATION IN getShipTypeId FUNCTION" << endl;
	return -1;
}

int getShipLength(int shipTypeId) {
	if (shipTypeId == CARRIER) return 5;
	if (shipTypeId == BATTLESHIP) return 4;
	if (shipTypeId == CRUISER) return 3;
	if (shipTypeId == DESTROYER) return 2;
	cout << "GOT BAD SHIP TYPE ID IN getShipLength FUNCTION" << endl;
	return -1;
}


// places ship on the board if it is possible
// czy i-th ship (shipId u mnie) trzeba sprawdza�? 
// po co w og�le to jest, je�li wiemy ile statk�w 
// mo�e by� maks (mo�emy zlicza� ile by�o dotychczas)?
// czy x, y odnosi si� do dzioba statku czy ko�ca (rufy)? 
// (zak�adam �e rufy)
bool placeShip(char board[21][10], Player player, int y, int x, int direction, int shipType) {
	int shipLength = getShipLength(shipType);
	int startX = x, endX = x, startY = y, endY = y;

	if (direction == NORTH) endY = y - shipLength;
	else if (direction == SOUTH) endY = y + shipLength;
	else if (direction == EAST) endX = x + shipLength;
	else if (direction == WEST) endX = x - shipLength;
	else cout << "BAD DIRECTION PROVIDED PLACE_SHIP COMMAND" << endl;

	if (!checkPlace(player.id, startX, endX, startY, endY))
		// invalid position for ship
		return false;

	for (int i = 0; i < shipLength; i++) {
		if (direction == NORTH) board[startY - i][startX] = SHIP_CHAR;
		else if (direction == SOUTH) board[startY + i][startX] = SHIP_CHAR;
		else if (direction == EAST) board[startY][startX + i] = SHIP_CHAR;
		else if (direction == WEST) board[startY][startX - i] = SHIP_CHAR;
	}

	return true;
}


void fillBoard(char board[21][10]) {
	for (int y = 0; y < BOARD_HEIGHT; y++) {
		for (int x = 0; x < BOARD_WIDTH; x++) {
			if (y == DIVIDING_LINE) board[y][x] = '-'; // to visualize border
			else board[y][x] = ' ';
		}
		cout << endl;
	}
}

// Board visualisation:
// 0 ... 9 (X-axis)
// ... (player 0 part of board)
// 10  (DIVIDING_LINE)
// ... (player 1 part of board)
// 20
// (Y-axis)
// board[y][x]
int main()
{
	char board[BOARD_HEIGHT][BOARD_WIDTH];
	fillBoard(board);

	Player Players[2] = {}; // Problems with Designated Initializers
	Players[0].id = ALICE;
	Players[1].id = BOB;

	string command;
	int currentPlayer = 0;
	char playerInitials;

	while (cin >> command) {
		switch (getCommandId(command)) {
		case PRINT: {
			printBoard(board);
			break;
		}
		case SET_FLEET: {
			cin >> playerInitials;
			Player playerToSetFleet = Players[getPlayerId(playerInitials)];
			setFleet(playerToSetFleet);
			break;
		}
		case NEXT_PLAYER: {
			// currentPlayer = (currentPlayer + 1) % 2;
			cin >> playerInitials;
			currentPlayer = getPlayerId(playerInitials);
			break;
		}
		case PLACE_SHIP: {
			Player playerPlacingShip = Players[currentPlayer];
			int y, x, i; // is i variable neccessary?
			char shipDir;
			string shipType; // The classes are denoted by [CAR]RIER, [BAT]TLESHIP, [CRU]ISER, [DES]TROYER.
			cin >> y >> x >> shipDir >> i >> shipType;
			placeShip(board, playerPlacingShip, y, x, getDirectionId(shipDir), getShipTypeId(shipType));
			break;
		}
		case SHOOT: {
			break;
		}
		default:
			cout << "INVALID COMMAND";
		}
	}
}


