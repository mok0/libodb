/*
    C++ class to deal with an O database (collection of datablocks).
    Copyright (C) 2003-2007 Morten Kjeldgaard

    This program is free software: you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public License
    as published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
//   -*- mode: c++; mode: font-lock; fill-column: 75; comment-column: 50; -*-
//
// $Id: 
//

#include "odb.h"
#include "datablock.h"
#include "ofile.h"
#include <cstdlib>
#include <iostream>
#include <iterator>

using std::cout;
using std::cerr;
using std::endl;

// Constructor 1
ODatabase::ODatabase(void) {}

// Constructor 2
ODatabase::ODatabase(std::string path) 
{
  open(path);
}

// Destructor
ODatabase::~ODatabase(void)
{
  // Here we need to zap all datablocks!
  // Create an iterator for the _db map and delete all
  // the datablocks (they have been created with 'new')
}

// List contents of the database
void ODatabase::dir(void)
{
  maptype::iterator it;

  it = _db.begin();
  while (it != _db.end()) {

    cout.flush();
    cout << std::left;
    cout.width(27);
    cout << it->first;
    cout.width(3);
    cout << it->second->get_type();
    cout << std::right;
    cout.width(10);
    cout << it->second->size();
    cout.width(10);
    cout << it->second->get_byte_size();
    cout << endl;

    it++;
  }
}

// Write database to (formatted) file...
void ODatabase::write(std::string path)
{
  // bla bla...
}


void ODatabase::open(std::string path)
{
  int i = _path.find_last_of('/');     // last occurrence of '/'
  _path = path;
  if (i != std::string::npos)
    _name = _path.substr(i); 	      // substring from i to eos
  else
    _name = path;
}


// Populate database from O format file
void ODatabase::get() 
{
  char nam[ODB_DBNAME_SIZE], fmt[ODB_FMTSIZ], typ, *s;
  int siz;
  void *vect, *data;
  OFile ofile;

  if (!ofile.open(_path))
    return;

  while (ofile.get_header(nam, typ, siz, fmt)) {

    // strip spaces off end of datablock name 
    s = &nam[ODB_DBNAME_SIZE - 2];
    while (*s <= 32 && s > nam)
      *s-- = '\0';
    nam[ODB_DBNAME_SIZE-1] = '\0';

    switch(typ) {

    case 'I':
      {
	IntBlock *ib;
	ib = new IntBlock(siz);

	ib->_data.resize(siz);
	ofile.get_intblock (&ib->_data[0], siz);
	ib->set_name(nam);
	_db[std::string(nam)] = ib;
	//cerr << ib->_data[0] << endl;
      }
      break;

    case 'R':
      {
	RealBlock *rb;
	rb = new RealBlock(siz);

	rb->_data.resize(siz);
	ofile.get_realblock (&rb->_data[0], siz);
	rb->set_name(nam);
	_db[std::string(nam)] = rb;
      }
      break;
      
    case 'C':
      {
	CharBlock *cb;
	char buf[8], *ch;
	cb = new CharBlock(siz);
	buf[6] = 0;
	s = new char[siz*6*sizeof(char)];  
	ofile.get_charblock (s, siz, fmt);
	cb->set_name(nam);
	for (int i=0; i < siz; i++) {
	  memcpy(buf, &s[6*i], 6);
	  // strip spaces off end 
	  ch = &buf[6];
	  while (*ch <= 32 && ch > buf)
	    *ch-- = '\0';
	  cb->_data.push_back(std::string(buf));
	}
	delete [] s;
	_db[std::string(nam)] = cb;
      }			  // add to dictionary
      break;

    case 'T':
      {
	TextBlock *tb;
	char *ch, *t;
	int bufsiz;
	int nrec = 0;

	tb = new TextBlock(siz);
	bufsiz = siz*73*sizeof(char); // O text blocks have max 73 chars per line
	s = new char [bufsiz];
	t = new char [bufsiz];
	ofile.get_textblock (s, siz);
	tb->set_name(nam);

	// extract the individual strings into 't'
	for (int i=0, j=0; i<bufsiz; i++) { 
	  t[j] = s[i];
	  if (t[j] == '\000')
	    break;
	  if (t[j] == '\r') {
	    ch = &t[j];
	    while (*ch <= 32 && ch > t) // strip spaces off end
	      *ch-- = '\0';

	    // create string and store
	    tb->_data.push_back(std::string(t));
	    nrec++;
	    j=0;
	  } else {
	    j++;
	  }
	}
	tb->set_size(nrec);
	delete [] t;
	delete [] s;
	_db[std::string(nam)] = tb;		  // add to dictionary
      }
      break;

    } // end switch (typ)
  }
  ofile.close();
  return;
}

ODatablock *ODatabase::fetch(std::string nam) {
  
  return _db[nam];
}

// Local variables:
// mode: c++
// mode: font-lock
// fill-column: 75
// comment-column: 50
// End:
