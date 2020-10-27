#ifndef NOTE_SHEET_HPP_
#define NOTE_SHEET_HPP_

#include <concepts>
#include <memory>
#include <ostream>
#include <set>
#include <sstream>
#include <utility>

#include "Note.hpp"
#include "NoteEvent.hpp"

// TODO: append note with offset instead of absolute time

struct EventComparator {
  bool operator()(const std::unique_ptr<Event>& left, const std::unique_ptr<Event>& right) const {
    return *left < *right;
  }
};

class NoteSheet {
public:
  bool add_note(Duration when, unsigned char channel, Note added) {
    return _events.emplace(std::make_unique<NoteOnEvent>(added.on_event(when, channel))).second &&
           _events.emplace(std::make_unique<NoteOffEvent>(added.off_event(when, channel))).second;
  }

  template <std::derived_from<Event> EventType, class... Args>
  bool add_event(Duration when, unsigned char channel, Args... args) {
    return _events.emplace(std::make_unique<EventType>(when, channel, std::forward<Args>(args)...)).second;
  }

  bool remove_note(Duration when, unsigned char channel, Note removed) {
    return _events.erase(std::make_unique<NoteOnEvent>(removed.on_event(when, channel))) &&
           _events.erase(std::make_unique<NoteOffEvent>(removed.off_event(when, channel)));
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
    return add_note(end_time() + after, channel, added);
  }

  template <std::derived_from<Event> EventType, class... Args>
  bool append_event(Duration after, unsigned char channel, Args... args) {
    auto added = std::make_unique<EventType>(end_time() + after, channel, std::forward<Args>(args)...);
    added->set_when(added->when() + end_time());
    return _events.emplace(std::move(added)).second;
  }

  Duration end_time() const {
    return (_events.empty()) ? 0 : (*_events.crbegin())->when();
  }

  bool empty() const { return _events.empty(); }
  void clear() { _events.clear(); }

  std::size_t size() { return _events.size(); }

  friend std::ostream &operator<<(std::ostream &stream,
                                  const NoteSheet &note_sheet) {
    std::uint32_t length{0};
    Duration time{0};
    std::stringstream buff{};

    stream << "MTrk";

    for (auto &&event : note_sheet._events) {
      buff << event->when() - time;
      buff << *event;
      length += buff.str().length();
      time = event->when();
      buff.str({});
      buff.clear();
    }

    time = Duration{0};

    // TODO: valid only for LE
    for (unsigned char *c = reinterpret_cast<unsigned char *>(&length + 1);
         c-- > reinterpret_cast<unsigned char *>(&length);) {
      stream.put(*c);
    }

    for (auto &&event : note_sheet._events) {
      stream << event->when() - time;
      stream << *event;
      time = event->when();
    }

    return stream;
  }

private:
  std::set<std::unique_ptr<Event>, EventComparator> _events;
};

#endif // NOTE_SHEET_HPP_
