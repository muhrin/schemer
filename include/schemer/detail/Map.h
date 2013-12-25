/*
 * SchemaMap.h
 *
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

#ifndef SCHEMA_MAP_DETAIL_H
#define SCHEMA_MAP_DETAIL_H

// INCLUDES /////////////////////////////////////////////
#include <boost/foreach.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/type_traits/is_fundamental.hpp>

#include "schemer/Scalar.h"

// DEFINES //////////////////////////////////////////////

// FORWARD DECLARATIONS ////////////////////////////////////

namespace schemer {
namespace detail {

template< typename T, class IsCppFundamental = ::boost::is_fundamental< T> >
  struct SchemaEntryAutoGenerator
  {
    static ElementBase< T> *
    generate()
    {
      // If we know nothing else, assume it's a scalar.
      // See ScalarMap.cpp for specialisations
      return new Scalar< T>();
    }
  };

template< class T>
  class SchemaHeteroMapEntryBase
  {
  public:
    SchemaHeteroMapEntryBase() :
        myRequired(false)
    {
    }
    SchemaHeteroMapEntryBase(const SchemaHeteroMapEntryBase & toCopy) :
        myRequired(toCopy.myRequired)
    {
    }
    virtual
    ~SchemaHeteroMapEntryBase()
    {
    }

    SchemaHeteroMapEntryBase *
    required()
    {
      myRequired = true;
      return this;
    }
    bool
    isRequired() const
    {
      return myRequired;
    }

    virtual bool
    valueToNode(YAML::Node & node, const T & map,
        const bool useDefaultOnFail) const = 0;
    virtual bool
    nodeToValue(ParseLog & parse, T & map, const YAML::Node & node,
        const bool useDefaultOnFail) const = 0;
    virtual bool
    defaultValueToMap(T & map) const = 0;

    virtual SchemaHeteroMapEntryBase *
    clone() const = 0;

  private:
    bool myRequired;
  };

template< class T>
  SchemaHeteroMapEntryBase< T> *
  new_clone(const SchemaHeteroMapEntryBase< T> & entry)
  {
    return entry.clone();
  }

template< class T, typename U>
  class SchemaHeteroMapEntry : public SchemaHeteroMapEntryBase< T>
  {
    typedef U (T::* const Member);
    typedef ElementBase< U> ElementSchema;
  public:
    using SchemaHeteroMapEntryBase< T>::isRequired;

    SchemaHeteroMapEntry(Member member, ElementSchema * const element);
    SchemaHeteroMapEntry(const SchemaHeteroMapEntry & toCopy);
    virtual
    ~SchemaHeteroMapEntry()
    {
    }

    ElementSchema *
    element();

    // From SchemaElementBase ///////////////
    virtual bool
    valueToNode(YAML::Node & node, const T & map,
        const bool useDefaultOnFail) const;
    virtual bool
    nodeToValue(ParseLog & parse, T & map, const YAML::Node & node,
        const bool useDefaultOnFail) const;
    virtual bool
    defaultValueToMap(T & map) const;
    virtual SchemaHeteroMapEntry *
    clone() const;
    // End from SchemaElementBase /////////////

  private:
    Member myMember;
    ::boost::scoped_ptr< ElementSchema> myElement;
  };

template< class T, typename U>
  SchemaHeteroMapEntry< T, U>::SchemaHeteroMapEntry(Member member,
      ElementSchema * const element) :
      myMember(member), myElement(element)
  {
  }

template< class T, typename U>
  SchemaHeteroMapEntry< T, U>::SchemaHeteroMapEntry(
      const SchemaHeteroMapEntry< T, U> & toCopy) :
      SchemaHeteroMapEntryBase< T>(toCopy), myMember(toCopy.myMember), myElement(
          toCopy.myElement->clone())
  {
  }

template< class T, typename U>
  typename SchemaHeteroMapEntry< T, U>::ElementSchema *
  SchemaHeteroMapEntry< T, U>::element()
  {
    return myElement.get();
  }

template< class T, typename U>
  bool
  SchemaHeteroMapEntry< T, U>::valueToNode(YAML::Node & node, const T & map,
      const bool useDefaultOnFail) const
  {
    return myElement->valueToNode(node, map.*myMember, useDefaultOnFail);
  }

template< class T, typename U>
  bool
  SchemaHeteroMapEntry< T, U>::nodeToValue(ParseLog & parse, T & map,
      const YAML::Node & node, const bool useDefaultOnFail) const
  {
    // TODO: Move this line to SchemaHeteroMap when parsing node
    //ParseLog::PathPusher pusher(parse, getName());

    if(!node.IsDefined())
    {
      // Check for a default
      if(myElement->getDefault())
        return defaultValueToMap(map);
      else if(isRequired())
        parse.logError(ParseLogErrorCode::REQUIRED_VALUE_MISSING,
            "Required map entry missing.");
      return false;
    }

    return myElement->nodeToValue(parse, map.*myMember, node, useDefaultOnFail);
  }

template< class T, typename U>
  bool
  SchemaHeteroMapEntry< T, U>::defaultValueToMap(T & map) const
  {
    if(!myElement->getDefault())
      return false;

    map.*myMember = *myElement->getDefault();
    return true;
  }

template< class T, typename U>
  SchemaHeteroMapEntry< T, U> *
  SchemaHeteroMapEntry< T, U>::clone() const
  {
    return new SchemaHeteroMapEntry(*this);
  }

} // namespace detail

template< typename T>
  Map< T>::Map()
  {
    myAllowUnknownEntries = true;
  }

template< typename T>
  bool
  Map< T>::valueToNode(YAML::Node & node, const BindingType & value,
      const bool useDefaultOnFail) const
  {
    typename EntriesMap::const_iterator it;
    BOOST_FOREACH(typename BindingType::const_reference entry, value)
    {
      it = myEntries.find(entry.first);
      if(it != myEntries.end())
      {
        YAML::Node entryNode;
        it->second->valueToNode(entryNode, entry.second, useDefaultOnFail);
        node[entry.first] = entryNode;
      }
      else if(myAllowUnknownEntries)
      {
        YAML::Node entryNode;
        myDefaultEntry.valueToNode(entryNode, entry.second, useDefaultOnFail);
        node[entry.first] = entryNode;
      }
    }
    return true;
  }

template< typename T>
  bool
  Map< T>::nodeToValue(ParseLog & parse, BindingType & value,
      const YAML::Node & node, const bool useDefaultOnFail) const
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
        if(entriesIt->second->nodeToValue(parse, mappedValue, it->second,
            useDefaultOnFail))
          value[it->first.Scalar()] = mappedValue;
      }
      else if(myAllowUnknownEntries)
      {
        typename BindingType::mapped_type mappedValue;
        if(myDefaultEntry.nodeToValue(parse, mappedValue, it->second,
            useDefaultOnFail))
          value[it->first.Scalar()] = mappedValue;
      }
    }
    return true;
  }

template< typename T>
  void
  Map< T>::addEntry(const ::std::string & name, const T & element)
  {
    myEntries.insert(name, element);
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
  bool
  HeteroMap< T>::valueToNode(YAML::Node & node, const BindingType & value,
      const bool useDefaultOnFail) const
  {
    BOOST_FOREACH(typename EntriesMap::const_reference entry, myEntries)
    {
      YAML::Node entryNode;
      if(entry.second->valueToNode(entryNode, value, useDefaultOnFail))
        node[entry.first] = entryNode;
    }
    return true;
  }

template< typename T>
  bool
  HeteroMap< T>::nodeToValue(ParseLog & parse, BindingType & map,
      const YAML::Node & node, const bool useDefaultOnFail) const
  {
    if(node.IsNull())
    {
      // We are a map with no entries, check that we have no required entries without
      // a default
      bool succeeded = true;
      BOOST_FOREACH(typename EntriesMap::const_reference entry, myEntries)
      {
        if(!entry.second->defaultValueToMap(map) && entry.second->isRequired())
        {
          parse.logError(ParseLogErrorCode::REQUIRED_VALUE_MISSING,
              "Required map entry missing: " + entry.first + ".");
          succeeded = false;
        }
      }
      return succeeded;
    }

    if(!node.IsMap())
    {
      parse.logError(ParseLogErrorCode::NODE_TYPE_WRONG, "Expected map.");
      return false;
    }

    BOOST_FOREACH(typename EntriesMap::const_reference entry, myEntries)
    {
      entry.second->nodeToValue(parse, map, node[entry.first],
          useDefaultOnFail);
    }
    return true;

  }

template< class T>
  HeteroMap< T> *
  HeteroMap< T>::clone() const
  {
    return new HeteroMap(*this);
  }

template< class T>
  template< typename U>
    detail::SchemaHeteroMapEntry< T, U> *
    HeteroMap< T>::addEntry(const ::std::string & name,
        U (T::* const member), detail::ElementBase< U> * const element)
    {
      detail::SchemaHeteroMapEntry< T, U> * const entry =
          new detail::SchemaHeteroMapEntry< T, U>(member, element);

      // WARNING: This will only insert the new entry if one with the same name doesn't
      // exist already.  Otherwise the new one will be destroyed and the old one returned.
      // This may not be what the user expects.
      myEntries.insert(name, entry);

      return entry;
    }

template< class T>
  template< typename U>
    detail::SchemaHeteroMapEntry< T, U> *
    HeteroMap< T>::addScalarEntry(const ::std::string & name,
        U (T::* const member))
    {
      return addEntry(name, member,
          detail::SchemaEntryAutoGenerator< U>::generate());
    }

} // namespace schemer

#endif /* SCHEMA_MAP_DETAIL_H */
