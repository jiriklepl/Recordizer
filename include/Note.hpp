#ifndef NOTE_HPP_
#define NOTE_HPP_

#include "Duration.hpp"
#include "NoteEvent.hpp"

class Note {
public:
  Note(Duration duration, unsigned char note, unsigned char on_velocity,
       unsigned char off_velocity)
      : _duration{duration}, _note{note}, _on_velocity{on_velocity},
        _off_velocity{off_velocity} {}

  Note(Duration duration, unsigned char note, unsigned char velocity)
      : Note{duration, note, velocity, velocity} {}

  NoteEvent on_event(Duration when, unsigned char channel) const {
    return {when, channel, note(), on_velocity(), NoteEvent::EventType::ON};
  }

  NoteEvent off_event(Duration when, unsigned char channel) const {
    return {when + duration(), channel, note(), off_velocity(),
            NoteEvent::EventType::OFF};
  }

  Duration duration() const { return _duration; }
  void set_duration(Duration duration) { _duration = duration; }

  unsigned char note() const { return _note; }
  void set_note(unsigned char note) { _note = note; }

  unsigned char on_velocity() const { return _on_velocity; }
  void set_on_velocity(unsigned char velocity) { _on_velocity = velocity; }

  unsigned char off_velocity() const { return _off_velocity; }
  void set_off_velocity(unsigned char velocity) { _off_velocity = velocity; }

private:
  Duration _duration;
  unsigned char _note;
  unsigned char _on_velocity;
  unsigned char _off_velocity;
};

#endif // NOTE_HPP_
