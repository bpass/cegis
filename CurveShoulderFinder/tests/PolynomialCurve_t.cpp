/**
 * @file PolynomialCurve_t.cpp
 * @author Austin Hartman
 *
 * $Id: PolynomialCurve_t.cpp,v 1.2 2005/06/16 22:29:23 ahartman Exp $
 */

#include <cmath>

#include "PolynomialCurve.h"

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

    {
        const type tolerance = .0001;

        // create the points
        const size_t numPoints = 6;
        PolynomialCurve<type>::Points points;
        points.reserve(numPoints);
        points.push_back(Point2D<type>(0, 2.1));
        points.push_back(Point2D<type>(1, 7.7));
        points.push_back(Point2D<type>(2, 13.6));
        points.push_back(Point2D<type>(3, 27.2));
        points.push_back(Point2D<type>(4, 40.9));
        points.push_back(Point2D<type>(5, 61.1));

        // create the polynomial to fit those points
        const size_t order = 2;
        PolynomialCurve<type> curve(points, order);

        // check the found coefficients against the expected values
        const type expectedCoefficients[] = { 2.47857, 2.35929, 1.86071 };

#ifdef PRINT_VALUES
        for(size_t i = 0; i < order + 1; ++i)
        {
            cout << "a[" << i << "] = " << curve.getCoefficient(i) << '\n';
        }
#endif

        for(size_t i = 0; i < order + 1; ++i)
        {
            assert(std::abs(curve.getCoefficient(i) - 
                            expectedCoefficients[i]) < 
                   tolerance);
        }
    }

    {
        const type tolerance = .001;

        // create the points
        const size_t numPoints = 9;
        PolynomialCurve<type>::Points points;
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
        PolynomialCurve<type> curve(points, order);

        // check the found coefficients against the expected values
        const type expectedCoefficients[] = 
        { 10.122, -2.2722, 3.4774, -1.2662, .1552, -.0062, -2E-11 };

#ifdef PRINT_VALUES
        for(size_t i = 0; i < order + 1; ++i)
        {
            cout << "a[" << i << "] = " << curve.getCoefficient(i) << '\n';
        }
#endif

        for(size_t i = 0; i < order + 1; ++i)
        {
            assert(std::abs(curve.getCoefficient(i) - 
                            expectedCoefficients[i]) < 
                   tolerance);
        }
    }
}

