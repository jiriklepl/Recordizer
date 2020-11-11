#include <fstream>
#include <iostream>

#include "TransformatorBuilder.hpp"
#include "NoteConstants.hpp"
#include "NoteBundle.hpp"

int main(int argc, char *argv[]) {
  std::ofstream soubor{"soubor.mid", std::ios::binary};
  Transformator trans{};
  TransformatorBuilder builder{trans, 0};
  NoteBundle notes{100, 100, C + 4, E + 4, G + 4};
  builder << ProgramChange{75}
          << notes
          << notes
          << notes
          << notes
          << notes
          << notes
          << notes
          << notes
          << notes
          << notes
          << notes
          << notes
          << notes
          << notes
          << notes;

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
  soubor.put(50);
  soubor << trans;
  std::cout << "File written!" << std::endl;
}
