#ifndef NOTE_SHEET_HPP_
#define NOTE_SHEET_HPP_

#include <ostream>
#include <set>

#include "Note.hpp"
#include "NoteEvent.hpp"

// TODO: append note with offset instead of absolute time

class NoteSheet {
public:
  bool add_note(Duration when, const char channel, Note added) {
    return _notes.emplace(added.on_event(when, channel)).second &&
           _notes.emplace(added.off_event(when, channel)).second;
  }

  bool remove_note(Duration when, const char channel, Note removed) {
    return _notes.erase(removed.on_event(when, channel)) &&
           _notes.erase(removed.off_event(when, channel));
  }

  bool shift_note(Duration old_when, const char channel, Note shifted,
                  Duration new_when) {
    return remove_note(old_when, channel, shifted) &&
           add_note(new_when, channel, shifted);
  }

  bool change_note(Duration when, const char channel, Note old_note,
                   Note new_note) {
    return remove_note(when, channel, old_note) &&
           add_note(when, channel, new_note);
  }

  void clear() { _notes.clear(); }

  friend std::ostream &operator<<(std::ostream &stream,
                                  const NoteSheet &note_sheet) {
    Duration time;

    for (auto &&note_event : note_sheet._notes) {
      stream << (note_event.when() - time);
      stream << note_event;
      time = note_event.when();
    }

    return stream;
  }

private:
  std::set<NoteEvent> _notes;
};

#endif // NOTE_SHEET_HPP_
