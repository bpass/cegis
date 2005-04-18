template<class T>
T
Muller<T>::operator()(const Polynomial<T>& p, const T& rootGuess, const T& h, 
                      const T& epsilon, const size_t& maxIterations) const
{
    T xr = rootGuess;
    T x2 = xr;
    T x1 = xr + h * xr;
    T x0 = xr - h * xr;

    size_t iter = 0;

    T dxr;
    do
    {
        ++iter;

        T h0 = x1 - x0;
        T h1 = x2 - x1;

        T d0 = (p(x1) - p(x0)) / h0;
        T d1 = (p(x2) - p(x1)) / h1;

        T a = (d1 - d0) / (h1 + h0);
        T b = a * h1 + d1;
        T c = p(x2);

        T rad = std::sqrt(b*b - 4*a*c);

        T den;
        if(b >= 0)
        {
            den = b + rad;
        }
        else
        {
            den = b - rad;
        }

        dxr = -2*c / den;
        xr = x2 + dxr;

        x0 = x1;
        x1 = x2;
        x2 = xr;
    } while( (std::abs(dxr) > epsilon * xr) && (iter < maxIterations) );

    cerr << __FILE__ << ':' << __LINE__ << ": number of iterations = "
         << iter << '\n';
    
    return xr;
}

