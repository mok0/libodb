//   -*- mode: c++; mode: font-lock; fill-column: 75; comment-column: 50; -*-
// $Id$ 
//

#include <iostream>
#include <vector>
#include "odb.h"

using namespace std;

int main (int argc, char **argv) 
{
  ODatabase X;
  ODatablock *y;
  int n;

  cout << argc << endl;

  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " odbfile dbname\n";
    return 1;
  }

  // X.open("binary.o");
  X.open(string(argv[1]));
  X.get();
  X.dir();					  // print directory

  if (argc == 2) {
    return 0;
  }

  y = X.fetch(argv[2]);
  if (!y) {
    cerr << "datablock not found" << endl;
    return 1;
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


////
