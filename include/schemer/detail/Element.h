/*
 * Element.h
 *
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

#ifndef SCHEMER_ELEMENT_DETAIL_H
#define SCHEMER_ELEMENT_DETAIL_H

// INCLUDES /////////////////////////////////////////////

#include <boost/optional.hpp>

#include <yaml-cpp/yaml.h>

#include "schemer/Parse.h"

// DEFINES //////////////////////////////////////////////

// FORWARD DECLARATIONS ////////////////////////////////////

namespace schemer {

template< typename T>
  Element< T> *
  Element< T>::defaultValue(const BindingType & defaultValue)
  {
    myDefault = defaultValue;
    return this;
  }

template< typename T>
  const ::boost::optional< typename T::BindingType> &
  Element< T>::getDefault() const
  {
    return myDefault;
  }

template< typename T>
  bool
  Element< T>::valueToNode(YAML::Node & node, const BindingType & value) const
  {
    bool succeeded = myType.valueToNode(node, value);
    if(!succeeded && myDefault)
    {
      node = *myDefault;
      succeeded = true;
    }

    return succeeded;
  }

template< typename T>
  bool
  Element< T>::nodeToValue(ParseLog & parse, BindingType & value,
      const YAML::Node & node) const
  {
    if(!node.IsDefined())
    {
      if(isRequired())
        parse.logError(ParseLogErrorCode::REQUIRED_VALUE_MISSING,
            "Required scalar value missing.");
      return false;
    }

    bool succeeded = myType.nodeToValue(parse, value, node);
    if(!succeeded && myDefault)
    {
      value = *myDefault;
      succeeded = true;
    }
    return succeeded;
  }

template< typename T>
  bool
  Element< T>::defaultValueToNode(YAML::Node & node) const
  {
    if(!myDefault)
      return false;

    return myType.valueToNode(node, *myDefault);
  }

template< typename T>
  OptionalElement< T> *
  OptionalElement< T>::defaultValue(const CppType & value)
  {
    myDefault = value;
    return this;
  }

template< typename T>
  const ::boost::optional< typename OptionalElement< T>::CppType> &
  OptionalElement< T>::getDefault() const
  {
    return myDefault;
  }

template< typename T>
  bool
  OptionalElement< T>::valueToNode(YAML::Node & node,
      const BindingType & value) const
  {
    if(!value)
      return true;

    bool succeeded = myType.valueToNode(node, *value);
    if(!succeeded && myDefault)
    {
      node = *myDefault;
      succeeded = true;
    }

    return succeeded;
  }

template< typename T>
  bool
  OptionalElement< T>::nodeToValue(ParseLog & parse, BindingType & value,
      const YAML::Node & node) const
  {
    if(!node.IsDefined())
    {
      if(isRequired())
        parse.logError(ParseLogErrorCode::REQUIRED_VALUE_MISSING,
            "Required scalar value missing.");
      return false;
    }

    CppType nodeValue;
    bool succeeded = myType.nodeToValue(parse, nodeValue, node);
    if(!succeeded && myDefault)
    {
      value = *myDefault;
      succeeded = true;
    }
    else
      value = nodeValue;
    return succeeded;
  }

template< typename T>
  bool
  OptionalElement< T>::defaultValueToNode(YAML::Node & node) const
  {
    if(!myDefault)
      return false;

    return myType.valueToNode(node, *myDefault);
  }

template< typename T>
  Element< T> *
  new_clone(const Element< T> & element)
  {
    return element.clone();
  }

}

#endif /* SCHEMER_ELEMENT_DETAIL_H */
