#ifndef NOTE_SHEET_HPP_
#define NOTE_SHEET_HPP_

#include <set>

#include "Note.hpp"
#include "NoteEvent.hpp"

class NoteSheet {
public:
  bool add_note(Duration when, Note added) {
    return _notes.emplace(added.on_event(when)).second &&
           _notes.emplace(added.off_event(when)).second;
  }

  bool remove_note(Duration when, Note removed) {
    return _notes.erase(removed.on_event(when)) &&
           _notes.erase(removed.off_event(when));
  }

  bool shift_note(Duration old_when, Note shifted, Duration new_when) {
    return remove_note(old_when, shifted) && add_note(new_when, shifted);
  }

  bool change_note(Duration when, Note old_note, Note new_note) {
    return remove_note(when, old_note) && add_note(when, new_note);
  }

private:
  std::set<NoteEvent> _notes;
};

#endif // NOTE_SHEET_HPP_
