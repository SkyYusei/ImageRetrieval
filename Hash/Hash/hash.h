#pragma once

class Hash_function
{
public:
	Hash_function():para_a(0),para_b(0),para_c(0){
	}

	int hash(int x)
	{
		return ((para_a * (x >> 4) + para_b * x + para_c) & 131071);
	}
public:
	int para_a;
	int para_b;
	int para_c;
};



