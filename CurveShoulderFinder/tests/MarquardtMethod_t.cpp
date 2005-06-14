/**
 * @file MarquardtMethod_t.cpp
 * @author Austin Hartman
 *
 * $Id: MarquardtMethod_t.cpp,v 1.1 2005/06/14 22:27:03 ahartman Exp $
 */

//#define PRINT_VALUES

#include <cassert>
#include <cmath>

#ifdef PRINT_VALUES
#include <iostream>
#endif

#include "MarquardtMethod.h"
#include "Point2D.h"

template<class T>
T 
sumResiduals(const typename MarquardtMethod<T>::Points& points, 
             const typename MarquardtMethod<T>::Parameters& p)
{
    T sum = 0;
    for(size_t i = 0; i < points.size(); ++i)
    {
        const T factor = points[i].y() - 
                         p[0]*(1 - std::exp(-p[1]*points[i].x()));
        sum += factor*factor;
    }
    return sum;
}

template<class T>
T 
sumResidualsFirstPartialA(const typename MarquardtMethod<T>::Points& points, 
                          const typename MarquardtMethod<T>::Parameters& p)
{
    T sum = 2 * points.size() * p[0];
    for(size_t i = 0; i < points.size(); ++i)
    {
        const T expFactor = std::exp(p[1]*points[i].x());
        sum += -2*points[i].y();
        sum += 2*points[i].y() / expFactor;
        sum += -4*p[0] / expFactor;
        sum += 2*p[0] / (expFactor * expFactor);
    }
    return sum;
}

template<class T>
T 
sumResidualsFirstPartialB(const typename MarquardtMethod<T>::Points& points, 
                          const typename MarquardtMethod<T>::Parameters& p)
{
    T sum = 0;
    for(size_t i = 0; i < points.size(); ++i)
    {
        const T expFactor = std::exp(p[1]*points[i].x());
        sum += -2*p[0]*points[i].y()*points[i].x() / expFactor;
        sum += 2*p[0]*p[0]*points[i].x() / expFactor;
        sum += -2*p[0]*p[0]*points[i].x() / (expFactor * expFactor);
    }
    return sum;
}

template<class T>
T
sumResidualsSecondPartialAA(const typename MarquardtMethod<T>::Points& points, 
                            const typename MarquardtMethod<T>::Parameters& p)
{
    T sum = 2 * points.size();
    for(size_t i = 0; i < points.size(); ++i)
    {
        const T expFactor = std::exp(p[1]*points[i].x());
        sum += -4 / expFactor;
        sum += 2 / (expFactor * expFactor);
    }
    return sum;
}

template<class T>
T
sumResidualsSecondPartialAB(const typename MarquardtMethod<T>::Points& points, 
                            const typename MarquardtMethod<T>::Parameters& p)
{
    T sum = 0;
    for(size_t i = 0; i < points.size(); ++i)
    {
        const T expFactor = std::exp(p[1] * points[i].x());
        sum += -2*points[i].x()*points[i].y() / expFactor;
        sum += 4*p[0]*points[i].x() / expFactor;
        sum += -4*p[0]*points[i].x() / (expFactor * expFactor);
    }
    return sum;
}

template<class T>
T
sumResidualsSecondPartialBA(const typename MarquardtMethod<T>::Points& points, 
                            const typename MarquardtMethod<T>::Parameters& p)
{
    return sumResidualsSecondPartialAB<T>(points, p);
}

template<class T>
T
sumResidualsSecondPartialBB(const typename MarquardtMethod<T>::Points& points, 
                            const typename MarquardtMethod<T>::Parameters& p)
{
    T sum = 0;
    const T& a = p[0];
    const T& b = p[1];
    const T a2 = a*a;
    for(size_t i = 0; i < points.size(); ++i)
    {
        const T& x = points[i].x();
        const T& y = points[i].y();
        const T x2 = x*x;
        const T expFactor = std::exp(b*x);
        sum += 2*a*y*x2 / expFactor;
        sum += -2*a2*x2 / expFactor;
        sum += 4*a2*x2 / (expFactor * expFactor);
    }
    return sum;
}

template<class T>
bool
stoppingTest(const typename MarquardtMethod<T>::Points& points,
             const typename MarquardtMethod<T>::Parameters& currentParameters)
{
    const T tolerance = .0001;
    if(sumResiduals<T>(points, currentParameters) > tolerance)
    {
        return false;
    }
    return true;
}

int main()
{
#ifdef PRINT_VALUES
    using std::cout;
    using std::fixed;
    using std::setw;
    cout.precision(8);
    cout << fixed;
#endif

    typedef double type;

    // initialize the points
    const size_t numPoints = 5;
    MarquardtMethod<type>::Points points;
    points.reserve(numPoints);
    points.push_back(Point2D<type>(.25, .28));
    points.push_back(Point2D<type>(.75, .57));
    points.push_back(Point2D<type>(1.25, .68));
    points.push_back(Point2D<type>(1.75, .74));
    points.push_back(Point2D<type>(2.25, .79));

    // create the initial guesses
    const size_t numParameters = 2;
    const type initialGuessesArray[] = { 1.0, 1.0 };
    const MarquardtMethod<type>::Parameters 
        initialGuesses(initialGuessesArray, 
                       initialGuessesArray + numParameters);

    // create the first partials vector
    MarquardtMethod<type>::FirstPartials firstPartials;
    firstPartials.reserve(numParameters);
    firstPartials.push_back(sumResidualsFirstPartialA);
    firstPartials.push_back(sumResidualsFirstPartialB);

    // create the second partials vector
    MarquardtMethod<type>::SecondPartials secondPartials;
    secondPartials.reserve(numParameters * numParameters);
    secondPartials.push_back(sumResidualsSecondPartialAA);
    secondPartials.push_back(sumResidualsSecondPartialAB);
    secondPartials.push_back(sumResidualsSecondPartialBA);
    secondPartials.push_back(sumResidualsSecondPartialBB);

    // run the Marquardt method
    MarquardtMethod<type> mm;
    const MarquardtMethod<type>::Parameters parameters = 
        mm(points, initialGuesses, 
           sumResiduals<type>, firstPartials, secondPartials,
           stoppingTest<type>);

#ifdef PRINT_VALUES
    // print out the values
    cout << "a = " << parameters[0] << '\n';
    cout << "b = " << parameters[1] << '\n';
#endif

    // compare to the expected values
    const type tolerance = .0001;
    const type expectedValuesArray[] = { .79186, 1.6751 };
    const MarquardtMethod<type>::Parameters 
        expectedValues(expectedValuesArray, 
                       expectedValuesArray + numParameters);
    for(size_t i = 0; i < numParameters; ++i)
    {
        assert(std::abs(parameters[i] - expectedValues[i]) < tolerance);
    }
}

