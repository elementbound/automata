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
	grid reverse_lookup_table;
	unsigned automata_state;
	std::mt19937 rng;
	
	rng.seed(seed);
	automata_transitions = generate_latin_square(256, rng);
	automata_outputs 	 = generate_latin_square(256, rng);
	automata_state = rng() % 256;
	
	reverse_lookup_table.resize(256, 256);
	for(unsigned x=0; x<256; x++)
	{
		for(unsigned y=0; y<256; y++)
			for(unsigned cy=0; cy<256; cy++)
				if(automata_outputs(x,cy) == y)
				{
					reverse_lookup_table(x,y) = cy;
					break;
				}
	}
	
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
			int c = buffer[i];
			int d = reverse_lookup_table(automata_state, c);
			fos << (char)d;
			automata_state = automata_transitions(automata_state, d);
		}
	}
	
	delete [] buffer;
	
	return 0;
}