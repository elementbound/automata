#include <iostream>
#include <random>
#include "grid.h"

#define DEBUG(txt) (std::cerr << txt);
//#define DEBUG(txt) 

int main(int argc, char** argv)
{
	unsigned seed = 8;
	grid automata_transitions;
	grid automata_outputs;
	unsigned automata_state = 0;
	std::mt19937 rng;
	
	DEBUG("Generating automata... ");
	rng.seed(seed);
	automata_transitions = generate_latin_square(256, rng);
	automata_outputs 	 = generate_latin_square(256, rng);
	DEBUG("done\n");
	
	const unsigned buffer_size = 8192;
	unsigned char* buffer = new unsigned char[buffer_size];
	
	while(std::cin.good())
	{
		std::cin.read((char*)buffer, buffer_size);
		for(unsigned i=0; i<std::cin.gcount(); i++)
		{
			int c = buffer[i];
			
			//Do a reverse lookup
			for(unsigned y=0; y<256; y++)
			{
				if(automata_outputs(automata_state, y) == c)
				{
					std::cout << (char)y;
					automata_state = automata_transitions(automata_state, y);
					
					break;
				}
				
				if(y == 255)
					std::cerr << "No matching character found for " << c << "!\n";
			}
		}
	}
	
	delete [] buffer;
	
	return 0;
}