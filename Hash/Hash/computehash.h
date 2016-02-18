#pragma once

#include <iostream>
#include <cstdio>
#include "words.h"
#include "hash.h"
using namespace std;


class ComputeHash
{
public:
	ComputeHash()
	{
		sketchsize = 2;
		sketchnum = 2000;
		hash_sketches = NULL;
		words_count = 0;
	}

	void init_hash_sketches( char* hash_table_path);

	void build_database(int n );

	void load_hash_values();

	void hash_image(char* folder_path, char* file_name,int threadNum);

	int add_image_size(char* folder_path, char* file_name);

private:
	Hash_function** hash_sketches;
	float** hash_values;
	int sketchsize;
	int sketchnum;
	int words_count;
};