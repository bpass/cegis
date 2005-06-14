/**
 * @file MarquardtMethod.h
 * @author Austin Hartman
 *
 * $Id: MarquardtMethod.h,v 1.1 2005/06/14 23:40:51 ahartman Exp $
 */

#ifndef AUSTIN_MARQUARDTMETHOD_H
#define AUSTIN_MARQUARDTMETHOD_H

#include <vector>
#include "Point2D.h"

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

    Parameters operator()(const Points& points,
                          const Parameters& initialGuesses,
                          const SumResiduals& sumResiduals,
                          const FirstPartials& firstPartials,
                          const SecondPartials& secondPartials,
                          const StoppingTest& stoppingTest,
                          const size_t maxIterations = m_maxIterations) const;

    class InvalidFirstPartialsSize
    {};

    class InvalidSecondPartialsSize
    {};

private:
    static const size_t m_maxIterations;
};

#include "MarquardtMethod.hpp"

#endif

