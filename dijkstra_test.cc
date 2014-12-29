// Unit tests for Dijkstra algorithm and its data structures.

#include <unordered_map>

#include "dijkstra.h"
#include "gtest/gtest.h"

using namespace std;

TEST(priority_queue_test_suite, test_initialization) {
  PriorityQueue pq = PriorityQueue();
  EXPECT_EQ(0, pq.size()) << "Initial queue size should be 0.";
}

TEST(priority_queue_test_suite, test_insert_top) {
  PriorityQueue pq = PriorityQueue();
  int total = 3;
  for(int i = 0; i < total; i++)
    pq.insert(i, i);
  EXPECT_EQ(total, pq.size()) << "Initial queue size should be "
			      << total << ".";
  for(int i = 0, size = total-1; i < total; i++, size--) {
    QueueElement element = pq.top();
    EXPECT_EQ(i, element.get_vertex()) << "Top vertex should be "
				       << i << ".";
    EXPECT_EQ(static_cast<double>(i),
	      element.get_priority()) << "Top priority should be "
				      << i << ".";
    EXPECT_EQ(size, pq.size()) << "The queue size should be "
			       << size << ".";
  }
}

TEST(priority_queue_test_suite, test_heap_sort) {
  PriorityQueue pq = PriorityQueue();
  int total = 10;
  for(int i = total-1; i >= 0; i--)
    pq.insert(i, i);
  EXPECT_EQ(total, pq.size()) << "Initial queue size should be "
			      << total << ".";
  for(int i = 0; i < total; i++) {
    QueueElement element = pq.top();
    EXPECT_EQ(i, element.get_vertex()) << "Top vertex should be "
				       << i << ".";
    EXPECT_EQ(static_cast<double>(i),
	      element.get_priority()) << "Top priority should be "
				      << i << ".";
  }
  EXPECT_EQ(0, pq.size()) << "Final queue size should be 0.";
}

TEST(priority_queue_test_suite, test_change_priority) {
  PriorityQueue pq = PriorityQueue();
  int total = 10;
  for(int i = 0, priority = total; i < total; i++, priority++)
    pq.insert(i, priority);
  EXPECT_EQ(total, pq.size()) << "Initial queue size should be "
			      << total << ".";
  for(int i = total-1, priority = 0; i >= 0; i--, priority++)
    pq.insert(i, priority);
  for(int i = total-1, priority = 0; i >= 0; i--, priority++) {
    QueueElement element = pq.top();
    EXPECT_EQ(i, element.get_vertex()) << "Top vertex should be "
					<< i << ".";
    EXPECT_EQ(static_cast<double>(priority),
	      element.get_priority()) << "Top priority should be "
				      << priority << ".";
  }
  EXPECT_EQ(0, pq.size()) << "Final queue size should be 0.";
}

TEST(graph_test_suite, test_single_vertex) {
  Graph graph = Graph();
  graph.add_edge(0, 0);
  unordered_map<int, double> neighbor_info = graph.get_neighbor_info(0);
  EXPECT_EQ(0, neighbor_info.size()) << "Neighbor list size should be 0.";
}

TEST(graph_test_suite, test_add_duplicate_edges) {
  Graph graph = Graph();
  unordered_map<int, double> neighbor_info;
  
  graph.add_edge(0, 1);
  graph.add_edge(1, 0);
  EXPECT_TRUE(graph.has_vertex(0)) << "Graph should have vertex: 0.";
  EXPECT_TRUE(graph.has_vertex(1)) << "Graph should have vertex: 1.";
  neighbor_info = graph.get_neighbor_info(0);
  EXPECT_EQ(1, neighbor_info.size()) <<
    "Map size for neighbors from 0 should be 1.";
  neighbor_info = graph.get_neighbor_info(1);
  EXPECT_EQ(1, neighbor_info.size()) <<
    "Map size for neighbors from 1 should be 1.";
}

TEST(graph_test_suite, test_get_neighbor_info) {
  unordered_map<int, double> vertex_map = {{2, 20}, {3, 30}};
  Graph graph = Graph();
  int neighbor;

  for(auto& key_val: vertex_map)
    graph.add_edge(1, key_val.first, key_val.second);
  unordered_map<int, double> neighbor_info = graph.get_neighbor_info(1);
  for(auto& info: neighbor_info) {
    neighbor = info.first;
    unordered_map<int, double>::const_iterator result =
      vertex_map.find(neighbor);
    EXPECT_TRUE(result != vertex_map.end())
      << "Edge to unknown neighbor: " << neighbor << " is found.";
    EXPECT_EQ(result->second, info.second) <<
      "Distance to neighbor: " << neighbor << "should be " <<
      result->second << ".";
  }
}

