/******************************************
**** GaussianSolver functions *************
*******************************************/
template<class T>
GaussianSolver<T>::~GaussianSolver()
{}



/******************************************
**** Solution functions *******************
*******************************************/
template<class T>
inline
GaussianSolver<T>::Solution::Solution()
{}

template<class T>
inline
GaussianSolver<T>::Solution::Solution(const Matrix<T>& m,
                                      const MyVector<T>& v)
	: mat(m), vec(v)
{}

template<class T>
inline
DenseMatrix<T>&
GaussianSolver<T>::Solution::matrix()
{
	return mat;
}

template<class T>
inline
const DenseMatrix<T>&
GaussianSolver<T>::Solution::matrix() const
{
	return mat;
}

template<class T>
inline
MyVector<T>&
GaussianSolver<T>::Solution::vector()
{
	return vec;
}

template<class T>
inline
const MyVector<T>&
GaussianSolver<T>::Solution::vector() const
{
	return vec;
}



/********************************************
**** IncompatibleMatrixAndVector functions **
*********************************************/
template<class T>
inline
GaussianSolver<T>::IncompatibleMatrixAndVector::
IncompatibleMatrixAndVector(const size_t& matrixRows, 
                            const size_t& vectorSize)
	: matRows(matrixRows), vecSize(vectorSize)
{}

template<class T>
inline
const size_t&
GaussianSolver<T>::IncompatibleMatrixAndVector::getMatrixRows() const
{
	return matRows;
}

template<class T>
inline
const size_t&
GaussianSolver<T>::IncompatibleMatrixAndVector::getVectorSize() const
{
	return vecSize;
}

