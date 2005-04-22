/**
 * @file Polynomial.hpp
 * @author Austin Hartman
 * $Id: Polynomial.hpp,v 1.5 2005/04/22 16:54:36 ahartman Exp $
 */

/**************************************
 ******** Polynomial functions ********
 **************************************/
template<class T>
const char Polynomial<T>::letter = 'x';

template<class T>
const T Polynomial<T>::constZeroCoefficient = 0;

template<class T>
Polynomial<T>::Polynomial()
{}

template<class T>
Polynomial<T>::Polynomial(const T& coefficient, const size_t& power)
{
    if(coefficient != static_cast<T>(0))
    {
        terms.push_back(typename Polynomial<T>::Term(coefficient, power));
    }
}

template<class T>
Polynomial<T>&
Polynomial<T>::operator+=(const Polynomial<T>& rhs)
{
    std::vector<Polynomial<T>::Term> newTerms;
    newTerms.reserve(terms.size() + rhs.terms.size());

    // add the terms from rhs to this new polynomial, making sure to add
    // the coefficients whenever the powers are the same
    typename std::vector<Polynomial<T>::Term>::const_iterator 
        i = terms.begin(), 
        j = rhs.terms.begin();
    while(i != terms.end() && j != rhs.terms.end())
    {
        // if the powers are the same, add the sum of their coefficients
        // to the polynomial, assuming that they do not cancel each other
        // out
        if(i->power() == j->power())
        {
            const T sum = i->coefficient() + j->coefficient();
            if(sum != static_cast<T>(0))
            {
                newTerms.push_back(Term(sum, i->power()));
            }
            ++i;
            ++j;
        }
        else if(i->power() < j->power())
        {
            newTerms.push_back(*i);
            ++i;
        }
        else
        {
            newTerms.push_back(*j);
            ++j;
        }
    }
    
    // add all the remaining terms from the either of the two previous
    // polynomials
    while(i != terms.end())
    {
        newTerms.push_back(*i);
        ++i;
    }

    while(j != rhs.terms.end())
    {
        newTerms.push_back(*j);
        ++j;
    }

    terms.swap(newTerms);

    return *this;
}

template<class T>
Polynomial<T>
operator+(const Polynomial<T>& lhs, const Polynomial<T>& rhs)
{
    Polynomial<T> rv(lhs);
    rv += rhs;
    return rv;
}

template<class T>
Polynomial<T>
Polynomial<T>::operator-() const
{
    Polynomial<T> rv;
    for(typename std::vector<Term>::const_iterator i = rv.terms.begin();
        i != rv.terms.end(); ++i)
    {
        i->coefficient() = -(i->coefficient());
    }
    return rv;
}

template<class T>
Polynomial<T>&
Polynomial<T>::operator-=(const Polynomial<T>& rhs)
{
    *this += -rhs;
    return *this;
}

template<class T>
Polynomial<T>
operator-(const Polynomial<T>& lhs, const Polynomial<T>& rhs)
{
    Polynomial<T> rv(lhs);
    rv += -rhs;
    return rv;
}

template<class T>
const T&
Polynomial<T>::getCoefficient(const size_t& power) const
{
    std::pair<typename std::vector<Polynomial<T>::Term>::const_iterator,
              typename std::vector<Polynomial<T>::Term>::const_iterator>
        range = std::equal_range(terms.begin(), terms.end(), 
                                 typename Polynomial<T>::Term(0, power),
                                 typename Polynomial<T>::LowerPower());

    if(range.first != terms.end())
    {
        return range.first->coefficient();
    }
    else
    {
        return Polynomial<T>::constZeroCoefficient;
    }
}

template<class T>
size_t
Polynomial<T>::degree() const
{
    if(!terms.empty())
    {
        return (--terms.end())->power();
    }
    else return 0;
}

template<class T>
Polynomial<T>
Polynomial<T>::deflate(const T& root) const
{
    // set the new polynomial rv to contain as many terms as this polynomial
    // minus one.  rv will also contain terms with coefficients equal to 0
    // to make the polynomial deflation algorithm much easier to implement.
    // These terms will be removed at the end of this function.
    Polynomial<T> rv;
    const size_t rvSize = degree();
    rv.terms.reserve(rvSize);

    typename std::vector<Polynomial<T>::Term>::const_iterator iter = 
        terms.begin();
    for(size_t j = 0; j < rvSize; ++j)
    {
        if(iter->power() == j)
        {
            rv.terms.push_back(*iter);
            ++iter;
        }
        else
        {
            rv.terms.push_back(typename Polynomial<T>::Term(0, j));
        }
    }
    // rv is now set up
    
    // do the deflation algorithm
    T r = (--terms.end())->coefficient();
    for(typename std::vector<Polynomial<T>::Term>::reverse_iterator i = 
            rv.terms.rbegin();
        i != rv.terms.rend(); ++i)
    {
        T s = i->coefficient();
        i->coefficient() = r;
        r = s + r * root;
    }

    // remove the terms with coefficients equal to 0
    rv.removeZeroCoefficients();

    return rv;
}

