/*
 * Element.h
 *
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

#ifndef SCHEMER_ELEMENT_H
#define SCHEMER_ELEMENT_H

// INCLUDES /////////////////////////////////////////////

#include <boost/optional.hpp>

#include <yaml-cpp/yaml.h>

#include "schemer/Parse.h"

// DEFINES //////////////////////////////////////////////

// FORWARD DECLARATIONS ////////////////////////////////////

namespace schemer {

template< typename T>
  class Element
  {
  public:
    typedef typename T::BindingType BindingType;

    bool
    isRequired() const
    {
      return true;
    }

    Element *
    defaultValue(const BindingType & defaultValue);
    const ::boost::optional< BindingType> &
    getDefault() const;

    bool
    valueToNode(YAML::Node & node, const BindingType & value) const;
    bool
    nodeToValue(ParseLog & parse, BindingType & value,
        const YAML::Node & node) const;
    bool
    defaultValueToNode(YAML::Node & node) const;

  private:
    ::boost::optional< BindingType> myDefault;
    const T myType;
  };

template< typename T>
  class OptionalElement
  {
    typedef typename T::BindingType CppType;
  public:
    typedef ::boost::optional< CppType> BindingType;

    bool
    isRequired() const
    {
      return false;
    }

    OptionalElement *
    defaultValue(const CppType & defaultValue);
    const ::boost::optional< CppType> &
    getDefault() const;

    bool
    valueToNode(YAML::Node & node, const BindingType & value) const;
    bool
    nodeToValue(ParseLog & parse, BindingType & value,
        const YAML::Node & node) const;
    bool
    defaultValueToNode(YAML::Node & node) const;

  private:
    ::boost::optional< CppType> myDefault;
    const T myType;
  };

}

#include "schemer/detail/Element.h"

#endif /* SCHEMER_ELEMENT_H */
