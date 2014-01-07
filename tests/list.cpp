/*
 * list.cpp
 *
 *  Created on: Dec 24, 2013
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "schemer_test.h"

#include <boost/optional.hpp>
#include <boost/foreach.hpp>

#include <yaml-cpp/yaml.h>

#include <vector>

#include <schemer/Schemer.h>

BOOST_AUTO_TEST_CASE(ListTest)
{
  // Settings //
  static const size_t NUM_ELEMENTS = 100;

  ::std::vector<int> ORIGINAL_LIST(NUM_ELEMENTS);
  for(size_t i; i <= NUM_ELEMENTS; ++i)
    ORIGINAL_LIST[i] = ::std::rand();

  YAML::Node listNode;
  schemer::serialise(ORIGINAL_LIST, &listNode);

  for(size_t i = 0; i < NUM_ELEMENTS; ++i)
    BOOST_REQUIRE(ORIGINAL_LIST[i] == listNode[i].as<int>());

  ::std::vector<int> newList;
  schemer::ParseLog parse;
  schemer::parse(listNode, &newList, &parse);

  BOOST_REQUIRE(newList == ORIGINAL_LIST);
}
