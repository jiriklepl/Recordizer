#ifndef READER_PRIMITIVES_HPP_
#define READER_PRIMITIVES_HPP_

#include <exception>
#include <istream>
#include <memory>
#include <utility>

#include "Duration.hpp"
#include "Event.hpp"

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


// the returned time (regardles of the actual Event subtype) is actually the delta time
template <> inline std::unique_ptr<Event> read(std::istream &stream) {
  using EventType = Event::EventType;
  unsigned char c;
  Duration when;

  if (auto tmp{read<Duration>(stream)}; tmp)
    std::swap(when, *tmp);
  else
    return nullptr;

  // can receive the very first byte?
  if (!stream.get(reinterpret_cast<char &>(c)))
    return nullptr;

  // Is the most significant bit set (as expected in every Event)?
  if (!(c & 1U << 7))
    return nullptr;

  unsigned char channel = c & 0xf;

  // most of the events have only two bytes of data:
  char data[2];
  switch ((EventType)c) {
  case EventType::NOTE_OFF:
    if (stream.get(data[0]) && stream.get(data[1]))
      return std::make_unique<NoteOffEvent>(when, channel, data[0], data[1]);
    else
      return nullptr;
    break;

  case EventType::NOTE_ON:
    if (stream.get(data[0]) && stream.get(data[1]))
      return std::make_unique<NoteOnEvent>(when, channel, data[0], data[1]);
    else
      return nullptr;
    break;

  case EventType::POLY_KEY:
    if (stream.get(data[0]) && stream.get(data[1]))
      return std::make_unique<PolyKeyEvent>(when, channel, data[0], data[1]);
    else
      return nullptr;
    break;

  case EventType::PROGRAM_CHANGE:
    if (stream.get(data[0]))
      return std::make_unique<ProgramChangeEvent>(when, channel, data[0]);
    else
      return nullptr;
    break;

  default:
    throw std::runtime_error("This event is not implemented yet");
  }
}

#endif // READER_PRIMITIVES_HPP_
