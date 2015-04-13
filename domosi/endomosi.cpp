#include <iostream>
#include <fstream>
#include <random>
#include <cstdlib> //exit
#include <cstdint> //uintmax_t
#include "automata.h"
#include "bitstream.h"

#define die(msg) {std::cerr << msg << '\n'; exit(1);}
//#define debug(msg) {std::cout << msg;}
#define debug(msg) {}

typedef uintmax_t signal_t;
typedef uintmax_t state_t;

int main(int argc, char** argv)
{
	if(argc < 3)
		die("Usage: " << argv[0] << " <input file> <output file>");
	
	unsigned block_size = 8; //in bits
	unsigned code_size = 4; //in steps
	
	const char* input_file_name = argv[1];
	const char* output_file_name = argv[2];
	
	//
	
	std::ifstream fis(input_file_name, std::ios::binary);
	if(!fis)
		die("Couldn't open input file: " << input_file_name);
	
	std::ofstream fos(output_file_name, std::ios::binary | std::ios::trunc);
	if(!fos)
		die("Couldn't open output file: " << output_file_name);
	
	ibitstream bis; bis.istream(&fis);
	obitstream bos; bos.ostream(&fos);
	
	//
	
	automata encoder;
	grid reverse_lookup_table;
	unsigned signal_count = 1 << (block_size); // = std::pow(2, 8*input_block_size)
	std::mt19937 rng;
	rng.seed(0);
	
	debug("Generating a " << signal_count << " latin square... ");
	encoder.transitions() = generate_latin_square(signal_count, rng); 
	debug("Done\n");
	
	debug("Generating reverse lookup table... ")
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
	debug("Done!\n");
	
	encoder.state(0);
	
	while(1)
	{
		signal_t signal;
		state_t input;
			bis.get(input, block_size);
		
		//Ran out of bytes, stop encoding
		if(!fis) 
			break;
		
		debug("Encoding character " << int(input) << std::endl);
		
		for(unsigned i=0; i < code_size-1; i++)
		{
			signal = rng() % signal_count;
			encoder.signal(signal);
			
			debug("\tGot signal#" << int(signal) << ", current state is " << int(encoder.state()) << std::endl);
			bos.put(signal, block_size);
		}
		
		signal = reverse_lookup_table(encoder.state(), input);
		encoder.signal(signal);
		debug("\tFound signal#" << int(signal) << ", current state is " << int(encoder.state()) << std::endl);
		bos.put(signal, block_size);
	}
	
	return 0;
}