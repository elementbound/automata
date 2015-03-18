#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <cstdlib> //atoi, exit
#include <cctype>

#define die(msg) {std::cerr << msg << '\n'; exit(1);}

int main(int argc, char** argv)
{
	if(argc < 4)
		die("Usage: " << argv[0] << " <wordset file> <output size> <output file>");
	
	const char*	words_file 	= argv[1];
	unsigned	output_size = atoi(argv[2]);
	const char*	output_file = argv[3];
	
	std::ifstream fis(words_file);
	std::ofstream fos(output_file, std::ios::trunc);
	
	if(!fis)
		die("Couldn't open wordset file: " << words_file);
	
	if(!fos)
		die("Couldn't open output file: " << output_file);
	
	std::set<std::string> wordset_unique;
	while(fis.good())
	{
		std::string word; 
		fis >> word;
		
		word.erase(std::remove_if(word.begin(), word.end(), 
			[](char c) -> bool {
				return (!isalnum(c) || isspace(c) || ispunct(c));}
		), word.end());
		
		std::transform(word.begin(), word.end(), word.begin(), 
			[](char c) -> char { 
				return tolower(c);
			}
		);
		
		if(!word.empty())
			wordset_unique.insert(word);
	}
	
	std::vector<std::string> wordset_rand;
	wordset_rand.reserve(wordset_unique.size());
	for(const std::string& word: wordset_unique)
		wordset_rand.push_back(word);
	
	std::default_random_engine rng;
	rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
	
	for(unsigned size=0; size < output_size; )
	{
		unsigned word_id = rng() % wordset_rand.size();
		fos << wordset_rand[word_id] << " ";
		
		size += wordset_rand[word_id].length() + 1;
	}
	
	return 0;
}