void check_dijkstra_results(int source,
			    unordered_map<int, double>& distance_map,
			    unordered_map<int, double>& results) {
  int exp_size = distance_map.size();
  double exp_distance;
  EXPECT_EQ(exp_size, results.size()) <<
    "Result size of shortest paths from " << source << " should be " <<
    exp_size << ".";
  for(auto& vd: results) {
    exp_distance = distance_map[vd.first];
    EXPECT_EQ(exp_distance, vd.second) <<
      "Distance from " << source << " to " << vd.first <<
      " should be " << exp_distance << ".";
  }
} 

TEST(dijkstra_test_suite, test_single_vertex) {
  Graph graph = Graph();
  unordered_map<int, double> results, distance_map;

  graph.add_edge(0, 0);
  Dijkstra dsa = Dijkstra(graph);
  results = dsa.shortest_paths(0);
  distance_map = {{0, 0.0}};
  check_dijkstra_results(0, distance_map, results);
}

TEST(dijkstra_test_suite, test_graph1) {
  Graph graph = Graph();
  double exp_distance;
  unordered_map<int, double> results, distance_map;

  graph.add_edge(0, 1, 1);
  graph.add_edge(0, 2, 3);
  graph.add_edge(1, 0, 1);
  graph.add_edge(1, 2, 1);
  graph.add_edge(2, 1, 1);
  graph.add_edge(2, 3, 3);
  Dijkstra dsa = Dijkstra(graph);

  // Shortest path from 0. 
  results = dsa.shortest_paths(0);
  distance_map = {
    {0, 0.0},
    {1, 1.0},
    {2, 2.0},
    {3, 5.0}};
  check_dijkstra_results(0, distance_map, results);

  // Shortest path from 1. 
  results = dsa.shortest_paths(1);
  distance_map = {
    {0, 1.0},
    {1, 0.0},
    {2, 1.0},
    {3, 4.0}};
  check_dijkstra_results(1, distance_map, results);

  // Shortest path from 2. 
  results = dsa.shortest_paths(2);
  distance_map = {
    {0, 2.0},
    {1, 1.0},
    {2, 0.0},
    {3, 3.0}};
  check_dijkstra_results(2, distance_map, results);

  // Shortest path from 3. 
  results = dsa.shortest_paths(3);
  distance_map = {
    {0, 5.0},
    {1, 4.0},
    {2, 3.0},
    {3, 0.0}};
  check_dijkstra_results(3, distance_map, results);
}

TEST(dijkstra_test_suite, test_graph2) {
  Graph graph = Graph();
  double exp_distance;
  unordered_map<int, double> results, distance_map;

  graph.add_edge(0, 1, 1);
  graph.add_edge(0, 2, 4);
  graph.add_edge(1, 0, 1);
  graph.add_edge(1, 2, 2);
  graph.add_edge(1, 3, 5);
  graph.add_edge(2, 0, 4);
  graph.add_edge(2, 1, 2);
  graph.add_edge(2, 3, 1);
  graph.add_edge(3, 1, 5);
  graph.add_edge(3, 2, 1);
  Dijkstra dsa = Dijkstra(graph);

  // Shortest path from 0. 
  results = dsa.shortest_paths(0);
  distance_map = {
    {0, 0.0},
    {1, 1.0},
    {2, 3.0},
    {3, 4.0}};
  check_dijkstra_results(0, distance_map, results);

  // Shortest path from 1. 
  results = dsa.shortest_paths(1);
  distance_map = {
    {0, 1.0},
    {1, 0.0},
    {2, 2.0},
    {3, 3.0}};
  check_dijkstra_results(1, distance_map, results);

  // Shortest path from 2. 
  results = dsa.shortest_paths(2);
  distance_map = {
    {0, 3.0},
    {1, 2.0},
    {2, 0.0},
    {3, 1.0}};
  check_dijkstra_results(2, distance_map, results);

  // Shortest path from 3. 
  results = dsa.shortest_paths(3);
  distance_map = {
    {0, 4.0},
    {1, 3.0},
    {2, 1.0},
    {3, 0.0}};
  check_dijkstra_results(3, distance_map, results);
}

TEST(dijkstra_test_suite, test_graph3) {
  Graph graph = Graph();
  double exp_distance;
  unordered_map<int, double> results, distance_map;

  graph.add_edge(1, 2, 7);
  graph.add_edge(1, 3, 9);
  graph.add_edge(1, 6, 14);
  graph.add_edge(2, 1, 7);
  graph.add_edge(2, 3, 10);
  graph.add_edge(2, 4, 15);
  graph.add_edge(3, 4, 11);
  graph.add_edge(3, 6, 2);
  graph.add_edge(4, 5, 6);
  graph.add_edge(5, 6, 9);
  Dijkstra dsa = Dijkstra(graph);

  // Shortest path from 1. 
  results = dsa.shortest_paths(1);
  distance_map = {
    {1, 0.0},
    {2, 7.0},
    {3, 9.0},
    {4, 20.0},
    {5, 20.0},
    {6, 11.0}};
  check_dijkstra_results(1, distance_map, results);
}
