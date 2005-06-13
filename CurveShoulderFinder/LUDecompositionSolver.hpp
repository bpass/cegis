/**
 * @file LUDecompositionSolver.hpp
 * @author Austin Hartman
 *
 * $Id: LUDecompositionSolver.hpp,v 1.1 2005/06/13 18:30:13 ahartman Exp $
 */

template<class T>
MyVector<T>
LUDecompositionSolver<T>::
operator()(const typename LUDecomposition<T>::Solution& decomposition,
           const MyVector<T>& b) const
{
    const LowerTriangularMatrix<T>& L = decomposition.L();
    const UpperTriangularMatrix<T>& U = decomposition.U();
    const typename LUDecomposition<T>::OrderType& order = 
        decomposition.orderVector();

    if(L.getNumRows() != b.getSize())
    {
        throw typename LUDecompositionSolver<T>::IncompatibleMatrixAndVector();
    }

    MyVector<T> bPermutation(b);

    for(size_t i = 1; i < L.getNumRows(); ++i)
    {
        T sum = bPermutation[order[i]];
        for(size_t j = 0; j < i; ++j)
        {
            sum -= L[order[i]][j] * bPermutation[order[j]];
        }
        bPermutation[order[i]] = sum;
    }

    MyVector<T> x(U.getNumCols());

    const size_t lastRow = x.getSize() - 1;
    x[lastRow] = bPermutation[order[lastRow]] / U[order[lastRow]][lastRow];

    for(int i = L.getNumRows() - 1; i >= 0; --i)
    {
        T sum = 0;
        for(size_t j = i+1; j < L.getNumRows(); ++j)
        {
            sum += U[order[i]][j]*x[j];
        }
        x[i] = (bPermutation[order[i]] - sum) / U[order[i]][i];
    }

    return x;
}

