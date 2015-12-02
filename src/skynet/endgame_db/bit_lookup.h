/*
 * virtual_addr.h
 * Author: Aven Bross
 * Date: 11/14/2015
 * Windows/OSX/Linux compatible virtual memory management
*/

//! Byte swap unsigned int
unsigned long swap_uint32(unsigned long val )
{
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF ); 
    return (val << 16) | (val >> 16);
}

bool _BitScanReverse(int * Index, unsigned long Mask)
{
	if(Mask == 0) return false;
	
	static const unsigned long bval[] =
    	{0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4};

    unsigned int r = 0;
    
    if (Mask & 0xFFFF0000) { r += 16/1; Mask >>= 16/1; }
    if (Mask & 0x0000FF00) { r += 16/2; Mask >>= 16/2; }
    if (Mask & 0x000000F0) { r += 16/4; Mask >>= 16/4; }
    
    if(Mask > 15) return false;
    
    *Index = r + bval[Mask];
    
    return true;
}

bool _BitScanForward(int * Index, unsigned long Mask)
{
	Mask = swap_uint32(Mask);
	return _BitScanReverse(Index, Mask);
}


