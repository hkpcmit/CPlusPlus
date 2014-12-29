// Unit tests for Prim algorithm and its data structures using Googletest:
//   http://code.google.com/p/googletest/

#include "prim.h"
#include "gtest/gtest.h"

#include <string>

using namespace std;

TEST(prim_test_suite, test_sample1) {
  Graph graph = Graph();
  graph.add_edge(1, 2, 1);
  graph.add_edge(1, 4, 3);
  graph.add_edge(1, 3, 4);
  graph.add_edge(2, 4, 2);
  graph.add_edge(3, 4, 5);
  Prim prim = Prim(graph);
  EXPECT_EQ(prim.mst().edge_costs(), 7)
    << "Min. cost should be 7.";
}

TEST(prim_test_suite, test_sample2) {
  Graph graph = Graph();
  graph.add_edge(1, 2, 2);
  graph.add_edge(1, 4, 4);
  graph.add_edge(2, 3, 4);
  graph.add_edge(2, 4, 4);
  graph.add_edge(2, 5, 3);
  graph.add_edge(2, 6, 1);
  graph.add_edge(3, 6, 5);
  graph.add_edge(4, 5, 2);
  graph.add_edge(6, 5, 5);
  Prim prim = Prim(graph);
  EXPECT_EQ(prim.mst().edge_costs(), 12)
    << "Min. cost should be 12.";
}

TEST(prim_test_suite, test_sample3) {
  Graph graph = Graph();
  graph.add_edge(1, 2, 3);
  graph.add_edge(1, 6, 2);
  graph.add_edge(2, 3, 17);
  graph.add_edge(2, 4, 16);
  graph.add_edge(3, 4, 8);
  graph.add_edge(3, 9, 18);
  graph.add_edge(4, 5, 11);
  graph.add_edge(4, 9, 4);
  graph.add_edge(5, 6, 1);
  graph.add_edge(5, 7, 6);
  graph.add_edge(5, 8, 5);
  graph.add_edge(5, 9, 10);
  graph.add_edge(6, 7, 7);
  graph.add_edge(7, 8, 15);
  graph.add_edge(8, 9, 12);
  graph.add_edge(8, 10, 13);
  graph.add_edge(9, 10, 9);
  Prim prim = Prim(graph);
  EXPECT_EQ(prim.mst().edge_costs(), 48)
    << "Min. cost should be 48.";
}

TEST(prim_test_suite, test_sample4) {
  Graph graph = Graph();
  graph.add_edge(4, 5, 0.35);
  graph.add_edge(4, 6, 0.93);
  graph.add_edge(4, 7, 0.37);
  graph.add_edge(5, 7, 0.28);
  graph.add_edge(0, 7, 0.16);
  graph.add_edge(1, 5, 0.32);
  graph.add_edge(0, 4, 0.38);
  graph.add_edge(2, 3, 0.17);
  graph.add_edge(1, 7, 0.19);
  graph.add_edge(0, 2, 0.26);
  graph.add_edge(1, 2, 0.36);
  graph.add_edge(1, 3, 0.29);
  graph.add_edge(2, 7, 0.34);
  graph.add_edge(6, 2, 0.40);
  graph.add_edge(3, 6, 0.52);
  graph.add_edge(6, 0, 0.58);
  graph.add_edge(6, 4, 0.93);
  Prim prim = Prim(graph);
  Graph prim_mst = prim.mst();
  EXPECT_EQ(prim_mst.edge_costs(), 1.81)
    << "Min. cost should be 1.81.";
}

TEST(prim_test_suite, test_sample5) {
  // Homework from Stanford's Algorithms: Design and Analysis II.
  string filename = "edges.txt"; 
  Graph graph = Graph(filename);
  Prim prim = Prim(graph);
  EXPECT_EQ(prim.mst().edge_costs(), -3612829)
    << "Min. cost should be -3612829.";
}
