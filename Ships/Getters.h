#pragma once

int getCommandId(char* command);
int getPlayerId(char playerInitials);
int getDirectionId(char dirAbbreviation);
int getShipTypeId(char* shipTypeAbbreviation);
int getShipLength(int shipTypeId);
int getMoveDirectionId(char moveChar);
const char* getShipTypeAbbrv(int shipTypeId);
const char getPlayerInitials(int playerId);
const char getDirectionChar(int directionId);