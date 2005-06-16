/**
 * @file ThomasAlgorithm.hpp
 * @author Austin Hartman
 *
 * $Id: ThomasAlgorithm.hpp,v 1.3 2005/06/16 23:10:08 ahartman Exp $
 */

#ifdef AUSTIN_THOMASALGORITHM_H
#ifndef AUSTIN_THOMASALGORITHM_HPP
#define AUSTIN_THOMASALGORITHM_HPP

/******************************************
**** ThomasAlgorithm functions ************
*******************************************/
template<class T>
typename ThomasAlgorithm<T>::Solution
ThomasAlgorithm<T>::operator()(const TridiagonalMatrix<T>& matrix, 
                               const MyVector<T>& vec) const
{
	if(matrix.getNumCols() != vec.getSize())
	{
		throw typename ThomasAlgorithm<T>::
			IncompatibleMatrixAndVector(matrix.getNumCols(), 
			                            vec.getSize());
	}

	Solution solution(matrix, vec);
	TridiagonalMatrix<T>& tm = solution.matrix();
	for(size_t k = 1; k < tm.getNumRows(); ++k)
	{
		tm[k][k-1] /= tm[k-1][k-1];
		tm[k][k] -= tm[k][k-1] * tm[k-1][k];
	}

	// do the forward substitution
	MyVector<T> r(vec);
	for(size_t k = 1; k < vec.getSize(); ++k)
	{
		r[k] -= tm[k][k-1] * r[k-1];
	}

	// do the back substitution
	MyVector<T>& x = solution.vector();
	x[x.getSize()-1] = 
		r[r.getSize()-1] / tm[tm.getNumRows()-1][tm.getNumCols()-1];
	for(int k = x.getSize()-2; k >= 0; --k)
	{
                x[k] = (r[k] - tm[k][k+1] * x[k+1]) / tm[k][k];
	}

	return solution;
}



/******************************************
**** Solution functions *******************
*******************************************/
template<class T>
inline
ThomasAlgorithm<T>::Solution::Solution()
{}

template<class T>
inline
ThomasAlgorithm<T>::Solution::Solution(const TridiagonalMatrix<T>& m,
				       const MyVector<T>& v)
	: mat(m), vec(v)
{}

template<class T>
inline
TridiagonalMatrix<T>&
ThomasAlgorithm<T>::Solution::matrix()
{
	return mat;
}

template<class T>
inline
const TridiagonalMatrix<T>&
ThomasAlgorithm<T>::Solution::matrix() const
{
	return mat;
}

template<class T>
inline
MyVector<T>&
ThomasAlgorithm<T>::Solution::vector()
{
	return vec;
}

template<class T>
inline
const MyVector<T>&
ThomasAlgorithm<T>::Solution::vector() const
{
	return vec;
}



/********************************************
**** IncompatibleMatrixAndVector functions **
*********************************************/
template<class T>
inline
ThomasAlgorithm<T>::IncompatibleMatrixAndVector::
IncompatibleMatrixAndVector(const size_t& matrixCols, 
                            const size_t& vectorSize)
	: matCols(matrixCols), vecSize(vectorSize)
{}

template<class T>
inline
const size_t&
ThomasAlgorithm<T>::IncompatibleMatrixAndVector::getMatrixCols() const
{
	return matCols;
}

template<class T>
inline
const size_t&
ThomasAlgorithm<T>::IncompatibleMatrixAndVector::getVectorSize() const
{
	return vecSize;
}

#endif
#endif

