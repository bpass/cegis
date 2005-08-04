/**
 * @file PolynomialRegression_t.cpp
 * @author Austin Hartman
 *
 * $Id: PolynomialRegression_t.cpp,v 1.3 2005/08/04 17:50:44 ahartman Exp $
 */

#include <cmath>

#include "PolynomialRegression.h"

//#define PRINT_VALUES

#ifdef PRINT_VALUES
#include <iostream>
#endif

int main()
{
#ifdef PRINT_VALUES
    using std::cout;
    using std::setw;
    using std::fixed;
    cout << fixed;
    cout.precision(8);
    const size_t width = cout.precision() + 4;
#endif

    typedef double type;

    // test using example from _Numerical Methods for Engineers_ by Steven C.
    // Chapra and Raymond P. Canale on p. 457-459
    {
        const type tolerance = .0001;

        // create the points
        const size_t numPoints = 6;
        PolynomialRegression<type>::Points points;
        points.reserve(numPoints);
        points.push_back(Point2D<type>(0, 2.1));
        points.push_back(Point2D<type>(1, 7.7));
        points.push_back(Point2D<type>(2, 13.6));
        points.push_back(Point2D<type>(3, 27.2));
        points.push_back(Point2D<type>(4, 40.9));
        points.push_back(Point2D<type>(5, 61.1));

        // create the polynomial to fit those points
        const size_t order = 2;
        PolynomialRegression<type> polyRegress;
        PolynomialRegression<type>::Coefficients coefficients = 
            polyRegress(points, order);

        // check the found coefficients against the expected values
        const type expectedCoefficients[] = { 2.47857, 2.35929, 1.86071 };

#ifdef PRINT_VALUES
        for(size_t i = 0; i < order + 1; ++i)
        {
            cout << "a[" << i << "] = " << coefficients[i] << '\n';
        }
#endif

        for(size_t i = 0; i < order + 1; ++i)
        {
            assert(std::abs(coefficients[i] - expectedCoefficients[i]) < 
                   tolerance);
        }
    }

    // test using values Mike Finn made up and comparing results to those 
    // acquired from a calculator
    {
        const type tolerance = .001;

        // create the points
        const size_t numPoints = 9;
        PolynomialRegression<type>::Points points;
        points.reserve(numPoints);
        points.push_back(Point2D<type>(0.16, 9.84));
        points.push_back(Point2D<type>(0.20, 9.80));
        points.push_back(Point2D<type>(0.25, 9.75));
        points.push_back(Point2D<type>(4.00, 9.00));
        points.push_back(Point2D<type>(5.00, 5.00));
        points.push_back(Point2D<type>(6.00, 1.00));
        points.push_back(Point2D<type>(9.75, 0.25));
        points.push_back(Point2D<type>(9.80, 0.20));
        points.push_back(Point2D<type>(9.84, 0.16));

        // create the polynomial to fit those points
        const size_t order = 6;
        PolynomialRegression<type> polyRegress;
        PolynomialRegression<type>::Coefficients coefficients = 
            polyRegress(points, order);

        // check the found coefficients against the expected values
        const type expectedCoefficients[] = 
            { 10.122, -2.2722, 3.4774, -1.2662, .1552, -.0062, -2E-11 };

#ifdef PRINT_VALUES
        for(size_t i = 0; i < order + 1; ++i)
        {
            cout << "a[" << i << "] = " << coefficients[i] << '\n';
        }
#endif

        for(size_t i = 0; i < order + 1; ++i)
        {
            assert(std::abs(coefficients[i] - expectedCoefficients[i]) < 
                   tolerance);
        }
    }
}

