/*
 * Enumeration.h
 *
 *
 *  Created on: Jan 3, 2013
 *      Author: Martin Uhrin
 */

#ifndef SCHEMER_ENUMERATION_H
#define SCHEMER_ENUMERATION_H

// INCLUDES /////////////////////////////////////////////
#include <map>
#include <string>

#include <boost/optional.hpp>

#include "schemer/Scalar.h"

// DEFINES //////////////////////////////////////////////

// FORWARD DECLARATIONS ////////////////////////////////////

namespace schemer {

template< typename T>
  class Enumeration : public detail::Type< T>
  {
  public:
    typedef T BindingType;

    virtual
    ~Enumeration()
    {
    }

    void
    enumeration(const ::std::string & name, const T value);

    virtual bool
    valueToNode(YAML::Node & node, const T & value) const;
    virtual bool
    nodeToValue(ParseLog & parse, T & value, const YAML::Node & node) const;
    virtual Enumeration *
    clone() const;

  private:
    ::std::map< ::std::string, T> myEnumMap;
  };

}

#include "schemer/detail/Enumeration.h"

#endif /* SCHEMER_ENUMERATION_H */