template<class T>
Polynomial<T>
Polynomial<T>::derivative() const
{
    // the derivative will have at most the same number of terms as
    // its parent
    Polynomial<T> rv;
    rv.terms.reserve(terms.size()); 

    typename std::vector<Polynomial<T>::Term>::const_iterator i = terms.begin();
    
    // the check for whether the power equals 0 only needs to be done
    // for the first term, because the terms are stored in order of increasing
    // powers
    if(i != terms.end())
    {
        if(i->power() != 0)
        {
            rv.terms.push_back(
                typename Polynomial<T>::Term(i->coefficient() * 
                                                 static_cast<T>(i->power()), 
                                             i->power() - 1));
        }
        ++i;
    }

    // now continue with the rest of the terms
    for(; i != terms.end(); ++i)
    {
        rv.terms.push_back(Term(i->coefficient() * static_cast<T>(i->power()), 
                                i->power() - 1));
    }

    return rv;
}

template<class T>
T
Polynomial<T>::operator()(const T& x) const
{
    T rv(0);
    for(typename std::vector<Polynomial<T>::Term>::const_reverse_iterator i = 
            terms.rbegin(); 
        i != terms.rend(); ++i)
    {
        rv = rv * x + i->coefficient();

        // the code from below is used because terms with coefficients 
        // equal to 0 are not stored
        size_t pow = i->power() - 1;
        typename std::vector<Polynomial<T>::Term>::
            const_reverse_iterator j(i);
        ++j;
        if(j != terms.rend())
        {
            while(pow > j->power())
            {
                rv *= x;
                --pow;
            }
        }
    }
    return rv;
}

template<class T>
void
Polynomial<T>::removeZeroCoefficients()
{
    terms.erase(
         std::remove_if(terms.begin(), terms.end(), 
                        typename Polynomial<T>::ZeroCoefficient()), 
         terms.end());
}



template<class T>
ostream&
operator<<(ostream& os, const Polynomial<T>& p)
{
    typename std::vector<typename Polynomial<T>::Term>::const_reverse_iterator i = 
        p.terms.rbegin();
    if(i != p.terms.rend())
    {
        os << i->coefficient() << '*' << p.letter << '^' << i->power();
        ++i;
        while(i != p.terms.rend())
        {
            os << ' ';
            if(i->coefficient() >= static_cast<T>(0))
            {
                os << "+ " << i->coefficient();
            }
            else
            {
                os << "- " << -(i->coefficient());
            }
            os << '*' << p.letter << '^' << i->power();
            ++i;
        }
    }
    return os;
}

template<class T>
ofstream&
operator<<(ofstream& ofs, const Polynomial<T>& p)
{
    ofs << p.terms.size() << '\n';
    for(typename std::vector<typename Polynomial<T>::Term>::const_iterator i = 
            p.terms.begin(); 
        i != p.terms.end(); 
        ++i)
    {
        ofs << i->coefficient() << ' ' << i->power() << '\n';
    }
    return ofs;
}

template<class T>
istream&
operator>>(istream& is, Polynomial<T>& p)
{
    // clear all the previously existing terms of the polynomial
    p.terms.clear();

    // read in the number of terms of the polynomial to follow
    size_t size;
    is >> size;

    // make sure there is enough capacity in the vector to hold this size,
    // so that reallocations do not have to be done
    p.terms.reserve(size);

    // read in the terms of the polynomial and add them to the polynomial
    // the terms must be ordered so the powers are in increasing order
    T coeff;
    size_t pow;
    for(size_t i = 0; i < size; ++i)
    {
        is >> coeff >> pow;
        p.terms.push_back(typename Polynomial<T>::Term(coeff, pow));
    }

    // Remove the zero coefficients
    p.removeZeroCoefficients();

    // sort the polynomial so it's in the proper format
    std::sort(p.terms.begin(), p.terms.end(), 
              typename Polynomial<T>::LowerPower()); 
    
    return is;
}



/**************************************
 ********* Term functions *************
 **************************************/
template<class T>
inline
Polynomial<T>::Term::Term(const T& coeff, const size_t& pow)
    : power_(pow), coefficient_(coeff)
{}

template<class T>
inline
const size_t&
Polynomial<T>::Term::power() const
{
    return power_;
}

template<class T>
inline
T&
Polynomial<T>::Term::coefficient()
{
    return coefficient_;
}

template<class T>
inline
const T&
Polynomial<T>::Term::coefficient() const
{
    return coefficient_;
}



/**************************************
 ***** ZeroCoefficient functions ******
 **************************************/
template<class T>
inline
bool
Polynomial<T>::ZeroCoefficient::
operator()(const typename Polynomial<T>::Term& t) const
{
    return t.coefficient() == static_cast<T>(0);
}



/**************************************
 ******* LowerPower functions *********
 **************************************/
template<class T>
inline
bool
Polynomial<T>::LowerPower::
operator()(const typename Polynomial<T>::Term& t1,
           const typename Polynomial<T>::Term& t2) const
{
    return t1.power() < t2.power();
}



/**************************************
 ******* EqualPowers functions ********
 **************************************/
template<class T>
inline
bool
Polynomial<T>::EqualPowers::
operator()(const typename Polynomial<T>::Term& t1,
           const typename Polynomial<T>::Term& t2) const
{
    return t1.power() == t2.power();
}

