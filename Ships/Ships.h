#pragma once

void printProblem(char* commandText, const char* problemText);
bool switchPlayerTurn(
	int currentCommandPlayerId, bool* playerCommandsToChange,
	int* currentStatePlayer, int* previousStatePlayer
);
int handleInput(char* command, int* currentStatePlayer, int* previousStatePlayer);
void handleResult(int commandId, int resultId, char* commandText);