#include <vector_types.h>

extern "C"
void computeGold(char *reference, char *idata, const unsigned int len);
extern "C"
void computeGold2(int2 *reference, int2 *idata, const unsigned int len);

void computeGold(char *reference, char *idata, const unsigned int len)
{
	for (unsigned int i = 0; i < len; ++i)
		reference[i] = idata[i] - 10;
}

void computeGold2(int2 *reference, int2 *idata, const unsigned int len)
{
	for (unsigned int i = 0; i < len; ++i)
	{
		reference[i].x = idata[i].x - idata[i].y;
		reference[i].y = idata[i].y;
	}
}