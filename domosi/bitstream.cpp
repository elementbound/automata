#include "bitstream.h"

//#define debug(msg) {std::cout << msg;}
#define debug(msg) {}

//=============================================================================================
//ibitstream

bool ibitstream::get() {
	if(m_ReserveCount > 0) {
		bool ret = m_ReserveData & 0x1;
		m_ReserveData >>= 1;
		m_ReserveCount--;
		
		return ret;
	}
	else 
	{
		m_ReserveData = m_InputStream->get();
		m_ReserveCount = 8;
		
		return this->get();
	}
}

void ibitstream::get(char* data, size_t bit_count) {
	for(size_t i=0; i<bit_count; i++) {
		size_t byte_id = i/8;
		size_t bit_id = i%8;
		
		if(bit_id == 0)
			data[byte_id] = 0;
		
		data[byte_id] |= this->get() << bit_id;
	}
}

void ibitstream::istream(std::istream* is) {
	m_InputStream = is;
}

std::istream& ibitstream::istream() {
	return *m_InputStream;
}

const std::istream& ibitstream::istream() const {
	return *m_InputStream;
}

//=============================================================================================
//obitstream

void obitstream::put(bool v) {
	debug("obitstream::put(" << v << ")\n");
	
	m_ReserveData = (m_ReserveData | (v<<7));
	m_ReserveCount++;
	
	if(m_ReserveCount == 8)
		this->flush();
	else 
		m_ReserveData >>= 1;
}

void obitstream::put(const char* data, size_t bit_count) {
	debug("obitstream::put(" << (void*)data << ", " << bit_count << ")\n");
	
	for(size_t i=0; i<bit_count; i++) {
		size_t byte_id = i/8;
		size_t bit_id = i%8;
		
		this->put(data[byte_id] & (1 << bit_id));
	}
}

void obitstream::flush() {
	debug("obitstream::flush()\n");
	if(!m_ReserveCount)
		return;
	
	m_OutputStream->put(m_ReserveData);
	m_ReserveCount = 0;
	m_ReserveData = 0;
}

void obitstream::ostream(std::ostream* os) {
	m_OutputStream = os;
}

std::ostream& obitstream::ostream() {
	return *m_OutputStream;
}

const std::ostream& obitstream::ostream() const {
	return *m_OutputStream;
}