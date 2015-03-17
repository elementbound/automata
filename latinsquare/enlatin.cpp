#include <iostream>
#include <random>
#include "grid.h"

int main(int argc, char** argv)
{
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
	
	while(std::cin.good())
	{
		std::cin.read((char*)buffer, buffer_size);
		for(unsigned i=0; i<std::cin.gcount(); i++)
		{
			unsigned char c = buffer[i];
			
			std::cout << (char)automata_outputs(automata_state, c);
			automata_state = automata_transitions(automata_state, c);
		}
	}
	
	delete [] buffer;
	
	return 0;
}