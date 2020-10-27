#include <fstream>
#include <iostream>

#include "Transformator.hpp"
#include "NoteConstants.hpp"

int main(int argc, char *argv[]) {
  std::ofstream soubor{"soubor.mid", std::ios::binary};
  Transformator trans{};

  trans.append_event<ProgramChange>(0, 0, 75);
  trans.append_note(100, 0, {100, C + 4, 100});
  trans.append_note(0, 0, {100, E + 4, 100});
  trans.append_note(0, 0, {100, C + 4, 100});
  trans.append_note(0, 0, {100, E + 4, 100});
  trans.append_note(0, 0, {200, G + 4, 100});
  trans.append_note(0, 0, {100, G + 4, 100});
  trans.append_note(0, 0, {100, G + 4, 100});

  trans.append_note(0, 0, {100, C + 4, 100});
  trans.append_note(0, 0, {100, E + 4, 100});
  trans.append_note(0, 0, {100, C + 4, 100});
  trans.append_note(0, 0, {100, E + 4, 100});
  trans.append_note(0, 0, {200, D + 4, 100});
  trans.append_note(0, 0, {100, D + 4, 100});
  trans.append_note(0, 0, {100, D + 4, 100});

  soubor << "MThd";
  soubor.put(0);
  soubor.put(0);
  soubor.put(0);
  soubor.put(6);
  soubor.put(0);
  soubor.put(1);
  soubor.put(0);
  soubor.put(1);
  soubor.put(0);
  soubor.put(100);
  soubor << trans;
  std::cout << "File written!" << std::endl;
}
