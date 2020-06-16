#include "UnionFindTree.hpp"
#include <iostream>

namespace utils{
  int UnionFindTree::find(int x) {
    if(x == par[x]) return x;
    return par[x] = find(par[x]);
  }

  bool UnionFindTree::same(int x, int y) {
    return find(x) == find(y);
  }

  void UnionFindTree::unite(int x, int y) {
    x = find(x);
    y = find(y);
    if(x == y) return;
    if(rnk[x] < rnk[y]) {
      par[x] = y;
      size[y] += size[x];
    } else {
      if(rnk[x] == rnk[y]) rnk[x]++;
      par[y] = x;
      size[x] += size[y];
    }
  }

  std::vector<std::vector<int> > UnionFindTree::getSets() {
    std::vector<std::vector<int> > tmp_vec(n);
    for(int i = 0; i < n; i++) tmp_vec[find(i)].push_back(i);
    
    std::vector<std::vector<int> > ret_vec;
    for(int i = 0; i < tmp_vec.size(); i++)
      if(tmp_vec[i].size() > 0) ret_vec.push_back(tmp_vec[i]);
    
    return ret_vec;
  }

  void UnionFindTree::dump() {
    std::vector<std::vector<int> > sets = this->getSets();
    for(int i=0; i<sets.size(); i++){
      for(int j=0; j<sets[i].size(); j++){
	std::cout << j << " ";
      }
      std::cout << std::endl;
    }
  }
}
