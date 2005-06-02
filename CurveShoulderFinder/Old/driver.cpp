#include <iostream>
#include <iomanip>

#include "TridiagonalMatrix.h"
#include "CubicSplineSolver.h"
#include "CubicSplineEvaluator.h"

//#define DO_THOROUGHTESTS
#ifdef DO_THOROUGHTESTS
#include "thoroughTests.h"
#endif

using namespace std;

void matrixBaseTest();

int main(int argc, char *argv[])
{
	#ifdef DO_THOROUGHTESTS
	thoroughTests();
	cout << '\n';
	return 0;
	#endif

	cout << '\n';

	// test the matrix interface base class
//	matrixBaseTest();
//	cout << '\n';

	// do the cubic splines stuff
	// open the file if one has been specified
	if(argc <= 1)
	{
		cerr << argv[0] << ':' << " no input file\n";
		return 1;
	}
	ifstream inputFile(argv[1]);
	cout << "Calculating cubic splines from the data in the input file\n"
	     << "---------------------------------------------------------\n";

	// set up some typedefs for easier reading/modification
	typedef double type;

	// read in the number of pairs of x and y coordinates
	size_t numPairs;
	inputFile >> numPairs;

	// check to see if the number of pairs is valid
	if(numPairs < 2)
	{
		cerr << "The number of pairs is too small to find an "
			"interpolating polynomial\n";
		return 2;
	}

	// read in the points
	MyVector<type> xCoords(numPairs);
	MyVector<type> yCoords(numPairs);
	for(size_t i = 0; i < numPairs; ++i)
	{
		inputFile >> xCoords[i];
		inputFile >> yCoords[i];
	}

	// get the cubic splines
	CubicSplineSolver<type> solver;
	CubicSplineSolver<type>::Solution splines = solver(xCoords, yCoords);

	// output the resulting cubic splines
	cout << "Cubic Splines:\n";
	for(size_t i = 0; i < splines.size(); ++i)
	{
		cout << 'S' << i << " = " << splines[i] << '\n';
	}
	cout << '\n';

	// output the evaluation of the various terms
	CubicSplineEvaluator<type> evaluator;
//	cout << "Evaluating points with the splines:\n";
//	//type term = -2.0;
//	//cout << "S(" << term << ") = " << evaluator(splines, term) << '\n';
//	type term = -.9;
//	cout << "S(" << term << ") = " << evaluator(splines, term) << '\n';
//	term = -.7;
//	cout << "S(" << term << ") = " << evaluator(splines, term) << '\n';
//	term = .1;
//	cout << "S(" << term << ") = " << evaluator(splines, term) << '\n';

	cout << '\n';
}

void
matrixBaseTest()
{
	typedef double type;
	const size_t matrixSize = 5;

	TridiagonalMatrix<type> tm(matrixSize);
	Matrix<type>& triAsMatrix = tm;

	cout << "Matrix interface base class test\n"
	     << "--------------------------------\n";
	cout << "Initializing a tridiagonal matrix of size "
	     << triAsMatrix.getNumRows() << 'x' << triAsMatrix.getNumCols()
	     << "\nwith its elements set to count upward going by columns.\n"
	        "All of this is done using a matrix base reference\n"
	        "referring to a tridiagonal matrix.\n\n";

	size_t count = 1;
	triAsMatrix[0][0] = count++;
	triAsMatrix[1][0] = count++;
	for(size_t j = 1; j < triAsMatrix.getNumCols() - 1; ++j)
	{
		for(size_t i = j - 1; i <= j + 1; ++i)
		{
			triAsMatrix[i][j] = count++;
		}
	}
	triAsMatrix[triAsMatrix.getNumRows() - 2]
	           [triAsMatrix.getNumCols() - 1] = count++;
	triAsMatrix[triAsMatrix.getNumRows() - 1]
	           [triAsMatrix.getNumCols() - 1] = count++;
	
	cout << "Here's the matrix:\n";
	cout << setw(2) << triAsMatrix << '\n';

	cout << "Now let's see the double bracket operators:\n";
	cout << "matrix[0][0] = " << triAsMatrix[0][0] << '\n';
	cout << "matrix[0][" << triAsMatrix.getNumCols()-1 
	     << "] = " << triAsMatrix[0][triAsMatrix.getNumCols()-1] << '\n';
	cout << "matrix[" << triAsMatrix.getNumRows()-1 << "][" 
	     << triAsMatrix.getNumCols()-1 << "] = " 
	     << triAsMatrix[triAsMatrix.getNumRows()-1]
	                   [triAsMatrix.getNumCols()-1] << "\n\n";

	cout << "Checking the getNumRows() and getNumCols() functions:\n";
	cout << "matrix.getNumRows() = " << triAsMatrix.getNumRows() << '\n';
	cout << "matrix.getNumCols() = " << triAsMatrix.getNumCols() << "\n\n";

	cout << "Creating a vector of size " << triAsMatrix.getNumCols()
	     << " to test multiplying the matrix by a vector:\n";
	MyVector<type> vec(triAsMatrix.getNumCols());
	for(size_t i = 0; i < vec.getSize(); ++i)
	{
		vec[i] = 2*i;
	}
	cout << "vector = " << vec << "\n\n";

	cout << "Result of multiplying the matrix by the vector:\n"
	     << "result = " << triAsMatrix*vec << "\n\n";
}
