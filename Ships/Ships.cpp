#include <iostream>
#include <string>
#include "Ships.h"

using namespace std;


// COMMUNICATES

struct Result {
	const char* PLACE_SHIP[3] = {
		"NOT IN STARTING POSITION", // invalidPosition
		"SHIP ALREADY PRESENT", // shipAlreadyPresent
		"ALL SHIPS OF THE CLASS ALREADY SET" // shipsExcess
	};
	enum PLACE_SHIP_ENUM { undefined = -2, success = -1, invalidPosition, shipAlreadyPresent, shipsExcess };


} Result;


void printProblem(char* commandText, const char* problemText) {
	cout << "INVALID OPERATION \"" << commandText << "\": " << problemText << endl;
}


// PLAYER

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


// places ship on the board if it is possible
// x, y is the bow of the ship
// czy i-th ship (shipId u mnie) trzeba sprawdzaæ? 
// po co w ogóle to jest, jeœli wiemy ile statków 
// mo¿e byæ maks (mo¿emy zliczaæ ile by³o dotychczas)?
int placeShip(char board[BOARD_HEIGHT][BOARD_WIDTH], Player *player, int x, int y, int shipId, int direction, int shipType) {
	int shipLength = getShipLength(shipType);
	int startX = x, endX = x, startY = y, endY = y;

	// -1 because startY takes one cell
	if (direction == NORTH) endY = y + shipLength - 1;
	else if (direction == SOUTH) endY = y - shipLength + 1;
	else if (direction == EAST) endX = x - shipLength + 1;
	else if (direction == WEST) endX = x + shipLength - 1;
	else return Result.undefined;

	if (!checkPlace(player->id, startX, endX, startY, endY)) {
		// invalid position for ship
		return Result.invalidPosition;
	}

	bool shipAlreadyUsed = player->availableFleet->isShipUsed(shipType, shipId);
	if (shipAlreadyUsed) return Result.shipAlreadyPresent;

	bool enoughShips = player->availableFleet->useShip(shipType);
	if (!enoughShips) return Result.shipsExcess;

	for (int i = 0; i < shipLength; i++) {
		if (direction == NORTH) board[startY + i][startX] = SHIP_CHAR;
		else if (direction == SOUTH) board[startY - i][startX] = SHIP_CHAR;
		else if (direction == EAST) board[startY][startX - i] = SHIP_CHAR;
		else if (direction == WEST) board[startY][startX + i] = SHIP_CHAR;
	}

	return Result.success;
}


// returns if shot hits
bool shoot(int x, int y) {
	return true;
}


// set number of each ship for player
void setFleet(Player *player) {
	int quantity;
	for (int shipType = CARRIER; shipType <= DESTROYER; shipType++) {
		cin >> quantity;
		int actualShipsNumber = player->availableFleet->shipsNumber[shipType];
		int actualShipsRemaining = player->availableFleet->shipsNumber[shipType];
		int usedShipsNumber = actualShipsNumber - actualShipsRemaining;
		
		player->availableFleet->shipsNumber[shipType] = quantity;
		player->availableFleet->remainingShips[shipType] = quantity-usedShipsNumber;
	}
}


// GETTERS - mo¿e da siê to ujednoliciæ do jednej krótkiej funkcji?
int getCommandId(string command) {
	if (command == "PRINT") return PRINT;
	if (command == "SET_FLEET") return SET_FLEET;
	if (command == "NEXT_PLAYER") return NEXT_PLAYER;
	if (command == "PRINT") return PRINT;
	if (command == "PLACE_SHIP") return PLACE_SHIP;
	if (command == "SHOOT") return SHOOT;
	if (command == "CLEAR") return CLEAR;
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
	if (shipTypeId == CARRIER) return CARRIER_LENGTH;
	if (shipTypeId == BATTLESHIP) return BATTLESHIP_LENGTH;
	if (shipTypeId == CRUISER) return CRUISER_LENGTH;
	if (shipTypeId == DESTROYER) return DESTROYER_LENGTH;
	cout << "GOT BAD SHIP TYPE ID IN getShipLength FUNCTION" << endl;
	return -1;
}


// BOARD
void prepareBoard(char board[BOARD_HEIGHT][BOARD_WIDTH]) {
	for (int y = 0; y < BOARD_HEIGHT; y++) {
		for (int x = 0; x < BOARD_WIDTH; x++) {
			if (y == DIVIDING_LINE) board[y][x] = DIVIDING_LINE_CHAR; // to visualize border
			else board[y][x] = ' ';
		}
	}
}


