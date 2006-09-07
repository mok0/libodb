//   -*- mode: c++; mode: font-lock; fill-column: 75; comment-column: 50; -*-
//
// $Id: ofile.cpp,v 1.4 2003/06/04 08:39:14 mok Exp $
//
// Define member functions for the Ofile class that deals with files
// in O format, either formatted or binary.  Morten Kjeldgaard, 20-Apr-2003.

#include "ofile.h"
#include <iostream>

using std::cerr;

/*
  A simple parser for the fortran formats that the type C datablocks
  are stored in. Generate a string describing the format, so the
  program knows what to expect in the file. The function knows that 
  type C variables are 6 bytes always. Examples:

  (2a)         -> 123456123456
  (1x,5a)      -> _123456123456123456123456123456
  (5(1x,a6))   -> _123456_123456_123456_123456_123456
  (1x,2(2x,a)) -> ___123456__123456
  (aaaaa)      -> 123456123456123456123456123456

*/
#define RSIZ 256

static char *parse_format(char *fmt)
{
  register int i;
  char *result, *f, *r, *t;
  int mult = 0;

  result = new char[RSIZ];
  memset (result, 0, RSIZ);
  
  f = fmt;
  r = result;
  
  while (*f) {
    switch (*f) {
    case '(':
      t = parse_format(++f);
      if (mult == 0) mult = 1;
      for (i=0; i < mult; i++)
	strncat(result, t, RSIZ);
      delete [] t;
      return result;
      break;
    case 'x':
    case 'X':
      if (mult == 0) mult = 1;
      for (i=0; i < mult; i++)
	*r++ = '_';
      mult = 0;
      break;
    case 'a':
    case 'A':
      if (mult == 0) mult = 1;
      for (i=0; i < mult; i++) {
	*r++ = '0'+1;
	*r++ = '0'+2;
	*r++ = '0'+3;
	*r++ = '0'+4;
	*r++ = '0'+5;
	*r++ = '0'+6;
      }
      mult = 0;
      break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      mult = mult*10+(*f)-'0';
      break;
    case ')':
      return result;
    case ',':
      mult = 0;
      break;
    }
    f++;
  }
  cerr << "parse_format: shouldn't happen\n";
  return NULL;
}


// Class constructors

OFile::OFile(void) {
}

OFile::OFile(std::string path) {
  open(path);
}

// Open the file. Determine whether file is binary or formatted. Determine
// whether or not to swap bytes. Open file and save descriptor.

bool OFile::open(std::string path) {

  _path = path;
  
  switch (binfil()) {

  case 1: 
    _binary = true;
    _file.open(_path.c_str(), std::ios::in | std::ios::binary);
    break;

  case 0:
    _binary = false;
    _file.open(_path.c_str(), std::ios::in);
    break;

  otherwise:
    return false;
  }

  if (_file.fail()) return false;

  // determine endianism...
  union {
    unsigned int ui;
    char byte[4];
  } i;

  i.ui = 0;
  i.byte[3]= 1;

  if (i.ui == 1) {
    _swap = false; 
  }
  else {
    _swap = true;
  }
  return true;
}

// Class destructor, closes file.

OFile::~OFile (void)
{
  _file.close();
}

//  binfil -- a private class member. Attempt to open file and read in
//  the first eight bytes. Return 'true' if fnam is a binary O file, else
//  'false'. An O binary file normally has the byte pattern [0 0 0 036 .] in
//  the first 5 bytes of the file.

int OFile::binfil(void)
{
  std::ifstream File;
  int n;
  char buf[8];

  File.open(_path.c_str(), std::ios::in | std::ios::binary);

  if (File.fail()) {
    cerr << "binfil: could not open file\n";
    return -1;
  }

  File.read((char *)buf, 8);
  File.close();

  if ((buf[0]&buf[1]&buf[2]) == 0 && buf[3] == 30 && buf[4] == '.') {
    // file has binary signature
    return 1;
  }
  if ((buf[0]&buf[1]&buf[2]) == 0 && buf[3] == 30) {
    // The only binary O files that do _not_ have a '.' in the fifth byte
    // are the dgnl data files
    return 1;
  }

  // file is formatted
  return 0;
}


