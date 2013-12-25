/*
 * SchemaWrapper.h
 *
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

#ifndef SCHEMA_WRAPPER_DETAIL_H
#define SCHEMA_WRAPPER_DETAIL_H

// INCLUDES /////////////////////////////////////////////
#include "schemer/Parse.h"

// DEFINES //////////////////////////////////////////////

// FORWARD DECLARATIONS ////////////////////////////////////

namespace schemer {

template< typename Wrap>
  bool
  Wrapper< Wrap>::valueToNode(YAML::Node & node, const BindingType & value,
      const bool useDefaultOnFail) const
  {
    // TODO: Find out how to check if transcoding failed
    Wrapper wrap(value);
    node = wrap;
    return true;
  }

template< typename Wrap>
  bool
  Wrapper< Wrap>::nodeToValue(ParseLog & parse, BindingType & value,
      const YAML::Node & node, const bool useDefaultOnFail) const
  {
    if(!node.IsDefined())
      return false;

    try
    {
      // TODO: Check behaviour of as if value is invalid (i.e. does it just use the default?)
      if(useDefaultOnFail && getDefault())
      {
        value = *node.as < Wrapper > (Wrapper(*getDefault()));
      }
      else
      {
        value = *node.as< Wrap>();
      }
    }
    catch(const YAML::Exception & /*e*/)
    {
      parse.logError(ParseLogErrorCode::TYPE_CONVERSION_FAILED,
          "Failed to convert value.  Probably a typo.");
      return false;
    }
    return true;
  }

template< typename Wrap>
  Wrapper< Wrap> *
  Wrapper< Wrap>::clone() const
  {
    return new Wrapper(*this);
  }

}

#endif /* SCHEMA_WRAPPER_DETAIL_H */
