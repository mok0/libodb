/*
    C++ class to deal with an O database (collection of datablocks)
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

#include <map>
#include <string>

#include "datablock.h"

#ifndef _odb_h
#define _odb_h

typedef std::map<std::string, class ODatablock *, std::less<std::string> > maptype;


class ODatabase {
private:
  std::string _path;
  std::string _name;
  maptype _db;

public:
  // Constructors
  ODatabase(void);
  ODatabase(std::string);

  // Destructor
  ~ODatabase(void);

  // Utils...
  void open(std::string);
  void get (void);		 // suck in a file, binary or formatted ...
  void write(std::string path);		 // we only write out formatted...
  // NOTE: Overload << and >> !!!!!!!!!

  void dir (void);                    // directory...

  ODatablock *fetch(std::string);

};

#endif

// Local variables:
// mode: c++
// mode: font-lock
// fill-column: 75
// comment-column: 50
// End:
