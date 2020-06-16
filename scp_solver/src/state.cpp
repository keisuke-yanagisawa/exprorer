#include <iostream>
#include <vector>
#include <set>
#include <cassert>
#include <numeric>   //std::iota
#include <algorithm> //std::sort

#include "utils.hpp"
#include "state.hpp"
#include "problem.hpp"

state::state(const problem& pr){
  this->pr = pr;
  Z_UB = 1e8;
  Z_LB = -1e8;
  init_t();
  init_P();
}

void state::init_t(){
  t = std::vector<double>(pr.rows, 1e8);
  for(int j=0; j<pr.cols; j++){
    for(const auto& elem: pr.sets[j].member){
      t[elem] = std::min(t[elem], pr.sets[j].cost);
    }
  }
}

void state::init_P(){
  for(const auto& elem: pr.sets){
    P.push_back(elem.cost);
  }
}
