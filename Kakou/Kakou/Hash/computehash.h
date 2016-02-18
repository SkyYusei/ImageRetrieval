#pragma once

#include <iostream>
#include <cstdio>
#include "words.h"
#include "hash.h"
using namespace std;

#define HASHES 4000
#define STEP 5
#define LENGTH 2

struct score
{
	score()
	{
		count = 0;
		maxbin = 0;
		gvp = 0;
	}
	int count;
	int maxbin;
	int gvp;
};

class HashSketches
{
public:
	HashSketches();
	~HashSketches();
	void init_hash_sketches( char* hash_table_path);
	void hash_qtz(char* qtz_path, char* hash_path);
	void cmp_hash(char* hash_path1,char* hash_path2);
private:
	int sketchsize;
	int sketchnum;
	Hash_function* hash_sketches;
	
};
