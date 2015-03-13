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
				{
					std::cout.width(3);
					std::cout << m_Matrix(x,y) << ' ';
				}
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
			std::uniform_int_distribution<unsigned> udr(0, s-1);
			
			std::generate(m_ValueSet.begin(), m_ValueSet.end(), 
				[]() -> unsigned {
					static unsigned i=0;
					return i++;
				});
			
			for(unsigned y=0; y<s; y++)
				for(unsigned x=0; x<s; x++)
					m_Matrix(x,y) = (x+y)%s;
				
			//Shuffle rows 
			for(unsigned y=s-1; y>0; y--)
			{
				unsigned swap_row = udr(rng) % y;
				for(unsigned x=0; x<s; x++)
					std::swap(m_Matrix(x,y), m_Matrix(x,swap_row));
			}
			
			//Randomly remap values
			std::shuffle(m_ValueSet.begin(), m_ValueSet.end(), rng);
			for(unsigned y=0; y<s; y++)
				for(unsigned x=0; x<s; x++)
					m_Matrix(x,y) = m_ValueSet[m_Matrix(x,y)];
		}
};

int main(int argc, char** argv)
{
	unsigned size = (argc>1) ? atoi(argv[1]) : 10;
	unsigned seed = (argc>2) ? atoi(argv[2]) : std::chrono::system_clock::now().time_since_epoch().count();
	unsigned test_count = 8;
	
	std::mt19937 rng;
	rng.seed(seed);
	
	std::cout << "A sample latinsquare(" << size << ")" << std::endl;
	latin_square sqr;
	sqr.generate(size, rng);
	sqr.print();
	
	return 0;
}