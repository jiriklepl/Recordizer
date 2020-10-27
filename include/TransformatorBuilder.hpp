#ifndef TRANSFORMATOR_BUILDER_HPP_
#define TRANSFORMATOR_BUILDER_HPP_

#include "ProgramChange.hpp"
#include "Transformator.hpp"

struct shift_t {
  Duration amount;
  bool positive;

  void reset() {
    amount = 0;
    positive = true;
  }
};

shift_t shift(Duration amount, bool positive = true) {
  return {.amount = amount, .positive = positive};
};

struct channel_t {
  unsigned char number;
};

channel_t channel(unsigned char number) { return {.number = number}; };

class TransformatorBuilder {
public:
  TransformatorBuilder(Transformator &transformator, unsigned char channel)
      : _transformator{transformator}, _shift{.amount = 0,
                                              .positive = false}, _channel{channel} {}

  TransformatorBuilder &operator<<(Note added) {
    _transformator.append_note(_shift.amount, _channel, added);
    _shift.reset();
    return *this;
  }

  TransformatorBuilder &operator<<(shift_t shift) {
    _shift = shift;
    return *this;
  }

  TransformatorBuilder &operator<<(ProgramChange pc) {
    _transformator.append_event<ProgramChangeEvent>(_shift.amount, _channel, pc.pc_num());
    return *this;
  }

protected:

private:
  Transformator &_transformator;
  shift_t _shift;
  unsigned char _channel;
};

#endif // TRANSFORMATOR_BUILDER_HPP_
