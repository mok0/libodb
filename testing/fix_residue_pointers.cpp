//   -*- mode: c++; mode: font-lock; fill-column: 75; comment-column: 50; -*-
//
// $Id: 
//
// Jiffy program to recompute the residue_pointers array from the atom
// names. Quick and dirty no-checks version. 
// Morten Kjeldgaard, 10-Dec-2003

#include "odb.h"
#include "datablock.h"
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

int main (int argc, char **argv) 
{
  ODatabase X;
  ODatablock *y, *yy;
  int size, n;

  if (argc < 3) {
    cerr << "usage: " << argv[0] << " database mol" << endl;
    exit(1);
  }
  X.open(argv[1]);
  X.get();
  //X.dir();					  // print directory

  string nam = string(argv[2])+ "_atom_name";
  cerr << nam << endl;
  y = X.fetch(nam);
  if (!y) {
    cerr << "datablock not found" << endl;
    exit (1);
  }

  n =  y->size();
  cerr << "size of datablock: " << n << endl;

  nam = string(argv[2])+ "_residue_type";
  yy = X.fetch(nam);
  if (!yy)
    size = 0;
  else
    size = 2*yy->size();

  std::vector<std::string> cv;
  CharBlock *z = (CharBlock *)y;
  cv = z->fetch_data();

  if (size == 0)
    cout << argv[2] << "_residue_pointers i SIZE (2i6)" << endl;
  else
    cout << argv[2] << "_residue_pointers i " << size << " (2i6)" << endl;

  std::string N("N");
  int k=0;
  for (int i=0; i<y->size(); i++) {
    if (cv[i] == N) {
      if (i !=0) {
	cout << k+1 << "," << i << endl;
      } 
      k = i;
    }
  }
}

 /*
   Local Variables:
   compile-command: "g++ -DTESTING odb.cpp ofile.cpp -static"
   End:
*/

