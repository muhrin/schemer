/*
 * SchemaScalar.h
 *
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

#ifndef SCHEMA_SCALAR_H
#define SCHEMA_SCALAR_H

// INCLUDES /////////////////////////////////////////////
#include <boost/optional.hpp>

#include "schemer/detail/Element.h"

// DEFINES //////////////////////////////////////////////

// FORWARD DECLARATIONS ////////////////////////////////////

namespace schemer {

template< typename T>
  class Scalar : public detail::ElementBase< T>
  {
  public:
    typedef T BindingType;

    // NOTE: Have to pull in any methods that we want to use directly (i.e. without this->)
    // here because of the way identifier lookup works with template base classes.
    // See: e.g. http://stackoverflow.com/questions/5286922/g-template-parameter-error
    using detail::ElementBase< T>::getDefault;
    using detail::ElementBase< T>::isRequired;

    virtual
    ~Scalar()
    {
    }

    virtual bool
    valueToNode(YAML::Node & node, const T & value,
        const bool useDefaultOnFail) const;
    virtual bool
    nodeToValue(ParseLog & parse, T & value, const YAML::Node & node,
        const bool useDefaultOnFail) const;
    virtual Scalar *
    clone() const;
  };

template< typename T>
  bool
  Scalar< T>::valueToNode(YAML::Node & node, const T & value,
      const bool useDefaultOnFail) const
  {
    // TODO: Find out how to check if transcoding failed
    node = value;
    return true;
  }

template< typename T>
  bool
  Scalar< T>::nodeToValue(ParseLog & parse, T & value,
      const YAML::Node & node, const bool useDefaultOnFail) const
  {
    if(!node.IsDefined())
    {
      if(isRequired())
        parse.logError(ParseLogErrorCode::REQUIRED_VALUE_MISSING,
            "Required scalar value missing.");
      return false;
    }
    // TODO: Eventually reinstate this conditional
    //if(!node.IsScalar())
    //{
    //  parse.logError(ParseLogErrorCode::NODE_TYPE_WRONG, "Expected scalar.");
    //  return false;
    //}

    try
    {
      // TODO: Check behaviour of as if value is invalid (i.e. does it just use the default?)
      if(useDefaultOnFail && getDefault())
        value = node.as< T>(*getDefault());
      else
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

#endif /* SCHEMA_SCALAR_H */
