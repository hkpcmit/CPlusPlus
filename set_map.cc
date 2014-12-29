// Set unordered_map.

#include <iostream>
#include <unordered_map>

using namespace std;
const int MAX_IDX = 120;
const int MAX_TRIALS = 120000;

int main() {
  unordered_map<int, int> m1, m2, result;

  for(int i = 0; i < MAX_IDX; i++) {
    m1[i] = i*i;
    m2[i] = 2*i;
  } 
  for(int t = 0; t < MAX_TRIALS; t++) {
    // Use map set.
    for(int i = 0; i < MAX_IDX; i++)
      result[i] = m1[i];
    for(int i = 0; i < MAX_IDX; i++)
      result[i] = m2[i];
  }
  return 0;
}
