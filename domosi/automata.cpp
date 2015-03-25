#include "automata.h"

unsigned automata::state() const {return m_State;}
void 	 automata::state(unsigned v) {m_State = v;}

unsigned automata::signal(unsigned s)
{
	m_State = m_Transitions(m_State, s);
	return m_State;
}

	  grid& automata::transitions() {return m_Transitions;}
const grid& automata::transitions() const {return m_Transitions;}