/*
 * List.h
 *
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

#ifndef SCHEMER_LIST_DETAIL_H
#define SCHEMER_LIST_DETAIL_H

// INCLUDES /////////////////////////////////////////////
#include <sstream>

#include <boost/foreach.hpp>

#include "schemer/Registry.h"

// DEFINES //////////////////////////////////////////////

// FORWARD DECLARATIONS ////////////////////////////////////

namespace schemer {

template< class T>
  List< T>::List() :
      myLength(-1)
  {
  }

template< class T>
  List< T>::List(const List & toCopy) :
      myLength(toCopy.myLength)
  {
  }

template< class T>
  bool
  List< T>::valueToNode(YAML::Node & node, const BindingType & list) const
  {
    BOOST_FOREACH(typename BindingType::const_reference value, list)
    {
      YAML::Node listEntry;
      if(myEntryType.valueToNode(listEntry, value))
        node.push_back(listEntry);
    }
    return true;
  }

template< class T>
  bool
  List< T>::nodeToValue(ParseLog & parse, BindingType & list,
      const YAML::Node & node) const
  {
    if(!node.IsSequence())
    {
      parse.logError(ParseLogErrorCode::NODE_TYPE_WRONG, "Expected sequence.");
      return false;
    }

    if(myLength != -1 && myLength != static_cast< int>(node.size()))
    {
      parse.logError(ParseLogErrorCode::SEQUENCE_LENGTH_MISMATCH,
          "Sequence is wrong length");
      return false;
    }

    for(size_t i = 0; i < node.size(); ++i)
    {
      ::std::stringstream ss;
      ss << "[" << i << "]";
      ParseLog::PathPusher pusher(parse, ss.str());

      typename T::BindingType entryValue;
      if(myEntryType.nodeToValue(parse, entryValue, node[i]))
        list.push_back(entryValue);
    }

    return true;
  }

template< class EntrySchema>
  List< EntrySchema> *
  List< EntrySchema>::length(const int length)
  {
    myLength = length;
    return this;
  }

template< class T>
  List< T> *
  List< T>::clone() const
  {
    return new List< T>(*this);
  }

}

#endif /* SCHEMER_LIST_DETAIL_H */
