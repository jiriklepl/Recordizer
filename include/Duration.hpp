#ifndef DURATION_HPP_
#define DURATION_HPP_

#include <iostream>

class Duration {
public:
  explicit Duration(std::size_t length) : _length{length} {}

  Duration() : _length{0} {}

  std::size_t length() const { return _length; }
  void set_length(std::size_t length) { _length = length; }

  bool operator==(const Duration &other) const {
    return length() == other.length();
  }
  bool operator!=(const Duration &other) const { return !(*this == other); }
  bool operator<(const Duration &other) const {
    return length() < other.length();
  }
  bool operator>(const Duration &other) const {
    return length() > other.length();
  }
  Duration operator+(const Duration &other) const {
    return Duration(length() + other.length());
  }
  Duration operator-(const Duration &other) const {
    return Duration(length() - other.length());
  }

  friend std::ostream &operator<<(std::ostream &, const Duration &);

private:
  static constexpr std::size_t byte_size = 7;
  static constexpr std::size_t byte_mask = (1ULL << byte_size) - 1;
  std::size_t _length;

  template <std::size_t where>
  std::ostream &print_bytes(std::ostream &stream,
                            unsigned char byte_mess = 0) const {
    const unsigned char byte = (_length >> (where * byte_size)) & byte_mask;
    byte_mess |= byte;

    if (byte_mess)
      stream.put(byte | (byte_mask + 1));

    return print_bytes<where - 1>(stream, byte_mess);
  }
};

template <>
inline std::ostream &Duration::print_bytes<0>(std::ostream &stream,
                                              unsigned char) const {
  stream.put(_length & byte_mask);
  return stream;
}

inline std::ostream &operator<<(std::ostream &stream,
                                const Duration &duration) {
  constexpr std::size_t bytes =
      (sizeof(std::size_t) * 8 + Duration::byte_size - 1) / Duration::byte_size;
  return duration.print_bytes<bytes - 1>(stream);
}

#endif // DURATION_HPP_
