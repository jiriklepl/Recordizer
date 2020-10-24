#ifndef TRANSFORMATOR_HPP_
#define TRANSFORMATOR_HPP_

#include <cmath>
#include <ostream>
#include <random>

#include "NoteSheet.hpp"

// TODO: custom copy, move constructors and operators because of using primitive
// types and then fix clear() so it resets generator

class Transformator {
public:
  Transformator()
      : _max_splits{5}, _min_split{0}, _base_note{60}, _current_error{0},
        _max_sq_error{4}, _error_chance{0x1P-5}, _error_multiplier{.0},
        _split_chance{0x1P-6}, _preference_multiplier{.0} {}

  std::size_t max_splits() const { return _max_splits; }
  void set_max_splits(std::size_t max_splits) { _max_splits = max_splits; }

  Duration min_split() const { return _min_split; }
  void set_min_split(Duration min_split) { _min_split = min_split; }

  unsigned char base_note() const { return _base_note; }
  void set_base_note(unsigned char base_note) { _base_note = base_note; }

  char current_error() const { return _current_error; }
  void set_current_error(char current_error) { _current_error = current_error; }

  unsigned char max_sq_error() const { return _max_sq_error; }
  void set_max_sq_error(unsigned char max_sq_error) {
    _max_sq_error = max_sq_error;
  }

  double error_chance() const { return _error_chance; }
  void set_error_chance(double error_chance) { _error_chance = error_chance; }

  double error_multiplier() const { return _error_multiplier; }
  void set_error_multiplier(double error_multiplier) {
    _error_multiplier = error_multiplier;
  }

  double split_chance() const { return _split_chance; }
  void set_split_chance(double split_chance) { _split_chance = split_chance; }

  double preference_multiplier() const { return _preference_multiplier; }
  void set_preference_multiplier(double preference_multiplier) {
    _preference_multiplier = preference_multiplier;
  }

  void add_note(Duration when, const char channel, Note added,
                std::size_t splits_done = 0) {
    using delta_type = int;
    delta_type error_change = 0;
    delta_type note_delta = (delta_type)base_note() - (delta_type)added.note();

    if (random() <
        error_chance() + 1. -
            std::pow(1. - error_multiplier(), std::abs((double)note_delta))) {
      double top_preference = 1. - std::pow(1. - preference_multiplier(),
                                            std::abs((double)note_delta));
      top_preference = .5 + top_preference / 2.;
      error_change += (delta_type)(random() < top_preference) * 2 - 1;
    }

    if ((current_error() + error_change) * (current_error() + error_change) <=
        _max_sq_error) {
      set_current_error(current_error() + error_change);
    }

    if (error_change > 0) {
      if (added.note() <= 127 - error_change) {
        added.set_note(added.note() + error_change);
      } else {
        added.set_note(127);
      }
    } else {
      if (added.note() >= -error_change) {
        added.set_note(added.note() + error_change);
      } else {
        added.set_note(0);
      }
    }

    if (splits_done < max_splits() && added.duration() >= min_split() &&
        random() < split_chance()) {
      Duration first_duration = added.duration() / 2;
      Note splitted = added;

      splitted.set_duration(added.duration() - first_duration);
      added.set_duration(first_duration);

      note_sheet.add_note(when, channel, added);
      add_note(when + added.duration(), channel, splitted, splits_done + 1);
    } else {
      note_sheet.add_note(when, channel, added);
    }
  }

  void clear() {
    note_sheet.clear();
    set_current_error(0);
  }

  friend std::ostream &operator<<(std::ostream &stream,
                                  const Transformator &transformator) {
    return stream << transformator.note_sheet;
  }

private:
  using generator_type = std::default_random_engine;
  using distribution_type = std::uniform_real_distribution<double>;

  generator_type &generator() { return _generator; }
  const generator_type &generator() const { return _generator; }

  distribution_type &distribution() { return _distribution; }
  const distribution_type &distribution() const { return _distribution; }

  double random() { return distribution()(generator()); }

  generator_type _generator;
  distribution_type _distribution{0.0, 1.0};

  std::size_t _max_splits;
  Duration _min_split;

  unsigned char _base_note;
  char _current_error;
  unsigned char _max_sq_error;
  double _error_chance;
  double _error_multiplier;
  double _split_chance;
  double _preference_multiplier;

  NoteSheet note_sheet;
};

#endif // TRANSFORMATOR_HPP_
