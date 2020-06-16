#ifndef _UTILS_H_
#define _UTILS_H_

#include "common.hpp"

#include <string>
#include <iostream>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/trim.hpp>

namespace utils {
  
  template<typename T>
  struct Vec3D{
    T x;
    T y;
    T z;
    bool operator<(const Vec3D<T> &o) const{
      return x!=o.x ? x<o.x : (y!=o.y?y<o.y:z<o.z);
    }
    bool operator!=(const Vec3D<T> &o) const{
      return x!=o.x || y!=o.y || z!=o.z;
    }
    
  };
  
  std::string trim_tail(const std::string &str, const char* space = " \t");
  std::string trim_head(const std::string &str, const char* space = " \t");
  
  struct bad_argument{};
  template<typename T>
  T cast_substring(const std::string &str, size_t start, size_t len){
    if(len <= 0 || start + len >= str.length()) throw bad_argument();
    std::string trimed = boost::trim_copy(str.substr(start, len));
    if(trimed.size() == 0){
      return T();
    } else {
      return boost::lexical_cast<T>(trimed);
    }
  }
  
  int round(fltype val);

  //Generate fltype random value between lower and upper. 
  fltype randf(fltype lower, fltype upper);

  //Extract extension from full path.
  std::string GetExtension(const std::string &path);

  std::string getDate();
} // namespace utils

#endif
