#include <iostream>
#include <random>
#include <chrono> //std::system_clock::now
#include <cstdlib> //atoi

#include "grid.h"

int main(int argc, char** argv)
{
	unsigned size = (argc>1) ? atoi(argv[1]) : 10;
	unsigned seed = (argc>2) ? atoi(argv[2]) : std::chrono::system_clock::now().time_since_epoch().count();
	unsigned test_count = (argc>3) ? atoi(argv[3]) : 16;
	unsigned test_success = test_count;
	
	std::mt19937 rng;
	rng.seed(seed);
	grid sqr;

	if(size < 32)
	{
		std::cout << "A sample latinsquare(" << size << ", " << seed << ")" << std::endl;
		sqr = generate_latin_square(size, rng);
		std::cout << sqr;
	}
	
	for(unsigned test_id = 0; test_id < test_count; test_id++)
	{
		std::cout << "Doing test " << (test_id+1) << "/" << test_count << '\r';
		sqr = generate_latin_square(size, rng);
		
		for(unsigned check_y=0; check_y<size; check_y++)
		{
			for(unsigned check_x=0; check_x<size; check_x++)
			{
				//Check row
				for(unsigned x=0; x<size; x++)
					if(sqr(x,check_y) == sqr(check_x,check_y) && x!=check_x)
					{
						std::cout << "\nTest fail!" << std::endl;
						std::cout << sqr;
						--test_success;
						break; //from check row
						break; break; //from each-cell
					}
					
				//Check column
				for(unsigned y=0; y<size; y++)
					if(sqr(check_x,y) == sqr(check_x,check_y) && y!=check_y)
					{
						std::cout << "\nTest fail!" << std::endl;
						std::cout << sqr;
						--test_success;
						break; //from check column
						break; break; //from each-cell
					}
			}
		}
	}
	
	std::cout << std::endl << test_success << "/" << test_count << " passed" << std::endl;
	
	return 0;
}