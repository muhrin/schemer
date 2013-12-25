/*
 * ParseLog.cpp
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

// INCLUDES //////////////////////////////////
#include "schemer/Parse.h"

#include <iostream>
#include <sstream>

#include <boost/foreach.hpp>

// NAMESPACES ////////////////////////////////

namespace schemer {

ParseLogError &
ParseLogError::operator =(const ParseLogError & rhs)
{
  myCode = rhs.myCode;
  myPath = rhs.myPath;
  myMessage = rhs.myMessage;
  return *this;
}

void
ParseLogError::print() const
{
  ::std::cout << myPath << ": " << myMessage << " code: " << myCode
      << ::std::endl;
}

ParseLog::PathPusher::PathPusher(ParseLog & parse, const ::std::string & path) :
    myParse(parse), myPath(path)
{
  myParse.doPushPath(myPath);
}

ParseLog::PathPusher::~PathPusher()
{
  myParse.doPopPath();
}

bool
ParseLog::hasErrors() const
{
  return !myParseErrors.empty();
}

const ParseLog::ParseErrors &
ParseLog::getErrors() const
{
  return myParseErrors;
}

::std::string
ParseLog::pathString() const
{
  ::std::stringstream ss;
  for(size_t i = 0; i < myParsePath.size(); ++i)
  {
    ss << myParsePath[i] << ".";
  }
  if(!myParsePath.empty())
    ss << myParsePath.back();
  return ss.str();
}

void
ParseLog::logError(const ParseLogErrorCode::Value code)
{
  myParseErrors.push_back(ParseLogError(code, pathString()));
}

void
ParseLog::logError(const ParseLogErrorCode::Value code,
    const ::std::string & message)
{
  myParseErrors.push_back(ParseLogError(code, pathString(), message));
}

void
ParseLog::printErrors() const
{
  BOOST_FOREACH(const ParseLogError & error, myParseErrors)
  {
    error.print();
  }
}

void
ParseLog::doPushPath(const std::string & path)
{
  myParsePath.push_back(path);
}

void
ParseLog::doPopPath()
{
  myParsePath.pop_back();
}

}