void OFile::close(void)
{
  _file.close();
}

//  Swap bytes in n 4-byte words
static void swap4 (char *buffer, int n)
{
  char j;

  for (int i=0; i < n*4; i+=4) {
    j = buffer[i];
    buffer[i] = buffer[i+3];
    buffer[i+3] = j;
    j = buffer[i+1];
    buffer[i+1] = buffer[i+2];
    buffer[i+2] =j; 
  }
}

// In the code below we use a bunch of C string functions. Declare them here...

#include <cstring>

/// Read the datablock header.

bool OFile::get_header(char *nam, char &typ, int &siz, char *fmt) 
{


  if (_file.fail()) {
    cerr << "failed\n";
    return false;
  }

  if (_file.eof()) {
    cerr << "end of file reached\n";
    return false;
  }

  if (_binary) {
    int n, j;
    //    long rl1, rl2;
    int rl1, rl2;

    _file.read ((char *)&rl1, sizeof(int));
    if (_file.fail()) return false;
    if (_swap) swap4 ((char *)&rl1, 1);
    _file.read(nam, 25);

    // convert datablock name to lower case
    for (n=0; n<25; n++)
      nam[n] = tolower(nam[n]);
    _file.read ((char *)&typ, sizeof (char));
    _file.read ((char *)&siz, sizeof (int));
    if (_swap) swap4 ((char *)&siz, 1);
    _file.read((char *)&rl2, sizeof (int));
    if (_swap) swap4 ((char *)&rl2, 1);
    assert (rl1 == rl2);
    return true;

  } else {

    // file is formatted.
    const int sz = 256;
    char *stat;
    char buf[sz], *cp;
    int i;
    char num[64];

    memset (buf, 0, sz);
    memset (num, 0, 64);
    memset (fmt, 0, ODB_FMTSIZ);
    memset (nam, 0, 26);

    // Get the first non-comment line.

    buf[0] = '!';
    while (buf[0] == '!' || buf[0] == '\n' || buf[0] == 0) {
      _file.getline(buf,sz);
      if (_file.eof())
	return false;
    }

    i=0;
    cp = nam;
    while (!isspace(buf[i]))			  // copy name
      *cp++ = tolower(buf[i++]);

    while (isspace(buf[i]))			  // step forward to type
      i++;
    typ = toupper(buf[i++]);			  // copy type


    while (isspace(buf[i]))			  // step forward to size
      i++;
    cp = num;
    while (!isspace(buf[i]))			  // copy size 
      *cp++ = buf[i++];
    siz = (int)strtol(num, &stat, 10);		  // decode size
    if (*stat) {
      cerr << "error decoding size\n";
      return false;
    }

    while (isspace(buf[i]))			  // step forward until fmt string
      i++;
    cp = fmt;
    while (!isspace(buf[i]) && buf[i])		  // copy fmt string
      *cp++ = buf[i++];

    return true;
  }
}


///  Read 'size' integers from the binary fortran file. Swap bytes if
///  necessary, file is always in big-endian order.

bool OFile::get_intblock(int *istore, int size) 
{
  if (_binary) {
    int n;
    int rl1, rl2;

    _file.read ((char *)&rl1, sizeof(int));  
    if (_file.fail()) {
      return -1;
    }
    if (_swap) swap4 ((char *)&rl1, 1);
    _file.read ((char *)istore, rl1);
    if (_swap) swap4 ((char *)istore, size);
    _file.read ((char *)&rl2, sizeof(int));  
    if (_swap) swap4 ((char *)&rl2, 1);

    assert (rl1 == rl2);

    if (4*size != rl2) {
      cerr <<  "get_intblock: expected " << 4*size << " got " << rl2 << " elements\n";
      return false;
    }
  } else {

    // formatted file...

    char buf[64], *stat;

    for (int i=0; i<size; i++) {
      _file >> buf;
      istore[i] = (int)strtol(buf, &stat, 10); 
      if (*stat) {
	cerr <<  "non-digits in datablock\n";
	return false; 
      }
    }
  }
  return true;
}

