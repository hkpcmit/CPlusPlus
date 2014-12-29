#include "hex_game.h"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>

using namespace std;
const int TRIALS = 3;

int main() {
  vector<int> keys = { 0, 1, 2, 3, 4, 5 };
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  for(int i=0; i < TRIALS; i++) {
    shuffle(keys.begin(), keys.end(), default_random_engine(seed));

    cout << "Trial: " << i+1 << endl;
    for(auto key: keys)
      cout << key << ' ';
    cout << endl << endl;
  }
  

  return 0;
}
