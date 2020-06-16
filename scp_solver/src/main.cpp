#include <iostream>
#include <set>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <stack>

#include "utils.hpp"
#include "problem.hpp"
#include "state.hpp"
#include "beasley1990.hpp"


std::set<int> optimize(const problem& pr, state st){
  if(pr.cols == 0) return st.active_cols;

  std::cout << "optimize" << std::endl;
  //utils::dump(pr.col_costs);
  utils::dump(st.t);
  std::vector<double> tmp(pr.sets.size());
  for(int c=0; c<pr.sets.size(); c++){
    tmp[c] = pr.sets[c].cost;
  }
  for(int c=0; c<pr.cols; c++){
    for(const auto& elem: pr.sets[c].member){
      tmp[c] -= st.t[elem];
    }
  }
  std::set<int> ZUB_set = st.Z_UB_set;
  double now_ZUB = st.Z_UB;
  double now_ZLB = st.Z_LB;

  std::stack<std::vector<bool> > stk;
  stk.push(std::vector<bool>(1, true));
  stk.push(std::vector<bool>(1, false));
  while(!stk.empty()){
    std::vector<bool> now = stk.top();
    std::cout << "opt: "; utils::dump(now);
    stk.pop();
    
    problem new_pr = pr;
    std::set<int> new_actives = st.active_cols;
    int true_cnt = 0;
    for(int i=now.size()-1; i>=0; i--){
      if(now[i]){
	new_actives.insert(new_pr.sets[i].ext_idx);
	new_pr.remove_col(i, true);
	true_cnt++;
      }else{
	new_pr.remove_col(i, false);
      }
    }
    if(!new_pr.solvable()) continue;
    
    st.active_cols = new_actives;
    state ret = primal_dual(new_pr, st);
    std::cout << "opt-ret: " << ret.Z_LB << " " << ret.Z_UB << std::endl;
    if(now_ZUB > ret.Z_UB){
      now_ZUB = ret.Z_UB;
      ZUB_set = ret.Z_UB_set;
    }else if(now_ZUB <= ret.Z_LB){
      continue;
      // unfeasible
    }else{
      // branching
      now.push_back(true);
      stk.push(now);
      now[now.size()-1] = false;
      stk.push(now);
    }
  }

  return ZUB_set;
}

int main(void){
  problem pr;
  pr.parse(std::cin);
  if(!pr.solvable()) return 1;

  state result = primal_dual(pr);
  utils::dump(result.Z_UB_set);
  std::set<int> best_set = optimize(pr, result);
  
  utils::dump(best_set);
  return 0;
}
