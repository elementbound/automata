#include <iostream>
#include <map>
#include <cstdlib> //atoi

typedef unsigned char byte_t;
typedef signed long pos_t; 

int main(int argc, char** argv)
{
	byte_t automata = 30;
	unsigned iter_count = 24;
	
	if(argc > 1)
		automata = atoi(argv[1]);
	
	if(argc > 2)
		iter_count = atoi(argv[2]);
	
	iter_count = 32;
	
	std::map<pos_t, bool> universe[2];
	universe[0].insert({0, 1});
	
	for(unsigned iteration=iter_count; iteration; iteration--)
	{
		std::pair<pos_t, pos_t> bounds;
			bounds.first  = universe[0].begin()->first - 1;
			bounds.second = universe[0].rbegin()->first + 1;
			
		for(pos_t i = bounds.first; i <= bounds.second; i++ )
		{
			byte_t state = 0;
			
			state |= universe[0][i-1];
			state |= universe[0][i  ] << 1;
			state |= universe[0][i+1] << 2;
			
			universe[1][i] = (automata & ( 1 << state )) != 0;
			
			//std::cout << universe[1][i];
		}
		
		universe[0].swap(universe[1]);
		//std::cout << std::endl;
	}
	
	unsigned val = 0;
	for(unsigned i=0; i<32; i++)
		val = (val << 1) | universe[0][i];
	
	std::cout << val << std::endl;
	
	return 0;
}