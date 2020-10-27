#include <fstream>
#include <iostream>

#include "TransformatorBuilder.hpp"
#include "NoteConstants.hpp"

int main(int argc, char *argv[]) {
  std::ofstream soubor{"soubor.mid", std::ios::binary};
  Transformator trans{};
  TransformatorBuilder builder{trans, 0};

  builder << ProgramChange{75}
          << shift(100)
          << Note{200, A + 4, 100}
          << Note{50, H + 4, 100}
          << Note{50, A + 4, 100}

          << Note{100, E + 4, 100}
          << Note{200, G + 4, 100}
          << Note{50, F + 4, 100}
          << Note{50, E + 4, 100}

          << Note{300, D + 4, 100}
          << Note{50, D + 4, 100}
          << Note{50, D + 4, 100};

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
