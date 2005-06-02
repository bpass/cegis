#ifndef AUSTIN_LINEARREGRESSION_H
#define AUSTIN_LINEARREGRESSION_H

#include <vector>

#include "Point2D.h"

template<class T>
class LinearRegression
{
public:
    typedef std::vector<Point2D<T> > Points;
    class Solution;
    Solution operator()(const Points& points) const;

    class Solution
    {
    public:
        Solution();
        Solution(const T& a1Init, const T& a0Init);

        T& a1();
        const T& a1() const;
        T& a0();
        const T& a0() const;
    private:
        T m_a1, m_a0;
    };
};

#include "LinearRegression.hpp"

#endif

