#include <fstream>

#include "Duration.hpp"


int main(int argc, char *argv[])
{
	std::ofstream soubor("soubor.txt");
	soubor << Duration(0x0);
	soubor << Duration(0x40);
	soubor << Duration(0x7F);
	soubor << Duration(0x80);
	soubor << Duration(0x2000);
	soubor << Duration(0x3FFF);
	soubor << Duration(0x4000);
	soubor << Duration(0x100000);
	soubor << Duration(0x1FFFFF);
	soubor << Duration(0x200000);
	soubor << Duration(0x8000000);
	soubor << Duration(0xFFFFFFF);
	std::cout << "All sheet written to!" << std::endl;
}
