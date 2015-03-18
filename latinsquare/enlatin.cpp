#include <iostream>
#include <fstream>
#include <random>
#include <cstdlib> //atoi
#include "grid.h"

#define die(msg) {std::cerr << msg << '\n'; return 1;}

int main(int argc, char** argv)
{
	if(argc < 3)
		die("Usage: " << argv[0] << " <seed> <input file> <output file>");
	
	unsigned seed = atoi(argv[1]);
	const char* input_file = argv[2];
	const char* output_file = argv[3];
	grid automata_transitions;
	grid automata_outputs;
	unsigned automata_state;
	std::mt19937 rng;
	
	rng.seed(seed);
	automata_transitions = generate_latin_square(256, rng);
	automata_outputs 	 = generate_latin_square(256, rng);
	automata_state = rng() % 256;
	
	const unsigned buffer_size = 8192;
	unsigned char* buffer = new unsigned char[buffer_size];
	
	std::ifstream fis(input_file, std::ios::binary);
	std::ofstream fos(output_file, std::ios::binary | std::ios::trunc);
	
	if(!fis.good())
		die("Couldn't open input file: " << input_file);
	
	if(!fos.good())
		die("Couldn't open output file: " << output_file);

	while(fis.good() && fos.good())
	{
		fis.read((char*)buffer, buffer_size);
		
		for(unsigned i=0; i<fis.gcount(); i++)
		{
			unsigned char c = buffer[i];
			
			buffer[i] = (char)automata_outputs(automata_state, c);
			automata_state = automata_transitions(automata_state, c);
		}
		
		fos.write((char*)buffer, fis.gcount());
	}
	
	delete [] buffer;
	
	return 0;
}