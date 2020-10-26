#ifndef NOTE_SHEET_HPP_
#define NOTE_SHEET_HPP_

#include <ostream>
#include <set>
#include <sstream>

#include "Note.hpp"
#include "NoteEvent.hpp"

// TODO: append note with offset instead of absolute time

class NoteSheet {
public:
  bool add_note(Duration when, unsigned char channel, Note added) {
    return _notes.emplace(added.on_event(when, channel)).second &&
           _notes.emplace(added.off_event(when, channel)).second;
  }

  bool remove_note(Duration when, unsigned char channel, Note removed) {
    return _notes.erase(removed.on_event(when, channel)) &&
           _notes.erase(removed.off_event(when, channel));
  }

  bool shift_note(Duration old_when, unsigned char channel, Note shifted,
                  Duration new_when) {
    return remove_note(old_when, channel, shifted) &&
           add_note(new_when, channel, shifted);
  }

  bool change_note(Duration when, unsigned char channel, Note old_note,
                   Note new_note) {
    return remove_note(when, channel, old_note) &&
           add_note(when, channel, new_note);
  }

  Duration append_note(Duration after, unsigned char channel, Note added) {
    return add_note(_notes.rbegin()->when() + after, channel, added);
  }

  Duration end_time() const {
    return (_notes.empty()) ? 0 : _notes.crbegin()->when();
  }

  void clear() { _notes.clear(); }

  std::size_t size() { return _notes.size(); }

  friend std::ostream &operator<<(std::ostream &stream,
                                  const NoteSheet &note_sheet) {
    std::uint32_t length{0};
    Duration time{0};
    std::stringstream buff{};

    stream << "MTrk";

    for (auto &&note_event : note_sheet._notes) {
      buff << note_event.when() - time;
      length += buff.str().length() + 3;
      time = note_event.when();
      buff.str({});
      buff.clear();
    }

    time = Duration{0};

    // TODO: valid only for LE
    for (unsigned char *c = reinterpret_cast<unsigned char *>(&length + 1);
         c-- > reinterpret_cast<unsigned char *>(&length);) {
      stream.put(*c);
    }

    for (auto &&note_event : note_sheet._notes) {
      stream << note_event.when() - time;
      stream << note_event;
      time = note_event.when();
    }

    return stream;
  }

private:
  std::set<NoteEvent> _notes;
};

#endif // NOTE_SHEET_HPP_
