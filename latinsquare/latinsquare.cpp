#include <iostream>
#include <algorithm> //std::shuffle, std::swap
#include <vector>
#include <utility> //std::pair
#include <random>
#include <chrono> //std::system_clock::now
#include <cstdlib> //atoi

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
		
		unsigned width() const { return m_Width; }
		unsigned height() const { return m_Height; }
};

class latin_square
{
	private: 
		matrix 					m_Matrix;
		std::vector<unsigned> 	m_ValueSet;
		
		std::pair<bool, unsigned> check_row_collision(unsigned x, unsigned y, unsigned value)
		{
			for(unsigned cx = 0; cx < x; cx++)
			{
				if(m_Matrix(cx,y) == value)
					return {true, cx};
			}
			
			return {false, 0};
		}
		
	public: 
		void print()
		{
			for(unsigned y=0; y<m_Matrix.height(); y++)
			{
				for(unsigned x=0; x<m_Matrix.width(); x++)
					std::cout << m_Matrix(x,y) << ' ';
				std::cout << '\n';
			}
		}
		
		void generate(unsigned s)
		{
			this->generate(s, std::default_random_engine());
		}
		
		template <typename RNG> 
		void generate(unsigned s, RNG rng)
		{
			m_Matrix.resize(s,s);
			m_ValueSet.resize(s);
			
			std::generate(m_ValueSet.begin(), m_ValueSet.end(), 
				[]() -> unsigned {
					static unsigned i=0;
					return i++;
				});
				
			for(unsigned column = 0; column < s; column++)
			{
				//Generate a random column
				std::shuffle(m_ValueSet.begin(), m_ValueSet.end(), rng);
				
				//Copy into the matrix
				for(unsigned row = 0; row < s; row++)
					m_Matrix(column, row) = m_ValueSet[row];
				
				//Check for collisions, if necessary 
				if(column == 0)
					continue;
				
				for(unsigned row=0; row<s; row++)
				{
					std::pair<bool, unsigned> collision = check_row_collision(column, row, m_Matrix(column, row));
					if(!collision.first) continue; //No collision 
					bool collision_resolved = 0;
					
					//Found a collision, now find a row where we can swap the last values 
					//to resolve the collision
					for(unsigned swap_row=0; swap_row<s; swap_row++)
					{
						if(check_row_collision(column, row, m_Matrix(column, swap_row)).first)
							continue; 
						
						if(check_row_collision(column, swap_row, m_Matrix(column, row)).first)
							continue;
						
						//Found a swap, great! 
						std::swap(m_Matrix(column, swap_row), m_Matrix(column, row));
						collision_resolved = 1;
						break;
					}
					
					//If no swap was found, fall back to permutations 
					while(!collision_resolved)
					{
						std::next_permutation(m_ValueSet.begin(), m_ValueSet.end());
						collision_resolved = 1;
						
						for(unsigned row=0; row<s; row++)
							if(check_row_collision(column, row, m_ValueSet[row]).first)
							{
								collision_resolved = 0;
								break;
							}
							
						//Copy into column
						if(collision_resolved) 
							for(unsigned row = 0; row < s; row++)
								m_Matrix(column, row) = m_ValueSet[row];
					}
				}
			}
		}
};

int main(int argc, char** argv)
{
	unsigned size = (argc>1) ? atoi(argv[1]) : 10;
	unsigned seed = (argc>2) ? atoi(argv[2]) : std::chrono::system_clock::now().time_since_epoch().count();
	
	std::mt19937 rng;
	rng.seed(seed);
	
	latin_square sqr;
	sqr.generate(size, rng);
	sqr.print();
	
	return 0;
}