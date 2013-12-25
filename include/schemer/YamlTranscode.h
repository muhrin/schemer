/*
 * YamlTranscode.h
 *
 *  Created on: Dec 24, 2013
 *      Author: Martin Uhrin
 */

#ifndef YAML_TRANSCODE_H
#define YAML_TRANSCODE_H

// INCLUDES //////////////////////////////////

#include <boost/optional.hpp>

#include <yaml-cpp/yaml.h>

// NAMESPACES ////////////////////////////////

// Some custom YAML transcoders
namespace YAML {

// build_cell::Sphere
template< typename T>
  struct convert< ::boost::optional< T> >
  {
    static Node
    encode(const ::boost::optional< T> & value)
    {
      Node ret;
      if(value)
        ret = *value;
      return ret;
    }
    static bool
    decode(const Node & node, ::boost::optional< T> & value)
    {
      if(node.IsNull())
      {
        value.reset();
        return true;
      }
      else
      {
        try
        {
          value.reset(node.as<T>());
          return true;
        }
        catch(const YAML::BadConversion & /*e*/)
        {
          return false;
        }
      }
    }
  };

}

#endif /* YAML_TRANSCODE_H */
