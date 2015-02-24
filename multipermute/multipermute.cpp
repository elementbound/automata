#include <iostream>
#include <string>
#include <random>
#include <algorithm> //shuffle
#include <cstdlib> //atoi

int main(int argc, char** argv)
{
	if(argc < 3)
	{
		std::cerr << "Usage: " << argv[0] << " state-count password\n"
				  << "( password length should be 4*state-count)" << std::endl;
		return 1;
	}
	
	unsigned state_count = atoi(argv[1]);
	std::string password = argv[2];
	
	unsigned pass_pos = 0;
	for(unsigned i=0; i<state_count; i++)
	{
		unsigned seed = 0;
		for(unsigned j=0; j<sizeof(unsigned); j++)
		{
			seed = (seed<<8) | password[pass_pos];
			pass_pos = (pass_pos+1)%password.length();
		}
		
		std::cout << "Seed#" << i << ": " << seed << std::endl;
	}
	
	return 0;
}