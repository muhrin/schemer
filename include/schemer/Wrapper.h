/*
 * SchemaWrapper.h
 *
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

#ifndef SCHEMA_WRAPPER_H
#define SCHEMA_WRAPPER_H

// INCLUDES /////////////////////////////////////////////
#include "schemer/detail/Element.h"

// DEFINES //////////////////////////////////////////////

// FORWARD DECLARATIONS ////////////////////////////////////

namespace schemer {

template< typename Wrap>
  class Wrapper : public detail::ElementBase< typename Wrap::ValueType>
  {
  public:
    typedef typename Wrap::ValueType BindingType;

    // NOTE: Have to pull in any methods that we want to use directly (i.e. without this->)
    // here because of the way identifier lookup works with template base classes.
    // See: e.g. http://stackoverflow.com/questions/5286922/g-template-parameter-error
    using detail::ElementBase< BindingType>::getDefault;
    using detail::ElementBase< BindingType>::isRequired;

    virtual ~Wrapper()
    {
    }

    virtual bool
    valueToNode(YAML::Node & node, const BindingType & value,
        const bool useDefaultOnFail) const;
    virtual bool
    nodeToValue(ParseLog & parse, BindingType & value,
        const YAML::Node & node, const bool useDefaultOnFail) const;
    virtual Wrapper *
    clone() const;
  };

}

#include "schemer/detail/Wrapper.h"

#endif /* SCHEMA_WRAPPER_H */
