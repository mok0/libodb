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
  char dbtyp;

  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " odbfile" << endl;
    return 1;
  }

  X.open(string(argv[1]));
  X.get();
  X.dir();					  // print directory
}


////
