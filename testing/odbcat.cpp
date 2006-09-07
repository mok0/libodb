//   -*- mode: c++; mode: font-lock; fill-column: 75; comment-column: 50; -*-
// $Id: testodb1.cpp 8 2006-09-07 09:35:39Z mok $ 
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
  char dbtyp;

  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " odbfile [dbname]" << endl;
    return 1;
  }

  X.open(string(argv[1]));
  X.get();

  if (argc == 2) {
    return 0;
  }

  y = X.fetch(argv[2]);
  if (!y) {
    cerr << "datablock not found" << endl;
    return 1;
  }
  
  n =  y->size();
  dbtyp = y->get_type();

  switch (dbtyp) {
  case 'I': 
    {
      std::vector<int> cv;
      IntBlock *z = (IntBlock *)y;
      z->write();
    }
    break;
  case 'R': 
    {
      std::vector<float> cv;
      RealBlock *z = (RealBlock *)y;
      z->write();
    }
    break;
  case 'C': 
    {
      std::vector<std::string> cv;
      CharBlock *z = (CharBlock *)y;
      z->write();
    }
    break;
  case 'T': 
    {
      std::vector<std::string> cv;
      TextBlock *z = (TextBlock *)y;
      z->write();

    }
    break;
  }
}


////
