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
			
			for (int i=0; i<SIZE_X; i++)
				Assert::AreEqual(dirtyBoard[0][i], ' ');
		}
	};
}
