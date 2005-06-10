#include "NonlinearRegression.h"

#include <cmath>
#include <iostream>

template<class T>
T bookExampleFunction(const typename NonlinearRegression<T>::Parameters& p, 
                      const T& x);
template<class T>
T bookExampleFirstPartialA0(const typename NonlinearRegression<T>::Parameters& p, 
                            const T& x);
template<class T>
T bookExampleFirstPartialA1(const typename NonlinearRegression<T>::Parameters& p, 
                            const T& x);
template<class T>
bool bookExampleStoppingTest(const typename NonlinearRegression<T>::
                                            Parameters& parameterChanges);

int main()
{
    using std::cout;
    cout.precision(8);

    typedef double type;
    NonlinearRegression<type>::Points points;
    points.push_back(Point2D<type>(0.25, 0.28));
    points.push_back(Point2D<type>(0.75, 0.57));
    points.push_back(Point2D<type>(1.25, 0.68));
    points.push_back(Point2D<type>(1.75, 0.74));
    points.push_back(Point2D<type>(2.25, 0.79));
    
    const size_t numParameters = 2;

    NonlinearRegression<type>::Parameters initialGuesses;
    initialGuesses.reserve(numParameters);
    initialGuesses.push_back(1.0);
    initialGuesses.push_back(1.0);

    NonlinearRegression<type>::FirstPartials partials;
    partials.reserve(numParameters);
    partials.push_back(bookExampleFirstPartialA0);
    partials.push_back(bookExampleFirstPartialA1);

    NonlinearRegression<type> nlr;
    NonlinearRegression<type>::Parameters parameters = 
        nlr(points, initialGuesses, 
            bookExampleFunction<type>, partials, 
            bookExampleStoppingTest<type>, 10000);

    const type tolerance = .0001;
    const type expectedParameterValues[] = { .79186, 1.6751 };
    for(size_t i = 0; i < parameters.size(); ++i)
    {
        if(std::abs(parameters[i] - expectedParameterValues[i]) > tolerance)
        {
            return EXIT_FAILURE;
        }
    }
}

template<class T>
T 
bookExampleFunction(const typename NonlinearRegression<T>::Parameters& p, 
                    const T& x)
{
    return p[0]*(1-std::exp(-p[1]*x));
}

template<class T>
T 
bookExampleFirstPartialA0(const typename NonlinearRegression<T>::Parameters& p, 
                          const T& x)
{
    return 1-std::exp(-p[1]*x);
}

template<class T>
T 
bookExampleFirstPartialA1(const typename NonlinearRegression<T>::Parameters& p,
                          const T& x)
{
    return p[0]*x*std::exp(-p[1]*x);
}

template<class T>
bool 
bookExampleStoppingTest(const typename NonlinearRegression<T>::
                                       Parameters& parameterChanges)
{
    const T epsilon = .000000001;
    for(size_t i = 0; i < parameterChanges.size(); ++i)
    {
        if(std::abs(parameterChanges[i]) > epsilon)
        {
            return false;
        }
    }
    return true;
}

