#include "pch.h"
#include "CppUnitTest.h"
#include "Ships.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BoardTests
{
	TEST_CLASS(BoardTests)
	{
	public:
		TEST_METHOD(BoardPreparation)
		{
			const int SIZE_Y = 21, SIZE_X = 10;
			char dirtyBoard[SIZE_Y][SIZE_X] = { "to_erase" };

			prepareBoard(dirtyBoard);

			for (int i = 0; i < SIZE_X; i++)
				Assert::AreEqual(dirtyBoard[0][i], ' ');
		}

		TEST_METHOD(PlaceShipInEmptySpaceAllDirections)
		{
			const int SIZE_Y = 21, SIZE_X = 10;
			char board[SIZE_Y][SIZE_X];
			Player testPlayer;
			testPlayer.id = 0;
			int shipPosX = 5, shipPosY = 5;

			for (int direction = NORTH; direction < WEST; direction++) {
				int shipId = direction;
				prepareBoard(board);
				bool is_placed = placeShip(board, &testPlayer, shipPosX, shipPosY, shipId, direction, DESTROYER);
				int shipLength = getShipLength(DESTROYER);

				Assert::AreEqual(is_placed, true);
				for (int i = 0; i < shipLength; i++) {
					if (direction == NORTH) Assert::AreEqual(board[shipPosY + i][shipPosX], SHIP_CHAR);
					else if (direction == SOUTH) Assert::AreEqual(board[shipPosY - i][shipPosX], SHIP_CHAR);
					else if (direction == EAST) Assert::AreEqual(board[shipPosY][shipPosX - i], SHIP_CHAR);
					else if (direction == WEST) Assert::AreEqual(board[shipPosY][shipPosX + i], SHIP_CHAR);
				}
			}
		}
		TEST_METHOD(PlaceShipOnBorder)
		{
			const int SIZE_Y = 21, SIZE_X = 10;
			char board[SIZE_Y][SIZE_X];
			Player testPlayer;
			testPlayer.id = 0;
			int shipPosX = 0, shipPosY = 0;

			for (int direction = NORTH; direction < WEST; direction++) {
				int shipId = direction;
				prepareBoard(board);
				bool is_placed = true;
				if (direction == NORTH) is_placed = placeShip(board, &testPlayer, 0, DIVIDING_LINE - 1, shipId, direction, DESTROYER);
				else if (direction == SOUTH) is_placed = placeShip(board, &testPlayer, 0, 0, shipId, direction, DESTROYER);
				else if (direction == EAST) is_placed = placeShip(board, &testPlayer, 0, 0, shipId, direction, DESTROYER);
				else if (direction == WEST) is_placed = placeShip(board, &testPlayer, SIZE_X - 1, 0, shipId, direction, DESTROYER);
				Assert::AreEqual(is_placed, false);
			}
		}

	};
}
