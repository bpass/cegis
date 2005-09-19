/**
 * @file testsUtility.hpp
 * @author Austin Hartman
 *
 * $Id: testsUtility.hpp,v 1.2 2005/09/19 21:23:56 ahartman Exp $
 */

template<class T>
bool matricesEqual(const Matrix<T>& m1, const Matrix<T>& m2, 
                   const T& tolerance)
{
    if((m1.getNumRows() != m2.getNumRows()) || 
       (m1.getNumCols() != m2.getNumCols()))
    {
        return false;
    }

    for(size_t j = 0; j < m1.getNumCols(); ++j)
    {
        for(size_t i = 0; i < m1.getNumRows(); ++i)
        {
            if(std::abs(m1[i][j] - m2[i][j]) > tolerance)
            {
                return false;
            }
        }
    }

    return true;
}

template<class T>
bool
floatsEqual(T v1, T v2)
{
    const T epsilon = .001;
    if(std::abs(v1 - v2) > epsilon)
    {
        return false;
    }
    return true;
}

