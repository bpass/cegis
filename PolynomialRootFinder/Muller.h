#ifndef AUSTIN_MULLER_H
#define AUSTIN_MULLER_H

#include <cmath> // for std::abs and std::sqrt

#include "Polynomial.h"

template<class T>
class Muller
{
public:
    T operator()(const Polynomial<T>& p, const T& rootGuess, const T& h, 
                 const T& epsilon, const size_t& maxIterations) const;
};

#include "Muller.hpp"

#endif

