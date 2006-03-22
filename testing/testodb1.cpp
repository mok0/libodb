//   -*- mode: c++; mode: font-lock; fill-column: 75; comment-column: 50; -*-
// $Id:$ 
//

#include <iostream>
#include <vector>
#include "odb.h"

using std::cout;
using std::endl;
using std::cerr;

int main (int argc, char **argv) 
{
  ODatabase X;
  ODatablock *y;
  int n;

  // X.open("binary.o");
  X.open(argv[1]);
  X.get();
  X.dir();					  // print directory
  //cout << "size of database " << X.size() << endl;

  y = X.fetch(argv[2]);
  if (!y) {
    cerr << "datablock not found" << endl;
    exit (1);
  }
  cerr << "========================================================\n";

  n =  y->size();
  cerr << "size of datablock: " << n << endl;
  cerr << "... in bytes: " << sizeof (*y) << endl;

  switch (y->get_type()) {
  case 'I': 
    {
      std::vector<int> cv;
      IntBlock *z = (IntBlock *)y;
      cv = z->fetch_data();
      for (int i=0; i < n; i++) 
	cerr << cv[i] << endl;
    }
    break;
  case 'R': 
    {
      std::vector<float> cv;
      RealBlock *z = (RealBlock *)y;
      cv = z->fetch_data();
      for (int i=0; i < n; i++) 
	cerr << cv[i] << endl;
    }
    break;
  case 'C': 
    {
      std::vector<std::string> cv;
      CharBlock *z = (CharBlock *)y;
      cv = z->fetch_data();
      for (int i=0; i < n; i++) 
	cerr << cv[i] << endl;
    }
    break;
  case 'T': 
    {
      std::vector<std::string> cv;
      TextBlock *z = (TextBlock *)y;
      cv = z->fetch_data();
      for (int i=0; i < n; i++) 
	cerr << cv[i] << endl;
    }
    break;
  }
}


 /*
   Local Variables:
   mode: font-lock
   End:
*/
