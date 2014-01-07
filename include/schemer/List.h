/*
 * SchemaList.h
 *
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

#ifndef SCHEMER_LIST_H
#define SCHEMER_LIST_H

// INCLUDES /////////////////////////////////////////////
#include <vector>

#include "schemer/detail/Type.h"

// DEFINES //////////////////////////////////////////////

// FORWARD DECLARATIONS ////////////////////////////////////

namespace schemer {

template< class EntryType>
  class List : public detail::Type<
      ::std::vector< typename EntryType::BindingType> >
  {
    typedef typename EntryType::BindingType EntryBindingType;
  public:
    typedef ::std::vector< EntryBindingType> BindingType;

    List();
    List(const List & toCopy);
    virtual
    ~List()
    {
    }

    virtual bool
    valueToNode(YAML::Node & node, const BindingType & list) const;
    virtual bool
    nodeToValue(ParseLog & parse, BindingType & list,
        const YAML::Node & node) const;

    List *
    length(const int length);

    virtual List *
    clone() const;

  private:
    int myLength;
    const EntryType myEntryType;
  };

}

#endif /* SCHEMER_LIST_H */
