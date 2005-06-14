/**
 * @file driver.cpp
 * @author Austin Hartman
 *
 * $Id: driver.cpp,v 1.3 2005/06/14 23:47:54 ahartman Exp $
 */

#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "TridiagonalMatrix.h"
#include "CubicSplineSolver.h"
#include "CubicSplineEvaluator.h"
#include "CubicSplineCurve.h"
#include "LogisticCurve.h"
#include "CurvePartitioner.h"

using namespace std;

template<class T>
void printValues(const size_t n, const T& lowValue, const T& highValue, 
                 const FittingCurve<T>& curve);

template<class T>
class LessPoint2DX
{
public:
    bool operator()(const Point2D<T>& p1, const Point2D<T>& p2) const;
};

template<class T>
bool
LessPoint2DX<T>::operator()(const Point2D<T>& p1, const Point2D<T>& p2) const
{
    return p1.x() < p2.x();
}

template<class T>
class LessPoint2DY
{
public:
    bool operator()(const Point2D<T>& p1, const Point2D<T>& p2) const;
};

template<class T>
bool
LessPoint2DY<T>::operator()(const Point2D<T>& p1, const Point2D<T>& p2) const
{
    return p1.y() < p2.y();
}

int main(int argc, char* argv[])
{
	// set up some typedefs for easier reading/modification
	typedef double type;

    // set up the output options
    cout.precision(4);
    cout << fixed;

    // open the input file, after first making sure one was specified
	if(argc <= 1)
	{
		cerr << argv[0] << ':' << " no input file\n";
		return 1;
	}
	ifstream inputFile(argv[1]);

	// read in the number of pairs of x and y coordinates
	size_t numPoints;
	inputFile >> numPoints;

	// check to see if the number of pairs is valid
	if(numPoints < 2)
	{
		cerr << "The number of pairs is too small to find an "
		        "interpolating polynomial\n";
		return 2;
	}
    
    // read in the points
    CubicSplineCurve<type>::Points points(numPoints);
    for(size_t i = 0; i < numPoints; ++i)
    {
        inputFile >> points[i];
    }

    // create the fitting curve
    CubicSplineCurve<type> splineCurve(points);
//    LogisticCurve<type> logisticCurve(points, 9.81, 1.03E-5, -2.28);
    LogisticCurve<type> logisticCurve(points, 10, .1, -2);
//    LogisticCurve<type> logisticCurve(points, 10, 1, -1);
//    FittingCurve<type>& curve = logisticCurve;
    FittingCurve<type>& curve = splineCurve;

    // get the differences between successive y values along the curve
    CurvePartitioner<type> partitioner;
    const type lowerBound = 0.0;
    const type xValueCeil = std::ceil(std::max_element(points.begin(), 
                                                       points.end(), 
                                                       LessPoint2DX<type>())->x());
    std::ostringstream oss;
    oss << static_cast<long>(xValueCeil);
    const int numDigits = oss.str().length();
    const type dividend = std::pow(10.0, numDigits - 1);
    const type upperBound = std::ceil(xValueCeil / dividend) * dividend;
    const size_t numberOfPartitions = 200;
    CurvePartitioner<type>::CurveDifferences differences = 
        partitioner.getDifferences(curve, numberOfPartitions, 
                                   lowerBound, upperBound);

//    const size_t width = cout.precision() + 4;

    // print out some values at various points
//    cout << "lowerBound = " << lowerBound << '\n';
//    cout << "upperBound = " << upperBound << '\n';
//    cout << "L = " << logisticCurveL << '\n';

//    const type range = upperBound - lowerBound;
//    for(size_t i = 0; i <= numberOfPartitions; ++i)
//    {
//        const type currentX = lowerBound + (range / numberOfPartitions) * i;
//        cout << currentX << ", " << curve(currentX) << '\n';
//    }

//    printValues(numberOfPartitions, lowerBound, upperBound, curve);

//    for(size_t i = 0; i < differences.size(); ++i)
//    {
//        cout << "differences[" << setw(3) << i << "] = " 
//             << setw(width) << std::abs(differences[i]) << '\n';
//    }
    cout << '\n';
}

template<class T>
void
printValues(const size_t n, const T& lowValue, const T& highValue, 
            const FittingCurve<T>& curve)
{
    const size_t width = cout.precision() + 4;

    // print out the values at specified intervals
    const T range = highValue - lowValue;
    for(size_t i = 0; i <= n; ++i)
    {
        const T currentX = lowValue + (range / n) * i;
        cout << "curve(" << setw(width) << currentX << ") = "
             << setw(width) << curve(currentX) << '\n';
    }
}

