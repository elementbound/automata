#include <iostream>
#include <map>

typedef unsigned char byte_t;
typedef signed long pos_t; 

int main()
{
	byte_t automata = 30;
	std::map<pos_t, bool> universe;

	universe.insert({0, 1});
	for(pos_t i = universe.begin()->first - 1; 
		i < universe.rbegin()->first + 1;
		i++ )
	{
		byte_t state = 0;
		
		//This thing is so ugly I'm enjoying it
		std::map<pos_t, bool>::iterator it;
		state |= ((it = universe.find(i-1)) != universe.end()) ? it->second : 0;
		state |= (((it = universe.find(i  )) != universe.end()) ? it->second : 0) << 1;
		state |= (((it = universe.find(i+1)) != universe.end()) ? it->second : 0) << 2;
	}
	
	return 0;
}