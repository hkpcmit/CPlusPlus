// Homework 3
// Implement Prim MST algorithm.

#include "prim.h"

#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include <string>

namespace po = boost::program_options;
using namespace std;

// Parse command line arguments and return file name for input graph.
string parse_cmd_line(int argc, char* argv[], bool& cost_only) {
  string filename;

  // Parse and handle command line options.
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "Produce help message.")
    ("cost_only,c", "Print MST cost only.")
    ("file,f", po::value<string>(&filename), "Name of input graph file.");
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  // --help option.
  if(vm.count("help")) {
    cout << desc << endl;
    exit(1);
  }
  
  // --file option.
  ifstream ifs(filename.c_str());
  if(!ifs) {
    cout << "Please provide name of input graph file." << endl;
    cout << desc << endl;
    exit(1);
  }

  // --cost_only option.
  cost_only = vm.count("cost_only");
  return filename;
} 

// Main routine.
int main(int argc, char* argv[]) {
  bool cost_only = false;
  string filename = parse_cmd_line(argc, argv, cost_only);
  cout << "Input graph file: " << filename << endl;

  // Compute MST for input graph.
  Graph graph = Graph(filename);
  Prim prim = Prim(graph);
  Graph prim_mst = prim.mst();

  cout << "MST cost: " << prim_mst.edge_costs() << endl;
  if(!cost_only)
    prim_mst.print_edges();
  return 0;
}
