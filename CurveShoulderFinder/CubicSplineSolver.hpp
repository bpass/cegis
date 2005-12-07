/**
 * @file CubicSplineSolver.hpp
 * @author Austin Hartman
 *
 * $Id: CubicSplineSolver.hpp,v 1.5 2005/12/07 23:49:04 ahartman Exp $
 */

#ifdef AUSTIN_CUBICSPLINESOLVER_H
#ifndef AUSTIN_CUBICSPLINESOLVER_HPP
#define AUSTIN_CUBICSPLINESOLVER_HPP

/********************************************
******* CubicSplineSolver functions *********
*********************************************/
template<class T>
typename CubicSplineSolver<T>::Solution
CubicSplineSolver<T>::operator()(const MyVector<T>& xCoords, 
                                 const MyVector<T>& yCoords) const
{
	// create the solution
	typename CubicSplineSolver<T>::Solution solution(xCoords.getSize()-1);

	// set the a's and x's
	for(size_t j = 0; j < solution.size(); ++j)
	{
		solution[j].x() = xCoords[j];
		solution[j].a() = yCoords[j];
	}

        // create the vector of h's
	MyVector<T> h(xCoords.getSize() - 1);
	for(size_t j = 0; j < h.getSize(); ++j)
	{
                h[j] = xCoords[j+1] - xCoords[j];
	}

	// create the tridiagonal matrix to solve for the c's
	TridiagonalMatrix<T> tm(xCoords.getSize());
	tm[0][0] = 1;
	tm[0][1] = 0;
	for(size_t k = 1; k < tm.getNumRows() - 1; ++k)
	{
		tm[k][k-1] = h[k-1];
		tm[k][k] = 2*(h[k-1] + h[k]);
		tm[k][k+1] = h[k];
	}
	tm[tm.getNumRows()-2][tm.getNumCols()-1] = 0;
	tm[tm.getNumRows()-1][tm.getNumCols()-1] = 1;

	// create the b vector to be solved with the tridiagonal matrix
	MyVector<T> bVec(tm.getNumCols());
	bVec[0] = 0;
	for(size_t k = 1; k < bVec.getSize() - 1; ++k)
	{
		// use the yCoords here because they contain an extra
		// term not present in the solution variable
		bVec[k] = (3/h[k])*(yCoords[k+1] - yCoords[k]) - 
		          (3/h[k-1])*(yCoords[k] - yCoords[k-1]);
	}
	bVec[bVec.getSize() - 1] = 0;

	// solve the system of the matrix and vector to get the c's
	ThomasAlgorithm<T> ta;
	typename ThomasAlgorithm<T>::Solution taSolution = ta(tm, bVec);

	// copy the c's from the solution
	// the last one from the taSolution vector will be trimmed off 
	// because it's not needed for the solution this function will
	// return
	for(size_t k = 0; k < solution.size(); ++k)
	{
		solution[k].c() = taSolution.vector()[k];
	}

	// find the d's
	// use the c's from the taSolution vector because they contain
	// an extra c that is needed to calculate the d's
	for(size_t k = 0; k < solution.size(); ++k)
	{
		solution[k].d() = 
			(taSolution.vector()[k+1] - taSolution.vector()[k]) /
			(3*h[k]);
	}

	// find the b's
	// again, use the c's from the taSolution because they contain an
	// extra member not present in the solution for this function
	// also, use the yCoords vector for the same reason
	for(size_t k = 0; k < solution.size(); ++k)
	{
		solution[k].b() = 
			(1/h[k]) * (yCoords[k+1]-yCoords[k]) -
			(h[k]/3) *
			(2*taSolution.vector()[k] + taSolution.vector()[k+1]);
	}

	return solution;
}



/********************************************
********* CubicSpline functions *************
*********************************************/
template<class T>
inline
T&
CubicSpline<T>::a()
{
	return a_;
}

template<class T>
inline
const T&
CubicSpline<T>::a() const
{
	return a_;
}

template<class T>
inline
T&
CubicSpline<T>::b()
{
	return b_;
}

template<class T>
inline
const T&
CubicSpline<T>::b() const
{
	return b_;
}

template<class T>
inline
T&
CubicSpline<T>::c()
{
	return c_;
}

template<class T>
inline
const T&
CubicSpline<T>::c() const
{
	return c_;
}

template<class T>
inline
T&
CubicSpline<T>::d()
{
	return d_;
}

template<class T>
inline
const T&
CubicSpline<T>::d() const
{
	return d_;
}

template<class T>
inline
T&
CubicSpline<T>::x()
{
	return x_;
}

template<class T>
inline
const T&
CubicSpline<T>::x() const
{
	return x_;
}

template<class T>
T
CubicSpline<T>::operator()(const T& term) const
{
	T eval = term - x_;
	T returnValue = a_ +  
	                b_ * eval  +  
	                c_ * eval * eval +
	                d_ * eval * eval * eval;
	return returnValue;
}



/****************************************
******* Global stream operators *********
*****************************************/
template<class T>
std::ostream&
operator<<(std::ostream& os, const CubicSpline<T>& spline)
{
    std::ostringstream oss;
	oss << "(x " << (spline.x() >= 0 ? '-' : '+') 
	    << ' ' << std::abs(spline.x()) << ')';
	os << spline.a();
    os << ' ' << (spline.b() >= 0 ? '+' : '-') << ' ' << std::abs(spline.b()) 
       << '*' << oss.str();
	os << ' ' << (spline.c() >= 0 ? '+' : '-') << ' ' << std::abs(spline.c()) 
       << '*' << oss.str() << "^2";
	os << ' ' << (spline.d() >= 0 ? '+' : '-') << ' ' << std::abs(spline.d()) 
       << '*' << oss.str() << "^3";

	return os;
}

#endif
#endif

