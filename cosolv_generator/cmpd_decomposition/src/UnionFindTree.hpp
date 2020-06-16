#include <vector>

#ifndef _UNION_FIND_TREE_H_
#define _UNION_FIND_TREE_H_

namespace utils{

  class UnionFindTree {
    int n;
    std::vector<int> rnk;
    std::vector<int> par;
    std::vector<int> size;
  public:
    UnionFindTree(int n) : n (n){
      rnk = std::vector<int>(n, 0);
      par = std::vector<int>(n);
      size = std::vector<int>(n, 1);
      for(int i = 0; i < n; i++)
        par[i] = i;
    }

    int find(int x);
    bool same(int x, int y);    
    void unite(int x, int y);
    int getSize(int x) { return size[find(x)]; }
    
    std::vector<std::vector<int> > getSets();
    void dump();
  };
  
}

#endif
