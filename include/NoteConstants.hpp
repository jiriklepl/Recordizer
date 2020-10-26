#ifndef NOTE_CONSTANTS_HPP_
#define NOTE_CONSTANTS_HPP_

#include <cassert>
#include <cstddef>

enum Modifier {
  Flat,
  Sharp,
};

class Constant {
public:
  constexpr Constant(unsigned char value) : _value{value} {}
  constexpr Constant(unsigned char value, Modifier mod) : _value{value} {
    switch (mod) {
    case Flat:
      _value--;
      break;
    case Sharp:
      _value++;
      break;
    }
  }

  constexpr Constant operator+(unsigned char octave) const {
    assert(_value + 12 * (std::size_t)octave <= 127);
    return Constant(_value + 12 * octave);
  }
  constexpr Constant operator-(unsigned char octave) const {
    assert(_value >= 12 * (std::size_t)octave);
    return Constant(_value - 12 * octave);
  }

  constexpr Constant operator+(int octave) const {
    assert(_value + 12 * octave <= 127);
    return Constant(_value + 12 * octave);
  }
  constexpr Constant operator-(int octave) const {
    assert(_value >= 12 * octave);
    return Constant(_value - 12 * octave);
  }

  constexpr operator unsigned char() const { return _value; }

  unsigned char _value;
} C{12}, Cis{C, Sharp}, Ces{C, Flat}, D{14}, Dis{D, Sharp}, Des{D, Flat}, E{16},
    /*F*/ Eis{E, Sharp}, Es{E, Flat}, F{17}, Fis{F, Sharp}, /*E*/ Fes{F, Flat},
    G{19}, Gis{G, Sharp}, Ges{G, Flat}, A{21}, Ais{A, Sharp}, As{A, Flat},
    B{23}, Bis{B, Sharp}, Bes{B, Flat}, H{23}, His{H, Sharp}, Hes{H, Flat};

#endif // NOTE_CONSTANTS_HPP_
