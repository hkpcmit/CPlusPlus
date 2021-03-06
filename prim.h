// Header file for Prim algorithm and its data structures.

#ifndef PRIM_H_
#define PRIM_H_

#include <cstdlib>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;
const int INIT_CAPACITY = 50;     // Initial priority queue capacity.

// Queue element encapsulates the priority and vertex ID of element
// that will be inserted into the priority queue. 
class QueueElement {
 public:
  // Construct a queue element.
  QueueElement(int vertex, double priority, int parent)   
    : vertex_(vertex),
      priority_(priority),
      parent_(parent) {}
  // Return the parent vertex.
  int get_parent() { return parent_; }
  // Return the priority of the element.
  double get_priority() { return priority_; }
  // Return the vertex ID of the element.
  int get_vertex() { return vertex_; }
  // Set or modify the parent of the element.
  void set_parent(int vertex) { parent_ = vertex; }
  // Set or modify the priority of the element.
  void set_priority(double value) { priority_ = value; }

 private:
  // Parent vertex.
  int parent_;
  // Priority of the element.
  double priority_;
  // Vertex ID of the element.
  int vertex_;
};

// A priority queue for Prim algorithm.  The instance allows:
// - insertion of vertices and their priorities as queue elements.
// - retrieval of vertex with the minimum priority.
class PriorityQueue {
 public:
  // Construct the priority queue.
  PriorityQueue()
    : priority_queue_({}), vertex_map_({}) {
    // Reserve capacity for the queue vector.
    priority_queue_.reserve(INIT_CAPACITY);
  }
  // Delete the priority queue.
  ~PriorityQueue() {
    // Free the priority queue capacity.
    vector<QueueElement>().swap(priority_queue_);
  }
  // Insert queue element for vertex, its priority and parent vertex.
  // The method checks if the given vertex already exists in the queue.
  // If so, its existing priority is compared with the given priority
  // and checked if the priority needs to be updated.  If the vertex is
  // not found in the queue, it will be pushed onto the queue. 
  void insert(int vertex, double priority, int parent);
  // Return the number of queue elements.
  int size() { return priority_queue_.size(); }
  // Pop and return the queue element at the top of the priority queue.
  QueueElement top();

 private:
  // Change priority of the given vertex within the priority queue.
  // Optionally, change parent vertex if provided.  For the purpose of
  // Prim algorithm, only the reduction of priority is supported.
  void change_priority(int vertex, double priority, int parent=0);
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
  // Push the vertex, priority and parent vertex into the priority queue.
  void push(int vertex, double priority, int parent);
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

// Graph representation for Prim algorithm.  It uses edge maps to
// represent edges.  See description of vertex_edge_map_ below for
// more details.
class Graph {
 public:
  // Construct the graph.
  Graph()
    : vertex_edge_map_({}),
      edge_costs_(0),
      vertices_({}) {}
  // Construct the graph by reading from file.
  Graph(string& filename)
    : vertex_edge_map_({}),
      edge_costs_(0),
      vertices_({}) { read_file(filename); }
  // Add edge to the graph.
  void add_edge(int vertex1, int vertex2, double cost=0);
  // Get all neighbor info from the given vertex.  The neighbor info is
  // represented as map with the following format:
  // {{neighbor 1, cost 1}, ..., {neighbor i, cost i}}
  // Such representation enables:
  // - faster look-up of neighbors and their costs.
  // - convenient iteration of all neighbors for a given vertex.
  unordered_map<int, double> get_neighbor_info(int vertex) {
    return vertex_edge_map_[vertex];
  }
  // Return a vertex from the graph.
  int get_vertex() { return vertex_edge_map_.begin()->first; }
  // Check if the graph contains the given vertex.
  bool has_vertex(int vertex) {
    return vertex_edge_map_.count(vertex) != 0; }
  // Print edges.  To be consistent with the input graph data, this will
  // print each edge twice in the following format:
  // - vertex1 vertex2
  // - vertex1 vertex3
  // - vertex2 vertex1
  // - vertex3 vertex1
  void print_edges();
  // Return the (vertex) size of the graph.
  int size() { return vertex_edge_map_.size(); }
  // Return total edge costs.
  double edge_costs() { return edge_costs_; };

 private:
  // Get next token from string line.
  string get_token(istringstream& ss) {
    string token;
    getline(ss, token, ' ');
    return token;
  }
  // Parse edge line and get vertices and cost.
  void parse_edge_line(string& line, int& vertex1,
		       int& vertex2, double& cost);
  // Print neighbors for the given vertex.
  void print_neighbors(int vertex);
  // Read graph from file.
  void read_file(string& filename);
  // Convert string to double.
  double string_to_double(string str) { return atof(str.c_str()); }
  // Convert string to integer.
  int string_to_int(string str) { return atoi(str.c_str()); }
  // Total edge costs.
  double edge_costs_;
  // Edge mapping of vertex to its neighbors.  The edges are represented
  // in two maps with the following format:
  //   {{vertex 1, {{neighbor 1, cost 1},
  //                ...,
  //                {neighbor i, cost i}},
  //    ...,
  //    {vertex j, {{neighbor 1, cost 1},
  //                ...,
  //                {neighbor k, cos k}}}}
  // Such representation serves the following purposes:
  // - Avoid addition of duplicate edges.
  // - Enable O(E) run-time to visit all edges.
  // - Faster look-up of all neighbors and their costs from a given
  //   vertex.
  // - Convenient iteration of all neighbors for a given vertex.
  unordered_map<int, unordered_map<int, double> > vertex_edge_map_;
  // Ordered vertex set.
  set<int> vertices_;
};

// A class to compute MST using Prim algorithm.  Following similar
// approach in implementing the shortest-path algorithm in HW2, the
// Prim algorithm is implmented as a separate class instead of being
// a class method of Graph.  Such approach is more modular and enables
// the algorithm to work with a variety of graphs.  
class Prim {
 public:
  // Construct the Prim algorithm instance.
  Prim(Graph& graph)
    : graph_(graph), priority_queue_(PriorityQueue()) {}
  // Compute and return MST as a subgraph of the original graph.
  // Ideally, the returned graph should be a derived class of Graph
  // representing MST.  This approach can be implemented after
  // inheritance is taught in the near future.
  // Note that the MST is not unique if the original graph is disjoint.
  Graph mst();

 private:
  // Explore all edges from the given vertex.
  void explore_edges_from(int vertex,
			  const unordered_set<int>& closest_set);
  // Get the queue element with the next closest vertex and update the
  // closest set and MST.
  QueueElement get_closest_element(unordered_set<int>& closest_set,
				   Graph& mst);
  // Undirected graph.
  Graph& graph_;
  // Priority queue.
  PriorityQueue priority_queue_;
};

#endif // PRIM_H_
