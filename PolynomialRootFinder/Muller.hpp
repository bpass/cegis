/**
 * @file Muller.hpp
 * @author Austin Hartman
 * $Id: Muller.hpp,v 1.5 2005/04/22 16:54:36 ahartman Exp $
 */

template<class T>
T
Muller<T>::operator()(const Polynomial<T>& p, const T& rootGuess, const T& h, 
                      const T& epsilon, const size_t& maxIterations) const
{
    T xr = rootGuess;
    T x2 = xr;
    T x1 = xr + h * xr;
    T x0 = xr - h * xr;

//    cerr << __FILE__ << ':' << __LINE__ << '\n';
//    cerr << "\txr = " << xr << '\n';
//    cerr << "\tx2 = " << x2 << '\n';
//    cerr << "\tx1 = " << x1 << '\n';
//    cerr << "\tx0 = " << x0 << '\n';
    
    size_t iterations = 0;

    T dxr;
    do
    {
        ++iterations;

        const T h0 = x1 - x0;
        const T h1 = x2 - x1;

        const T d0 = (p(x1) - p(x0)) / h0;
        const T d1 = (p(x2) - p(x1)) / h1;

        const T a = (d1 - d0) / (h1 + h0);
        const T b = a * h1 + d1;
        const T c = p(x2);

        const T radInside = b*b - static_cast<T>(4)*a*c;
        const T rad = std::sqrt(radInside);

//        cerr << __FILE__ << ':' << __LINE__ << '\n';
//        cerr << "\th0 = " << h0 << '\n';
//        cerr << "\th1 = " << h1 << '\n';
//        cerr << "\td0 = " << d0 << '\n';
//        cerr << "\t\tp(x0) = " << p(x0) << '\n';
//        cerr << "\t\tp(x1) = " << p(x1) << '\n';
//        cerr << "\t\tp(x2) = " << p(x2) << '\n';
//        cerr << "\td1 = " << d1 << '\n';
//        cerr << "\ta = " << a << '\n';
//        cerr << "\tb = " << b << '\n';
//        cerr << "\tc = " << c << '\n';
//        cerr << "\tradInside = " << radInside << '\n';
//        cerr << "\trad = " << rad << '\n';

        T den;
        if(b >= static_cast<T>(0))
        {
            den = b + rad;
        }
        else
        {
            den = b - rad;
        }

        dxr = static_cast<T>(-2)*c / den;
        xr = x2 + dxr;

        x0 = x1;
        x1 = x2;
        x2 = xr;

//        cerr << __FILE__ << ':' << __LINE__ << '\n';
//        cerr << "\tden = " << den << '\n';
//        cerr << "\tdxr = " << dxr << '\n';
//        cerr << "\txr = " << xr << '\n';
//        cerr << "\tx0 = " << x0 << '\n';
//        cerr << "\tx1 = " << x1 << '\n';
//        cerr << "\tx2 = " << x2 << '\n';
    } while( (std::abs(dxr) > epsilon * xr) && (iterations < maxIterations) );

//    cerr << __FILE__ << ':' << __LINE__ << ": number of iterations = "
//         << iterations << '\n';
    
    return xr;
}

