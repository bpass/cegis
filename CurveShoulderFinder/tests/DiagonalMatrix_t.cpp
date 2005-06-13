/**
 * @file LUDecompositionSolver_t.cpp
 * @author Austin Hartman
 *
 * $Id: DiagonalMatrix_t.cpp,v 1.1 2005/06/13 20:17:25 ahartman Exp $
 */

#include <cassert>
#include <iostream>

#include "DiagonalMatrix.h"
#include "MyVector.h"

#include "testsUtility.h"

int main()
{
    typedef double type;
    const type tolerance = .0001;

    using std::cout;
    using std::setw;
    using std::fixed;
    cout.precision(8);
    cout << fixed;
    const size_t width = cout.precision() + 3;

	DiagonalMatrix<type> blankMatrix;
	DiagonalMatrix<type> fiveMatrix(5, 5);

	// test copy constructor
	{
		DiagonalMatrix<type> fiveMatrixCopy(fiveMatrix);
        assert(matricesEqual(fiveMatrix, fiveMatrixCopy, 0.00000001));
	}

	// test operator=
	{
		blankMatrix = fiveMatrix;
        assert(matricesEqual(fiveMatrix, blankMatrix, 0.00000001));
		blankMatrix = DiagonalMatrix<type>();
	}

	// test operator*(MyVector)
	{
		DiagonalMatrix<type> fourMatrix(4, 4);
		for(size_t i = 1; i <= fourMatrix.getNumCols(); ++i)
		{
			fourMatrix[i-1][i-1] = i;
		}

		MyVector<type> fourVector(4);
		for(size_t i = 0, j = fourVector.getSize() - 1;
		    i < fourVector.getSize(); ++i, --j)
		{
			fourVector[i] = j;
		}

        MyVector<type> temp = fourMatrix * fourVector;
	}

	// test operator+=, operator+, operator-=, operator-(unary), 
	// operator-(binary), and operator*(DiagonalMatrix)
	{
		DiagonalMatrix<type> matrix1(5, 4);
		matrix1[matrix1.getNumRows()/2][matrix1.getNumCols()/2] = 8;

		DiagonalMatrix<type> matrix2(5, 3);

		matrix1 += matrix2;
        DiagonalMatrix<type> sum = matrix1 + matrix2;

		matrix2 -= matrix2;

		matrix2 = DiagonalMatrix<type>(5, 3);
        DiagonalMatrix<type> negMatrix2 = -matrix2;

        DiagonalMatrix<type> difference = matrix2 - matrix1;

        DiagonalMatrix<type> product1 = matrix1 * matrix2;
        DiagonalMatrix<type> product2 = matrix2 * matrix1;

		matrix1 *= matrix2;
	}

	// test getNumRows and getNumCols
	{
        const size_t size_1 = 6;
		DiagonalMatrix<type> matrix1(size_1, 8);
		assert(matrix1.getNumRows() == size_1);
		assert(matrix1.getNumCols() == size_1);

        const size_t size_2 = 9;
		DiagonalMatrix<type> matrix2(9, 9);
		assert(matrix2.getNumRows() == size_2);
		assert(matrix2.getNumCols() == size_2);
	}
}

