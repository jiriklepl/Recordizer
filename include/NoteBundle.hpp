#ifndef NOTE_BUNDLE_HPP_
#define NOTE_BUNDLE_HPP_

#include <bitset>
#include <compare>
#include <concepts>

#include "Note.hpp"
#include "NoteConstants.hpp"

template <class T>
concept note_type = std::same_as<unsigned char, std::remove_reference_t<T>> ||
                    std::same_as<Constant, std::remove_reference_t<T>>;

class NoteBundle {
private:
  template <class type> class meta_iterator;

public:
  using iterator = meta_iterator<NoteBundle &>;
  using const_iterator = meta_iterator<const NoteBundle &>;

  template <note_type... xs>
  NoteBundle(Duration duration, unsigned char on_velocity, xs &&... others)
      : NoteBundle{duration, on_velocity, on_velocity,
                   std::forward<std::remove_reference_t<xs>>(others)...} {}
  template <note_type x, note_type... xs>
  NoteBundle(Duration duration, unsigned char on_velocity,
             unsigned char off_velocity, x &&first, xs &&... others)
      : NoteBundle{duration, on_velocity, off_velocity,
                   std::forward<std::remove_reference_t<xs>>(others)...} {
    add_note(std::forward<std::remove_reference_t<x>>(first));
  }
  template <note_type x>
  NoteBundle(Duration duration, unsigned char on_velocity,
             unsigned char off_velocity, x &&first)
      : _duration{duration}, _on_velocity{on_velocity}, _off_velocity{
                                                            off_velocity} {
    add_note(std::forward<std::remove_reference_t<x>>(first));
  }
  Duration duration() const { return _duration; }
  void set_duration(Duration duration) { _duration = duration; }

  unsigned char on_velocity() const { return _on_velocity; }
  void set_on_velocity(unsigned char on_velocity) {
    _on_velocity = on_velocity;
  }

  unsigned char off_velocity() const { return _off_velocity; }
  void set_off_velocity(unsigned char off_velocity) {
    _off_velocity = off_velocity;
  }

  void add_note(unsigned char note) { _notes.set(note); }
  void remove_note(unsigned char note) { _notes.reset(note); }

  iterator begin();
  iterator end();

  const_iterator begin() const;
  const_iterator end() const;

private:
  std::bitset<128> _notes;
  Duration _duration;
  unsigned char _on_velocity;
  unsigned char _off_velocity;
};

template <class type> class NoteBundle::meta_iterator {
  friend class NoteBundle;

public:
  using difference_type = void;
  using value_type = Note;
  using reference = Note;
  using iterator_category = std::bidirectional_iterator_tag;

  operator Note() const = delete;

  std::strong_ordering operator<=>(const meta_iterator &other) const {
    return _index <=> other._index;
  }
  bool operator==(const meta_iterator &other) const {
    return _index == other._index;
  }
  reference operator*() const {
    return Note{_bundle.duration(), (unsigned char)_index,
                _bundle.on_velocity(), _bundle.off_velocity()};
  }
  meta_iterator &operator++() {
    do {
    } while (_index < 128 && !_bundle._notes[++_index]);
    return *this;
  }
  meta_iterator &operator--() {
    do {
    } while (_index > 0 && !_bundle._notes[--_index]);
    if (!_bundle._notes[_index])
      _index = 128;
    return *this;
  }
  meta_iterator operator--(int) {
    meta_iterator copy = *this;
    --*this;
    return copy;
  }
  meta_iterator operator++(int) {
    meta_iterator copy = *this;
    ++*this;
    return copy;
  }

private:
  meta_iterator(type bundle, std::size_t index)
      : _bundle{bundle}, _index{index} {}
  meta_iterator(type bundle) : _bundle{bundle}, _index{0} {}

  type _bundle;
  std::size_t _index;
};

auto inline NoteBundle::begin() -> iterator { return iterator{*this}; };
auto inline NoteBundle::end() -> iterator { return iterator{*this, 128}; };

auto inline NoteBundle::begin() const -> const_iterator {
  return const_iterator{*this};
};
auto inline NoteBundle::end() const -> const_iterator {
  return const_iterator{*this, 128};
};

#endif // NOTE_BUNDLE_HPP_
