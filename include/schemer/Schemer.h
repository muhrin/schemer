/*
 * Schemer.h
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#ifndef SCHEMER_H
#define SCHEMER_H

#include "schemer/Enumeration.h"
#include "schemer/List.h"
#include "schemer/Map.h"
#include "schemer/Parse.h"
#include "schemer/Registry.h"
#include "schemer/Scalar.h"
#include "schemer/Types.h"
#include "schemer/Variant.h"
#include "schemer/YamlTranscode.h"
#include "schemer/detail/List.h"
#include "schemer/detail/Map.h"

namespace schemer {

template< typename T>
  bool
  parse(const YAML::Node & node, T * const valueOut, ParseLog * const log)
  {
    return getType< T>().nodeToValue(*log, *valueOut, node);
  }
template< typename T>
  bool
  serialise(const T & value, YAML::Node * const node)
  {
    return getType< T>().valueToNode(*node, value);
  }
}

#endif // SCHEMER_H
