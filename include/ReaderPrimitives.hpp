#ifndef READER_PRIMITIVES_HPP_
#define READER_PRIMITIVES_HPP_

#include <istream>
#include <memory>
#include <utility>

#include "Duration.hpp"

/**
 * @brief reads a value of type T from the input stream
 *
 * @tparam T
 * @param stream
 * @return std::unique_ptr<T> pointing to the value or nullptr if the function
 * fails
 */
template <class T> std::unique_ptr<T> read(std::istream &stream);

template <>
inline std::unique_ptr<Duration> read<Duration>(std::istream &stream) {
  Duration out;
  char c;

  while (stream.get(c)) {
    out += (std::size_t)(c & (1 << 7) - 1U);

    if ((unsigned char)c >> 7 == 1) {
      return std::make_unique<Duration>(std::move(out));
    }
  }

  return nullptr;
}

#endif // READER_PRIMITIVES_HPP_