///   Read 'size' floats from the binary fortran file. Swap bytes if
///   necessary, file is always in big-endian order.

bool OFile::get_realblock(float *rstore, int size) 
{
  if (_binary) {
    int n;
    int rl1, rl2;

    _file.read ((char *)&rl1, sizeof(int));  
    if (_file.fail()) {
      return -1;
    }
    if (_swap) swap4 ((char *)&rl1, 1);
    _file.read ((char *)rstore, rl1);
    if (_swap) swap4 ((char *)rstore, size);
    _file.read ((char *)&rl2, sizeof(int));  
    if (_swap) swap4 ((char *)&rl2, 1);

    assert (rl1 == rl2);

    if (4*size != rl2) {
      cerr <<  "get_realblock: expected " << 4*size << " got " << rl2 << " elements\n";
      return false;
    }
  } else {

    // formatted file...
    char buf[64], *stat;
    
    for (int i=0; i<size; i++) {
      _file >> buf;
      rstore[i] = (float)strtod(buf, &stat); 
      if (*stat) {
	cerr <<  "non-digits in datablock\n";
	return false; 
      }
    }
  }
  return true;
}

/// Read 'size' C6 variables

bool OFile::get_charblock(char *cstore, int size, char *fmt)
{
  if (_binary) {
    int n;
    int rl1, rl2;

    _file.read ((char *)&rl1, sizeof(int));  
    if (_file.fail()){
      return false;
    }
    if (_swap) swap4 ((char *)&rl1, 1);
    _file.read (cstore, rl1);
    _file.read ((char *)&rl2, sizeof(int));  
    if (_swap) swap4 ((char *)&rl2, 1);

    assert (rl1 == rl2);

    if (6*size != rl2) {
      cerr << "get_charblock: expected " << 6*size << ", got " << rl2 << " elements\n";
      return false;
    }
    return true;

  } else {

    // Formatted input
    char c, *a, *t, *s;
    int i, eol, inword;

    t = parse_format(fmt);
    s = t;
    a = cstore;
  
    i = 0;
    eol = 0;

    while (i < size) {
      if (!*s) { // if end of format string, rewind it.
	s = t;   // and reset everything
	eol = 0;
	while (_file.get() != '\n')
	  ;
      }	      

      inword = 1;
      while (inword) {
	if (!eol)
	  c = _file.get();
	if (c == '\n')
	  eol = 1;
	switch (*s++) {
	case '_':
	  break;
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	  if (eol)
	    //*a++ = ' ';
	    *a++ = '#';
	  else
	    *a++ = c;
	  break;
	case '6':
	  if (eol)
	    //*a++ = ' ';
	    *a++ = '6';
	  else
	    *a++ = c;
	  i++;
	  inword = 0;
	  break;
	}
      }  // end get one word
    }
    delete [] t;
    return true;

  }
}


///  Read a text datablock. The definition of the size of a text block
/// is different in binary format, where 'size' is the number of bytes,
/// or formatted, where 'size' is the number of lines...

bool OFile::get_textblock(char *text, int &size) 
{
  if (_binary) {
    int n;
    int rl1, rl2;

    _file.read ((char *)&rl1, sizeof(int));  
    if (_file.fail()) {
      return false;
    }
    if (_swap) swap4 ((char *)&rl1, 1);
    _file.read (text, rl1);
    _file.read ((char *)&rl2, sizeof(int));  
    if (_swap) swap4 ((char *)&rl2, 1);

    assert (rl1 == rl2);

    if (size != rl2) {
      cerr << "get_textblock: expected " << size <<", got" << rl2 << " bytes\n";
    }

    // count no of lines and return in size
    size = 0;
    for (int i=0; i<rl2; i++)
      if (text[i] == '\r')
	size++;

  } else {

    // formatted text block...

    char *ch, buf[256];
    int j, n;

    j = 0;
    for (int i=0; i<size; i++) {
      _file.getline (buf, 256);
      n = strlen(buf);
      buf[n++] = '\r';				  // like in binary file
      strncpy (text+j, buf, n);
      j += n;
    }
    text[j] = '\000';
  }
  return true;
}

////
