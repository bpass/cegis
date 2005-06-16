/**
 * @file MarquardtMethod.h
 * @author Austin Hartman
 *
 * $Id: MarquardtMethod.h,v 1.3 2005/06/16 01:33:58 ahartman Exp $
 */

#ifndef AUSTIN_MARQUARDTMETHOD_H
#define AUSTIN_MARQUARDTMETHOD_H

#include <vector>
#include "Point2D.h"

/**
 * A functor that implements the Levenberg-Marquardt algorithm to find the
 * values of the parameters of a function that will result in the lowest
 * sum of the residuals between the actual data values passed in and the
 * values given by the function using those parameters.
 */
template<class T>
class MarquardtMethod
{
public:
    typedef std::vector<Point2D<T> > Points;
    typedef std::vector<T> Parameters;
    typedef T (*SumResiduals)(const Points& points, const Parameters& p);
    typedef T (*FirstPartial)(const Points& points, const Parameters& p);
    typedef T (*SecondPartial)(const Points& points, const Parameters& p);
    typedef bool (*StoppingTest)(const Points& points, 
                                 const Parameters& currentParameters);
    typedef std::vector<FirstPartial> FirstPartials;
    typedef std::vector<SecondPartial> SecondPartials;

    /**
     * A function to find parameters that minimize a sum of residuals function.
     * This is used to do nonlinear regression.
     *
     * @param points The set of points that the method tries to fit a curve to.
     * @param initialGuesses The initial guesses for the parameters of the
     * curve function.  The results of the method are generally very dependent
     * on how good these initial guesses are.
     * @param sumResiduals A function that should return the sum of the
     * residuals when it is passed the points and a set of parameters
     * @param firstPartials A set of functions that are the first partials
     * of the sum of residuals function with respect to each parameter.  There
     * should be one first partial for each parameter.
     * @param secondPartials A set of functions that are the second partials
     * of the sum of residuals function.  They should be partials of the first
     * partials with respect to each parameter.  There should be 
     * (initialGuesses.size())^2 partials.
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
                          const SumResiduals& sumResiduals,
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
    static const size_t m_maxIterations;
};

#include "MarquardtMethod.hpp"

#endif

