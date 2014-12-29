// Homework 2
// Implement Dijkstra algorithm to compute single-source shortest paths.

#include "dijkstra.h"

#include <assert.h>
#include <iostream>
#include <unordered_set>

using namespace std;

// ===============
//  PriorityQueue
// ===============

// Change priority of the given vertex within the priority queue.  For
// the purpose of Dijkstra algorithm, only the reduction of priority is
// supported.
void PriorityQueue::change_priority(int vertex, double priority) {
  assert(contains(vertex));
  int position = vertex_map_[vertex];
  QueueElement& this_element = priority_queue_[position];
  assert(this_element.get_priority() > priority);
  
  // Change priority and percolate upward.
  this_element.set_priority(priority);
  percolate_up(position);
}

// Insert the vertex and priority into the priority queue.  Check if the
// given vertex already exists in the queue and if its existing priority
// needs to be updated.
void PriorityQueue::insert(int vertex, double priority) {
  if(contains(vertex)) {
    int position = vertex_map_[vertex];
    QueueElement& this_element = priority_queue_[position];

    // Check if the priority for this vertex needs to be updated.
    if(this_element.get_priority() > priority)
      change_priority(vertex, priority);
    return;
  }

  // Push this new vertex onto the priority queue.
  push(vertex, priority);
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

// Push the vertex and priority into the priority queue.
void PriorityQueue::push(int vertex, double priority) {
  assert(!contains(vertex));
  int position;

  // Insert the vertex to the tail of priority queue first.
  position = priority_queue_.size();
  priority_queue_.push_back(QueueElement(vertex, priority));

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
void Graph::add_edge(int vertex1, int vertex2, double distance) {
  int temp;
  unordered_map<int, double>& neighbor_info1 = vertex_edge_map_[vertex1];
  unordered_map<int, double>& neighbor_info2 = vertex_edge_map_[vertex2];

  // Don't add an edge from a vertex to itself.
  if(vertex1 == vertex2)
    return;
  neighbor_info1.emplace(vertex2, distance);
  neighbor_info2.emplace(vertex1, distance);
}

// ==========
//  Dijkstra 
// ==========

// Get the queue element with the next closest vertex and update the
// closest set and shortest path vector.
QueueElement Dijkstra::get_closest_element(unordered_set<int>& closest_set,
					   unordered_map<int, double>& spaths) {
  QueueElement element = priority_queue_.top();
  int vertex = element.get_vertex();

  // Some implementation may choose to push vertex with reduced priority onto
  // the priority queue even though it is already in the queue.  The use of
  // .emplace() in the following code should handle this without changing the
  // shortest-path distances.
  closest_set.emplace(vertex);
  spaths.emplace(vertex, element.get_priority());
  return element;
}

// Explore all edges from the given vertex.
void Dijkstra::explore_edges_from(int vertex, double distance,
				  const unordered_set<int>& closest_set) {
  int neighbor;
  double priority;

  for(auto& info: graph_.get_neighbor_info(vertex)) {
    neighbor = info.first;

    // Skip this neighbor if it is already a closest one.
    if(closest_set.count(neighbor))
      continue;
    priority = distance + info.second;

    // The algorithm requires us to compare this priority/distance with any
    // existing one.  This will require some table for look-up.  Since the
    // priority queue already has such info, we let it handle this on our
    // behalf.
    priority_queue_.insert(neighbor, priority);
  } 
}

// Compute shortest paths from the given source.
unordered_map<int, double> Dijkstra::shortest_paths(int source) {
  unordered_set<int> closest_set = {};    // Contains closest vertices.
  unordered_map<int, double> spaths = {}; // Map of shortest paths.

  // Initialize the priority queue.
  priority_queue_.insert(source, 0);
  while(priority_queue_.size()) {
    QueueElement element = get_closest_element(closest_set, spaths);
    explore_edges_from(element.get_vertex(), element.get_priority(),
		       closest_set);
  }
  return spaths;
}
