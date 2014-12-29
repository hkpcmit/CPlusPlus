#include <iostream>
#include <stdio.h>

using namespace std;

int main() {
  int a = 2, b = -3, c = 2;
  bool tval1 = false, tval2 = true;
  char ch = 'b'; 

  cout << "Q1: " << (b+c*a) << endl;
  cout << "Q2: " << (c%5/2) << endl;
  cout << "Q3: " << (-a*c++) << endl;
  cout << "Q4: " << (tval1&&tval2) << endl;
  cout << "Q5: " << (ch+=2) << endl;
}
