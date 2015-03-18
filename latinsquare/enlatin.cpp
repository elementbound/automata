#include <iostream>
#include <fstream>
#include <random>
#include "grid.h"

#define die(msg) {std::cerr << msg << '\n'; return 1;}

int main(int argc, char** argv)
{
	if(argc < 2)
		die("Usage: " << argv[0] << " <input file> <output file>");
	
	unsigned seed = 8;
	grid automata_transitions;
	grid automata_outputs;
	unsigned automata_state = 0;
	std::mt19937 rng;
	
	rng.seed(seed);
	automata_transitions = generate_latin_square(256, rng);
	automata_outputs 	 = generate_latin_square(256, rng);
	
	const unsigned buffer_size = 8192;
	unsigned char* buffer = new unsigned char[buffer_size];
	
	std::ifstream fis(argv[1], std::ios::binary);
	std::ofstream fos(argv[2], std::ios::binary | std::ios::trunc);
	
	if(!fis.good())
		die("Couldn't open input file: " << argv[1]);
	
	if(!fos.good())
		die("Couldn't open output file: " << argv[2]);

	while(fis.good() && fos.good())
	{
		fis.read((char*)buffer, buffer_size);
		
		for(unsigned i=0; i<fis.gcount(); i++)
		{
			unsigned char c = buffer[i];
			
			fos << (char)automata_outputs(automata_state, c);
			automata_state = automata_transitions(automata_state, c);
		}
	}
	
	delete [] buffer;
	
	return 0;
}