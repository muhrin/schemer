/*
 * SchemaVariant.h
 *
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

#ifndef SCHEMA_VARIANT_H
#define SCHEMA_VARIANT_H

// INCLUDES /////////////////////////////////////////////
#include <boost/variant.hpp>

#include "schemer/detail/Element.h"

// DEFINES //////////////////////////////////////////////

// FORWARD DECLARATIONS ////////////////////////////////////

namespace schemer {

template <class ScalarSchema, class ListSchema, class MapSchema>
class SchemaScalarListMap :
  public detail::ElementBase<
    ::boost::variant<
      typename ScalarSchema::BindingType,
      typename ListSchema::BindingType,
      typename MapSchema::BindingType>
  >
{
public:
  typedef ::boost::variant<
    typename ScalarSchema::BindingType,
    typename ListSchema::BindingType,
    typename MapSchema::BindingType
  > BindingType;

  SchemaScalarListMap(
    const ScalarSchema & scalarSchema,
    const ListSchema & listSchema,
    const MapSchema & mapSchema);
  SchemaScalarListMap(const SchemaScalarListMap & toCopy);
  virtual ~SchemaScalarListMap()
  {
  }

  // NOTE: Have to pull in any methods that we want to use directly (i.e. without this->)
  // here because of the way identifier lookup works with template base classes.
  // See: e.g. http://stackoverflow.com/questions/5286922/g-template-parameter-error
  using detail::ElementBase<BindingType>::getDefault;
  using detail::ElementBase<BindingType>::isRequired;

  virtual bool valueToNode(YAML::Node & node, const BindingType & binding, const bool useDefaultOnFail) const;
  virtual bool nodeToValue(ParseLog & parse, BindingType & binding, const YAML::Node & node, const bool useDefaultOnFail) const;

  virtual SchemaScalarListMap * clone() const;

private:
  const ScalarSchema myScalarSchema;
  const ListSchema myListSchema;
  const MapSchema myMapSchema;
};

template <class ListSchema, class MapSchema>
class SchemaListMap :
  public detail::ElementBase<
    ::boost::variant<
      typename ListSchema::BindingType,
      typename MapSchema::BindingType>
  >
{
public:
  typedef ::boost::variant<
    typename ListSchema::BindingType,
    typename MapSchema::BindingType
  > BindingType;

  SchemaListMap();
  SchemaListMap(
    const ListSchema & listSchema,
    const MapSchema & mapSchema);
  SchemaListMap(const ListSchema & listSchema);
  SchemaListMap(const MapSchema & mapSchema);
  SchemaListMap(const SchemaListMap & toCopy);
  virtual ~SchemaListMap()
  {
  }

  // NOTE: Have to pull in any methods that we want to use directly (i.e. without this->)
  // here because of the way identifier lookup works with template base classes.
  // See: e.g. http://stackoverflow.com/questions/5286922/g-template-parameter-error
  using detail::ElementBase<BindingType>::getDefault;
  using detail::ElementBase<BindingType>::isRequired;

  virtual bool valueToNode(YAML::Node & node, const BindingType & binding, const bool useDefaultOnFail) const;
  virtual bool nodeToValue(ParseLog & parse, BindingType & binding, const YAML::Node & node, const bool useDefaultOnFail) const;

  virtual SchemaListMap * clone() const;

private:
  const ListSchema myListSchema;
  const MapSchema myMapSchema;
};


}
#include "schemer/detail/Variant.h"

#endif /* SCHEMA_VARIANT_H */
