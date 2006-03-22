//   -*- mode: c++; mode: font-lock; fill-column: 75; comment-column: 50; -*-
//
// $Id: ofile.h,v 1.2 2003/05/23 17:35:04 mok Exp $
//
// Declare member functions for the Ofile class that deals with files
// in O format, either formatted or binary.  Morten Kjeldgaard, 20-Apr-2003.

#ifndef _ofile_h
#define _ofile_h
#include <string>
#include <fstream>

#define ODB_FMTSIZ 64


class OFile {

 private:
  std::string _path;
  bool _binary;					  // true if file is binary
  bool _swap;				          // true if bytes must be swapped
  std::ifstream _file;				  // file descriptor 
  bool binfil(void);

 public:
  OFile( void );				  // constructor
  OFile( std::string );				  // constructor
  ~OFile();					  // destructor

  // Misc member functions...
  bool open(std::string);
  void close(void);
  bool get_header ( char*, char &, int &, char * );
  bool get_intblock ( int *, int );
  bool get_realblock ( float *, int );
  bool get_charblock ( char *, int, char * );
  bool get_textblock( char *, int &);
};
#endif

/////
