/*
 * Map.h
 *
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

#ifndef SCHEMER_MAP_DETAIL_H
#define SCHEMER_MAP_DETAIL_H

// INCLUDES /////////////////////////////////////////////
#include <boost/foreach.hpp>
#include <boost/optional.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/scoped_ptr.hpp>

#include "schemer/Element.h"
#include "schemer/Registry.h"

// DEFINES //////////////////////////////////////////////

// FORWARD DECLARATIONS ////////////////////////////////////

namespace schemer {
namespace detail {

template< class T>
  class HeteroMapElementBase
  {
  public:
    virtual
    ~HeteroMapElementBase()
    {
    }

    virtual bool
    valueToNode(YAML::Node & node, const T & map) const = 0;
    virtual bool
    nodeToValue(ParseLog & parse, T & map, const YAML::Node & node) const = 0;
    virtual bool
    defaultValueToMap(T & map) const = 0;

    virtual HeteroMapElementBase *
    clone() const = 0;
  };

template< class T>
  HeteroMapElementBase< T> *
  new_clone(const HeteroMapElementBase< T> & entry)
  {
    return entry.clone();
  }

template< class MapBindingType, typename T, typename MapBindingMemberType>
  class HeteroMapElement : public HeteroMapElementBase< MapBindingType>
  {
    typedef typename T::BindingType BindingType;
    typedef MapBindingMemberType (MapBindingType::* const Member);

  public:
    HeteroMapElement(Member member) :
        myMember(member)
    {
    }
    virtual
    ~HeteroMapElement()
    {
    }

    HeteroMapElement *
    defaultValue(const BindingType & defaultValue)
    {
      myDefault = defaultValue;
      return this;
    }

    // From HeteroMapElementBase ///////////////
    virtual bool
    valueToNode(YAML::Node & node, const MapBindingType & map) const
    {
      const MapBindingMemberType value = map.*myMember;
      return myType.valueToNode(node, value);
    }
    virtual bool
    nodeToValue(ParseLog & parse, MapBindingType & map,
        const YAML::Node & node) const
    {
      if(!node.IsDefined())
      {
        // Check for a default
        if(defaultValueToMap(map))
          return true;

        parse.logError(ParseLogErrorCode::REQUIRED_VALUE_MISSING,
            "Required map entry missing.");

        return false;
      }

      MapBindingMemberType value;
      if(!myType.nodeToValue(parse, value, node))
        return false;

      map.*myMember = value;
      return true;
    }
    virtual bool
    defaultValueToMap(MapBindingType & map) const
    {
      if(!myDefault)
        return false;

      map.*myMember = *myDefault;
      return true;
    }
    virtual HeteroMapElement *
    clone() const
    {
      return new HeteroMapElement(*this);
    }
    // End from HeteroMapElementBase /////////////

  private:
    T myType;
    Member myMember;
    ::boost::optional< MapBindingMemberType> myDefault;
  };

template< class MapBindingType, typename T, typename MapBindingMemberType>
  class HeteroMapElement< MapBindingType, T,
      ::boost::optional< MapBindingMemberType> > : public HeteroMapElementBase<
      MapBindingType>
  {
    typedef typename ::boost::optional< MapBindingMemberType> BindingType;
    typedef BindingType (MapBindingType::* const Member);

  public:
    HeteroMapElement(Member member) :
        myMember(member)
    {
    }
    virtual
    ~HeteroMapElement()
    {
    }

    HeteroMapElement *
    defaultValue(const BindingType & defaultValue)
    {
      if(!myDefault)
        return false;

      myDefault = defaultValue;
      return this;
    }

    // From HeteroMapElementBase ///////////////
    virtual bool
    valueToNode(YAML::Node & node, const MapBindingType & map) const
    {
      if(!(map.*myMember))
      {
        node = YAML::Node(); // Set to null node
        return true;
      }

      const MapBindingMemberType value = *(map.*myMember);
      return myType.valueToNode(node, value);
    }
    virtual bool
    nodeToValue(ParseLog & parse, MapBindingType & map,
        const YAML::Node & node) const
    {
      if(!node.IsDefined())
      {
        // Check for a default
        defaultValueToMap(map);
        return true;
      }

      MapBindingMemberType value;
      if(!myType.nodeToValue(parse, value, node))
        return false;

      map.*myMember = value;
      return true;
    }
    virtual bool
    defaultValueToMap(MapBindingType & map) const
    {
      map.*myMember = myDefault;
      return true;
    }
    virtual HeteroMapElement *
    clone() const
    {
      return new HeteroMapElement(*this);
    }
    // End from HeteroMapElementBase /////////////

  private:
    T myType;
    Member myMember;
    ::boost::optional< MapBindingMemberType> myDefault;
  };

template< class DerivedBinding, typename BaseBinding>
  class HeteroMapBaseEntryWrapper : public HeteroMapElementBase< DerivedBinding>
  {
  public:
    explicit
    HeteroMapBaseEntryWrapper(
        const HeteroMapElementBase< BaseBinding> & element) :
        myEntry(element.clone())
    {
    }
    HeteroMapBaseEntryWrapper(const HeteroMapBaseEntryWrapper & toCopy) :
        myEntry(toCopy.myEntry->clone())
    {
    }
    virtual
    ~HeteroMapBaseEntryWrapper()
    {
    }

    virtual bool
    valueToNode(YAML::Node & node, const DerivedBinding & map) const
    {
      return myEntry->valueToNode(node, map);
    }
    virtual bool
    nodeToValue(ParseLog & parse, DerivedBinding & map,
        const YAML::Node & node) const
    {
      return myEntry->nodeToValue(parse, map, node);
    }
    virtual bool
    defaultValueToMap(DerivedBinding & map) const
    {
      return myEntry->defaultValueToMap(map);
    }

    virtual HeteroMapBaseEntryWrapper *
    clone() const
    {
      return new HeteroMapBaseEntryWrapper(*this);
    }

  private:
    ::boost::scoped_ptr< HeteroMapElementBase< BaseBinding> > myEntry;
  };

} // namespace detail

template< typename T>
  Map< T>::Map()
  {
    myAllowUnknownEntries = true;
  }

template< typename EntryType>
  bool
  Map< EntryType>::valueToNode(YAML::Node & node,
      const BindingType & value) const
  {
    typename EntriesMap::const_iterator it;
    BOOST_FOREACH(typename BindingType::const_reference entry, value)
    {
      it = myEntries.find(entry.first);
      if(it != myEntries.end())
      {
        YAML::Node entryNode;
        it->second.valueToNode(entryNode, entry.second);
        node[entry.first] = entryNode;
      }
      else if(myAllowUnknownEntries)
      {
        EntryType element;
        YAML::Node entryNode;
        element.valueToNode(entryNode, entry.second);
        node[entry.first] = entryNode;
      }
    }
    return true;
  }

template< typename EntryType>
  bool
  Map< EntryType>::nodeToValue(ParseLog & parse, BindingType & value,
      const YAML::Node & node) const
  {
    if(!node.IsMap())
    {
      parse.logError(ParseLogErrorCode::NODE_TYPE_WRONG, "Expected map");
      return false;
    }

    typename EntriesMap::const_iterator entriesIt;
    for(YAML::Node::const_iterator it = node.begin(), end = node.end();
        it != end; ++it)
    {
      ParseLog::PathPusher pusher(parse, it->first.Scalar());

      entriesIt = myEntries.find(it->first.Scalar());
      if(entriesIt != myEntries.end())
      {
        typename BindingType::mapped_type mappedValue;
        if(entriesIt->second.nodeToValue(parse, mappedValue, it->second))
          value[it->first.Scalar()] = mappedValue;
      }
      else if(myAllowUnknownEntries)
      {
        typename BindingType::mapped_type mappedValue;
        EntryType entryType;
        if(entryType.nodeToValue(parse, mappedValue, it->second))
          value[it->first.Scalar()] = mappedValue;
      }
    }
    return true;
  }

template< typename EntryType>
  typename Map< EntryType>::ElementType *
  Map< EntryType>::element(const ::std::string & name)
  {
    return &myEntries[name];
  }

template< typename T>
  Map< T> *
  Map< T>::clone() const
  {
    return new Map< T>(*this);
  }

template< typename T>
  bool
  Map< T>::areUnknownEntriesAllowed() const
  {
    return myAllowUnknownEntries;
  }

template< typename T>
  void
  Map< T>::setAllowUnknownEntries(const bool allowUnknownEntries)
  {
    myAllowUnknownEntries = allowUnknownEntries;
  }

template< class T>
  typename HeteroMap< T>::EntryIterator
  HeteroMap< T>::entriesBegin() const
  {
    return myEntries.begin();
  }

template< class T>
  typename HeteroMap< T>::EntryIterator
  HeteroMap< T>::entriesEnd() const
  {
    return myEntries.end();
  }

template< class BindingType>
  bool
  HeteroMap< BindingType>::valueToNode(YAML::Node & node,
      const BindingType & value) const
  {
    BOOST_FOREACH(typename EntriesMap::const_reference entry, myEntries)
    {
      YAML::Node entryNode;
      if(entry.second->valueToNode(entryNode, value))
        node[entry.first] = entryNode;
    }
    return true;
  }

template< typename BindingType>
  bool
  HeteroMap< BindingType>::nodeToValue(ParseLog & parse, BindingType & map,
      const YAML::Node & node) const
  {
    if(node.IsNull())
    {
      // We have a map with no entries, check that we have no required entries without
      // a default
      bool succeeded = true;
      const YAML::Node nullNode;
      BOOST_FOREACH(typename EntriesMap::const_reference entry, myEntries)
      {
        if(!entry.second->nodeToValue(parse, map, nullNode))
          succeeded = false;
      }
      return succeeded;
    }

    if(!node.IsMap())
    {
      parse.logError(ParseLogErrorCode::NODE_TYPE_WRONG, "Expected map.");
      return false;
    }

    ::std::set< ::std::string> toProcess;
    BOOST_FOREACH(typename EntriesMap::const_reference entry, myEntries)
    {
      toProcess.insert(entry.first);
    }

    ::std::set< ::std::string>::const_iterator process;
    typename EntriesMap::const_iterator entry;
    for(YAML::Node::const_iterator it = node.begin(), end = node.end();
        it != end; ++it)
    {
      const ::std::string & entryName = it->first.Scalar();
      process = toProcess.find(entryName);
      if(process != toProcess.end())
      {
        entry = myEntries.find(entryName);
        if(entry != myEntries.end())
        {
          entry->second->nodeToValue(parse, map, it->second);
        }
      }
      else
      {
        // TODO: Error: unrecognised map entry
      }
      toProcess.erase(process);
    }

    // TODO: Check that toProcess is empty

    return true;

  }

template< class T>
  HeteroMap< T> *
  HeteroMap< T>::clone() const
  {
    return new HeteroMap(*this);
  }

template< class BindingType>
  template< typename ElementType, typename MemberType>
    detail::HeteroMapElement< BindingType, ElementType, MemberType> *
    HeteroMap< BindingType>::element(const ::std::string & name,
        MemberType (BindingType::* const member))
    {
      detail::HeteroMapElement< BindingType, ElementType, MemberType> * const element =
          new detail::HeteroMapElement< BindingType, ElementType, MemberType>(
              member);

      // WARNING: This will only insert the new entry if one with the same name doesn't
      // exist already.  Otherwise the new one will be destroyed and the old one returned.
      // This may not be what the user expects.
      myEntries.insert(name, element);
      return element;
    }

template< class BindingType>
  template< typename BaseType>
    void
    HeteroMap< BindingType>::extends()
    {
      typedef typename BaseType::BindingType BaseBinding;
      const BaseType base;
      BOOST_FOREACH(typename BaseType::EntriesMap::const_reference entry,
          ::boost::make_iterator_range(base.entriesBegin(), base.entriesEnd()))
      {
        myEntries.insert(entry.first,
            new detail::HeteroMapBaseEntryWrapper< BindingType, BaseBinding>(
                *entry.second));
      }
    }

} // namespace schemer

#endif /* SCHEMER_MAP_DETAIL_H */
