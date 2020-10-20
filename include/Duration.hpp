#ifndef DURATION_HPP_
#define DURATION_HPP_
#include <iostream>

class Duration {
 public:
    explicit Duration(std::size_t length) noexcept
    : _length{length} {
    }

    Duration() noexcept
    : _length{0} {
    }

    std::size_t length() const noexcept { return _length; }
    void set_length(std::size_t length) noexcept { _length = length; }

    friend std::ostream& operator<<(std::ostream&, const Duration&);

 private:
    static constexpr std::size_t byte_length = 7;
    static constexpr std::size_t byte_mask = (1ULL << byte_length) - 1;
    std::size_t _length;

    template<std::size_t where>
    std::ostream&
    print_bytes(std::ostream& stream, unsigned char byte_mess = 0) const noexcept
    {
        const unsigned char byte = (_length >> (where * byte_length)) & byte_mask;
        byte_mess |= byte;

        if (byte_mess) stream.put(byte | (byte_mask + 1));
        return print_bytes<where - 1>(stream, byte_mess);
    }
};

template<>
inline std::ostream&
Duration::print_bytes<0>(std::ostream& stream, unsigned char) const noexcept
{
    stream.put(_length & byte_mask);
    return stream;
}

inline std::ostream&
operator<<(std::ostream& stream, const Duration& duration)
{
    return duration.print_bytes<sizeof(std::size_t) * 8 / 7>(stream);
}

#endif  // DURATION_HPP_