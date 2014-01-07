/*
 * Fundamental.h
 *
 *
 *  Created on: May 29, 2012
 *      Author: Martin Uhrin
 */

#ifndef SCHEMER_FUNDAMENTAL_DETAIL_H
#define SCHEMER_FUNDAMENTAL_DETAIL_H

// INCLUDES /////////////////////////////////////////////

// DEFINES //////////////////////////////////////////////

// FORWARD DECLARATIONS ////////////////////////////////////


namespace schemer {

template <typename T>
bool Fundamental<T>::valueToNode(YAML::Node & node, const T & value, const bool useDefaultOnFail) const
{
  // TODO: Find out how to check if transcoding failed
  node = value;
  return true;
}

template <typename T>
bool Fundamental<T>::nodeToValue(ParseLog & parse, T & value, const YAML::Node & node, const bool useDefaultOnFail) const
{
  if(!node.IsDefined())
    return false;

  try
  {
    // TODO: Check behaviour of as if value is invalid (i.e. does it just use the default?)
    if(useDefaultOnFail && getDefault())
      value = node.as<T>(*getDefault());
    else
      value = node.as<T>();
  }
  catch(const YAML::Exception & /*e*/)
  {
    parse.logError(ParseLogErrorCode::TYPE_CONVERSION_FAILED, "Failed to convert value.  Probably a typo.");
    return false;
  }
  return true;
}

template <typename T>
Fundamental<T> * Fundamental<T>::clone() const
{
  return new Fundamental(*this);
}

}

#endif /* SCHEMA_FUNDAMENTAL_DETAIL_H */
