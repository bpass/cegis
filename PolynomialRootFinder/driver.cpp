#include <iostream>
#include <fstream>

#include "Polynomial.h"
#include "Muller.h"
#include "QuadraticFormula.h"

using std::cout;
using std::cerr;
using std::ifstream;

void deflationTest();
void mullerTest();
void mullerTest2();
void quadraticFormulaTest();

int main(int argc, char* argv[])
{
    typedef double type;

//    mullerTest2();
//    return 0;
//    
//    mullerTest();
//    return 0;
//    
//    deflationTest();
//    return 0;

    if(argc < 2)
    {
        cerr << "No file specified... Aborting.\n";
        return 1;
    }
    
    ifstream file(argv[1]);
    Polynomial<type> p;
    file >> p;
    cout << "Polynomial: " << p << '\n';

    Polynomial<type> firstDeriv = p.derivative();
    cout << "First Derivative: " << firstDeriv << '\n';

    Polynomial<type> secondDeriv = firstDeriv.derivative();
    cout << "Second Derivative: " << secondDeriv << '\n';

    Polynomial<type> thirdDeriv = secondDeriv.derivative();
    cout << "Third Derivative: " << thirdDeriv << '\n';
//    cout << "Third Derivative(7.16098) = " << thirdDeriv(7.16098) << '\n';
//    cout << "Third Derivative(2.85193) = " << thirdDeriv(2.85193) << '\n';

    cout << "Polynomial degree: " << p.degree() << '\n';
    cout << "First Derivative degree: " << firstDeriv.degree() << '\n';
    cout << "Second Derivative degree: " << secondDeriv.degree() << '\n';
    cout << "Third Derivative degree: " << thirdDeriv.degree() << '\n';
}

void deflationTest()
{
    typedef double type;

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

void mullerTest()
{
    typedef double type;

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
    typedef double type;
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
    typedef double type;

    Polynomial<type> p(1.0, 2);
    p += Polynomial<type> p(-7.0, 1);
    p += Polynomial<type> p(10.0, 0);
}

