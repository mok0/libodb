/*
    Declare member functions for the Ofile class that deals with files
    in O format, either formatted or binary. 
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
  int binfil(void);

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

// Local variables:
// mode: c++
// mode: font-lock
// fill-column: 75
// comment-column: 50
// End:
