//  Homework 1
//  Calculate arithmetic series of certain size.

#include <iostream>
#include <vector>
using namespace std;
const int MAX = 40;  // Maximum data elements.

// Initialize the given data vector.
inline void InitData(vector<int>& data)
{
  for(int idx = 0; idx < data.size(); ++idx)
    data[idx] = idx;
}

// Iterate and find the sum of all data elements.  The
// data vector is passed into the function by reference
// in case the data is very large and takes longer time
// to copy it.
template<class summable>
inline summable Sum(vector<summable>& data)
{
  summable total = 0;

  for(summable val : data)
    total += val;
  return total;
}

// Main routine.
int main(void)
{
  vector<int> data(MAX);

  InitData(data);
  cout << "Sum is " << Sum(data) << "." << endl;
  return 0;
}
