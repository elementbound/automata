#include <iostream>
#include <fstream>
#include <random>
#include <cstdlib> //exit, atoi
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
	if(argc < 6)
		die("Usage: " << argv[0] << " <input file> <output file> <seed> <block size> <code size>");

	const char* input_file_name = argv[1];
	const char* output_file_name = argv[2];
	
	unsigned seed = atoi(argv[3]);
	
	unsigned block_size = atoi(argv[4]); //in bits
	unsigned code_size = atoi(argv[5]); //in steps
	
	//
	
	std::ifstream fis(input_file_name, std::ios::binary);
	if(!fis)
		die("Couldn't open input file: " << input_file_name);
	
	std::ofstream fos(output_file_name, std::ios::binary | std::ios::trunc);
	if(!fos)
		die("Couldn't open output file: " << output_file_name);
	
	if(block_size > 30)
		std::cerr << "Warning: block size greater than 30, this might cause issues\n";
	
	if((block_size*code_size)%8)
		std::cerr << "Output unit size is not a multiple of 8, padding bits could result in garbage characters in decoded file\n";
	
	ibitstream bis; bis.istream(&fis);
	obitstream bos; bos.ostream(&fos);
	
	//
	
	automata encoder;
	unsigned signal_count = 1 << block_size; // = std::pow(2, 8*output_block_size)
	std::mt19937 rng;
	rng.seed(seed);
	
	debug("Generating a " << signal_count << " latin square... ");
	encoder.transitions() = generate_latin_square(signal_count, rng); 
	encoder.state(0);
	debug("Done\n");
	
	while(1)
	{
		signal_t signal = 0;
		debug("Getting " << code_size << " signals\n");
		
		for(int i=0; i<code_size; i++)
		{
			signal = 0;
			ibitstream::get(bis, signal, block_size);
			encoder.signal(signal);
			
			debug("\tGot signal " << signal << ", current state is " << encoder.state() << '\n');
		}
		
		//In the previous loop fis might become invalid; 
		//stop decoding, ran out of input
		if(!fis) 
			break;
		
		debug("\tOutputting " << encoder.state() << '\n');
		obitstream::put(bos, encoder.state(), block_size);
	}
	
	bos.flush();
	
	return 0;
}