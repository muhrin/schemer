/*
 * SchemaElementBase.h
 *
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

#ifndef ELEMENT_BASE_H
#define ELEMENT_BASE_H

// INCLUDES /////////////////////////////////////////////

#include <boost/optional.hpp>

#include <yaml-cpp/yaml.h>

#include "schemer/Parse.h"

// DEFINES //////////////////////////////////////////////

// FORWARD DECLARATIONS ////////////////////////////////////

namespace schemer {
namespace detail {

template< typename T>
  class ElementBase
  {
  public:
    ElementBase();
    ElementBase(const ElementBase & toCopy);
    virtual
    ~ElementBase()
    {
    }

    ElementBase *
    required();
    bool
    isRequired() const;

    ElementBase *
    defaultValue(const T & defaultValue);
    const ::boost::optional< T>
    getDefault() const;

    virtual bool
    valueToNode(YAML::Node & node, const T & value,
        const bool useDefaultOnFail) const = 0;
    virtual bool
    nodeToValue(ParseLog & parse, T & value, const YAML::Node & node,
        const bool useDefaultOnFail) const = 0;
    bool
    defaultValueToNode(YAML::Node & node) const;

    virtual ElementBase *
    clone() const = 0;

  private:
    ::boost::optional< T> myDefault;
    bool myRequired;
  };

template< typename T>
  ElementBase< T>::ElementBase() :
      myRequired(false)
  {
  }

template< typename T>
  ElementBase< T>::ElementBase(const ElementBase & toCopy) :
      myDefault(toCopy.myDefault), myRequired(toCopy.myRequired)
  {
  }

template< typename T>
  ElementBase< T> *
  ElementBase< T>::required()
  {
    myRequired = true;
    return this;
  }

template< typename T>
  bool
  ElementBase< T>::isRequired() const
  {
    return myRequired;
  }

template< typename T>
  ElementBase< T> *
  ElementBase< T>::defaultValue(const T & defaultValue)
  {
    myDefault = defaultValue;
    return this;
  }

template< typename T>
  const ::boost::optional< T>
  ElementBase< T>::getDefault() const
  {
    return myDefault;
  }

template< typename T>
  bool
  ElementBase< T>::defaultValueToNode(YAML::Node & node) const
  {
    if(!myDefault)
      return false;

    return valueToNode(node, *myDefault, false);
  }

template< typename T>
  class ElementBase< ::boost::optional< T> >
  {
  public:
  ElementBase() {}
  ElementBase(const ElementBase & toCopy):
      myDefault(toCopy.myDefault)
    {
    }
    virtual
    ~ElementBase()
    {
    }

    bool
    isRequired() const
    {
      return false;
    }

    ElementBase *
    defaultValue(const T & defaultValue)
    {
      myDefault.reset(defaultValue);
      return this;
    }
    const ::boost::optional< T>
    getDefault() const
    {
      return myDefault;
    }

    virtual bool
    valueToNode(YAML::Node & node, const ::boost::optional< T> & value,
        const bool useDefaultOnFail) const = 0;
    virtual bool
    nodeToValue(ParseLog & parse, ::boost::optional< T> & value,
        const YAML::Node & node, const bool useDefaultOnFail) const = 0;
    bool
    defaultValueToNode(YAML::Node & node) const
    {
      if(!myDefault)
        return false;
      node = *myDefault;
      return true;
    }

    virtual ElementBase *
    clone() const = 0;

  private:
    ::boost::optional< T> myDefault;
  };

}
}

#endif /* SCHEMA_ELEMENT_BASE_H */
