#ifndef _H_BITSTREAM_H_
#define _H_BITSTREAM_H_

#include <iostream>
#include <cstdint> //uint8_t

class ibitstream
{
	private: 
		std::istream* m_InputStream;
		//Reserved bits storage and number of bits
		uint8_t m_ReserveData = 0;
		uint8_t m_ReserveCount = 0;
		
	public: 
		bool get();
		void get(char* data, size_t bit_count);
		
		template <typename T>
		void get(T& data, size_t bit_count = sizeof(T)) {this->get((char*)&data, bit_count);}
		
		void 				istream(std::istream*);
		std::istream& 		istream();
		const std::istream& istream() const;
};

class obitstream
{
	private: 
		std::ostream* m_OutputStream;
		//Reserved bits storage and number of bits
		uint8_t m_ReserveData = 0;
		uint8_t m_ReserveCount = 0;
		
	public: 
		void put(bool);
		void put(const char* data, size_t bit_count);
		
		template <typename T>
		void put(const T& data, size_t bit_count = sizeof(T)) {this->put((char*)&data, bit_count);}
		
		void flush();
		
		void 				ostream(std::ostream*);
		std::ostream& 		ostream();
		const std::ostream& ostream() const;
};

#endif