#ifndef EVENT_HPP_
#define EVENT_HPP_

#include <cassert>
#include <compare>
#include <ostream>

#include "Duration.hpp"

// TODO: add support for unsupported events, aliases for program change (piano,
// etc.)
class Event {
public:
  enum class EventType : unsigned char {
    NOTE_OFF =                             0B1000U,
    NOTE_ON =                              0B1001U,
    /*NOT SUPPORTED*/ POLY_KEY =           0B1010U,
    /*NOT SUPPORTED*/ CONTROL_CHANGE =     0B1011U,
    PROGRAM_CHANGE =                       0B1100U,
    /*NOT SUPPORTED*/ CHANNEL_PRESSURE =   0B1101U,
    /*NOT SUPPORTED*/ PITCH_WHEEL_CHANGE = 0B1110U,
    /*NOT SUPPORTED*/ META_EVENT =         0B1111U,
  };

  Duration when() const { return _when; }
  void set_when(Duration when) { _when = when; }

  virtual EventType type() const = 0;

  std::strong_ordering operator<=>(const Event &other) const {
    if (auto cmp = when() <=> other.when(); cmp != 0)
      return cmp;
    else if (auto cmp = event_priority(type()) <=> event_priority(other.type());
             cmp != 0)
      return cmp;
    else
      return _compare(other);
  }

  friend std::ostream &operator<<(std::ostream &stream, const Event &event) {
    event._print(stream);

    return stream;
  }

protected:
  Event(Duration when) : _when(when) {}

  virtual std::strong_ordering _compare(const Event &) const = 0;
  virtual void _print(std::ostream &stream) const = 0;

private:
  Duration _when;

  static constexpr std::size_t event_priority(EventType type) {
    switch (type) {
    case EventType::POLY_KEY:
      return 10;
      break;
    case EventType::NOTE_ON:
      return 9;
      break;
    case EventType::NOTE_OFF:
      return 8;
      break;
    case EventType::CONTROL_CHANGE:
      return 7;
      break;
    case EventType::PITCH_WHEEL_CHANGE:
      return 6;
      break;
    case EventType::CHANNEL_PRESSURE:
      return 5;
      break;
    case EventType::PROGRAM_CHANGE:
      return 4;
      break;
    }
    assert(false);
  }
};

class VoiceMessageEvent : public Event {
public:

  unsigned char channel() const { return _channel; }
  void set_channel(unsigned char channel) { _channel = channel; }

protected:
  std::strong_ordering _compare(const Event &other) const final {
#ifdef NDEBUG
    auto &&cast_other = static_cast<const VoiceMessageEvent &>(other);
#else
    auto &&cast_other = dynamic_cast<const VoiceMessageEvent &>(other);
#endif
    if (auto cmp = channel() <=> cast_other.channel(); cmp != 0)
      return cmp;
    else
      return _compare_voice(cast_other);
  }

protected:
  VoiceMessageEvent(Duration when, unsigned char channel) : Event{when}, _channel(channel) {}

  void _print(std::ostream &stream) const final {
    stream.put((unsigned char)type() << 4 | channel());
    _print_voice(stream);
  }

  virtual std::strong_ordering _compare_voice(const VoiceMessageEvent &other) const = 0;
  virtual void _print_voice(std::ostream &stream) const = 0;

private:
  unsigned char _channel;
};

class NoteEvent : public VoiceMessageEvent {
public:
  unsigned char note() const { return _note; }
  void set_note(unsigned char note) { _note = note; }

  unsigned char velocity() const { return _velocity; }
  void set_velocity(unsigned char velocity) { _velocity = velocity; }

  // other should be a NoteEvent
  std::strong_ordering _compare_voice(const VoiceMessageEvent &other) const override {
#ifdef NDEBUG
    auto &&cast_other = static_cast<const NoteEvent &>(other);
#else
    auto &&cast_other = dynamic_cast<const NoteEvent &>(other);
#endif
    if (auto cmp = note() <=> cast_other.note(); cmp != 0)
      return cmp;
    else {
      assert(velocity() == cast_other.velocity());
      return std::strong_ordering::equal;
    }
  }

  void _print_voice(std::ostream &stream) const override {
    stream.put(note());
    stream.put(velocity());
  }

protected:
  NoteEvent(Duration when, unsigned char channel, unsigned char note,
            unsigned char velocity)
      : VoiceMessageEvent{when, channel}, _note{note}, _velocity{velocity} {}

private:
  unsigned char _note;
  unsigned char _velocity;
};

class NoteOffEvent : public NoteEvent {
public:
  NoteOffEvent(Duration when, unsigned char channel, unsigned char note,
               unsigned char velocity)
      : NoteEvent{when, channel, note, velocity} {}

  EventType type() const override { return EventType::NOTE_OFF; }
};

class NoteOnEvent : public NoteEvent {
public:
  NoteOnEvent(Duration when, unsigned char channel, unsigned char note,
              unsigned char velocity)
      : NoteEvent{when, channel, note, velocity} {}

  EventType type() const override { return EventType::NOTE_ON; }
};

class PolyKeyEvent : public NoteEvent {
public:
  PolyKeyEvent(Duration when, unsigned char channel, unsigned char note,
               unsigned char velocity)
      : NoteEvent{when, channel, note, velocity} {}

  EventType type() const override { return EventType::POLY_KEY; }
};

class ProgramChangeData {
public:
  ProgramChangeData(unsigned char pc_num) : _pc_num{pc_num} {}
  unsigned char pc_num() const { return _pc_num; }
  void set_pc_num(unsigned char pc_num) { _pc_num = pc_num; }

private:
  unsigned char _pc_num;
};
class ProgramChangeEvent : public VoiceMessageEvent, public ProgramChangeData {
public:
  ProgramChangeEvent(Duration when, unsigned char channel, unsigned char pc_num)
      : VoiceMessageEvent{when, channel}, ProgramChangeData{pc_num} {}

  EventType type() const override { return EventType::PROGRAM_CHANGE; }

  // the other should be a ProgramChangeEvent
  std::strong_ordering _compare_voice(const VoiceMessageEvent &other) const override {
#ifdef NDEBUG
    auto &&cast_other = static_cast<const ProgramChangeEvent &>(other);
#else
    auto &&cast_other = dynamic_cast<const ProgramChangeEvent &>(other);
#endif
    return pc_num() <=> cast_other.pc_num();
  }

  void _print_voice(std::ostream &stream) const override { stream.put(pc_num()); }
};

class MetaEvent : public Event { /* TODO: implement MetaEvents */ };

#endif // EVENT_HPP_
