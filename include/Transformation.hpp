#ifndef TRANSFORMATION_HPP_
#define TRANSFORMATION_HPP_

class Transformator {
public:
  Transformator()
      : _base_note{0}, _current_error{0}, _max_error{2}, _error_chance{0x1P-5},
        _split_chance{0x1P-6}, _error_multiplier{0}, _top_preference{0x1P-1},
        _preference_multiplier{0} {}

private:
  unsigned char _base_note;
  char _current_error;
  unsigned char _max_error;
  double _error_chance;
  double _split_chance;
  double _error_multiplier;
  double _top_preference;
  double _preference_multiplier;
};

#endif // TRANSFORMATION_HPP_
