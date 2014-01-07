/*
 * SchemaMap.h
 *
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

#ifndef SCHEMER_MAP_H
#define SCHEMER_MAP_H

// INCLUDES /////////////////////////////////////////////

#include <map>
#include <string>

#include <boost/ptr_container/ptr_map.hpp>
#include <boost/scoped_ptr.hpp>

#include "schemer/Element.h"
#include "schemer/detail/Type.h"

// DEFINES //////////////////////////////////////////////

namespace schemer {

// FORWARD DECLARATIONS ////////////////////////////////////
namespace detail {
template< class T, typename U, typename V>
  class HeteroMapElement;
template< class T>
  class HeteroMapElementBase;
template< class T>
  HeteroMapElementBase< T> *
  new_clone(const HeteroMapElementBase< T> & entry);
}

template< typename EntryType>
  class Map : public detail::Type<
      ::std::map< ::std::string, typename EntryType::BindingType> >
  {
    // TODO: Test this class and make sure it's doing the right thing
    typedef typename EntryType::BindingType EntryBinding;
    typedef Element< EntryType> ElementType;
  public:
    typedef ::std::map< ::std::string, EntryBinding> BindingType;

    Map();
    virtual
    ~Map()
    {
    }

    virtual bool
    valueToNode(YAML::Node & node, const BindingType & value) const;
    virtual bool
    nodeToValue(ParseLog & parse, BindingType & value,
        const YAML::Node & node) const;

    ElementType *
    element(const ::std::string & name);

    virtual Map *
    clone() const;

    bool
    areUnknownEntriesAllowed() const;
    void
    setAllowUnknownEntries(const bool allowUnknownEntries);

  private:
    typedef ::std::map< const ::std::string, ElementType> EntriesMap;

    EntriesMap myEntries;
    bool myAllowUnknownEntries;
  };

template< class BT>
  class HeteroMap : public detail::Type< BT>
  {
  public:
    typedef BT BindingType;
    typedef detail::HeteroMapElementBase< BindingType> Entry;
    typedef ::boost::ptr_map< const ::std::string, Entry> EntriesMap;
    typedef typename EntriesMap::const_iterator EntryIterator;

    virtual
    ~HeteroMap()
    {
    }

    EntryIterator
    entriesBegin() const;
    EntryIterator
    entriesEnd() const;

    virtual bool
    valueToNode(YAML::Node & node, const BindingType & map) const;
    virtual bool
    nodeToValue(ParseLog & parse, BindingType & map,
        const YAML::Node & node) const;

    template< typename ElementType, typename MemberType>
      detail::HeteroMapElement< BindingType, ElementType, MemberType > *
      element(const ::std::string & name,
          MemberType (BindingType::* const member));

    template< typename BaseType>
      void
      extends();

    virtual HeteroMap *
    clone() const;

  private:
    EntriesMap myEntries;
  };

}

#endif /* SCHEMER_MAP_H */