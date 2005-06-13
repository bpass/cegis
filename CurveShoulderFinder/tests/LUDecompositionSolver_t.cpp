/**
 * @file LUDecompositionSolver_t.cpp
 * @author Austin Hartman
 *
 * $Id: LUDecompositionSolver_t.cpp,v 1.1 2005/06/13 18:35:36 ahartman Exp $
 */

#include <cassert>
//#include <iostream>

#include "DenseMatrix.h"
#include "MyVector.h"
#include "LUDecomposition.h"
#include "LUDecompositionSolver.h"

#include "testsUtility.h"

int main()
{
    typedef double type;
    const type tolerance = .0001;

    DenseMatrix<type> A(3, 3);
    A[0][0] = 3;   A[0][1] = -0.1; A[0][2] = -0.2;
    A[1][0] = 0.1; A[1][1] =  7;   A[1][2] = -0.3;
    A[2][0] = 0.3; A[2][1] = -0.2; A[2][2] = 10;

    MyVector<type> b(3);
    b[0] = 7.85; b[1] = -19.3; b[2] = 71.4;

    LUDecomposition<type> luDecomp;
    LUDecomposition<type>::Solution luSolution = luDecomp(A);

    LUDecompositionSolver<type> luSolver;
    MyVector<type> x = luSolver(luSolution, b);
    
    MyVector<type> xExpected(x.getSize());
    xExpected[0] = 3; xExpected[1] = -2.5; xExpected[2] = 7.00003;

    assert(matricesEqual(DenseMatrix<type>(x), DenseMatrix<type>(xExpected), 
                         tolerance));

//    using std::cout;
//    using std::setw;
//    using std::fixed;
//    cout.precision(8);
//    cout << fixed;
//    const size_t width = cout.precision() + 3;
//    cout << "x =\n" << setw(width) << DenseMatrix<type>(x);
}

