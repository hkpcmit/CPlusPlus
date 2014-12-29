// Homework 2
// Implement Prim algorithm to compute MST.

#include "prim.h"

#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>

using namespace std;

// ===============
//  PriorityQueue
// ===============

// Change priority of the given vertex within the priority queue.
void PriorityQueue::change_priority(int vertex,
				    double priority,
				    int parent) {
  assert(contains(vertex));
  int position = vertex_map_[vertex];
  QueueElement& this_element = priority_queue_[position];

  assert(this_element.get_priority() > priority);
  if(parent)
    this_element.set_parent(parent);
  
  // Change priority and percolate upward.
  this_element.set_priority(priority);
  percolate_up(position);
}

// Insert the vertex and priority into the priority queue.  Check if the
// given vertex already exists in the queue and if its existing priority
// needs to be updated.
void PriorityQueue::insert(int vertex, double priority, int parent) {
  if(contains(vertex)) {
    int position = vertex_map_[vertex];
    QueueElement& this_element = priority_queue_[position];

    // Check if the priority for this vertex needs to be updated.
    if(this_element.get_priority() > priority)
      change_priority(vertex, priority, parent);
    return;
  }

  // Push this new vertex onto the priority queue.
  push(vertex, priority, parent);
}

// For a given position, return the position of the child with
// the minimum priority.
int PriorityQueue::min_child(int position) {
  int left_child_position = left_child(position);
  assert(valid_position(left_child_position));
  int right_child_position = right_child(position);
  
  // There is no right child, return the left one.
  if(!valid_position(right_child_position))
    return left_child_position;

  // Compare priorities and return the position for the min. child.
  QueueElement left_element = priority_queue_[left_child_position];
  QueueElement right_element = priority_queue_[right_child_position];
  if(left_element.get_priority() < right_element.get_priority())
    return left_child_position;
  return right_child_position;
}

// Recursively move the queue element downward to its rightful
// position.
void PriorityQueue::percolate_down(int position) {
  assert(valid_position(position));
  QueueElement this_element = priority_queue_[position];
  int min_child_position;

  // Base case.  Stop if there is no children.
  if(!valid_position(left_child(position)))
    return;
  min_child_position = min_child(position);
  QueueElement min_child_element = priority_queue_[min_child_position];

  // Check if this element is in its rightful position.
  if(this_element.get_priority() < min_child_element.get_priority())
    return;

  // Swap positions and continue percolation.
  swap(position, min_child_position);
  percolate_down(min_child_position);
}

// Recursively move the queue element upward to its rightful
// position.
void PriorityQueue::percolate_up(int position) {
  assert(valid_position(position));
  QueueElement this_element = priority_queue_[position];
  int parent_position;

  // Base case.
  if(position == 0)
    return;

  // Get parent of this element.
  parent_position = parent(position);
  QueueElement parent_element = priority_queue_[parent_position];

  // Check if this element is in its rightful position.
  if(parent_element.get_priority() <= this_element.get_priority())
    return;

  // Swap positions and continue percolation.
  swap(parent_position, position);
  percolate_up(parent_position);
}

// Push the vertex, priority and parent vertex into the priority queue.
void PriorityQueue::push(int vertex, double priority, int parent) {
  assert(!contains(vertex));
  int position;

  // Insert the vertex to the tail of priority queue first.
  position = priority_queue_.size();
  priority_queue_.push_back(QueueElement(vertex, priority, parent));

  // Update the queue position and percolate upward.
  vertex_map_.emplace(vertex, position);
  percolate_up(position);
}

// Pop and return the queue element at the top of the priority queue.
QueueElement PriorityQueue::top() {
  assert(!priority_queue_.empty());
  QueueElement top_element = priority_queue_[0];

  // Remove the vertex from the vertex_map.
  vertex_map_.erase(top_element.get_vertex());

  QueueElement last_element = priority_queue_.back();
  priority_queue_.pop_back();

  // Just return if there is no element in the queue.
  if(size() == 0)
    return top_element;

  // Move the last element in the queue to the top and percolate
  // downward.
  priority_queue_[0] = last_element;
  vertex_map_[last_element.get_vertex()] = 0;
  percolate_down(0);
  return top_element;
}

