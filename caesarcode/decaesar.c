#include <stdio.h>
#define BUFFER_SIZE 8192

char interval_subtract(char a, char x, char range_min, char range_max)
{
	a -= range_min; 
	if(a < x)
	{
		x -= a+1;
		a = range_max-range_min;
	}
	
	return (a-x) + range_min;
}

int main()
{
	char signal_table[256];
	for(unsigned i=0; i<256; i++)
	{
		if(i>='a' && i<='z')
			signal_table[i] = interval_subtract(i, 3, 'a', 'z');
		else if(i>='A' && i<='Z')
			signal_table[i] = interval_subtract(i, 3, 'A', 'Z');
		else
			signal_table[i] = (char)i;
	}
	
	char buffer[BUFFER_SIZE];
	while(fgets(buffer, BUFFER_SIZE, stdin) != NULL)
	{
		for(char* p = buffer; *p != 0; p++)
			putchar(signal_table[*p]);
	}
	
	return 0;
}