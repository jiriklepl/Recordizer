#include <fstream>
#include <iostream>

#include "Transformator.hpp"

int main(int argc, char *argv[]) {
  std::ofstream soubor("soubor.txt");
  Transformator trans{};
  trans.add_note(Duration{100}, 0, Note{Duration{100},60,50});
  trans.add_note(Duration{200}, 0, Note{Duration{100},64,50});
  trans.add_note(Duration{300}, 0, Note{Duration{100},67,50});
  trans.add_note(Duration{400}, 0, Note{Duration{50},67,50});
  trans.add_note(Duration{500}, 0, Note{Duration{50},67,50});
  trans.add_note(Duration{600}, 0, Note{Duration{100},60,50});
  trans.add_note(Duration{700}, 0, Note{Duration{100},64,50});
  trans.add_note(Duration{800}, 0, Note{Duration{100},62,50});
  trans.add_note(Duration{900}, 0, Note{Duration{50},62,50});
  trans.add_note(Duration{1000}, 0, Note{Duration{50},62,50});
  soubor << trans;
  std::cout << "All sheet written to!" << std::endl;
}
