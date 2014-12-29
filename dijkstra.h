// Header file for Dijkstra algorithm and its data structures.

#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;
const int MAX_VERTICES = 50;     // Maximum amount of vertices.

// Queue element encapsulates the priority and vertex ID of element
// that will be inserted into the priority queue. 
class QueueElement {
 public:
  // Construct a queue element.
  QueueElement(int vertex, double priority)   
    : vertex_(vertex), priority_(priority) {}
  // Return the priority of the element.
  double get_priority() { return priority_; }
  // Return the vertex ID of the element.
  int get_vertex() { return vertex_; }
  // Set or modify the priority of the element.
  void set_priority(double value) { priority_ = value; }

 private:
  // Priority of the element.
  double priority_;
  // Vertex ID of the element.
  int vertex_;
};

// A priority queue for Dijkstra algorithm.  The instance allows:
// - insertion of vertices and their priorities as queue elements.
// - retrieval of vertex with the minimum priority.
class PriorityQueue {
 public:
  // Construct the priority queue.
  PriorityQueue()
    : priority_queue_({}), vertex_map_({}) {
    // Reserve capacity for the queue vector.
    priority_queue_.reserve(MAX_VERTICES);
  }
  // Delete the priority queue.
  ~PriorityQueue() {
    // Free the priority queue capacity.
    vector<QueueElement>().swap(priority_queue_);
  }
  // Insert queue element for vertex and its priority.  The method
  // checks if the given vertex already exists in the queue.  If so,
  // its existing priority is compared with the given priority and
  // checked if the priority needs to be updated.  If the vertex is
  // not found in the queue, it will be pushed onto the queue. 
  void insert(int vertex, double priority);
  // Return the number of queue elements.
  int size() { return priority_queue_.size(); }
  // Pop and return the queue element at the top of the priority queue.
  QueueElement top();

 private:
  // Change priority of the given vertex within the priority queue.
  void change_priority(int vertex, double priority);
  // Check if the priority queue contains the given vertex.
  bool contains(int vertex) { return vertex_map_.count(vertex) != 0; }
  // Return the position of left child for a queue element at the
  // given position.
  int left_child(int position) { return position*2 + 1; }
  // Return the position of right child for a queue element at the
  // given position.
  int right_child(int position) { return left_child(position) + 1; }
  // Return the position of the child with the minimum priority.
  int min_child(int position);
  // Return parent position of a given queue element within the queue.
  int parent(int position) { return (position + 1)/2 - 1; }
  // Move the queue element downward to its rightful position.
  void percolate_down(int position);
  // Move the queue element upward to its rightful position.
  void percolate_up(int position);
  // Push the vertex and priority into the priority queue.
  void push(int vertex, double priority);
  // Swap queue elements for the given pair of queue positions.
  void swap(int position1, int position2);
  // Return if the given position is a valid one in the priority queue.
  bool valid_position(int position) {
    return (0 <= position) && (position <= size() - 1);
  }
  // Priority queue vector.
  vector<QueueElement> priority_queue_; 
  // Mapping of vertex to queue position.
  unordered_map<int, int> vertex_map_;
};

// Graph representation for Dijkstra algorithm.  In light of the small
// edge densities (less than 0.5), it uses edge maps to represent edges
// since the graph for this homework is relatively sparse.  See
// description of vertex_edge_map_ below for more details.
class Graph {
 public:
  // Construct the graph.
  Graph()
    : vertex_edge_map_({}) {}
  // Add edge to the graph.
  void add_edge(int vertex1, int vertex2, double distance=0);
  // Get all neighbor info from the given vertex.  The neighbor info is
  // represented as map with the following format:
  // {{neighbor 1, distance 1}, ..., {neighbor i, distance i}}
  // Such representation enables:
  // - faster look-up of neighbors and their distances.
  // - convenient iteration of all neighbors for a given vertex.
  unordered_map<int, double> get_neighbor_info(int vertex) {
    return vertex_edge_map_[vertex];
  }
  // Check if the graph contains the given vertex.
  bool has_vertex(int vertex) {
    return vertex_edge_map_.count(vertex) != 0; }
  // Return the (vertex) size of the graph.
  int size() { return vertex_edge_map_.size(); }

 private:
  // Edge mapping of vertex to its neighbors.  The edges are represented in
  // two maps with the following format:
  //   {{vertex 1, {{neighbor 1, distance 1},
  //                ...,
  //                {neighbor i, distance i}},
  //    ...,
  //    {vertex j, {{neighbor 1, distance 1},
  //                ...,
  //                {neighbor k, distance k}}}}
  // Such representation serves the following purposes:
  // - Avoid addition of duplicate edges.
  // - Enable O(E) run-time to visit all edges for Dijkstra algorithm.
  //   (Contrast with O(V^2) for edge matrix representation.)
  // - Faster look-up of all neighbors and their distances from a given
  //   vertex.
  // - Convenient iteration of all neighbors for a given vertex.
  unordered_map<int, unordered_map<int, double> > vertex_edge_map_;
};

// A class to compute single-source shortest-paths using Dijkstra algorithm.
class Dijkstra {
 public:
  // Construct the Dijkstra algorithm instance.
  Dijkstra(Graph& graph)
    : graph_(graph), priority_queue_(PriorityQueue()) {}
  // Compute shortest paths from the given source.  The results are returned
  // in map with the following format:
  // {{destination 1, distance 1}, ..., {destination i, distance i}}
  // This enables faster look-up of the shortest path distance for a given
  // destination.
  // Ideally, this method should also return the parent vertex for each
  // destination vertex so that the path between the source and destination
  // vertices can be determined.  However since this info is not required in
  // this assignment, such result is not provided.
  unordered_map<int, double> shortest_paths(int source);

 private:
  // Explore all edges from the given vertex.
  void explore_edges_from(int vertex, double distance,
			  const unordered_set<int>& closest_set);
  // Get the queue element with the next closest vertex and update the
  // closest set and shortest path vector.
  QueueElement get_closest_element(unordered_set<int>& closest_set,
				   unordered_map<int, double>& spaths);
  // Undirected graph.
  Graph& graph_;
  // Priority queue.
  PriorityQueue priority_queue_;
};

#endif // DIJKSTRA_H_
