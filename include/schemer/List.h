/*
 * SchemaList.h
 *
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

#ifndef SCHEMA_LIST_H
#define SCHEMA_LIST_H

// INCLUDES /////////////////////////////////////////////

#include <vector>

#include "schemer/detail/Element.h"

// DEFINES //////////////////////////////////////////////

// FORWARD DECLARATIONS ////////////////////////////////////

namespace schemer {

template< class EntrySchema>
  class List : public detail::ElementBase<
      ::std::vector< typename EntrySchema::BindingType> >
  {
    typedef typename EntrySchema::BindingType ListEntryType;
  public:
    typedef ::std::vector< ListEntryType> BindingType;

    List();
    List(const EntrySchema & entrySchema);
    List(const List & toCopy);
    virtual
    ~List()
    {
    }

    virtual bool
    valueToNode(YAML::Node & node, const BindingType & list,
        const bool useDefaultOnFail) const;
    virtual bool
    nodeToValue(ParseLog & parse, BindingType & list,
        const YAML::Node & node, const bool useDefaultOnFail) const;

    List *
    length(const int length);

    virtual List *
    clone() const;

  private:
    int myLength;
    const EntrySchema myEntrySchema;
  };

}

#include "schemer/detail/List.h"

#endif /* SCHEMA_LIST_H */
