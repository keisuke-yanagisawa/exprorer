#include <iostream>

#ifndef UTILS_HPP_
#define UTILS_HPP_

namespace utils{
  template <typename T> 
  void dump(T v){
    std::cout << "[ ";
    for(const auto& elem: v) std::cout << elem << " ";
    std::cout << "]" << std::endl;
  }
}

#endif
