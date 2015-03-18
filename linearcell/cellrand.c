#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned char byte_t;

const char* to_binary(byte_t val)
{
	static char str[8];
	for(unsigned j=0; j<8; j++)
		str[j] = (val & (1<<j)) ? '1' : '0';
	
	return str;
}

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

void set_bit(char* data, size_t size, int pos, int value)
{
	if(pos < 0)
		return; 
	if(pos >= size*8)
		return;
	
	int byte_id = pos/8;
	int bit_id = pos%8;
	
	data[byte_id] &= ~(1<<bit_id);
	data[byte_id] |= (value?1:0) << bit_id;
}

void cellrand(void* data, size_t size, byte_t automata)
{
	char* buffer = (char*)data;
	char* swap = malloc(size);
	byte_t result_byte;
	
	memset(data, 0, size);
	memset(swap, 0, size);
	set_bit(data, size, size*4, 1);
	
	unsigned bounds_min = size*4 - 1;
	unsigned bounds_max = size*4 + 1;
	
	while(1)
	{
		for(size_t i=0; i<size; i++)
		{
			byte_t result_byte = buffer[i];
			
			for(size_t j=0; j<8; j++)
			{
				if(i*8 + j < bounds_min)
					continue;
				
				if(i*8 +j > bounds_max)
					continue;
				
				byte_t current_state = get_bit(buffer, size, i*8+j-1) | (get_bit(buffer, size, i*8+j) << 1) | (get_bit(buffer, size, i*i+j+1) << 2);
				result_byte |= ((automata >> current_state) & 1) << j;
			}
			
			swap[i] = result_byte;
			printf("%s ", to_binary(buffer[i]));
		}
		
		char* t = swap;
		swap = buffer;
		buffer = swap;
	
		printf("\n");
		
		if(bounds_min == 0)
			break;
		
		bounds_min--; bounds_max++;
	}
}

int main()
{
	unsigned x[4] = {};
	for(unsigned n = 128; n; n--)
	{
		cellrand(&x, 1*sizeof(unsigned), 37);
		printf("%u %u %u %u\n", x[0], x[1], x[2], x[3]);
		
		break;
	}
	
	return 0;
}