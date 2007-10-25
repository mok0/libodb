// $Id: 
// C++ class to deal with O datablocks.
// Morten Kjeldgaard, 30.3.2003

#ifndef _datablock_h
#define _datablock_h

#include <iostream>
#include <vector>
#include <string>
#include <cassert>

// Length of parameter block names
#define ODB_DBNAME_SIZE 26


// An O "character" variable is 5 bytes.
// TODO! Overload the [] operator!

typedef struct ochar {
  char c[6];
} ochar;


// Datablock base class 
class ODatablock {
protected:
  char _name[ODB_DBNAME_SIZE];
  char _type;
  int _size;
  int _bytes;

public:

  // Constructor
  ODatablock(void) { _size = -1; _type = '*'; _bytes = 0; };

  // Destructor
  virtual ~ODatablock(void) {};

  virtual void set_name(char * nam) {
    memcpy (_name, nam, ODB_DBNAME_SIZE);
    return;
  };

  virtual char *get_name() { return _name; };
  virtual char get_type() { return _type; };
  virtual int size () = 0; 
  virtual int capacity () = 0; 
  virtual int get_byte_size () = 0; 
  virtual void write () = 0;

};

// Class describing an integer datablock

class IntBlock : public ODatablock {

public:
  std::vector<int> _data; 
  IntBlock(void) {_type = 'I';};
  IntBlock(int size) { _type = 'I'; _data.reserve(size); _size = size; };
  ~IntBlock(void) { _data.clear();};
  
  // set size of the block
  void set_size(int size)
  {
    if (size > 0) {
      _size = size;
      _data.reserve(size);
    }
  };

  // return the size
  int size()
  {
    int s = _data.size();
    assert (s == _size);
    return _size;
  }

  // return allocated size (number of elements)
  int capacity() { return _data.capacity();}

  // return the size in bytes of this block
  int get_byte_size () { 
    if (_bytes == 0) {
      _bytes = _data.size()*sizeof(int); 
    }
    return _bytes;
  }

  // return the data
  std::vector<int> &fetch_data() { return _data; }


  // write the datablock to stdout
  void write()
    {
      std::cout << _name << " " << _type << " " << _size << "(i10)" << std::endl;
      for (int i=0; i < _size; i++) {
	std::cout << _data[i] << std::endl;
      }
    }
};


// Class describing a real datablock

class RealBlock : public ODatablock {

public:
  std::vector<float> _data; 
  RealBlock(void){ _type = 'R'; };
  RealBlock(int size){ _type = 'R'; _data.reserve(size); _size = size; };
  ~RealBlock(void){ _data.clear();};

  // set size of the block
  void set_size(int size)
  {
    if (size > 0) {
      _size = size;
      _data.reserve(size);
    }
  };

  // return the allocated size
  int size()
  {
    int s = _data.size();
    assert (s == _size);
    return _size;
  }

  // return allocated size (number of elements)
  int capacity()   {return _data.capacity();}

  // return the size in bytes of this block
  int get_byte_size () { 
    if (_bytes == 0) {
      _bytes = _data.size()*sizeof(float);
    }
    return _bytes;
  }

  // return the data
  std::vector<float> &fetch_data() { return _data; }

  // write the datablock to stdout
  void write()
    {
      std::cout << _name << " " << _type << " " << _size << " (f10.0)" << std::endl;
      for (int i=0; i < _size; i++) {
	std::cout << _data[i] << std::endl;
      }
    }

};


// Class describing a char datablock. In O,
// these are arrays of character*6.

class CharBlock : public ODatablock {

public:
  std::vector<std::string> _data; 
  CharBlock(void){ _type = 'C'; };
  CharBlock(int size){ _type = 'C'; _data.reserve(size); _size = size; };
  ~CharBlock(void){ _data.clear();};

  // set size of the block
  void set_size(int size)
  {
    if (size > 0) {
      _size = size;
      _data.reserve(size);
    }
  };

  // return the allocated size
  int size()
  {
    int s = _data.size();
    assert (s == _size);
    return _size;
  }

  // return allocated size (number of elements)
  int capacity()   {return _data.capacity();}

  // return the size in bytes of this block
  int get_byte_size () { 
    if (_bytes == 0) {
      for (int i=0; i < _data.size(); i++)
	_bytes += _data[i].size();
    }
    return _bytes;
  }

  // return the data
  std::vector<std::string> &fetch_data() { return _data; }

  // write the datablock to stdout
  void write()
    {
      std::cout << _name << " " << _type << " " << _size << "(a6)" << std::endl;
      for (int i=0; i < _size; i++) {
	std::cout << _data[i] << std::endl;
      }
    }

};


// Class describing a text datablock.

class TextBlock : public ODatablock {

public:
  std::vector<std::string> _data; 
  TextBlock(void){ _type = 'T'; };
  TextBlock(int size){ _type = 'T'; _data.reserve(size); _size = size; };
  ~TextBlock(void){ _data.clear();};

  // set size of the block
  void set_size(int size)
  {
    if (size > -1) {
      _size = size;
      _data.reserve(size);
    }
  };


  // return the true size == the actual number of values stored..
  int size()
  {
    int s = _data.size();
    assert (s == _size);
    return s;
  }


  // return the max length of any element
  int maxlength() {
    int curmax = 0;
    for (int i=0; i < _size; i++) {
      curmax = ( curmax > _data[i].length() )? curmax : _data[i].length();
    }
    return curmax;
  }


  // return allocated size (number of elements)
  int capacity()   {return _data.capacity();}


  // return the size in bytes of this block
  int get_byte_size () { 
    if (_bytes == 0) {
      for (int i=0; i < _data.size(); i++)
	_bytes += _data[i].size();
    }
    return _bytes;
  }

  // return the data
  std::vector<std::string> &fetch_data() { return _data; }

  // write the datablock to stdout
  void write()
    {
      std::cout << _name << " " << _type << " " << _size << " " 
		<< this->maxlength() << std::endl;
      for (int i=0; i < _size; i++) {
	std::cout << _data[i] << std::endl;
      }
    }
};

#endif

// Local Variables: 
// mode: c++
// mode: font-lock
// fill-column: 75
// comment-column: 50
// End:
