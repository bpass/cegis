/**
 * @file MarquardtMethod.h
 * @author Austin Hartman
 *
 * $Id: MarquardtMethod.h,v 1.4 2005/06/21 22:40:14 ahartman Exp $
 */

#ifndef AUSTIN_MARQUARDTMETHOD_H
#define AUSTIN_MARQUARDTMETHOD_H

#include <vector>
#include "Point2D.h"

/**
 * A functor that implements the Levenberg-Marquardt algorithm to find the
 * values of the parameters of a function that will result in the lowest
 * sum of the squares of the residuals between the actual data values passed 
 * in and the values given by the function using those parameters.
 */
template<class T>
class MarquardtMethod
{
public:
    /**
     * A set of points.
     */
    typedef std::vector<Point2D<T> > Points;

    /**
     * The values that the method will adjust in order to find the values
     * that minimize a sum of the squares of the residuals function.
     */
    typedef std::vector<T> Parameters;

    /**
     * A sum of the squares of the residuals function.
     */
    typedef T (*SumSquaresResiduals)(const Points& points, const Parameters& p);

    /**
     * A first partial of the sum of the squares of the residuals function.
     */
    typedef T (*FirstPartial)(const Points& points, const Parameters& p);

    /**
     * A second partial of the sum of the squares of the residuals function.
     */
    typedef T (*SecondPartial)(const Points& points, const Parameters& p);

    /**
     * A test to tell when the algorithm should stop.
     */
    typedef bool (*StoppingTest)(const Points& points, 
                                 const Parameters& currentParameters);

    /**
     * A set of first partial derivatives.
     */
    typedef std::vector<FirstPartial> FirstPartials;

    /**
     * A set of second partial derivatives.
     */
    typedef std::vector<SecondPartial> SecondPartials;

    /**
     * A function to find parameters that minimize a sum of residuals function.
     * This is used to do nonlinear regression.
     *
     * @param points The set of points that the method should try to fit a 
     * curve to.
     * @param initialGuesses The initial guesses for the parameters of the
     * curve function.  The results of the method are generally very dependent
     * on how good these initial guesses are.
     * @param sumResiduals A function that should return the sum of the
     * squares of the residuals when it is passed the points and a set of 
     * parameters.
     * @param firstPartials A set of functions that are the first partials
     * of the sum of the squares of the residuals function with respect to 
     * each parameter.  There should be one first partial for each parameter.  
     * The first partials should be ordered such that the partial with 
     * respect to the first parameter passed into initialGuesses comes first, 
     * followed by the second parameter, and so on.
     * @param secondPartials A set of functions that are the second partials
     * of the sum of the squares of the residuals function.  They should be 
     * partials of the first partials with respect to each parameter.  There 
     * should be * (initialGuesses.size())^2 partials.  The second partials 
     * should be ordered as follows: the second partials for all parameters 
     * with respect to the first parameter passed into initialGuesses should 
     * come first, followed by the second partials for all parameters with 
     * respect to the second parameter, and so on.
     * @param stoppingTest A function that should return true if the method
     * should stop or false if otherwise.  This should be used to stop the
     * algorithm if the sum of residuals is small enough that it doesn't need
     * to continue.
     * @param maxIterations An optional parameter that tells how many times
     * the function should loop before giving up and returning the best values
     * of the parameters it found.
     *
     * @return The best values of the parameters that the method found.
     * 
     * @exception InvalidFirstPartialsSize 
     * thrown if firstPartials.size() != initialGuesses.size()
     * @exception InvalidSecondPartialsSize
     * thrown if secondPartials.size() != 
     *           initialGuesses.size() * initialGuesses.size()
     */
    Parameters operator()(const Points& points,
                          const Parameters& initialGuesses,
                          const SumSquaresResiduals& sumResiduals,
                          const FirstPartials& firstPartials,
                          const SecondPartials& secondPartials,
                          const StoppingTest& stoppingTest,
                          const size_t maxIterations = m_maxIterations) const;

    /**
     * Exception class to be thrown whenever the size of the FirstPartials
     * parameter is incorrect.
     */
    class InvalidFirstPartialsSize
    {};

    /**
     * Exception class to be thrown whenever the size of the FirstPartials
     * parameter is incorrect.
     */
    class InvalidSecondPartialsSize
    {};

private:
    /**
     * The default number of iterations to do.
     */
    static const size_t m_maxIterations;
};

#include "MarquardtMethod.hpp"

#endif

