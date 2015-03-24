#include <iostream>
#include <map>

class automata
{
	private: 
		unsigned m_State = 0;
		std::map<std::pair<unsigned, unsigned>, unsigned> m_Transitions;
		
	public: 
		unsigned state() const;
		void state(unsigned v);
		
		unsigned signal(unsigned s);
		
		void add_transition(unsigned start_state, unsigned input_signal, unsigned result_state);
};

automata automata_from_grid(const grid& g)
{
	;
}

unsigned automata::state() const {
	return m_State;
}

void automata::state(unsigned v) {
	m_State = v;
}

unsigned automata::signal(unsigned s) 
{
	m_State = m_Transition[{m_State, s}];
	return m_State;
}

void automata::add_transition(unsigned start_state, unsigned input_signal, unsigned result_state)
{
	m_Transition.insert({{start_state, input_signal}, result_state});
}

int main(int argc, char** argv)
{
	unsigned block_length = 16;
	
}