// example on ostream::operator<<
#include <iostream>
using namespace std;

int main () {

  char str[] = "Test sentence";
  int val = 65;
  char ch = 'A';

  cout << str << endl;     // Insert string

  cout << ch << endl;      // Insert single character

  cout.width (10);
  cout << right;           // Insert manipulator

  cout << val << endl;     // Insert integer

  return 0;
}
