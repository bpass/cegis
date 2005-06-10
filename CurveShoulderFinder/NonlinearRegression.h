/**
 * @file NonlinearRegression.h
 * @author Austin Hartman
 *
 * $Id: NonlinearRegression.h,v 1.2 2005/06/10 21:52:23 ahartman Exp $
 */

#ifndef AUSTIN_NONLINEARREGRESSION_H
#define AUSTIN_NONLINEARREGRESSION_H

#include <vector>
#include "Point2D.h"
//#include "Matrix.h"
#include "DenseMatrix.h"

template<class T>
class NonlinearRegression
{
public:
    typedef std::vector<Point2D<T> > Points;
    typedef std::vector<T> Parameters;
    typedef T (*Function)(const Parameters& p, const T& x);
    typedef T (*FirstPartial)(const Parameters& p, const T& x);
    typedef bool (*StoppingTest)(const Parameters& parameterChanges);
    typedef std::vector<FirstPartial> FirstPartials;

    Parameters operator()(const Points& points, 
                          const Parameters& initialGuesses,
                          const Function& f, 
                          const FirstPartials& partials,
                          const StoppingTest& stoppingTest,
                          const size_t maxIterations = m_maxIterations) const;

    class DifferentParametersPartialsSizes
    {};

    class NotEnoughPoints
    {};

private:
    DenseMatrix<T> createZMatrix(const Points& points, 
                                 const FirstPartials& partials,
                                 const Parameters& parameters) const;
    MyVector<T> createDVector(const Points& points, 
                              const Function& f,
                              const Parameters& parameters) const;
    static const size_t m_maxIterations;
};

#include "NonlinearRegression.hpp"

#endif

