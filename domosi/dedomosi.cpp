#include <iostream>
#include <fstream>
#include <random>
#include <cstdlib> //exit
#include <cstdint> //uint8_t
#include "automata.h"

#define die(msg) {std::cerr << msg << '\n'; exit(1);}
//#define debug(msg) {std::cout << msg;}
#define debug(msg) {}

typedef uint8_t signal_t;
typedef uint8_t state_t;

int main(int argc, char** argv)
{
	if(argc < 3)
		die("Usage: " << argv[0] << " <input file> <output file>");

	const char* input_file_name = argv[1];
	const char* output_file_name = argv[2];
	
	//
	
	unsigned input_block_size = 4;
	
	//
	
	std::ifstream fis(input_file_name, std::ios::binary);
	if(!fis)
		die("Couldn't open input file: " << input_file_name);
	
	std::ofstream fos(output_file_name, std::ios::binary | std::ios::trunc);
	if(!fos)
		die("Couldn't open output file: " << output_file_name);
	
	//
	
	automata encoder;
	unsigned signal_count = 256; //1 << (8*output_block_size); // = std::pow(2, 8*output_block_size)
	std::mt19937 rng;
	rng.seed(0);
	
	debug("Generating a " << signal_count << " latin square... ");
	encoder.transitions() = generate_latin_square(signal_count, rng); 
	encoder.state(0);
	debug("Done\n");
	
	while(1)
	{
		signal_t signal;
		
		for(int i=0; i<input_block_size; i++)
		{
			signal = fis.get();
			encoder.signal(signal);
		}
		
		//In the previous loop fis might become invalid; 
		//stop decoding, ran out of input
		if(!fis) 
			break;
		
		fos.put(encoder.state());
	}
	
	return 0;
}