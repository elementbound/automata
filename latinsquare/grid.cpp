#include "grid.h"

void grid::resize(unsigned w, unsigned h)
{
	m_Width = w;
	m_Height = h;
	m_Data.resize(m_Width*m_Height);
}

unsigned& grid::get(unsigned x, unsigned y) { return m_Data[y*m_Width + x]; }  
const unsigned& grid::get(unsigned x, unsigned y) const { return m_Data[y*m_Width + x]; } 
unsigned& grid::operator()(unsigned x, unsigned y) { return this->get(x,y); }
const unsigned& grid::operator()(unsigned x, unsigned y) const { return this->get(x,y); } 

unsigned grid::width() const { return m_Width; }
unsigned grid::height() const { return m_Height; }

grid::iterator grid::begin() { return m_Data.begin(); }
grid::iterator grid::end() { return m_Data.end(); }
grid::reverse_iterator grid::rbegin() { return m_Data.rbegin(); }
grid::reverse_iterator grid::rend() { return m_Data.rend(); }

grid::const_iterator grid::cbegin() const { return m_Data.cbegin(); }
grid::const_iterator grid::cend() const { return m_Data.cend(); }
grid::const_reverse_iterator grid::crbegin() const { return m_Data.crbegin(); }
grid::const_reverse_iterator grid::crend() const { return m_Data.crend(); }

std::ostream& operator<<(std::ostream& os, const grid& g)
{
	for(unsigned y=0; y<g.height(); y++)
	{
		for(unsigned x=0; x<g.width(); x++)
			os << g(x,y) << ' ';
		os << '\n';
	}

	return os;
}