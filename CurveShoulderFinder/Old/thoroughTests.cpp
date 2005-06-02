#include "thoroughTests.h"

#include <iostream>
#include <iomanip>

using namespace std;

void thoroughTests()
{
	typedef double type;

	// test operator<<
	{
		TridiagonalMatrix<type> tm(10, 1);
		//cout << tm[3][3] << '\n';
		//cout << tm[3][1] << '\n';
	
		cout << setw(2) << tm << '\n';
	
		size_t count = 0;
		tm[0][0] = count++;
		tm[0][1] = count++;
		for(size_t j = 1; j < tm.getNumCols() - 1; ++j)
		{
			for(size_t i = j - 1; i <= j + 1; ++i)
			{
				tm[i][j] = count++;
			}
		}
		tm[tm.getNumRows() - 2][tm.getNumCols() - 1] = count++;
		tm[tm.getNumRows() - 1][tm.getNumCols() - 1] = count++;
		
		cout << setw(3) << tm << '\n';
	}

	// test first constructor
	{
		cout << "Testing primary constructor\n";

		TridiagonalMatrix<type> defaultMatrix;
		cout << defaultMatrix << '\n';

		TridiagonalMatrix<type> oneMatrix(1, 1);
		cout << oneMatrix << '\n';

		TridiagonalMatrix<type> twoMatrix(2, 2);
		cout << twoMatrix << '\n';
		TridiagonalMatrix<type> twoZeroMatrix(2);
		cout << twoZeroMatrix << '\n';
	}

	// test copy constructor
	{
		cout << "Testing copy constructor\n";

		TridiagonalMatrix<type> fourMatrix(4, 4);
		cout << fourMatrix << '\n';

		TridiagonalMatrix<type> fourMatrixCopy(fourMatrix);
		cout << fourMatrixCopy << '\n';
	}

	// test operator=
	{
		cout << "Testing operator=\n";

		TridiagonalMatrix<type> sixMatrix(6, 7);
		cout << sixMatrix << '\n';

		TridiagonalMatrix<type> fourMatrix(4, 2);
		cout << fourMatrix << '\n';

		fourMatrix = sixMatrix;
		cout << fourMatrix << '\n';
	}

	// test operator*(vector)
	{
		cout << "Testing operator*(vector)\n";

		TridiagonalMatrix<type> sixMatrix(6, 6);
		cout << sixMatrix << '\n';

		MyVector<type> sixVector(6);
		for(size_t i = 0; i < sixVector.getSize(); ++i)
		{
			sixVector[i] = static_cast<type>(6*i);
		}
		cout << sixVector << '\n';

		cout << sixMatrix * sixVector << '\n';

		cout << '\n';
		sixMatrix[3][3] = 4;
		cout << sixMatrix << '\n';
		cout << sixMatrix * sixVector << '\n';

		MyVector<type> fiveVector(5);
		try
		{
			cout << sixMatrix * fiveVector << '\n';
		}
		catch(TridiagonalMatrix<type>::IncompatibleMatrixSizes)
		{
			cout << "\nBad sizes caught\n";
		}
		cout << '\n';
	}

	// test operator+=, operator+, operator-(unary), operator-=, and
	// operator-(binary)
	{
		cout << "Testing operator+/operator-/etc\n";
		TridiagonalMatrix<type> sixThreeMatrix(6, 3);
		TridiagonalMatrix<type> sixNineMatrix(6, 9);

		cout << sixThreeMatrix << '\n'
		     << sixNineMatrix << '\n';

		sixThreeMatrix += sixNineMatrix;
		cout << setw(2) << sixThreeMatrix << '\n';

		sixNineMatrix -= sixThreeMatrix;
		cout << setw(2) << sixNineMatrix << '\n';

		cout << setw(2) << sixNineMatrix + sixThreeMatrix << '\n';
		cout << setw(2) << sixThreeMatrix - sixNineMatrix << '\n';

		cout << -sixNineMatrix << '\n';
	}

	// test getNumRows and getNumCols
	{
		cout << "Testing getNumRows and getNumCols\n";

		TridiagonalMatrix<type> sixMatrix(6, 4);
		cout << sixMatrix;
		cout << "\trows = " << sixMatrix.getNumRows() << '\n';
		cout << "\tcols = " << sixMatrix.getNumCols() << '\n';
		cout << '\n';

		sixMatrix = TridiagonalMatrix<type>(4, 3);
		cout << sixMatrix;
		cout << "\trows = " << sixMatrix.getNumRows() << '\n';
		cout << "\tcols = " << sixMatrix.getNumCols() << '\n';
		cout << '\n';

		sixMatrix = TridiagonalMatrix<type>();
		cout << sixMatrix;
		cout << "\trows = " << sixMatrix.getNumRows() << '\n';
		cout << "\tcols = " << sixMatrix.getNumCols() << '\n';
		cout << '\n';

		sixMatrix = TridiagonalMatrix<type>(1, 5);
		cout << sixMatrix;
		cout << "\trows = " << sixMatrix.getNumRows() << '\n';
		cout << "\tcols = " << sixMatrix.getNumCols() << '\n';
		cout << '\n';
	}
}

