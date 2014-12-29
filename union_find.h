// Header file for union-find data structures.

#ifndef UNION_FIND_H_
#define UNION_FIND_H_

#include <assert.h>
#include <unordered_map>

using namespace std;

typedef long Vertex;

// RootRank encapsulates the root & rank of a vertex in union-find.
struct RootRank {
  Vertex root;
  uint32_t rank;
  // For equality check.
  bool operator==(const RootRank& other) const {
    return this->root == other.root && this->rank == other.rank;
  }
};

class UnionFind {
 public:
  // Construct a union-find data instance.
  UnionFind() : vertex_map_({}) {}
  // Copy constructor of union-find class, which performs deep copy
  // of the vertex map.
  UnionFind(const UnionFind& uf) : vertex_map_(uf.vertex_map_) {} 
  // Return the root vertex of a given vertex.
  Vertex find(Vertex vertex);
  // Return RootRank of a given vertex.  Mainly for debugging.
  RootRank get_root_rank(Vertex vertex) {
    assert(vertex_map_.count(vertex));
    return vertex_map_[vertex];
  }
  // Join a pair of vertices.
  void join(Vertex vertex1, Vertex vertex2);

 private:
  // Perform path compression starting from the given vertex.  Return
  // the ultimate root after completing the path compresion.
  Vertex path_compression(Vertex vertex);
  // Vertex-to-RootRank mapping.
  unordered_map<Vertex, RootRank> vertex_map_;
};

#endif // UNION_FIND_H_
