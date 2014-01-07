/*
 * Registry.h
 *
 *  Created on: Dec 29, 2013
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#ifndef REGISTRY_H
#define REGISTRY_H

#include <map>
#include <vector>

#include <boost/type_traits/is_fundamental.hpp>

#include "schemer/Enumeration.h"
#include "schemer/List.h"
#include "schemer/Map.h"
#include "schemer/Scalar.h"

#define SCHEMER_MAP(NAME, TYPE) \
  struct NAME : public ::schemer::HeteroMap< TYPE> \
  { NAME(); }; \
  const NAME & getType(const TYPE & ) \
  { return ::schemer::getTypeInstance< NAME>(); } \
  NAME::NAME()

#define SCHEMER_HOMO_MAP(NAME) SCHEMER_HOMO_MAP_TYPED(NAME, ::schemer::String)

#define SCHEMER_HOMO_MAP_TYPED(NAME, TYPE) \
  struct NAME : public ::schemer::Map< TYPE> \
  { NAME(); }; \
  NAME::NAME()

#define SCHEMER_ENUM(NAME, TYPE) \
  struct NAME : public ::schemer::Enumeration< TYPE> \
  { NAME(); }; \
    const NAME & getType(const TYPE & ) \
  { return ::schemer::getTypeInstance< NAME>(); } \
  NAME::NAME()

namespace schemer {

template< typename T>
  const T &
  getTypeInstance()
  {
    static const T TYPE;
    return TYPE;
  }

template< typename T, bool IsCppFundamental = ::boost::is_fundamental< T>::value >
  struct TypeGetter
  {
  };

inline const Scalar< ::std::string> &
getType(const ::std::string &)
{
  return getTypeInstance< Scalar< ::std::string> >();
}

template< typename T>
  const List< Scalar< T> > &
  getType(const ::std::vector< T> &)
  {
    return getTypeInstance< List< Scalar< T> > >();
  }

template< typename T>
  const Map< Scalar< T> > &
  getType(const ::std::map< ::std::string, T> &)
  {
    return getTypeInstance< Map< Scalar< T> > >();
  }

// Use boost::is_fundamental to automatically create scalar types for fundamental
// types
template< typename T>
  struct TypeGetter<T, true>
  {
    static const Scalar< T> &
    get()
    {
      // If we know nothing else, assume it's a scalar.
      return getTypeInstance< Scalar< T> >();
    }
  };

template< typename T>
  struct TypeGetter<T, false>
  {
    static const detail::Type< T> &
    get()
    {
      // Try ADL
      return getType(T());
    }
  };

template< typename T>
  const detail::Type< T> &
  getType()
  {
    return TypeGetter<T>::get();
  }

}

#endif // REGISTRY_H
