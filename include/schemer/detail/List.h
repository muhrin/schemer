/*
 * SchemaList.h
 *
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

#ifndef SCHEMA_LIST_DETAIL_H
#define SCHEMA_LIST_DETAIL_H

// INCLUDES /////////////////////////////////////////////
#include <sstream>

#include <boost/foreach.hpp>

// DEFINES //////////////////////////////////////////////

// FORWARD DECLARATIONS ////////////////////////////////////

namespace schemer {

template <class EntrySchema>
List<EntrySchema>::List():
myLength(-1),
myEntrySchema()
{}

template <class EntrySchema>
List<EntrySchema>::List(const EntrySchema & entrySchema):
myEntrySchema(entrySchema),
myLength(-1),
myEntrySchema()
{}

template <class EntrySchema>
List<EntrySchema>::List(const List & toCopy):
myLength(toCopy.myLength),
myEntrySchema(toCopy.myEntrySchema)
{}

template <class EntrySchema>
bool List<EntrySchema>::valueToNode(
  YAML::Node & node,
  const BindingType & list,
  const bool useDefaultOnFail) const
{
  BOOST_FOREACH(typename BindingType::const_reference value, list)
  {
    YAML::Node listEntry;
    if(myEntrySchema.valueToNode(listEntry, value, useDefaultOnFail))
      node.push_back(listEntry);
  }
  return true;
}

template <class EntrySchema>
bool List<EntrySchema>::nodeToValue(
  ParseLog & parse,
  BindingType & list,
  const YAML::Node & node,
  const bool useDefaultOnFail) const
{
  if(!node.IsSequence())
  {
    parse.logError(ParseLogErrorCode::NODE_TYPE_WRONG, "Expected sequence.");
    return false;
  }

  if(myLength != -1 && myLength != static_cast<int>(node.size()))
  {
    parse.logError(ParseLogErrorCode::SEQUENCE_LENGTH_MISMATCH, "Sequence is wrong length");
    return false;
  }

  for(size_t i = 0; i < node.size(); ++i)
  {
    ::std::stringstream ss;
    ss << "[" << i << "]";
    ParseLog::PathPusher pusher(parse, ss.str());

    ListEntryType entryValue;
    if(myEntrySchema.nodeToValue(parse, entryValue, node[i], useDefaultOnFail))
      list.push_back(entryValue);
  }

  return true;
}

template <class EntrySchema>
List<EntrySchema> * List<EntrySchema>::length(const int length)
{
  myLength = length;
  return this;
}

template <class EntrySchema>
List<EntrySchema> * List<EntrySchema>::clone() const
{
  return new List<EntrySchema>(*this);
}


}

#endif /* SCHEMA_LIST_DETAIL_H */
