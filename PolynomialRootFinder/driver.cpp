/**
 * @file driver.cpp
 * @author Austin Hartman
 * $Id: driver.cpp,v 1.4 2005/04/28 21:31:55 ahartman Exp $
 */

#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <functional>
#include <complex>

#include "Polynomial.h"
#include "Muller.h"
#include "QuadraticFormula.h"
#include "PolynomialRootFinder.h"
#include "RealDouble.h"

using std::cout;
using std::cerr;
using std::ifstream;

typedef RealDouble type;
//typedef std::complex<double> type;

void deflationTest();
void deflationTest2();
void mullerTest();
void mullerTest2();
void quadraticFormulaTest();
void polynomialRootFinderTest(const Polynomial<type>& p);

int main(int argc, char* argv[])
{
//    quadraticFormulaTest();
//    return 0;
//
//    mullerTest2();
//    return 0;
//    
//    mullerTest();
//    return 0;
//    
//    deflationTest();
//    return 0;
//
//    deflationTest2();
//    return 0;

    if(argc < 2)
    {
        cerr << "No file specified... Aborting.\n";
        return 1;
    }
    
//    RealDouble::setEpsilon(0);
//    ifstream file(argv[1]);
//    Polynomial<type> origP;
//    file >> origP;
//
//    RealDouble::setEpsilon(.00000001);
//    const type scalar = 1000000000;
//    cout << "Read in Polynomial:\n" << origP << '\n';
//    cout << "p = " << scalar << " * origP.\n\n";
//    Polynomial<type> p(scalar * origP);

    RealDouble::setEpsilon(.00000001);
    ifstream file(argv[1]);
    Polynomial<type> p;
    file >> p;

    cout << "Original Polynomial:\n";
    polynomialRootFinderTest( p );
    cout << "First Derivative:\n";
    polynomialRootFinderTest( p.derivative() );
    cout << "Second Derivative:\n";
    polynomialRootFinderTest( p.derivative().derivative() );
    cout << "Third Derivative:\n";
    polynomialRootFinderTest( p.derivative().derivative().derivative() );
    cout << "Fourth Derivative:\n";
    polynomialRootFinderTest( p.derivative().derivative().derivative().derivative() );


//    Polynomial<type> firstDeriv = p.derivative();
//    cout << "First Derivative: " << firstDeriv << '\n';
//
//    Polynomial<type> secondDeriv = firstDeriv.derivative();
//    cout << "Second Derivative: " << secondDeriv << '\n';
//
//    Polynomial<type> thirdDeriv = secondDeriv.derivative();
//    cout << "Third Derivative: " << thirdDeriv << '\n';
////    cout << "Third Derivative(7.16098) = " << thirdDeriv(7.16098) << '\n';
////    cout << "Third Derivative(2.85193) = " << thirdDeriv(2.85193) << '\n';
//
//    cout << "Polynomial degree: " << p.degree() << '\n';
//    cout << "First Derivative degree: " << firstDeriv.degree() << '\n';
//    cout << "Second Derivative degree: " << secondDeriv.degree() << '\n';
//    cout << "Third Derivative degree: " << thirdDeriv.degree() << '\n';
}

void deflationTest()
{
    Polynomial<type> p(1.0, 3);
    p += Polynomial<type>(-7.0, 1);
    p += Polynomial<type>(-6.0, 0);
    cout << p << '\n';
    cout << "p(3) = " << p(3) << '\n';
    cout << "p(-2) = " << p(-2) << '\n';
    cout << "p(-1) = " << p(-1) << '\n';

    Polynomial<type> deflatedP(p.deflate(3.0));
    cout << deflatedP << '\n';
    cout << "deflatedP(-2) = " << deflatedP(-2) << '\n';
    cout << "deflatedP(-1) = " << deflatedP(-1) << '\n';
}

void deflationTest2()
{
    Polynomial<type> p(-120.0, 0);
    p += Polynomial<type>(-46.0, 1);
    p += Polynomial<type>(79, 2);
    p += Polynomial<type>(-3, 3);
    p += Polynomial<type>(-7, 4);
    p += Polynomial<type>(1, 5);

    cout << "p = " << p << '\n';
    cout.flush();
    cout << "p.deflate(-3) = " << p.deflate(-3) << '\n';
}

void mullerTest()
{
    Polynomial<type> p(1.0, 3);
    p += Polynomial<type>(-13.0, 1);
    p += Polynomial<type>(-12.0, 0);

    cout << "p = " << p << '\n';

    Muller<type> muller;

    type root = muller(p, 5, .5/5, .001, 100);
    cout << "root = " << root << '\n';
    cout << "p(root) = " << p(root) << '\n';

    Polynomial<type> p2 = p.deflate(root);
    cout << "p2 = " << p2 << '\n';
    type root2 = muller(p2, root, .5/root, .01, 100);

    cout << "root2 = " << root2 << '\n';
    cout << "p(root2) = " << p2(root2) << '\n';
}

void mullerTest2()
{
    const type epsilon = .0001;
    const size_t iterations = 20;

    Polynomial<type> p(-2E-11, 6);
    p += Polynomial<type>(-.0062, 5);
    p += Polynomial<type>(.1552, 4);
    p += Polynomial<type>(-1.2662, 3);
    p += Polynomial<type>(3.4774, 2);
    p += Polynomial<type>(-2.2722, 1);
    p += Polynomial<type>(10.122, 0);

    p = p.derivative();

    Muller<type> muller;

    cout << "p = " << p << '\n';
    type root = muller(p, 5, 1, epsilon, iterations);
    cout << "root = " << root << '\n';
    cout << "p(root) = " << p(root) << "\n\n";

    Polynomial<type> p2 = p.deflate(root);
    cout << "p2 = " << p2 << '\n';
    type root2 = muller(p2, 5, 1, epsilon, iterations);
    cout << "root2 = " << root2 << '\n';
    cout << "p(root2) = " << p(root2) << "\n\n";

    Polynomial<type> p3 = p2.deflate(root2);
    cout << "p3 = " << p3 << '\n';
    type root3 = muller(p3, 5, 1, epsilon, iterations);
    cout << "root3 = " << root3 << '\n';
    cout << "p(root3) = " << p(root3) << "\n\n";
}

void quadraticFormulaTest()
{
    Polynomial<type> p(1.0, 2);
    p += Polynomial<type>(2.0, 1);
    p += Polynomial<type>(-35.0, 0);

    QuadraticFormula<type> quad;
    QuadraticFormula<type>::Roots roots = quad(p);

    cout << "Roots = ";
    if(!roots.empty())
    {
        for(QuadraticFormula<type>::Roots::iterator i = roots.begin();
            i != roots.end(); ++i)
        {
            cout << *i << ' ';
        }
    }
    else
    {
        cout << "(none)";
    }
    cout << '\n';
}

void polynomialRootFinderTest(const Polynomial<type>& p)
{
    cout << "p = " << p << '\n';
    PolynomialRootFinder<type> finder;
    PolynomialRootFinder<type>::Roots roots = finder(p, 1000, 100, .000001, 100);
    std::sort(roots.begin(), roots.end(), std::greater<type>());

    cout << "Roots = ";
    std::copy(roots.begin(), roots.end(), 
              std::ostream_iterator<type>(cout, " "));
    cout << '\n';

    cout << "Values at roots = ";
    for(PolynomialRootFinder<type>::Roots::iterator i = roots.begin();
        i != roots.end(); ++i)
    {
        cout << p(*i) << ' ';
    }
    cout << "\n\n";
}

