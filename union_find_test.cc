// Unit tests for union-find data structures using Googletest:
//   http://code.google.com/p/googletest/

#include "union_find.h"
#include "gtest/gtest.h"

using namespace std;


TEST(union_find_test_suite, test_find_new_vertex) {
  UnionFind uf = UnionFind();
  Vertex vertex = 1;
  EXPECT_EQ(vertex, uf.find(vertex)) << "Find method should return vertex: "
				     << vertex;
}

TEST(union_find_test_suite, test_join_new_vertices) {
  UnionFind uf = UnionFind();
  Vertex vertex1 = 1, vertex2 = 2;
  uf.join(vertex1, vertex2);
  EXPECT_EQ(uf.find(vertex1), uf.find(vertex2))
    << "Both vertices should share the same root.";
  RootRank expect = { vertex1, 1 };
  EXPECT_EQ(expect, uf.get_root_rank(vertex1))
    << "vertex1 should have root: " << vertex1 << "; rank: 1.";
  expect = { vertex1, 0 };
  EXPECT_EQ(expect, uf.get_root_rank(vertex2))
    << "vertex1 should have root: " << vertex1 << "; rank: 0.";
}

TEST(union_find_test_suite, test_path_compression1) {
  UnionFind uf = UnionFind();
  uf.join(0, 1);
  uf.join(2, 3);
  uf.join(0, 2);
  EXPECT_EQ(uf.find(1), uf.find(3))
    << "Both vertices should share the same root.";
  RootRank expect = { 0, 0 };
  EXPECT_EQ(expect, uf.get_root_rank(3))
    << "vertex: 3 should have root: 0; rank: 1.";
  expect = { 0, 2 };
  EXPECT_EQ(expect, uf.get_root_rank(0))
    << "vertex: 0 should have root: 0; rank: 2.";
}

TEST(union_find_test_suite, test_path_compression2) {
  // http://www.cs.princeton.edu/~rs/AlgsDS07/01UnionFind.pdf
  UnionFind uf = UnionFind();
  uf.join(3, 4);
  uf.join(4, 9);
  uf.join(8, 0);
  uf.join(2, 3);
  uf.join(5, 6);
  uf.join(9, 5);
  uf.join(7, 3);
  uf.join(4, 8);
  uf.join(6, 1);
  vector<Vertex> vertices = { 1, 2, 4, 5, 6, 7, 8, 9 };
  for(Vertex v: vertices)
    EXPECT_EQ(3, uf.get_root_rank(v).root)
      << "vertex: " << v << " should have root: 3.";
}

TEST(union_find_test_suite, test_copy_union_find) {
  UnionFind uf1 = UnionFind();
  uf1.find(1);
  uf1.find(2);
  UnionFind uf2 = uf1;
  uf2.join(1, 2);
  EXPECT_EQ(1, uf1.find(1)) << "vertex: 1 should have root: 1";
  EXPECT_EQ(2, uf1.find(2)) << "vertex: 2 should have root: 2";
  EXPECT_EQ(1, uf2.find(1)) << "vertex: 1 should have root: 1";
  EXPECT_EQ(1, uf2.find(2)) << "vertex: 2 should have root: 1";
}
