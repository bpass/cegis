template<class T>
typename LinearRegression<T>::Solution
LinearRegression<T>::
operator()(const typename LinearRegression<T>::Points& points) const
{
    T sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
    for(typename LinearRegression<T>::Points::const_iterator i = points.begin();
        i != points.end(); ++i)
    {
        const T& x = i->x();
        const T& y = i->y();
        sumX += x;
        sumY += y;
        sumXY += x*y;
        sumX2 += x*x;
    }

    const size_t size = points.size();
    const T xAverage = sumX / size;
    const T yAverage = sumY / size;

    const T a1 = (size*sumXY - sumX*sumY) / (size*sumX2 - sumX*sumX);
    const T a0 = yAverage - a1*xAverage;

    return typename LinearRegression<T>::Solution(a1, a0);
}

template<class T>
inline
LinearRegression<T>::Solution::Solution()
    : m_a1(0), m_a0(0)
{}

template<class T>
inline
LinearRegression<T>::Solution::Solution(const T& a1Init, const T& a0Init)
    : m_a1(a1Init), m_a0(a0Init)
{}

template<class T>
inline
T&
LinearRegression<T>::Solution::a1()
{
    return m_a1;
}

template<class T>
inline
const T&
LinearRegression<T>::Solution::a1() const
{
    return m_a1;
}

template<class T>
inline
T&
LinearRegression<T>::Solution::a0()
{
    return m_a0;
}

template<class T>
inline
const T&
LinearRegression<T>::Solution::a0() const
{
    return m_a0;
}

