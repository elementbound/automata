#ifndef _H_AUTOMATA_H_
#define _H_AUTOMATA_H_

#include "latinsquare/grid.h"

class automata
{
	private: 
		unsigned m_State = 0;
		grid m_Transitions;
		//Automata output is ignored for this algorithm
		
	public: 
		unsigned state() const;
		void state(unsigned v);
		
		unsigned signal(unsigned s);
		
		grid& transitions();
		const grid& transitions() const;
};

#endif