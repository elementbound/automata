#ifndef _H_GRID_H_
#define _H_GRID_H_

#include <vector>
#include <iostream>

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

std::ostream& operator<<(std::ostream& os, const grid& g);

#endif