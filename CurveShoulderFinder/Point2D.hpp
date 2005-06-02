template<class T>
inline
Point2D<T>::Point2D()
{}

template<class T>
inline
Point2D<T>::Point2D(const T& x, const T& y)
    : m_x(x), m_y(y)
{}

template<class T>
inline
T&
Point2D<T>::x()
{
    return m_x;
}

template<class T>
inline
const T&
Point2D<T>::x() const
{
    return m_x;
}

template<class T>
inline
T&
Point2D<T>::y()
{
    return m_y;
}

template<class T>
inline
const T&
Point2D<T>::y() const
{
    return m_y;
}



template<class T>
std::ostream&
operator<<(std::ostream& os, const Point2D<T>& p)
{
    os << '(' << p.x() << ", " << p.y() << ')';
    return os;
}

template<class T>
std::ofstream&
operator<<(std::ofstream& ofs, const Point2D<T>& p)
{
    ofs << p.x() << ' ' << p.y();
    return ofs;
}

template<class T>
std::istream&
operator>>(std::istream& is, Point2D<T>& p)
{
    is >> p.x() >> p.y();
    return is;
}

