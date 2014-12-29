// Header file for random graph and its data structure.

#ifndef RANDOM_GRAPH_H_
#define RANDOM_GRAPH_H_

#include "dijkstra.h"

#include <chrono>
#include <iostream>
#include <random>
#include <unordered_map>
#include <unordered_set>

const int INIT_VERTEX = 1;  // Initial vertex.
using namespace std;

// A class to generate a random graph and calculate single-source
// shortest paths in the graph using Dijkstra algorithm.
class RandomGraph {
 public:
  // Construct a random graph.
  RandomGraph(double edge_density, double min_distance, double max_distance)
    : edge_density_(edge_density),
      min_distance_(min_distance),
      max_distance_(max_distance),
      destinations_({INIT_VERTEX}),
      edges_(0),
      graph_(Graph()),
      seed_(chrono::system_clock::now().time_since_epoch().count()),
      generator_(default_random_engine(seed_)),
      distance_distribution_(uniform_real_distribution<double>(min_distance_,
							       max_distance_)),
      edge_distribution_(uniform_real_distribution<double>(0.0, 1.0)) {
    get_random_graph();
  }
  // Compute shortest-path distances from INIT_VERTEX.  Return
  // destination-to-distance map in the following format:
  // {{destination 1, SP distance 1}, ..., {destination i, SP distance i}}
  unordered_map<int, double> shortest_paths();

 private:
  // Generate a random graph.
  void get_random_graph();
  // Return random distance.
  double random_distance() { return distance_distribution_(generator_); }
  // Randomly pick an edge based on edge density.
  bool random_edge() { return edge_distribution_(generator_) < edge_density_; }
  // Destination sets.
  unordered_set<int> destinations_;
  // Edge density.
  double edge_density_;
  // Total amount of edges.
  int edges_;
  // Undirected graph.
  Graph graph_;
  // Maximum distance.
  double max_distance_;
  // Minimum distance.
  double min_distance_;
  // Seed for random number generator.
  unsigned seed_;
  // Random generator.
  default_random_engine generator_;
  // Uniform distribution for distances.
  uniform_real_distribution<double> distance_distribution_;
  // Uniform distribution for edges.
  uniform_real_distribution<double> edge_distribution_;
}; 

// A class to collect and compute shortest-path distance statistics.
class SPDistanceStats {
 public:
  // Construct a satistics instance.
  SPDistanceStats()
    : sum_(0.0), total_(0) {}
  // Add distance data point.
  void add(double distance) {
    sum_ += distance;
    total_++;
  }
  // Compute average.
  double average() {
    if(total_)
      return sum_ / total_;
    return 0.0;
  }
 private:
  // Sum of distances.
  double sum_;
  // Total number of distance data points.
  int total_;
};

// A class to simulate and run multiple trials of shortest-path
// computation in random graphs.
class Simulation {
 public:
  // Construct a simulation instance.
  Simulation(double edge_density, double min_distance,
	     double max_distance, int trials)
    : edge_density_(edge_density),
      max_distance_(max_distance),
      min_distance_(min_distance),
      stats_(vector<SPDistanceStats>(MAX_VERTICES)),
      trials_(trials) {}
  // Run simulation and collect statistics.
  void run();
  // Print simulation results.
  void print();
 private:
  // Return average path distances over all shortest paths.
  double average();
  // Process results of shortest-path computation in random graph.
  void process_results(unordered_map<int, double>& results);
  // Index for vertex.
  int index(int vertex) { return vertex - INIT_VERTEX; }
  // Edge density.
  double edge_density_;
  // Maximum distance.
  double max_distance_;
  // Minimum distance.
  double min_distance_;
  vector<SPDistanceStats> stats_;
  // Total number of trials.
  int trials_;
};

#endif // RANDOM_GRAPH_H_
