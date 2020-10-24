#include <fstream>
#include <iostream>

#include "Transformator.hpp"

int main(int argc, char *argv[]) {
  std::ofstream soubor{"soubor.mid", std::ios::binary};
  Transformator trans{};
  Duration time{};
  trans.add_note(time, 0, Note{Duration{100},60,50}); time = time + Duration{100};
  trans.add_note(time, 0, Note{Duration{100},64,50}); time = time + Duration{100};
  trans.add_note(time, 0, Note{Duration{100},60,50}); time = time + Duration{100};
  trans.add_note(time, 0, Note{Duration{100},64,50}); time = time + Duration{100};
  trans.add_note(time, 0, Note{Duration{200},67,50}); time = time + Duration{100};
  trans.add_note(time, 0, Note{Duration{100},67,50}); time = time + Duration{100};
  trans.add_note(time, 0, Note{Duration{100},67,50}); time = time + Duration{100};
  trans.add_note(time, 0, Note{Duration{100},60,50}); time = time + Duration{100};
  trans.add_note(time, 0, Note{Duration{100},64,50}); time = time + Duration{100};
  trans.add_note(time, 0, Note{Duration{100},60,50}); time = time + Duration{100};
  trans.add_note(time, 0, Note{Duration{100},64,50}); time = time + Duration{100};
  trans.add_note(time, 0, Note{Duration{200},62,50}); time = time + Duration{100};
  trans.add_note(time, 0, Note{Duration{100},62,50}); time = time + Duration{100};
  trans.add_note(time, 0, Note{Duration{100},62,50}); time = time + Duration{100};

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
  soubor.put(200);
  soubor << trans;
  std::cout << "File written!" << std::endl;
}
