#include <iostream>
#include <fstream>
#include <random>
#include <cstdlib> //exit
#include "automata.h"

#define die(msg) {std::cerr << msg << '\n'; exit(1);}

int main(int argc, char** argv)
{
	if(argc < 3)
		die("Usage: " << argv[0] << " <input file> <output file>");
	
	unsigned input_block_size = 4; //in bytes
	unsigned output_block_size = 1; //in steps
	
	const char* input_file_name = argv[1];
	const char* output_file_name = argv[2];
	
	//
	
	std::ifstream fis(input_file_name, std::ios::binary);
	if(!fis)
		die("Couldn't open input file: " << input_file_name);
	
	std::ofstream fos(output_file_name, std::ios::binary | std::ios::trunc);
	if(!fos)
		die("Couldn't open output file: " << output_file_name);
	
	//
	
	automata encoder;
	grid reverse_lookup_table;
	unsigned signal_count = 1 << (8*output_block_size); // = std::pow(2, 8*output_block_size)
	std::mt19937 rng;
	rng.seed(0);
	
	std::cout << "Generating a " << signal_count << " latin square\n";
	encoder.transitions() = generate_latin_square(signal_count, rng); 
	
	std::cout << "Generating reverse lookup table... ";
	reverse_lookup_table.resize(signal_count,signal_count);
	for(unsigned x=0; x<signal_count; x++) //current state
	{
		for(unsigned y=0; y<signal_count; y++) //desired state
		{
			for(unsigned cy=0; cy<signal_count; cy++)
				if(encoder.transitions()(x,cy) ==y)
					reverse_lookup_table(x,y) = cy;
		}
	}
	std::cout << "Done!\n";
	
	char* buffer = new char[4];
	for(unsigned i=0; i<4; i++)
		buffer[i] = 0;
	
	encoder.state(0);
	
	while(fis)
	{
		unsigned signal;
		char state;
		for(unsigned i=0; i < input_block_size; i++)
		{
			fis.read((char*)&signal, sizeof(signal));
			state = encoder.signal(signal);
			
			//Not really portable, now is it? 
			fos.write((const char*)&state, sizeof(state));
		}
	}
	
	delete [] buffer;
	
	return 0;
}