/*
 * SchemaVariant.h
 *
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

#ifndef SCHEMER_VARIANT_H
#define SCHEMER_VARIANT_H

// INCLUDES /////////////////////////////////////////////
#include <boost/variant.hpp>

#include "schemer/detail/Element.h"

// DEFINES //////////////////////////////////////////////

// FORWARD DECLARATIONS ////////////////////////////////////

namespace schemer {

template< class ScalarType, class ListType, class MapType>
  class VariantScalarListMap : public detail::Type<
      ::boost::variant< typename ScalarType::BindingType,
          typename ListType::BindingType, typename MapType::BindingType> >
  {
  public:
    typedef ::boost::variant< typename ScalarType::BindingType,
        typename ListType::BindingType, typename MapType::BindingType> BindingType;

    virtual
    ~VariantScalarListMap()
    {
    }

    virtual bool
    valueToNode(YAML::Node & node, const BindingType & binding) const;
    virtual bool
    nodeToValue(ParseLog & parse, BindingType & binding,
        const YAML::Node & node) const;

    virtual VariantScalarListMap *
    clone() const;

  private:
    const ScalarType myScalarType;
    const ListType myListType;
    const MapType myMapType;
  };

template< class ListType, class MapType>
  class VariantListMap : public detail::Type<
      ::boost::variant< typename ListType::BindingType,
          typename MapType::BindingType> >
  {
  public:
    typedef ::boost::variant< typename ListType::BindingType,
        typename MapType::BindingType> BindingType;

    virtual
    ~VariantListMap()
    {
    }

    virtual bool
    valueToNode(YAML::Node & node, const BindingType & binding) const;
    virtual bool
    nodeToValue(ParseLog & parse, BindingType & binding,
        const YAML::Node & node) const;

    virtual VariantListMap *
    clone() const;

  private:
    const ListType myListType;
    const MapType myMapType;
  };

}
#include "schemer/detail/Variant.h"

#endif /* SCHEMER_VARIANT_H */