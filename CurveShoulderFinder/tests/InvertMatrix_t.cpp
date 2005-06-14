/**
 * @file InvertMatrix_t.cpp
 * @author Austin Hartman
 *
 * $Id: InvertMatrix_t.cpp,v 1.3 2005/06/14 22:25:29 ahartman Exp $
 */

#include <cassert>
//#include <iostream>

#include "DenseMatrix.h"
#include "DiagonalMatrix.h"
#include "InvertMatrix.h"

#include "testsUtility.h"

int main()
{
    typedef double type;
    const type tolerance = .00001;

//    using std::cout;
//    using std::setw;
//    using std::fixed;
//    cout.precision(8);
//    cout << fixed;
//    const size_t width = cout.precision() + 3;

    // test using example from _Numerical Methods for Engineers_ by Steven C.
    // Chapra and Raymond P. Canale on p. 274-275
    {
        DenseMatrix<type> A(3, 3);
        A[0][0] = 3;   A[0][1] = -0.1; A[0][2] = -0.2;
        A[1][0] = 0.1; A[1][1] =  7;   A[1][2] = -0.3;
        A[2][0] = 0.3; A[2][1] = -0.2; A[2][2] = 10;

        InvertMatrix<type> inverter;
        DenseMatrix<type> AInverse = inverter(A);

        DenseMatrix<type> AInverseExpected(A.getNumRows(), A.getNumCols());
        {
            DenseMatrix<type>& m = AInverseExpected;
            m[0][0] =  0.33249; m[0][1] = 0.004944; m[0][2] = 0.006798;
            m[1][0] = -0.00518; m[1][1] = 0.142903; m[1][2] = 0.004183;
            m[2][0] = -0.01008; m[2][1] = 0.00271;  m[2][2] = 0.09988;
        }

        const DiagonalMatrix<type> identity(A.getNumRows(), 1);

        assert(matricesEqual(AInverse, AInverseExpected, tolerance));
        assert(matricesEqual(AInverse*A, identity, tolerance));
        assert(matricesEqual(A*AInverse, identity, tolerance));

//        cout << "AInverse =\n" << setw(width) << AInverse;
    }

    // test on a 4x4 matrix from 
    // <http://algebra.math.ust.hk/matrix_linear_trans/06_inverse/lecture4.shtml>
    {
        DenseMatrix<type> A(4, 4);
        A[0][0] = 1; A[0][1] = 0; A[0][2] = 1; A[0][3] = 0;
        A[1][0] = 1; A[1][1] = 1; A[1][2] = 0; A[1][3] = 1;
        A[2][0] = 0; A[2][1] = 0; A[2][2] = 1; A[2][3] = 1;
        A[3][0] = 1; A[3][1] = 1; A[3][2] = 1; A[3][3] = 1;

        InvertMatrix<type> inverter;
        DenseMatrix<type> AInverse = inverter(A);

        DenseMatrix<type> AInverseExpected(A.getNumRows(), A.getNumCols());
        {
            DenseMatrix<type>& m = AInverseExpected;
            m[0][0] =  1; m[0][1] =  1; m[0][2] =  0; m[0][3] = -1;
            m[1][0] = -1; m[1][1] = -1; m[1][2] = -1; m[1][3] =  2;
            m[2][0] =  0; m[2][1] = -1; m[2][2] =  0; m[2][3] =  1;
            m[3][0] =  0; m[3][1] =  1; m[3][2] =  1; m[3][3] = -1; 
        }

        const DiagonalMatrix<type> identity(A.getNumRows(), 1);

        assert(matricesEqual(AInverse, AInverseExpected, tolerance));
        assert(matricesEqual(AInverse*A, identity, tolerance));
        assert(matricesEqual(A*AInverse, identity, tolerance));

//        cout << "AInverse =\n" << setw(width) << AInverse;
    }
}

