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
		typedef std::vector<unsigned>::iterator iterator;
	
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
		
		iterator begin() { return m_Data.begin(); }
		iterator end() { return m_Data.end(); }
};

std::ostream& operator<<(std::ostream& os, matrix& m)
{
	for(unsigned y=0; y<m.height(); y++)
	{
		for(unsigned x=0; x<m.width(); x++)
			os << m(x,y) << ' ';
		os << '\n';
	}

	return os;
}

template <typename RNG> 
matrix generate_latin_square(unsigned s, RNG rng)
{
	matrix retMatrix;
	std::vector<unsigned> value_set;
	
	retMatrix.resize(s,s);
	value_set.resize(s);
	std::uniform_int_distribution<unsigned> udr(0, s-1);
	
	std::generate(value_set.begin(), value_set.end(), 
		[]() -> unsigned {
			static unsigned i=0;
			return i++;
		});
	
	for(unsigned y=0; y<s; y++)
		for(unsigned x=0; x<s; x++)
			retMatrix(x,y) = (x+y)%s;
		
	//Shuffle rows 
	for(unsigned y=s-1; y>0; y--)
	{
		unsigned swap_row = udr(rng) % y;
		for(unsigned x=0; x<s; x++)
			std::swap(retMatrix(x,y), retMatrix(x,swap_row));
	}
	
	//Randomly remap values
	std::shuffle(value_set.begin(), value_set.end(), rng);
	std::transform(retMatrix.begin(), retMatrix.end(), retMatrix.begin(), 
		[&, value_set] (unsigned i) -> unsigned {
			return value_set[i];
		}
	);
	
	return retMatrix;
}

matrix generate_latin_square(unsigned s)
{
	return generate_latin_square(s, std::default_random_engine());
}

int main(int argc, char** argv)
{
	unsigned size = (argc>1) ? atoi(argv[1]) : 10;
	unsigned seed = (argc>2) ? atoi(argv[2]) : std::chrono::system_clock::now().time_since_epoch().count();
	unsigned test_count = (argc>3) ? atoi(argv[3]) : 16;
	unsigned test_success = test_count;
	
	std::mt19937 rng;
	rng.seed(seed);
	matrix sqr;

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