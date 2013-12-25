/*
 * SchemaMap.h
 *
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

#ifndef SCHEMA_MAP_H
#define SCHEMA_MAP_H

// INCLUDES /////////////////////////////////////////////

#include <map>
#include <string>

#include <boost/ptr_container/ptr_map.hpp>

#include "schemer/detail/Element.h"

// DEFINES //////////////////////////////////////////////

namespace schemer {

// FORWARD DECLARATIONS ////////////////////////////////////
namespace detail {
template< class T, typename U>
  class SchemaHeteroMapEntry;
template< class T>
  class SchemaHeteroMapEntryBase;
template< class T>
  SchemaHeteroMapEntryBase< T> *
  new_clone(const SchemaHeteroMapEntryBase< T> & entry);
}

template< typename T>
  class Map : public detail::ElementBase<
      ::std::map< ::std::string, typename T::BindingType> >
  {
    // TODO: Test this class and make sure it's doing the right thing
    typedef typename T::BindingType MapSecondType;
  public:
    typedef ::std::map< ::std::string, MapSecondType> BindingType;

    Map();
    virtual
    ~Map()
    {
    }

    virtual bool
    valueToNode(YAML::Node & node, const BindingType & value,
        const bool useDefaultOnFail) const;
    virtual bool
    nodeToValue(ParseLog & parse, BindingType & value,
        const YAML::Node & node, const bool useDefaultOnFail) const;

    void
    addEntry(const ::std::string & name, const T & element);

    virtual Map *
    clone() const;

    bool
    areUnknownEntriesAllowed() const;
    void
    setAllowUnknownEntries(const bool allowUnknownEntries);

  private:
    typedef ::boost::ptr_map< ::std::string, T> EntriesMap;

    T myDefaultEntry;
    EntriesMap myEntries;
    bool myAllowUnknownEntries;
  };

template< class T>
  class HeteroMap : public detail::ElementBase< T>
  {
  public:
    typedef T BindingType;

    virtual
    ~HeteroMap()
    {
    }

    virtual bool
    valueToNode(YAML::Node & node, const BindingType & map,
        const bool useDefaultOnFail) const;
    virtual bool
    nodeToValue(ParseLog & parse, BindingType & map, const YAML::Node & node,
        const bool useDefaultOnFail) const;

    template< typename U>
      detail::SchemaHeteroMapEntry< T, U> *
      addEntry(const ::std::string & name, U (T::* const member),
          detail::ElementBase< U> * const element);
    template< typename U>
      detail::SchemaHeteroMapEntry< T, U> *
      addScalarEntry(const ::std::string & name, U (T::* const member));

    virtual HeteroMap *
    clone() const;

  private:
    typedef ::boost::ptr_map< const ::std::string,
        detail::SchemaHeteroMapEntryBase< T> > EntriesMap;

    EntriesMap myEntries;
  };

}

#include "schemer/detail/Map.h"

#endif /* SCHEMA_MAP_H */
