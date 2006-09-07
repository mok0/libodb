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
    cout << "Usage: odbcat odbfile dbname\n";
    return 1;
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
      IntBlock *z = (IntBlock *)y;
      z->write();
    }
    break;
  case 'R': 
    {
      RealBlock *z = (RealBlock *)y;
      z->write();
    }
    break;
  case 'C': 
    {
      CharBlock *z = (CharBlock *)y;
      z->write();
    }
    break;
  case 'T': 
    {
      TextBlock *z = (TextBlock *)y;
      z->write();

    }
    break;
  }

  return 0;
}


////
