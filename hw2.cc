// Homework 2
// Monte Carlo simulation that calculates average shortest paths
// in a random graph.

#include "random_graph.h"

#include <iostream>

const double MAX_DISTANCE = 10.0;   // Maximum distance.
const double MIN_DISTANCE = 1.0;    // Minimum distance.
const int TRIALS = 1000;            // Total trials.
using namespace std;

// Main routine.
int main() {
  vector<double> edge_densities = { 0.2, 0.4 };
  for(double density: edge_densities) {
    Simulation sim = Simulation(density, MIN_DISTANCE, MAX_DISTANCE, TRIALS);
    sim.run();
    sim.print();
  }
  return 0;
}
