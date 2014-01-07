/*
 * Scalar.h
 *
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

#ifndef SCHEMER_SCALAR_H
#define SCHEMER_SCALAR_H

// INCLUDES /////////////////////////////////////////////
#include <boost/optional.hpp>

#include "schemer/detail/Type.h"

// DEFINES //////////////////////////////////////////////

// FORWARD DECLARATIONS ////////////////////////////////////

namespace schemer {

template< typename T>
  class Scalar : public detail::Type< T>
  {
  public:
    typedef T BindingType;

    virtual
    ~Scalar()
    {
    }

    virtual bool
    valueToNode(YAML::Node & node, const T & value) const;
    virtual bool
    nodeToValue(ParseLog & parse, T & value, const YAML::Node & node) const;
    virtual Scalar *
    clone() const;
  };

template< typename T>
  bool
  Scalar< T>::valueToNode(YAML::Node & node, const T & value) const
  {
    // TODO: Find out how to check if transcoding failed
    node = value;
    return true;
  }

template< typename T>
  bool
  Scalar< T>::nodeToValue(ParseLog & parse, T & value, const YAML::Node & node) const
  {
    // TODO: Eventually reinstate this conditional
    //if(!node.IsScalar())
    //{
    //  parse.logError(ParseLogErrorCode::NODE_TYPE_WRONG, "Expected scalar.");
    //  return false;
    //}

    try
    {
      value = node.as< T>();
    }
    catch(const YAML::Exception & /*e*/)
    {
      parse.logError(ParseLogErrorCode::TYPE_CONVERSION_FAILED,
          "Value is not a valid scalar of the right type.");
      return false;
    }
    return true;
  }

template< typename T>
  Scalar< T> *
  Scalar< T>::clone() const
  {
    return new Scalar(*this);
  }
}

#endif /* SCHEMER_SCALAR_H */
