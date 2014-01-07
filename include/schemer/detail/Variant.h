/*
 * Variant.h
 *
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

#ifndef SCHEMER_VARIANT_DETAIL_H
#define SCHEMER_VARIANT_DETAIL_H

// INCLUDES /////////////////////////////////////////////

// DEFINES //////////////////////////////////////////////

// FORWARD DECLARATIONS ////////////////////////////////////

namespace schemer {

struct _null
{
  typedef void * BindingType;
};

template< class ScalarType, class ListType, class MapType>
  class ValueToNodeVisitor : public boost::static_visitor< bool>
  {
  public:
    ValueToNodeVisitor(YAML::Node & node, const ScalarType & scalar,
        const ListType & list, const MapType & map) :
        myNode(node), myScalar(scalar), myList(list), myMap(map)
    {
    }

    bool
    operator()(const typename ScalarType::BindingType & binding) const
    {
      return myScalar.valueToNode(myNode, binding);
    }

    bool
    operator()(const typename ListType::BindingType & binding) const
    {
      return myList.valueToNode(myNode, binding);
    }

    bool
    operator()(const typename MapType::BindingType & binding) const
    {
      return myMap.valueToNode(myNode, binding);
    }

  private:
    YAML::Node & myNode;
    const ScalarType & myScalar;
    const ListType & myList;
    const MapType & myMap;
  };

template< class ScalarType, class ListType, class MapType>
  bool
  VariantScalarListMap< ScalarType, ListType, MapType>::valueToNode(
      YAML::Node & node, const BindingType & binding) const
  {
    ValueToNodeVisitor< ScalarType, ListType, MapType> valueToNodeVisitor(node,
        myScalarType, myListType, myMapType);

    return ::boost::apply_visitor(valueToNodeVisitor, binding);
  }

template< class ScalarType, class ListType, class MapType>
  bool
  VariantScalarListMap< ScalarType, ListType, MapType>::nodeToValue(
      ParseLog & parse, BindingType & binding, const YAML::Node & node) const
  {
    if(node.IsScalar())
    {
      // Make the variant contain the scalar binding type
      binding = ScalarType::BindingType();
      return myListType.nodeToValue(parse,
          ::boost::get< ScalarType::BindingType>(binding), node);
    }
    else if(node.IsSequence())
    {
      // Make the variant contain the list binding type
      binding = ListType::BindingType();
      return myListType.nodeToValue(parse,
          ::boost::get< ListType::BindingType>(binding), node);
    }
    else if(node.IsMap())
    {
      // Make the variant contain the list binding type
      binding = MapType::BindingType();
      return myMapType.nodeToValue(parse,
          ::boost::get< MapType::BindingType>(binding), node);
    }
  }

template< class ScalarType, class ListType, class MapType>
  VariantScalarListMap< ScalarType, ListType, MapType> *
  VariantScalarListMap< ScalarType, ListType, MapType>::clone() const
  {
    return new VariantScalarListMap(*this);
  }

template< class ListSchema, class MapSchema>
  bool
  VariantListMap< ListSchema, MapSchema>::valueToNode(YAML::Node & node,
      const BindingType & binding) const
  {
    ValueToNodeVisitor< _null, ListSchema, MapSchema> valueToNodeVisitor(node,
        _null(), myListType, myMapType);

    return ::boost::apply_visitor(valueToNodeVisitor, binding);
  }

template< class ListSchema, class MapSchema>
  bool
  VariantListMap< ListSchema, MapSchema>::nodeToValue(ParseLog & parse,
      BindingType & binding, const YAML::Node & node) const
  {
    if(node.IsSequence())
    {
      // Make the variant contain the list binding type
      binding = typename ListSchema::BindingType();
      return myListType.nodeToValue(parse,
          ::boost::get< typename ListSchema::BindingType>(binding), node);
    }
    else if(node.IsMap())
    {
      // Make the variant contain the list binding type
      binding = typename MapSchema::BindingType();
      return myMapType.nodeToValue(parse,
          ::boost::get< typename MapSchema::BindingType>(binding), node);
    }
    else
      return false;

    return true;
  }

template< class ListSchema, class MapSchema>
  VariantListMap< ListSchema, MapSchema> *
  VariantListMap< ListSchema, MapSchema>::clone() const
  {
    return new VariantListMap(*this);
  }

}

#endif /* SCHEMER_VARIANT_DETAIL_H */
