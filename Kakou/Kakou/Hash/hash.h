#pragma once
#include <math.h>

class Hash_function
{
public:
	Hash_function():para_a(0),para_b(0),para_c(0){
	}

	double hash(int x)
	{
		double dtemp = getfrac(0.653 * x * para_a);
		dtemp +=getfrac(0.564101 * (x>>4) * para_b* para_b);
		dtemp += getfrac(0.4131 * (x<<4) * para_c*para_c*para_c);
		return dtemp;
		//return ((para_a * (x >> 4) + para_b * x + para_c) & 131071);
	}
public:
	int para_a;
	int para_b;
	int para_c;
private:
	double getfrac(double a)
	{
		return a -floor(a);
	}
};



