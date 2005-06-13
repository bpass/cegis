/**
 * @file LUDecompositionSolver.h
 * @author Austin Hartman
 *
 * $Id: LUDecompositionSolver.h,v 1.1 2005/06/13 18:30:13 ahartman Exp $
 */

#ifndef AUSTIN_LUDECOMPOSITIONSOLVER_H
#define AUSTIN_LUDECOMPOSITIONSOLVER_H

#include "MyVector.h"
#include "LUDecomposition.h"

template<class T>
class LUDecompositionSolver
{
public:
    MyVector<T> operator()(
            const typename LUDecomposition<T>::Solution& decomposition,
            const MyVector<T>& b) const;

    class IncompatibleMatrixAndVector
    {};
};

#include "LUDecompositionSolver.hpp"

#endif

