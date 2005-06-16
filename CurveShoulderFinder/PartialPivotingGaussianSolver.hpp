/**
 * @file PartialPivotingGaussianSolver.hpp
 * @author Austin Hartman
 *
 * $Id: PartialPivotingGaussianSolver.hpp,v 1.3 2005/06/16 23:08:34 ahartman Exp $
 */

#ifdef AUSTIN_PARTIALPIVOTINGGAUSSIANSOLVER_H
#ifndef AUSTIN_PARTIALPIVOTINGGAUSSIANSOLVER_HPP
#define AUSTIN_PARTIALPIVOTINGGAUSSIANSOLVER_HPP

#include <algorithm> // for swap

template<class T>
PartialPivotingGaussianSolver<T>::~PartialPivotingGaussianSolver()
{}

template<class T>
typename GaussianSolver<T>::Solution
PartialPivotingGaussianSolver<T>::operator()(const Matrix<T>& m, 
                                             const MyVector<T>& v) const
{
	if(m.getNumRows() != v.getSize())
	{
		throw typename GaussianSolver<T>::
			IncompatibleMatrixAndVector(m.getNumRows(), 
			                            v.getSize());
	}

	typename GaussianSolver<T>::Solution solution(m, v);

	// get some easier names to read
	DenseMatrix<T>& mat = solution.matrix();
	MyVector<T>& vec = solution.vector();

	// do the forward elimination
	for(size_t i = 0; i < mat.getNumRows() - 1; ++i)
	{
		partialPivot(mat, vec, i, i);
		for(size_t j = i + 1; j < mat.getNumRows(); ++j)
		{
			T factor = -mat[j][i] / mat[i][i];
			mat.rowReplace(j, i, factor);
			vec[j] += vec[i] * factor;
		}
	}

	// do the back substitution
	for(size_t i = mat.getNumRows() - 1; i >= 1; --i)
	{
		// make the pivot in this row a 1
		vec[i] *= 1/mat[i][i];
		mat.rowScale(i, 1/mat[i][i]);

		for(size_t j = 0; j < i; ++j)
		{
			T factor = -mat[j][i];
			// mat[i][i] should be 1
			mat.rowReplace(j, i, factor);
			vec[j] += vec[i] * factor;
		}
	}

	// make the pivot in the first row a 1
	vec[0] *= 1/mat[0][0];
	mat.rowScale(0, 1/mat[0][0]);

	return solution;
}

template<class T>
void 
PartialPivotingGaussianSolver<T>::
partialPivot(DenseMatrix<T>& mat, MyVector<T>& vec,
             const size_t& row, const size_t& column) const
{
	size_t maxIndex = indexLargestElementRow(mat, row, column);
	if(maxIndex != row)
	{
		mat.rowInterchange(maxIndex, row);
		std::swap(vec[maxIndex], vec[row]);
	}
}

template<class T>
inline
size_t
PartialPivotingGaussianSolver<T>::
indexLargestElementRow(const Matrix<T>& mat,
                       const size_t& rowToStart,
                       const size_t& column) const
{
	size_t maxIndex = rowToStart;
	for(size_t i = rowToStart + 1; i < mat.getNumRows(); ++i)
	{
		if(std::abs(mat[i][column]) > std::abs(mat[maxIndex][column]))
		{
			maxIndex = i;
		}
	}
	return maxIndex;
}

#endif
#endif

