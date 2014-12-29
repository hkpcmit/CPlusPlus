#include <chrono>
#include <iostream>
#include <random>

using namespace std;

int main() {
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  default_random_engine generator(seed);
  uniform_real_distribution<double> distribution(0.0, 1.0);
  cout << "some random numbers between 0.0 and 1.0: ";
  for (int i = 0; i < 10; i++)
    cout << distribution(generator) << " ";
  cout << endl;
  return 0;
}
