// $Id:
//   C++ class to deal with an O database (collection of datablocks)
//   Morten Kjeldgaard, 22-Mar-2003

#include "datablock.h"
#include <map>
#include <string>

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

// Local variables:
// mode: c++
// mode: font-lock
// fill-column: 75
// comment-column: 50
// End:
