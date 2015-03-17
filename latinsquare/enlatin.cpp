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
	
	while(1)
	{
		int c = 0;
		c = std::cin.get();
		
		if( c == EOF) 
			break;
		
		std::cout << (char)automata_outputs(automata_state, c);
		automata_state = automata_transitions(automata_state, c);
	}
	
	return 0;
}