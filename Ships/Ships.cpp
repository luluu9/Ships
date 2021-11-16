#include <iostream>
#include "Ships.h"
#include "Commands.h"

using namespace std;



// COMMUNICATES

void printProblem(char* commandText, const char* problemText) {
	cout << "INVALID OPERATION \"" << commandText << "\": " << problemText << endl;
}


// PLAYER

bool isPointInsideBoard(int x, int y) {
	if (x < 0 || y < 0) return false;
	if (x >= BOARD_WIDTH || y >= BOARD_HEIGHT) return false;
	return true;
}


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
	if (!isPointInsideBoard(startX, startY)) return false;
	if (!isPointInsideBoard(endX, endY)) return false;

	// check collisions?

	return true;
}



// GETTERS - mo�e da si� to ujednolici� do jednej kr�tkiej funkcji?

int getCommandId(char *command) {
	if (strcmp(command, "PRINT") == 0) return PRINT;
	if (strcmp(command, "SET_FLEET") == 0) return SET_FLEET;
	if (strcmp(command, "NEXT_PLAYER") == 0) return NEXT_PLAYER;
	if (strcmp(command, "PLACE_SHIP") == 0) return PLACE_SHIP;
	if (strcmp(command, "SHOOT") == 0) return SHOOT;
	if (strcmp(command, "CLEAR") == 0) return CLEAR;
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


int getShipTypeId(char *shipTypeAbbreviation) {
	if (strcmp(shipTypeAbbreviation, "CAR") == 0) return CARRIER;
	if (strcmp(shipTypeAbbreviation, "BAT") == 0) return BATTLESHIP;
	if (strcmp(shipTypeAbbreviation, "CRU") == 0) return CRUISER;
	if (strcmp(shipTypeAbbreviation, "DES") == 0) return DESTROYER;
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


int checkWinner(Player players[2], char board[BOARD_HEIGHT][BOARD_WIDTH]) {
	// all ships have to be placed
	bool playerAplacedAll = players[ALICE].availableFleet->areAllShipsPlaced();
	bool playerBplacedAll = players[BOB].availableFleet->areAllShipsPlaced();
	if (!(playerAplacedAll && playerBplacedAll))
		return -1;

	int playerAPartsRemaining = countPartsRemaining(ALICE, board);
	int playerBPartsRemaining = countPartsRemaining(BOB, board);
	if (playerBPartsRemaining == 0)
		return ALICE;
	else if (playerAPartsRemaining == 0)
		return BOB;
	return -1; // nobody wins
}



// HANDLE INPUT/RESULT

// returns true if turn ends
bool switchPlayerTurn(int currentCommandPlayerId, bool* playerCommandsToChange, int* currentStatePlayer, int* previousStatePlayer) {
	*playerCommandsToChange = !*playerCommandsToChange; // flip bool
	if (*playerCommandsToChange) { // player starts turn
		*previousStatePlayer = *currentStatePlayer;
		*currentStatePlayer = currentCommandPlayerId;
		return false;
	}
	else { // player ends turn
		return true;
	}
}


int handleInput(char* command, int* currentStatePlayer, int* previousStatePlayer) {
	static bool stateCommands = false;
	static bool playerACommands = false;
	static bool playerBCommands = false;
	int turnEnds = false;

	if (strcmp(command, "[state]") == 0)
		stateCommands = !stateCommands; // flip bool
	else if (strcmp(command, "[playerA]") == 0) {
		turnEnds = switchPlayerTurn(ALICE, &playerACommands, currentStatePlayer, previousStatePlayer);
	}
	else if (strcmp(command, "[playerB]") == 0) {
		turnEnds = switchPlayerTurn(BOB, &playerBCommands, currentStatePlayer, previousStatePlayer);
	}
	else {
		if (stateCommands + playerACommands + playerBCommands > 1) {
			// two commands simultaneous, what to do?
			return OTHER_PLAYER_TURN;
		}

		if (*previousStatePlayer == *currentStatePlayer)
			return OTHER_PLAYER_TURN;

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
	}

	if (turnEnds)
		return END_TURN;

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
		const char* problemText = Result.SHOOT[resultId];
		printProblem(commandText, problemText);
		break;
	}
	case STATE: {
		const char* problemText = Result.STATE[resultId];
		printProblem(commandText, problemText);
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


	char command[100];
	int previousStatePlayer = -2;
	int currentStatePlayer = -1;
	char playerInitials;
	char fullCommand[100];

	while (cin >> command) {
		switch (handleInput(command, &currentStatePlayer, &previousStatePlayer)) {
		case PRINT: {
			int printMode;
			cin >> printMode;
			printBoard(board, printMode);
			break;
		}
		case SET_FLEET: {
			cin >> playerInitials;
			Player* playerToSetFleet = &Players[getPlayerId(playerInitials)];
			setFleet(playerToSetFleet);
			break;
		}
		case NEXT_PLAYER: {
			cin >> playerInitials;
			// playerDoingTurn = getPlayerId(playerInitials);
			break;
		}
		case PLACE_SHIP: {
			Player playerPlacingShip = Players[currentStatePlayer];
			int y, x, shipId;
			char shipDir;
			char shipType[3]; // The classes are denoted by [CAR]RIER, [BAT]TLESHIP, [CRU]ISER, [DES]TROYER.
			cin >> y >> x >> shipDir >> shipId >> shipType;

			sprintf_s(fullCommand, "PLACE_SHIP %d %d %c %d %s",
				y, x, shipDir, shipId, shipType);

			int result = placeShip(board, &playerPlacingShip,
				x, y, shipId,
				getDirectionId(shipDir),
				getShipTypeId(shipType));

			handleResult(PLACE_SHIP, result, fullCommand);
			break;
		}
		case SHOOT: { // Shooting can only start if all the ships were placed.
			int x, y;
			cin >> y >> x;
			sprintf_s(fullCommand, "SHOOT %d %d", y, x);
			int result = shoot(board, Players, x, y);
			handleResult(SHOOT, result, fullCommand);
			break;
		}
		case CLEAR: {
			prepareBoard(board);
			break;
		}
		case OTHER_PLAYER_TURN: {
			strcpy_s(fullCommand, (currentStatePlayer == ALICE) ? "[playerA] " : "[playerB] ");
			handleResult(STATE, Result.otherPlayerExcepted, fullCommand);
			break;
		}
		case END_TURN: {
			int winner = checkWinner(Players, board);
			if (winner == ALICE || winner == BOB) {
				printf("%c won", winner == ALICE ? 'A' : 'B');
				exit(0);
			}
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


