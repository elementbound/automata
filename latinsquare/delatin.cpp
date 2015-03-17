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
	
	while(std::cin.good())
	{
		int c = 0;
		c = std::cin.get();
		
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
	
	return 0;
}