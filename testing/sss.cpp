#include <iostream>
#include <string>

using std::cerr;
using std::endl;

main () 
{
  int i;
  int j;
  std::string x;

  i = 45;
  j = 123;
  x = "Morten";

  cerr.width(25);
  cerr << x ;
  cerr << "#" ;
  cerr.width(10);
  cerr << i << "#";
  cerr.width(5);
  cerr << j << "#";
  cerr << endl;

}
