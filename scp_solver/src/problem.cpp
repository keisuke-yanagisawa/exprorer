#include <iostream>
#include <vector>
#include <set>
#include <cassert>
#include <numeric>   //std::iota
#include <algorithm> //std::sort

#include "utils.hpp"
#include "problem.hpp"
namespace {
  template <typename T>
  bool issubset(const std::vector<T>& lhs, const std::vector<T>& rhs){
    assert(lhs.size() == rhs.size());
    for(int i=0; i<lhs.size(); i++){
      if(lhs[i]>rhs[i]) return false;
    }
    return true;
  }
  template <typename T>
  bool issubset(const std::set<T>& lhs, const std::set<T>& rhs){
    std::set<T> tmp = rhs;
    for(const auto& elem: lhs) tmp.insert(elem);
    return (tmp.size() - rhs.size()) == 0;
  }
}

// TODO ambiguious order init() and parse()
// now init() is just after parse()
void problem::init(){
  for(const auto& elem: sets){
    utils::dump(elem.member);
  }

  sort(sets.begin(), sets.end());
  //remove_inactive_cols();
}

void problem::remove_row(int i){
  std::cout << "remove_row(" << i << ")" << std::endl;
  for(int j=0; j<cols; j++){
    std::set<int> new_member;
    for(const auto& elem: sets[j].member){
      if(elem < i) new_member.insert(elem);
      else if(elem == i) continue;
      else if(elem > i) new_member.insert(elem-1);
    }
    sets[j].member = new_member;
  }
  rows--;
}

void problem::remove_col(int i, bool is_active){
  std::cout << "remove_col(" << i << ", " << is_active << ")" << std::endl;
  std::cout << "sets["<<i<<"].ext_idx = " << sets[i].ext_idx << std::endl;
  std::set<int> removed_cover = sets[i].member;
  sets.erase(sets.begin()+i);
  cols--;

  if(is_active){
    for(const auto& elem: removed_cover){
      remove_row(elem);
    }
  }
}

void problem::remove_inactive_cols(){
  std::vector<bool> is_possible_col = std::vector<bool>(cols, true);

  for(int i=0; i<sets.size(); i++){
    for(int j=i+1; j<sets.size(); j++){
      if(sets[i].cost <= sets[j].cost && is_possible_col[i]
	 && issubset(sets[j].member, sets[i].member)){
	is_possible_col[j] = false;
      }
    }
  }

  for(int i=cols-1; i>=0; i--){
    if(is_possible_col[i]) continue;
    is_possible_col.erase(is_possible_col.begin()+i);
    remove_col(i, false);
  }
}

bool problem::solvable() const{
  std::set<int> test_val;
  for(int j=0; j<cols; j++){
    for(const auto& elem: sets[j].member){
      test_val.insert(elem);
    }
  }
  return rows == test_val.size();
}

void problem::parse(std::istream& is){
  is >> rows >> cols;
  sets = std::vector<scp::set>(cols);
  for(int i=0; i<cols; i++){
    is >> sets[i].cost;
    sets[i].ext_idx = i; //0-origin
  }
  
  for(int i=0; i<rows; i++){
    int num;
    is >> num;
    for(int j=0; j<num; j++){
      int temp;
      is >> temp;
      sets[temp-1].member.insert(i);
    }
  }
  init();
}
