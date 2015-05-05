#include <iostream>
#include <map>

template <typename T>
class tape { 
	private:
		typedef unsigned pos_t;
		typedef int		 offset_t;

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
};

class substringAutomata {
	public: 
		static const char SYMBOL_REGULAR 	= 0;
		static const char SYMBOL_OCCURENCE 	= 1;
		static const char SYMBOL_CONTROL 	= 2; 

		static const char CHAR_OCCURENCE = 1;

		struct symbol_t {
			char value; 
			char type;
		};

		typedef unsigned state_t;

	public:
		tape<symbol_t> tapeHaystack;
		tape<symbol_t> tapeNeedle;
		tape<symbol_t> tapeIndicator;

		tape<symbol_t> tapeOutput;
};

int main()
{
	return 0;
}