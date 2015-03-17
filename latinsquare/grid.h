#ifndef _H_GRID_H_
#define _H_GRID_H_

#include <vector>
#include <iostream>
#include <random>
#include <algorithm>

class grid
{
	private: 
		std::vector<unsigned> m_Data;
		unsigned m_Width;
		unsigned m_Height;
		
	public: 
		typedef decltype(m_Data)::iterator 					iterator;
		typedef decltype(m_Data)::const_iterator 			const_iterator;
		typedef decltype(m_Data)::reverse_iterator 			reverse_iterator;
		typedef decltype(m_Data)::const_reverse_iterator 	const_reverse_iterator;
	
		void resize(unsigned w, unsigned h);
		
		unsigned& get(unsigned x, unsigned y);
		const unsigned& get(unsigned x, unsigned y) const;
		unsigned& operator()(unsigned x, unsigned y);
		const unsigned& operator()(unsigned x, unsigned y) const;
		
		unsigned width() const;
		unsigned height() const;
		
		iterator begin();
		iterator end();
		reverse_iterator rbegin();
		reverse_iterator rend();
		
		const_iterator cbegin() const;
		const_iterator cend() const; 
		const_reverse_iterator crbegin() const; 
		const_reverse_iterator crend() const;
};

//

template <typename RNG> 
grid generate_latin_square(unsigned s, RNG rng){
	grid retgrid;
	std::vector<unsigned> value_set;
	
	retgrid.resize(s,s);
	value_set.resize(s);
	std::uniform_int_distribution<unsigned> udr(0, s-1);
	
	std::generate(value_set.begin(), value_set.end(), 
		[]() -> unsigned {
			static unsigned i=0;
			return i++;
		});
	
	for(unsigned y=0; y<s; y++)
		for(unsigned x=0; x<s; x++)
			retgrid(x,y) = (x+y)%s;
		
	//Shuffle rows 
	for(unsigned y=s-1; y>0; y--)
	{
		unsigned swap_row = udr(rng) % y;
		for(unsigned x=0; x<s; x++)
			std::swap(retgrid(x,y), retgrid(x,swap_row));
	}
	
	//Randomly remap values
	std::shuffle(value_set.begin(), value_set.end(), rng);
	std::transform(retgrid.begin(), retgrid.end(), retgrid.begin(), 
		[&, value_set] (unsigned i) -> unsigned {
			return value_set[i];
		}
	);
	
	return retgrid;
}

inline grid generate_latin_square(unsigned s)
{
	return generate_latin_square(s, std::default_random_engine());
}

std::ostream& operator<<(std::ostream& os, const grid& g);

#endif