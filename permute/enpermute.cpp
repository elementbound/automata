#include <iostream>
#include <random>
#include <algorithm> //shuffle
#include <cstdlib> //atoi

const unsigned buffer_size = 8192;

int main(int argc, char** argv)
{
	if(argc < 2) 
	{
		std::cerr << "Usage: " << argv[0] << " seed" << std::endl;
		return 1; 
	}
	
	std::default_random_engine random_gen;
	random_gen.seed(atoi(argv[1]));
	
	char shuffled_charset[256];
	for(unsigned i=0; i<256; i++)
		shuffled_charset[i] = i;
	
	std::shuffle(shuffled_charset+1, shuffled_charset+256, random_gen);
	
	char signal_table[256];
	for(unsigned i=0; i<256; i++)
		signal_table[i] = shuffled_charset[i];
	
	char buffer[buffer_size];
	while(std::cin.good())
	{
		std::cin.read(buffer, buffer_size);
		unsigned read_count = std::cin.gcount();
		
		for(unsigned i=0; i<read_count; i++)
			std::cout.put(signal_table[buffer[i]]);
	}
	
	return 0;
}