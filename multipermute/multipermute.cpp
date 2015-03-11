#include <iostream>
#include <string>
#include <random>
#include <algorithm> //shuffle
#include <cstdlib> //atoi

const unsigned buffer_size = 8192;

class permute_coder
{
	private: 
		unsigned char m_EncoderTable[256];
		unsigned char m_DecoderTable[256];
		
	public: 
		permute_coder();
		permute_coder(const permute_coder&) = delete;
		permute_coder(permute_coder&&) = default;
		~permute_coder() = default;
		
		void seed(int val);
		
		char encode(char x);
		char decode(char x);
};

void print_usage(char* n)
{
	std::cerr << "Usage: " << n << " [en/de] state-count password\n"
			  << "( password length should be 4*state-count)" << std::endl;
}

int main(int argc, char** argv)
{
	if(argc < 4)
	{
		print_usage(argv[0]);
		return 1;
	}
	
	std::string mode = argv[1];
	
	if(mode!="en" && mode!="de")
	{
		print_usage(argv[0]);
		return 1;
	}
	
	unsigned state_count = atoi(argv[2]);
	std::string password = argv[3];
	
	permute_coder states[state_count];
	unsigned current_state = 0;
	
	unsigned pass_pos = 0;
	for(unsigned i=0; i<state_count; i++)
	{
		unsigned seed = 0;
		for(unsigned j=0; j<sizeof(unsigned); j++)
		{
			seed = (seed<<8) | password[pass_pos];
			pass_pos = (pass_pos+1)%password.length();
		}
		
		std::cout << "Seed#" << i << ": " << seed << std::endl;
		states[i].seed(seed);
	}
	
	unsigned char buffer[buffer_size];
	while(std::cin.good())
	{
		std::cin.read((char*)buffer, buffer_size);
		unsigned read_count = std::cin.gcount();
		
		for(unsigned i=0; i<read_count; i++)
		{
			if(mode == "en")
			{
				char c = buffer[i];
				char enc = states[current_state].encode(c);
				std::cout.put(enc);
				
				current_state = (current_state + c) % state_count;
			}
			
			if(mode == "de")
			{
				char c = buffer[i];
				char dec = states[current_state].decode(c);
				std::cout.put(dec);
				
				current_state = (current_state + dec) % state_count;
			}
		}
	}
	
	return 0;
}

permute_coder::permute_coder()
{
	this->seed(0);
}

void permute_coder::seed(int val)
{
	std::default_random_engine random_gen;
	random_gen.seed(val);
	
	unsigned char shuffled_charset[256];
	for(unsigned i=0; i<256; i++)
		shuffled_charset[i] = i;
	
	std::shuffle(shuffled_charset+'a', shuffled_charset+'z', random_gen);
	
	for(unsigned i=0; i<256; i++)
	{
		m_EncoderTable[i] = shuffled_charset[i];
		m_DecoderTable[shuffled_charset[i]] = i;
	}
}

char permute_coder::encode(char x)
{
	return m_EncoderTable[static_cast<unsigned char>(x)];
}

char permute_coder::decode(char x)
{
	return m_DecoderTable[static_cast<unsigned char>(x)];
}