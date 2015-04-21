#include <iostream>
#include <vector>
#include <iterator> //std::ostream_iterator
#include <algorithm> //std::swap

int main()
{
	unsigned universe_size = 8;
	std::vector<int> universe[2];
	
	universe[0].resize(universe_size);
	universe[1].resize(universe_size);
	
	std::fill(universe[0].begin(), universe[0].end(), 0);
	std::fill(universe[1].begin(), universe[1].end(), 0);
	
	//universe[0][0] = 1;
	
	std::copy(universe[0].begin(), universe[0].end(), std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	
	while(1)
	{
		for(unsigned i=0; i<universe[0].size(); i++)
		{
			int left, mid, right, out;
			left = (i-1 >= 0) ? universe[0][i-1] : 0;
			mid = universe[0][i];
			right = (i+1 < universe[0].size()) ? universe[0][i+1] : 0;
			
			out = mid;
			
			if(mid == 0)
			{
				if(left != 0)
					out = left+1;
			}
			else
			{
				if(mid == universe_size && right == 0)
					out = mid-1;
				else if(mid == left || mid == right)
					out = mid -1;
			}
			
			universe[1][i] = out;
		}
		
		std::copy(universe[1].begin(), universe[1].end(), std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		
		if(std::equal(universe[0].begin(), universe[0].end(), universe[1].begin()))
			break;
		
		std::swap(universe[0], universe[1]);
	}
	
	return 0;
}