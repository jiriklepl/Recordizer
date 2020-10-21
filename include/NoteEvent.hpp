#ifndef NOTE_EVENT_HPP_
#define NOTE_EVENT_HPP_

#include <cassert>

#include "Duration.hpp"

class NoteEvent {
public:
  enum class EventType : unsigned char {
    OFF = 0U,
    ON = 1U,
    /*NOT SUPPORTED*/ POLY = 2U
  };

  NoteEvent(Duration when, unsigned char note, unsigned char velocity,
            EventType type)
      : _when{when}, _note{note}, _velocity{velocity}, _type{type} {}

  bool operator==(const NoteEvent &other) const {
    if (when() == other.when() && note() == other.note()) {
      assert(velocity() == other.velocity());
      assert(type() == other.type());

      return true;
    } else {
      return false;
    }
  }

  bool operator!=(const NoteEvent &other) const { return !(*this == other); }

  bool operator<(const NoteEvent &other) const {
    if (when() < other.when())
      return true;
    else if (note() < other.note())
      return true;
    else {
      assert(velocity() == other.velocity());
      assert(type() == other.type());

      return false;
    }
  }

  bool operator>(const NoteEvent &other) const {
    if (when() > other.when())
      return true;
    else if (note() > other.note())
      return true;
    else {
      assert(velocity() == other.velocity());
      assert(type() == other.type());

      return false;
    }
  }

  Duration when() const { return _when; }
  void set_when(Duration when) { _when = when; }

  unsigned char note() const { return _note; }
  void set_note(unsigned char note) { _note = note; }

  unsigned char velocity() const { return _velocity; }
  void set_velocity(unsigned char velocity) { _velocity = velocity; }

  EventType type() const { return _type; }
  void set_type(EventType type) { _type = type; }

private:
  Duration _when;
  unsigned char _note;
  unsigned char _velocity;
  EventType _type;
};

#endif // NOTE_EVENT_HPP_
