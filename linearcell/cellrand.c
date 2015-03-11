#include <stdio.h>

typedef unsigned char byte_t;

int get_bit(char* data, size_t size, int pos)
{
	if(pos < 0)
		return 0;
	if(pos >= size*8)
		return 0;
	
	int byte_id = pos/8;
	int bit_id = pos%8;
	
	return (data[byte_id] >> bit_id) & 1;
}

void cellrand(void* data, size_t size, byte_t automata)
{
	char* buffer = (char*)data;
	byte_t result_byte;
	for(size_t i=0; i<size; i++)
	{	
		result_byte = 0;
		
		for(size_t j=0; j<8; j++)
		{
			byte_t current_state = get_bit(buffer, size, i*8+j-1) | (get_bit(buffer, size, i*8+j) << 1) | (get_bit(buffer, size, i*i+j+1) << 2);
			result_byte |= ((automata >> current_state) & 1) << j;
		}
		
		buffer[i] = result_byte;
	}
}

int main()
{
	unsigned x[4] = {324, 846, 453, 489};
	for(unsigned n = 128; n; n--)
	{
		printf("%u %u %u %u\n", x[0], x[1], x[2], x[3]);
		cellrand(&x, 4*sizeof(unsigned), 30);
	}
	
	return 0;
}