void printBoard(char board[BOARD_HEIGHT][BOARD_WIDTH], int printMode) {
	int partsRemainingPlayerA = countPartsRemaining(ALICE, board);
	int partsRemainingPlayerB = countPartsRemaining(BOB, board);
	if (partsRemainingPlayerA == 0 && partsRemainingPlayerB == 0)
		return;
	for (int y = 0; y < BOARD_HEIGHT; y++) {
		for (int x = 0; x < BOARD_WIDTH; x++) {
			cout << board[y][x];
		}
		cout << endl;
	}
	printf("PARTS REMAINING:: A : %d B : %d\n", partsRemainingPlayerA, partsRemainingPlayerB);
}


int countPartsRemaining(int playerId, char board[BOARD_HEIGHT][BOARD_WIDTH]) {
	int startY = playerId == ALICE ? 0 : DIVIDING_LINE + 1;
	int partsRemaining = 0;
	for (int y = startY; y < startY + BOARD_HEIGHT / 2; y++) {
		for (int x = 0; x < BOARD_WIDTH; x++) {
			if (board[y][x] == SHIP_CHAR)
				partsRemaining++;
		}
	}
	return partsRemaining;
}


// HANDLE INPUT

int handleInput(string command, int* currentStatePlayer) {
	static bool stateCommands = false;
	static bool playerACommands = false;
	static bool playerBCommands = false;

	if (command == "[state]")
		stateCommands = !stateCommands; // flip bool
	else if (command == "[playerA]")
		playerACommands = !playerACommands;
	else if (command == "[playerB]")
		playerBCommands = !playerBCommands;
	else {
		if (stateCommands + playerACommands + playerBCommands > 1) {
			// two commands simultaneous, what to do?
		}

		if (playerACommands) *currentStatePlayer = ALICE;
		else if (playerBCommands) *currentStatePlayer = BOB;

		int commandId = getCommandId(command);

		if (stateCommands) {
			if (commandId == PRINT ||
				commandId == SET_FLEET ||
				commandId == NEXT_PLAYER)
				return commandId;
		}
		else {
			if (commandId == PLACE_SHIP ||
				commandId == SHOOT)
				return commandId;
		}
		return -1;
	}
	return DO_NOTHING;
}


void handleResult(int commandId, int resultId, char* commandText) {
	if (resultId == Result.success) return; // everything ok

	switch (commandId) {
		case SET_FLEET: {
			break;
		}
		case PLACE_SHIP: {
			const char* problemText = Result.PLACE_SHIP[resultId];
			printProblem(commandText, problemText);
			break;
		}
		case SHOOT: {
			break;
		}
	}
	exit(0);
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
	prepareBoard(board);

	Player Players[2] = {}; // Problems with Designated Initializers
	Players[0].id = ALICE;
	Players[1].id = BOB;


	string command;
	int currentPlayer = ALICE;
	int currentStatePlayer = -1;
	char playerInitials;
	char commandBuffer[100];

	while (cin >> command) {
		switch (handleInput(command, &currentStatePlayer)) {
		case PRINT: {
			int printMode;
			cin >> printMode;
			printBoard(board, printMode);
			break;
		}
		case SET_FLEET: {
			cin >> playerInitials;
			Player *playerToSetFleet = &Players[getPlayerId(playerInitials)];
			setFleet(playerToSetFleet);
			break;
		}
		case NEXT_PLAYER: {
			cin >> playerInitials;
			currentPlayer = getPlayerId(playerInitials);
			break;
		}
		case PLACE_SHIP: {
			Player playerPlacingShip = Players[currentStatePlayer];
			int y, x, shipId;
			char shipDir;
			string shipType; // The classes are denoted by [CAR]RIER, [BAT]TLESHIP, [CRU]ISER, [DES]TROYER.
			cin >> y >> x >> shipDir >> shipId >> shipType;
			sprintf_s(commandBuffer, "PLACE_SHIP %d %d %c %d %s",
				y, x, shipDir, shipId, shipType.c_str());
			int result = placeShip(board, &playerPlacingShip,
				x, y, shipId, 
				getDirectionId(shipDir),
				getShipTypeId(shipType));
			handleResult(PLACE_SHIP, result, commandBuffer);
			break;
		}
		case SHOOT: {
			break;
		}
		case CLEAR: {
			prepareBoard(board);
			break;
		}
		case DO_NOTHING: {
			break;
		}
		default:
			cout << "INVALID COMMAND";
		}
	}
}


