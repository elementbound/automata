#include <iostream>
#include <string>
#include <map>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

template <typename T>
class tape { 
	private:
		typedef int pos_t;
		typedef int offset_t;

		std::map<pos_t, T> m_Data;
		pos_t m_Pos = 0;

	public:
		T defaultValue;

		const T& read() const {
			if(m_Data.count(m_Pos))
				return m_Data.at(m_Pos);
			else
				return defaultValue;
		}

		void write(T val) {
			m_Data[m_Pos] = val;
		}

		void step(offset_t dst) {
			m_Pos += dst;
		}

		pos_t position() const {
			return m_Pos;
		}

		void rewind() {
			m_Pos = 0;

			/*
			This could be easily done like so: 
				while(tape.read() != tape.defaultValue)
					tape.step(-1);
				tape.step(1);

			If you feel like being a purist. 
			*/
		}
};

//const unsigned OP_SEARCH = 1;

class substringAutomata {
	public: 
		struct symbol_t {
			char value; 
			char type;

			bool operator==(const symbol_t& rhs) const {
				return (this->value == rhs.value && this->type == rhs.type);
			}
		};

		typedef unsigned operation_t;
		typedef unsigned suboperation_t;
		typedef std::pair<operation_t, suboperation_t> state_t;

		//

		static const char SYMBOL_REGULAR 		= 0;
		static const char SYMBOL_OCCURENCE 		= 1;
		static const char SYMBOL_INDICATOR 		= 2; 

		static const char IND_EMPTY 			= 0;
		static const char IND_OCCURENCE 		= 1;
		static const char IND_FIRST_OCCURENCE	= 2;
		static const char IND_TERMINATE			= -1;

		//

		static const operation_t OP_SEARCH		= 1;
		static const operation_t OP_PRINT_MATCH	= 2;

		//

		tape<symbol_t> tapeHaystack;
		tape<symbol_t> tapeNeedle;
		tape<symbol_t> tapeIndicator;

		tape<symbol_t> tapeOutput;

		state_t startState = {OP_SEARCH, 0};
		state_t state = startState;
		state_t terminateState = {-1,-1};

		void init();
		bool step();
		void run();

		void print_tape(tape<symbol_t>& t, bool pos_indicator = 1);
};

typedef substringAutomata::symbol_t			symbol_t;
typedef substringAutomata::operation_t		operation_t;
typedef substringAutomata::suboperation_t	suboperation_t;
typedef substringAutomata::state_t			state_t;

const operation_t substringAutomata::OP_SEARCH;
const operation_t substringAutomata::OP_PRINT_MATCH;

void bold(bool on) {
	HANDLE std_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(std_handle, (on ? 0 : FOREGROUND_RED) | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void substringAutomata::init() {
	tapeHaystack.defaultValue = {'\0', SYMBOL_REGULAR};
	tapeNeedle.defaultValue = {'\0', SYMBOL_REGULAR};
	tapeIndicator.defaultValue = {IND_TERMINATE, SYMBOL_INDICATOR};
}

bool substringAutomata::step() {
	if(state.first == OP_SEARCH) {
		switch(state.second) {
			case 0: 
				if(tapeHaystack.read() == tapeNeedle.read()) {
					tapeIndicator.write({IND_FIRST_OCCURENCE, SYMBOL_INDICATOR});

					tapeIndicator.step(1);
					tapeHaystack.step(1);
					tapeNeedle.step(1);

					state.second++;
				}
				else {
					state = terminateState;
				}
			break;

			case 1: 
				if(tapeNeedle.read() == tapeNeedle.defaultValue) { //Full match
					tapeNeedle.rewind();
					tapeHaystack.rewind();
					tapeIndicator.rewind();

					state = {OP_PRINT_MATCH, 0};
				}
				else if(tapeHaystack.read() == tapeNeedle.read()) {
					tapeIndicator.write({IND_OCCURENCE, SYMBOL_INDICATOR});

					tapeIndicator.step(1);
					tapeHaystack.step(1);
					tapeNeedle.step(1);
				}
				else 
					state = terminateState;
			break;
		}
	}
	else if(state.first == OP_PRINT_MATCH) {
		switch(tapeIndicator.read().value) {
			case IND_TERMINATE:
				state = terminateState;
			break;

			case IND_EMPTY:
				tapeOutput.write({tapeHaystack.read().value, SYMBOL_REGULAR});
				tapeOutput.step(1);
			break;

			case IND_FIRST_OCCURENCE:
			case IND_OCCURENCE:
				tapeOutput.write({tapeHaystack.read().value, SYMBOL_OCCURENCE});
				tapeOutput.step(1);
			break;
		}

		tapeHaystack.step(1);
		tapeIndicator.step(1);
	}

	return state != terminateState;
}

void substringAutomata::print_tape(tape<symbol_t>& t, bool pos_indicator) {
	unsigned posToRestore = t.position();

	t.rewind();
	while(1)
	{
		symbol_t s = t.read();

		switch(s.type)
		{
			case SYMBOL_REGULAR:
				std::cout << s.value;
			break;

			case SYMBOL_INDICATOR: 
				switch(s.value) {
					case IND_EMPTY : std::cout << ' '; break;
					case IND_FIRST_OCCURENCE : std::cout << 'S'; break;
					case IND_OCCURENCE : std::cout << 'O'; break;
					case IND_TERMINATE : std::cout << '_'; break;
				}
			break;

			case SYMBOL_OCCURENCE:
				bold(true);
				std::cout << s.value;
				bold(false);
			break;
		}

		if(s == t.defaultValue)
			break;

		t.step(1);
	}
	std::cout << '\n';

	t.rewind();
	while((posToRestore--) > 0)
		t.step(1);

	if(pos_indicator) {
		for(int i = 0; i <= t.position(); i++)
			std::cout << (i == t.position() ? '^' : ' ');
		std::cout << '\n';
	}
}

int main()
{
	substringAutomata automata;
	automata.init();

	std::string haystack = "bababa";
	std::string needle = "baba";

	//Fill tapes
	for(char c : haystack) {
		automata.tapeHaystack.write({c, substringAutomata::SYMBOL_REGULAR});
		automata.tapeHaystack.step(1);

		automata.tapeIndicator.write({substringAutomata::IND_EMPTY, substringAutomata::SYMBOL_INDICATOR});
		automata.tapeIndicator.step(1);
	}

	for(char c : needle) {
		automata.tapeNeedle.write({c, substringAutomata::SYMBOL_REGULAR});
		automata.tapeNeedle.step(1);
	}

	//Rewind
	automata.tapeHaystack.rewind();
	automata.tapeNeedle.rewind();
	automata.tapeIndicator.rewind();

	//Print?
	do
	{
		std::cout << "Haystack: \n";
		automata.print_tape(automata.tapeHaystack); 
		std::cout << '\n';

		std::cout << "Needle: \n";
		automata.print_tape(automata.tapeNeedle); 
		std::cout << '\n';

		std::cout << "Indicator: \n";
		automata.print_tape(automata.tapeIndicator); 
		std::cout << '\n';

		std::cout << "Output: \n";
		automata.print_tape(automata.tapeOutput); 
		std::cout << '\n';

		std::cout << "=== === === ===\n";
	}
	while(automata.step());

	return 0;
}