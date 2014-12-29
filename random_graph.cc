// Homework 2
// Implment random graph to compute single-source shortest paths.

#include "dijkstra.h"
#include "random_graph.h"

#include <assert.h>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <unordered_map>

// Maximum vertex identifier.
const int MAX_VERTEX_ID = INIT_VERTEX + MAX_VERTICES - 1; 
// Show individual average path distances.  For debugging only.
const bool SHOW_AVERAGE_DISTANCES = false;
using namespace std;

// =============
//  RandomGraph
// =============

// Generate a random graph.
void RandomGraph::get_random_graph() {
  for(int vertex1 = INIT_VERTEX; vertex1 < MAX_VERTEX_ID; vertex1++)
    for(int vertex2 = vertex1 + 1; vertex2 <= MAX_VERTEX_ID; vertex2++)
      if(random_edge()) {
	graph_.add_edge(vertex1, vertex2, random_distance());

	// For statistics.
	destinations_.emplace(vertex1);
	destinations_.emplace(vertex2);
	edges_++;
      }
}

// Compute shortest-path distances from INIT_VERTEX.
unordered_map<int, double> RandomGraph::shortest_paths() {
  Dijkstra dsa = Dijkstra(graph_);
  unordered_map<int, double> results = dsa.shortest_paths(INIT_VERTEX);
  return results;
}

// ============
//  Simulation
// ============

// Return average path distances over all shortest paths.
double Simulation::average() {
  double avg_distance, total = 0.0;
  int number = 0;

  for(int i = 1; i < MAX_VERTICES; i++) {
    avg_distance = stats_[i].average();

    // Skip vertex without average distance.  This should be unlikely.
    if(avg_distance == 0.0)
      continue;
    total += avg_distance;
    number++;
  }
  if(!number)
    return 0.0;
  return total / number;
}

// Process results of shortest-path computation in random graph.
void Simulation::process_results(unordered_map<int, double>& results) {
  for(auto& result: results)
    stats_[index(result.first)].add(result.second);
}

// Run simulation and collect statistics.
void Simulation::run() {
  unordered_map<int, double> results;

  for(int trial = 0; trial < trials_; trial++) {
    RandomGraph rgraph = RandomGraph(edge_density_, min_distance_, max_distance_);
    results = rgraph.shortest_paths();
    process_results(results);
  }
}

// Print simulation results.
void Simulation::print() {
  const char separator = ' ';
  const int field_width = 10;
  const int num_width = 10;
  char path[10];

  cout << "Edge density: " << edge_density_ << endl;
  cout << "  Average path distance: " << fixed << setprecision(2) << average()
       << endl;
  if(!SHOW_AVERAGE_DISTANCES)
    return;

  // For debugging only.
  cout << endl;
  cout << left << setw(field_width)<< setfill(separator) << "";
  cout << right << setw(field_width) << setfill(separator) << "Average" << endl;
  cout << left << setw(field_width) << setfill(separator) << "Paths";
  cout << right << setw(field_width) << setfill(separator) << "Distances" << endl;
  cout << "======================" << endl;
  for(int i = 1, destination = INIT_VERTEX+1; i < MAX_VERTICES; i++, destination++) {
    sprintf(path, "%d to %d", INIT_VERTEX, destination);
    cout << left << setw(field_width) << setfill(separator) << path;
    cout << right << setw(num_width) << setfill(separator)
	 << fixed << setprecision(2) << stats_[i].average() << endl;
  }
  cout << endl;
}
