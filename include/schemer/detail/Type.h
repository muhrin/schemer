/*
 * Type.h
 *
 *
 *  Created on: Jan 6, 2012
 *      Author: Martin Uhrin
 */

#ifndef SCHEMER_TYPE_DETAIL_H
#define SCHEMER_TYPE_DETAIL_H

// INCLUDES /////////////////////////////////////////////

#include <boost/optional.hpp>

#include <yaml-cpp/yaml.h>

#include "schemer/Parse.h"

// DEFINES //////////////////////////////////////////////

// FORWARD DECLARATIONS ////////////////////////////////////

namespace schemer {
namespace detail {

template< typename T>
  class Type
  {
  public:
    virtual
    ~Type()
    {
    }

    virtual bool
    valueToNode(YAML::Node & node, const T & value) const = 0;
    virtual bool
    nodeToValue(ParseLog & parse, T & value, const YAML::Node & node) const = 0;

    virtual Type *
    clone() const = 0;
  };

template< typename T>
  Type< T> *
  new_clone(const Type< T> & element)
  {
    return element.clone();
  }

}
}

#endif /* SCHEMER_TYPE_DETAIL_H */
