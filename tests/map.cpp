/*
 * map.cpp
 *
 *  Created on: Dec 24, 2013
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "schemer_test.h"

#include <boost/optional.hpp>
#include <boost/foreach.hpp>

#include <yaml-cpp/yaml.h>

#include <map>
#include <string>

#include <schemer/Schemer.h>

struct Person
{
  bool
  operator ==(const Person & rhs)
  {
    return name == rhs.name && age == rhs.age && nickname == rhs.nickname
        && phone_no == rhs.phone_no;
  }
  ::std::string name;
  int age;
  ::boost::optional< ::std::string> nickname;
  ::boost::optional< int> phone_no;
};

BOOST_AUTO_TEST_CASE(HeteroMapTest)
{
  using ::std::string;

  // SETTINGS //
  const string NAME = "Bob";
  const int AGE = 29;
  const int PHONE_NO = 123456789;

  const string KEY_NAME = "name";
  const string KEY_AGE = "age";
  const string KEY_NICKNAME = "nickname";
  const string KEY_PHONE_NO = "phone_no";
  Person ORIGINAL_PERSON;
  ORIGINAL_PERSON.name = NAME;
  ORIGINAL_PERSON.age = AGE;
  ORIGINAL_PERSON.phone_no = PHONE_NO;

  schemer::ParseLog parse;

  schemer::HeteroMap< Person> personSchema;
  personSchema.addScalarEntry(KEY_NAME, &Person::name);
  personSchema.addScalarEntry(KEY_AGE, &Person::age);
  personSchema.addScalarEntry(KEY_NICKNAME, &Person::nickname);
  personSchema.addScalarEntry(KEY_PHONE_NO, &Person::phone_no);

  YAML::Node personNode;
  personSchema.valueToNode(personNode, ORIGINAL_PERSON, false);

  // Check that the node got all the data from ORIGINAL_PERSON
  BOOST_REQUIRE(personNode.IsMap());
  BOOST_REQUIRE(personNode[KEY_NAME].Scalar() == NAME);
  BOOST_REQUIRE(personNode[KEY_AGE].as< int>() == AGE);
  BOOST_REQUIRE(personNode[KEY_NICKNAME].IsNull());
  BOOST_REQUIRE(personNode[KEY_PHONE_NO].as< int>() == PHONE_NO);

  Person p;
  personSchema.nodeToValue(parse, p, personNode, false);

  // Check that the node was sucessfully converted back to a person
  BOOST_REQUIRE(p.name == NAME);
  BOOST_REQUIRE(p.age == AGE);
  BOOST_REQUIRE(!p.nickname);
  BOOST_REQUIRE(p.phone_no == PHONE_NO);

  BOOST_REQUIRE(ORIGINAL_PERSON == p);
}

BOOST_AUTO_TEST_CASE(MapTest)
{
  using ::std::string;

  // SETTINGS //
  typedef string Key;
  typedef int Value;
  typedef ::std::map< Key, Value> Map;
  Map ORIGINAL_MAP;
  ORIGINAL_MAP["Bob"] = 32523;
  ORIGINAL_MAP["Alice"] = 632;
  ORIGINAL_MAP["Charles"] = 9532;

  schemer::ParseLog parse;

  schemer::Map< schemer::Scalar< Value> > mapSchema;

  YAML::Node mapNode;
  mapSchema.valueToNode(mapNode, ORIGINAL_MAP, false);

  BOOST_FOREACH(Map::const_reference x, ORIGINAL_MAP)
  {
    BOOST_REQUIRE(!mapNode[x.first].IsNull());
    BOOST_REQUIRE(mapNode[x.first].as< Value>() == x.second);
  }

  Map newMap;
  mapSchema.nodeToValue(parse, newMap, mapNode, false);

  BOOST_REQUIRE(ORIGINAL_MAP == newMap);
}
