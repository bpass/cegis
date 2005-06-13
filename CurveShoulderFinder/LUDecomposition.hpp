/**
 * @file LUDecomposition.hpp
 * @author Austin Hartman
 *
 * $Id: LUDecomposition.hpp,v 1.3 2005/06/13 17:33:08 ahartman Exp $
 */

#include <cmath>

template<class T>
const T LUDecomposition<T>::m_tolerance = .000001;

template<class T>
typename LUDecomposition<T>::Solution
LUDecomposition<T>::operator()(const Matrix<T>& matrixToDecompose,
                               const T& tolerance) const
{
    DenseMatrix<T> matrix(matrixToDecompose);

    typename LUDecomposition<T>::ScalesType scales = createScales(matrix);

    typename LUDecomposition<T>::OrderType order(matrix.getNumRows());
    for(size_t i = 0; i < matrix.getNumRows(); ++i)
    {
        order[i] = i;
    }

    for(size_t k = 0; k < matrix.getNumRows() - 1; ++k)
    {
        // pivot the rows to reduce roundoff error
        pivot(matrix, order, scales, k);

        // check to make the matrix is not singular
        if(std::abs(matrix[order[k]][k] / scales[order[k]]) < tolerance)
        {
            throw typename LUDecomposition<T>::MatrixIsSingular();
        }

        // fill in the matrix with the correct values
        for(size_t i = k + 1; i < matrix.getNumRows(); ++i)
        {
            const T factor = matrix[order[i]][k] / matrix[order[k]][k];
            matrix[order[i]][k] = factor;
            for(size_t j = k+1; j < matrix.getNumRows(); ++j)
            {
                matrix[order[i]][j] = 
                    matrix[order[i]][j] - factor * matrix[order[k]][j];
            }
        }
    }

    // check one last time to make the matrix is not singular
    const size_t lastRow = matrix.getNumRows() - 1;
    if(std::abs(matrix[order[lastRow]][lastRow] / scales[order[lastRow]]) < 
        tolerance)
    {
        throw typename LUDecomposition<T>::MatrixIsSingular();
    }

    // create the solution that will be sent back
    // the lower triangular matrix is initialized with a value of 1 because
    // that is what goes on its main diagonal
    typename LUDecomposition<T>::Solution 
        solution(LowerTriangularMatrix<T>(matrix.getNumRows(), 1),
                 UpperTriangularMatrix<T>(matrix.getNumRows()),
                 order);
    LowerTriangularMatrix<T>& L = solution.L();
    UpperTriangularMatrix<T>& U = solution.U();

    // put the values from the single dense matrix into the two separate upper
    // and lower triangular matrices
    for(size_t j = 0; j < matrix.getNumCols(); ++j)
    {
        for(size_t i = j+1; i < matrix.getNumRows(); ++i)
        {
            L[i][j] = matrix[order[i]][j];
        }
    }
    for(size_t j = 0; j < matrix.getNumCols(); ++j)
    {
        for(size_t i = 0; i <= j; ++i)
        {
            U[i][j] = matrix[order[i]][j];
        }
    }

    return solution;
}

template<class T>
typename LUDecomposition<T>::ScalesType
LUDecomposition<T>::createScales(const Matrix<T>& matrix) const
{
    typename LUDecomposition<T>::ScalesType scales(matrix.getNumRows());
    for(size_t i = 0; i < scales.size(); ++i)
    {
        scales[i] = std::abs(matrix[i][0]);
        for(size_t j = 1; j < matrix.getNumCols(); ++j)
        {
            const T absCurrentElement = std::abs(matrix[i][j]);
            if(absCurrentElement > scales[i])
            {
                scales[i] = absCurrentElement;
            }
        }
    }
    return scales;
}

template<class T>
void
LUDecomposition<T>::
pivot(const Matrix<T>& matrix, 
      typename LUDecomposition<T>::OrderType& order,
      const typename LUDecomposition<T>::ScalesType& scales,
      const size_t row) const
{
    size_t largestPivotRow = row;
    T pivot = std::abs(matrix[order[row]][row] / scales[order[row]]);

    for(size_t i = row + 1; i < matrix.getNumRows(); ++i)
    {
        const T currentElement = 
            std::abs(matrix[order[i]][row] / scales[order[i]]);
        if(currentElement > pivot)
        {
            pivot = currentElement;
            largestPivotRow = i;
        }
    }

    std::swap(order[row], order[largestPivotRow]);
}

template<class T>
LUDecomposition<T>::Solution::Solution()
{}

template<class T>
LUDecomposition<T>::Solution::
Solution(const LowerTriangularMatrix<T>& L,
         const UpperTriangularMatrix<T>& U,
         const typename LUDecomposition<T>::OrderType& order)
    : m_L(L), m_U(U), m_order(order)
{}

template<class T>
inline
LowerTriangularMatrix<T>&
LUDecomposition<T>::Solution::L()
{
    return m_L;
}

template<class T>
inline
const LowerTriangularMatrix<T>&
LUDecomposition<T>::Solution::L() const
{
    return m_L;
}

template<class T>
inline
UpperTriangularMatrix<T>&
LUDecomposition<T>::Solution::U()
{
    return m_U;
}

template<class T>
inline
const UpperTriangularMatrix<T>&
LUDecomposition<T>::Solution::U() const
{
    return m_U;
}

template<class T>
inline
typename LUDecomposition<T>::OrderType&
LUDecomposition<T>::Solution::orderVector()
{
    return m_order;
}

template<class T>
inline
const typename LUDecomposition<T>::OrderType&
LUDecomposition<T>::Solution::orderVector() const
{
    return m_order;
}

template<class T>
DenseMatrix<T>
LUDecomposition<T>::Solution::P() const
{
    // create, fill in, and return the permutation matrix
    DenseMatrix<T> P(m_order.size(), m_order.size(), 0);
    for(size_t i = 0; i < m_order.size(); ++i)
    {
        P[i][m_order[i]] = 1;
    }
    return P;
}

