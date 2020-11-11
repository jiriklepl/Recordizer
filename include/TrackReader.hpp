#ifndef TRACK_READER_HPP_
#define TRACK_READER_HPP_

#include <compare>
#include <istream>
#include <memory>
#include <utility>

#include "Event.hpp"

// TODO

class TrackReader {
private:
  template <class type> class meta_iterator;
  template <class type> friend class TrackReader::meta_iterator;

public:
  using iterator = meta_iterator<TrackReader *const>;

  explicit TrackReader(std::istream &stream) : _stream{stream} {}

  iterator begin();
  iterator end();

private:
  std::istream &_stream;
};

template <class type> class TrackReader::meta_iterator {
  friend class TrackReader;

public:
  using difference_type = void;
  using value_type = std::shared_ptr<Event>;
  using reference = value_type;
  using iterator_category = std::input_iterator_tag;

  std::strong_ordering operator<=>(const meta_iterator &other) const {
    return _track_reader <=> other._track_reader;
  }

  std::shared_ptr<Event> operator*() const { return _current; }

  meta_iterator &operator++() {
    if (!_get_next())
      _track_reader = nullptr;
    return *this;
  }

  meta_iterator operator++(int) {
    meta_iterator it = *this;
    ++*this;
    return it;
  }

private:
  explicit meta_iterator(type track_reader)
      : _track_reader{track_reader}, _current{nullptr} {}
  bool _get_next() {
    // TODO
  }

  type _track_reader;
  std::shared_ptr<Event> _current;
};

auto inline TrackReader::begin() -> iterator {
  return iterator{this};
}

auto inline TrackReader::end() -> iterator {
  return iterator{nullptr};
}

#endif // TRACK_READER_HPP_
