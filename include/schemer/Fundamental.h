/*
 * SchemaFundamental.h
 *
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

#ifndef SCHEMA_FUNDAMENTAL_H
#define SCHEMA_FUNDAMENTAL_H

// INCLUDES /////////////////////////////////////////////
#include <boost/optional.hpp>

#include "schemer/detail/Element.h"

// DEFINES //////////////////////////////////////////////

// FORWARD DECLARATIONS ////////////////////////////////////

namespace schemer {

template< typename T>
  class Fundamental : public detail::ElementBase< T>
  {
  public:
    typedef T BindingType;

    // NOTE: Have to pull in any methods that we want to use directly (i.e. without this->)
    // here because of the way identifier lookup works with template base classes.
    // See: e.g. http://stackoverflow.com/questions/5286922/g-template-parameter-error
    using detail::ElementBase< T>::getDefault;
    using detail::ElementBase< T>::isRequired;

    virtual ~Fundamental()
    {
    }

    virtual bool
    valueToNode(YAML::Node & node, const T & value,
        const bool useDefaultOnFail) const;
    virtual bool
    nodeToValue(ParseLog & parse, T & value, const YAML::Node & node,
        const bool useDefaultOnFail) const;
    virtual Fundamental *
    clone() const;
  };

}

#include "schemer/detail/Fundamental.h"

#endif /* SCHEMA_FUNDAMENTAL_H */
