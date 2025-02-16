//   -*- mode: c++; mode: font-lock; fill-column: 75; comment-column: 50; -*-
//
// $Id: testodb2.cpp 4 2006-03-22 20:43:21Z mok $

#include <iostream>
#include <cstdlib>

#include "ofile.h"

using namespace std;

int main ()
{
  char nam[26], typ, fmt[ODB_FMTSIZ];
  int siz;
  int *istore;
  float *rstore;
  char *cstore;
  char *tstore;

  istore = new int [50000*4];
  rstore = (float *)malloc(150000*4);
  cstore = (char *)malloc(50000*6);
  tstore = (char *)malloc(250000);

  nam[25] = '\000';

  //  OFile ofile("binary.o");
  OFile ofile("startup.odb");
  while (ofile.get_header(nam, typ, siz, fmt)) {
    cout << "name=" << nam <<" type=" << typ << " size=" << siz << endl;
    switch (typ) {
    case 'I':
      if (!ofile.get_intblock(istore, siz))
	cerr << "problems\n";
      break;
    case 'R':
      if (!ofile.get_realblock(rstore, siz))
	cerr << "problems\n";
      break;
    case 'C':
      if(!ofile.get_charblock(cstore, siz, fmt))
	cerr << "problems\n";
      break;
    case 'T':
      if(!ofile.get_textblock(tstore, siz))
	cerr << "problems\n";
      break;
    }
  }
  cerr << "done.\n";

  ofile.close();

  delete [] istore;
  free(rstore);
  free(cstore);
  free(tstore);
}

// Local variables:
// mode: font-lock
// End:
