#include <vector>

#include "gtest/gtest.h"
#include "UnionFindTree.hpp"

TEST(UnionFindTreeTest, UnionFindTree){
  std::vector<std::vector<int> > sets;  
  utils::UnionFindTree uf = utils::UnionFindTree(10);
  
  uf.unite(3,5);

  sets = uf.getSets();
  EXPECT_EQ(2, sets[3].size());

  uf.unite(4,6);
  uf.unite(3,4);

  sets = uf.getSets();
  EXPECT_EQ(4, sets[3].size());
}

