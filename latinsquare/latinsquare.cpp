#include <iostream>
#include <algorithm> //random_shuffle
#include <vector>

class matrix
{
	private: 
		std::vector<unsigned> m_Data;
		unsigned m_Width;
		unsigned m_Height;
		
	public: 
		void resize(unsigned w, unsigned h)
		{
			m_Width = w;
			m_Height = h;
			m_Data.resize(m_Width*m_Height);
		}
		
		unsigned& operator()(unsigned x, unsigned y)
		{
			return m_Data[y*m_Width + x];
		}
}

int main()
{
	unsigned size = 8;
	
	std::vector<unsigned> valset;
	matrix square;
	
	valset.resize(size);
	for(unsigned i=0; i<size; i++)
		valset[i] = i;
	
	matrix.resize(size,size);
	
	for(unsigned x=0; x<size; x++)
	{
		std::random_shuffle(valset);
			
		for(unsigned y=0; y<size; y++)
			square(x,y) = valset[y];
		
		if(x==0)
			continue; 
		
		bool collision = 0;
		for(unsigned y=0; y<size; y++)
			for(unsigned cx=x-1; cx<x; x++)
				if(square(cx,y) == square(x,y))
				{
					collision = 1;
					break;
				}
				
		if(!collision)
			continue;
		
		for(unsigned sy=0; sy<size; sy++)
		{
			if(sy == y)
				continue;
			
			collision = 0;
			for(unsigned cx=x-1; x<x; x++)
			{
				
			}
		}
	}
}