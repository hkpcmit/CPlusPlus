// Implement union-find data structures.

#include "union_find.h"

#include <vector>

using namespace std;

// ===========
//  UnionFind
// ===========

// Return the root vertex of a given vertex.
Vertex UnionFind::find(Vertex vertex) {

  // Add new vertex.
  if(!vertex_map_.count(vertex)) {
    RootRank rr = { vertex, 0 };
    vertex_map_.emplace(vertex, rr);
    return vertex;
  }
  Vertex root = vertex_map_[vertex].root;
  if(vertex_map_[root].root == root)
    return root;
  return path_compression(vertex);
}

// Join a pair of vertices.
void UnionFind::join(Vertex vertex1, Vertex vertex2) {
  Vertex root1 = find(vertex1);
  Vertex root2 = find(vertex2);

  // Do nothing if both vertices share the same root.
  if(root1 == root2)
    return;
  
  // Ensure root1 has larger rank.
  if(vertex_map_[root2].rank > vertex_map_[root1].rank) {
    Vertex temp = root1;
    root1 = root2;
    root2 = temp;
  }
  vertex_map_[root2].root = root1;
  vertex_map_[root1].rank++;
}

// Perform path compression.
Vertex UnionFind::path_compression(Vertex vertex) {
  Vertex root = vertex_map_[vertex].root;
  vector<Vertex> updates = { vertex };

  // Get a vector of vertices whose roots need to be updated.
  while(vertex_map_[root].root != root) {
    updates.push_back(root);
    root = vertex_map_[root].root;
  }
  for(auto& v: updates)
    vertex_map_[v].root = root;
  return root;
}
