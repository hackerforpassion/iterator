#ifndef BOOST_ITERATOR_ADAPTORS_HPP
#define BOOST_ITERATOR_ADAPTORS_HPP

#include "boost/utility.hpp" // for prior

namespace boost {

template <class Derived>
struct iterator_comparisons { };

template <class D1, class D2>
inline bool operator==(const iterator_comparisons<D1>& xb,
                       const iterator_comparisons<D2>& yb)
{
    const D1& x = static_cast<const D1&>(xb);
    const D2& y = static_cast<const D2&>(yb);
    return x.equal(y);
}

template <class D1, class D2>
inline bool operator!=(const iterator_comparisons<D1>& xb,
                       const iterator_comparisons<D2>& yb)
{
    const D1& x = static_cast<const D1&>(xb);
    const D2& y = static_cast<const D2&>(yb);
    return !x.equal(y);
}

template <class D1, class D2>
inline bool operator<(const iterator_comparisons<D1>& xb,
                      const iterator_comparisons<D2>& yb)
{
    const D1& x = static_cast<const D1&>(xb);
    const D2& y = static_cast<const D2&>(yb);
    return y.distance(x) < 0;
}

template <class D1, class D2>
inline bool operator>(const iterator_comparisons<D1>& xb,
                      const iterator_comparisons<D2>& yb)
{
    const D1& x = static_cast<const D1&>(xb);
    const D2& y = static_cast<const D2&>(yb);
    return y.distance(x) > 0;
}

template <class D1, class D2>
inline bool operator>=(const iterator_comparisons<D1>& xb,
                       const iterator_comparisons<D2>& yb)
{
    const D1& x = static_cast<const D1&>(xb);
    const D2& y = static_cast<const D2&>(yb);
    return y.distance(x) >= 0;
}

template <class D1, class D2>
inline bool operator<=(const iterator_comparisons<D1>& xb,
                       const iterator_comparisons<D2>& yb)
{
    const D1& x = static_cast<const D1&>(xb);
    const D2& y = static_cast<const D2&>(yb);
    return y.distance(x) <= 0;
}


template <class Derived, class D>
struct iterator_arith : public iterator_comparisons<Derived> { };

template <class Derived, class D>
D operator+(iterator_arith<Derived, D> i, D n) { return i += n; }

template <class Derived, class D>
D operator+(D n, iterator_arith<Derived, D> i) { return i += n; }

template <class Derived, class D>
D operator-(const iterator_arith<Derived, D>& i,
            const iterator_arith<Derived, D>& j)
  { return static_cast<const Derived&>(j).distance(static_cast<const Derived&>(i)); }


template <class Derived, 
  class V, class R, class P, class C, class D>
class iterator_adaptor : public iterator_arith<Derived, D>
{
public:
  typedef V value_type;
  typedef R reference;
  typedef P pointer;
  typedef C iterator_category;
  typedef D difference_type;

  reference operator*() const
    { return derived().dereference(); }
  //operator->() const { return detail::operator_arrow(*this, iterator_category()); }
  reference operator[](difference_type n) const
    { return *(*this + n); }
  Derived& operator++()
    { derived().increment(); return derived(); }
  Derived operator++(int)
    { Derived tmp(derived()); ++*this; return tmp; }
  Derived& operator--()
    { derived().decrement(); return derived(); }
  Derived operator--(int)
    { Derived tmp(derived()); --*this; return tmp; }
  Derived& operator+=(difference_type n)
    { derived().advance(n); return derived(); }
  Derived& operator-=(difference_type n)
    { derived().advance(-n); return derived(); }
  Derived operator-(difference_type x) const
    { Derived result(derived()); return result -= x; }
private:
  Derived& derived() { return static_cast<Derived&>(*this); }
  const Derived& derived() const { return static_cast<const Derived&>(*this); }
};



template <class Base, 
  class V, class R, class P, class C, class D>
class reverse_iterator : public iterator_adaptor<reverse_iterator<Base, V, R, P, C, D>, V, R, P, C, D>
{
  typedef iterator_adaptor<reverse_iterator<Base, V, R, P, C, D>, V, R, P, C, D> super;
  friend class super;
  // hmm, I don't like the next two lines
  template <class Der> friend struct iterator_comparisons;
  template <class Der, class Dist> friend struct iterator_arith;
public:
  reverse_iterator(const Base& x) : m_base(x) { }

  reverse_iterator() { }

  template <class B2, class V2, class R2, class P2, class C2, class D2>
  reverse_iterator(const reverse_iterator<B2,V2,R2,P2,C2,D2>& y)
    : m_base(y.m_base) { }
  //private:  hmm, the above friend decls aren't doing the job.
  typename super::reference dereference() const { *boost::prior(m_base); }
  void increment() { --m_base; }
  void decrement() { ++m_base; }
  void advance(typename super::difference_type n)
    { m_base -= n; }

  template <class B2, class V2, class R2, class P2, class C2, class D2>  
  typename super::difference_type
  distance(const reverse_iterator<B2,V2,R2,P2,C2,D2>& y) const
    { return m_base - y.m_base; }

  Base m_base;
};




} // namespace boost


#endif // BOOST_ITERATOR_ADAPTORS_HPP
