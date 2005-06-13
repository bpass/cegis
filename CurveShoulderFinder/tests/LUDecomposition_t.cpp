/**
 * @file LUDecomposition_t.cpp
 * @author Austin Hartman
 *
 * $Id: LUDecomposition_t.cpp,v 1.3 2005/06/13 18:34:47 ahartman Exp $
 */

#include <cmath>
#include <cassert>

#include "LUDecomposition.h"
#include "Matrix.h"
#include "DenseMatrix.h"
#include "LowerTriangularMatrix.h"
#include "UpperTriangularMatrix.h"

#include "testsUtility.h"

int main()
{
    typedef double type;

    const type tolerance = .0001;

    // test using example from _Numerical Methods for Engineers_ by Steven C.
    // Chapra and Raymond P. Canale on p. 267-268
    {
        DenseMatrix<type> A(3, 3);
        A[0][0] = 3;   A[0][1] = -0.1; A[0][2] = -0.2;
        A[1][0] = 0.1; A[1][1] =  7;   A[1][2] = -0.3;
        A[2][0] = 0.3; A[2][1] = -0.2; A[2][2] = 10;

        LowerTriangularMatrix<type> expectedL(A.getNumRows());
        {
            LowerTriangularMatrix<type>& L = expectedL;
            L[0][0] =  1;
            L[1][0] =  0.0333333; L[1][1] =  1;
            L[2][0] =  0.1;       L[2][1] = -0.0271300; L[2][2] =  1;
        }

        UpperTriangularMatrix<type> expectedU(A.getNumRows());
        {
            UpperTriangularMatrix<type>& U = expectedU;
            U[0][0] =  3;         U[0][1] = -0.1;       U[0][2] = -0.2;
                                  U[1][1] =  7.00333;   U[1][2] = -0.293333;
                                                        U[2][2] = 10.0120;
        }

        LUDecomposition<type> luDecomp;
        LUDecomposition<type>::Solution solution = luDecomp(A, tolerance);

        assert(matricesEqual(solution.L(), expectedL, tolerance));
        assert(matricesEqual(solution.U(), expectedU, tolerance));
        assert(matricesEqual(solution.L()*solution.U(), solution.P()*A, tolerance));
    }

    // another test to ensure that pivoting is handled properly
    {
        DenseMatrix<type> A(4, 4);
        A[0][0] = 4; A[0][1] = 7; A[0][2] = 3; A[0][3] = 1;
        A[1][0] = 5; A[1][1] = 2; A[1][2] = 4; A[1][3] = 3;
        A[2][0] = 6; A[2][1] = 7; A[2][2] = 8; A[2][3] = 4;
        A[3][0] = 8; A[3][1] = 9; A[3][2] = 4; A[3][3] = 5;

        LUDecomposition<type> luDecomp;
        LUDecomposition<type>::Solution solution = luDecomp(A, tolerance);

        assert(matricesEqual(solution.L()*solution.U(), 
                             solution.P()*A, tolerance));
    }
}

