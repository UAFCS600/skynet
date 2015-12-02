#include "dblookup.c"
#include <iostream>

#define PRINT

int main(){
	char buffer[256];
	db_init(256, buffer);	// 256 mb as suggested in dblookup.c, not related to 256 byte buffer
	
	char test[256];
	db_infostring(test);
	
	std::cout << test << "\n";
	
	POSITION p;
	p.bm = 0x00000802;
	p.bk = 0x00000000;
	p.wm = 0x00400400;
	p.wk = 0x00000000;
	p.color = 2;
	
	std::cout << dblookup(&p, 0) << "\n";
	
	return 0;
}