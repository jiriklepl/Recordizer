#ifndef READER_HPP_
#define READER_HPP_

#include <istream>

#include "TrackReader.hpp"

// TODO: should read heder and then create TrackReaders

class Reader {
public:
  Reader(std::istream &stream) : _stream{stream} {}

private:
  std::istream &_stream;
};

#endif // READER_HPP_
