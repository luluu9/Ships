#include <iostream>
#include "Commands.h"
#include "Getters.h"
#include "Board.h"
#include "ShipsDataTypes.h"


// HANDLE INPUT/RESULT

void printProblem(char* commandText, const char* problemText) {
	std::cout << "INVALID OPERATION \"" << commandText << "\": " << problemText << std::endl;
}


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
int main() {
	Board *board = new Board();
	board->prepareBoard();

	Player Players[2] = {};
	Players[0].id = ALICE;
	Players[1].id = BOB;

	char command[100], fullCommand[100];
	int currentStatePlayer = -1, previousStatePlayer = -2;
	char playerInitials;

	while (std::cin >> command) {
		switch (handleInput(command, &currentStatePlayer, &previousStatePlayer)) {
		case PRINT: {
			int printMode;
			std::cin >> printMode;
			board->printBoard(printMode);
			break;
		}
		case SET_FLEET: {
			std::cin >> playerInitials;
			Player* playerToSetFleet = &Players[getPlayerId(playerInitials)];
			setFleet(playerToSetFleet);
			break;
		}
		case NEXT_PLAYER: {
			std::cin >> playerInitials;
			// playerDoingTurn = getPlayerId(playerInitials);
			break;
		}
		case PLACE_SHIP: {
			Player playerPlacingShip = Players[currentStatePlayer];
			int y, x, shipId;
			char shipDir;
			char shipType[3]; // The classes are denoted by [CAR]RIER, [BAT]TLESHIP, [CRU]ISER, [DES]TROYER.
			std::cin >> y >> x >> shipDir >> shipId >> shipType;

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
			std::cin >> y >> x;
			sprintf_s(fullCommand, "SHOOT %d %d", y, x);
			int result = shoot(board, Players, x, y);
			handleResult(SHOOT, result, fullCommand);
			break;
		}
		case CLEAR: {
			board->prepareBoard();
			break;
		}
		case OTHER_PLAYER_TURN: {
			strcpy_s(fullCommand, (currentStatePlayer == ALICE) ? "[playerA] " : "[playerB] ");
			handleResult(STATE, Result.otherPlayerExcepted, fullCommand);
			break;
		}
		case END_TURN: {
			int winner = board->checkWinner(Players);
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
			std::cout << "INVALID COMMAND" << std::endl;
		}
	}
}