// Swap queue elements for the given pair of queue positions.
void PriorityQueue::swap(int position1, int position2) {
  QueueElement e1 = priority_queue_[position1];
  QueueElement e2 = priority_queue_[position2];

  priority_queue_[position1] = e2;
  vertex_map_[e2.get_vertex()] = position1;
  priority_queue_[position2] = e1;
  vertex_map_[e1.get_vertex()] = position2;
}

// =======
//  Graph
// =======

// Add an edge to the graph.  Edge from a vertex to itself is not supported.
void Graph::add_edge(int vertex1, int vertex2, double cost) {
  unordered_map<int, double>& neighbor_info1 = vertex_edge_map_[vertex1];
  unordered_map<int, double>& neighbor_info2 = vertex_edge_map_[vertex2];

  // Don't add an edge from a vertex to itself.
  if(vertex1 == vertex2)
    return;
  neighbor_info1.emplace(vertex2, cost);
  neighbor_info2.emplace(vertex1, cost);
  edge_costs_ += cost;
  vertices_.emplace(vertex1);
  vertices_.emplace(vertex2);
}

// Parse edge line and get vertices and cost.
void Graph::parse_edge_line(string& line, int& vertex1,
			    int& vertex2, double& cost) {
  istringstream ss(line);

  vertex1 = string_to_int(get_token(ss));
  vertex2 = string_to_int(get_token(ss));
  cost = string_to_double(get_token(ss));
}

// Print all edges.
void Graph::print_edges() {
  cout << "Edges: " << endl;
  for(int vertex: vertices_)
    print_neighbors(vertex);
}

// Print neighbors for the given vertex.
void Graph::print_neighbors(int vertex) {
  unordered_map<int, double> neighbor_info = vertex_edge_map_[vertex];
  vector<int> neighbors;
  const char separator = ' ';
  const int num_width = 7;

  // Get and sort the neighbors.
  neighbors.reserve(neighbor_info.size());
  for(auto& kv: neighbor_info)
    neighbors.push_back(kv.first);
  sort(neighbors.begin(), neighbors.end());
  for(int neighbor: neighbors)
    cout << right << setw(num_width) << setfill(separator) << vertex
	 << right << setw(num_width) << setfill(separator) << neighbor << endl;
}

// Read graph from file.
void Graph::read_file(string& filename) {
  string line, token;
  int vertex1, vertex2;
  double cost;
  ifstream my_file(filename);
  assert(my_file.is_open());
  
  // Get number of vertices.
  getline(my_file, line);

  // Iterate and read each edge.
  while(getline(my_file, line)) {
    parse_edge_line(line, vertex1, vertex2, cost);
    add_edge(vertex1, vertex2, cost);
  }
}

// ======
//  Prim 
// ======

// Get the queue element with the next closest vertex and update the
// closest set and shortest path vector.
QueueElement Prim::get_closest_element(unordered_set<int>& closest_set,
				       Graph& mst) {
  QueueElement element = priority_queue_.top();
  int vertex = element.get_vertex();

  closest_set.emplace(vertex);
  mst.add_edge(element.get_parent(), vertex, element.get_priority());
  return element;
}

// Explore all edges from the given vertex.
void Prim::explore_edges_from(int vertex,
			      const unordered_set<int>& closest_set) {
  int neighbor;
  double priority;

  for(auto& info: graph_.get_neighbor_info(vertex)) {
    neighbor = info.first;

    // Skip this neighbor if it is already a closest one.
    if(closest_set.count(neighbor))
      continue;

    // The algorithm requires us to compare this priority/cost with any
    // existing one.  This will require some table for look-up.  Since the
    // priority queue already has such info, we let it handle this on our
    // behalf.
    priority_queue_.insert(neighbor, info.second, vertex);
  } 
}

// Compute and return MST.
Graph Prim::mst() {
  Graph prim_mst = Graph();
  int vertex = graph_.get_vertex();
  int graph_size = graph_.size();
  unordered_set<int> closest_set = {vertex};

  // Initialize the priority queue.
  for(auto& info: graph_.get_neighbor_info(vertex))
    priority_queue_.insert(info.first, info.second, vertex);
  while(priority_queue_.size()) {
    QueueElement element = get_closest_element(closest_set, prim_mst);
    explore_edges_from(element.get_vertex(), closest_set);
  }
  return prim_mst;
}
