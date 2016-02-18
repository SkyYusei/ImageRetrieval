#pragma once

#include <iostream>
#include <math.h>
#include <io.h>
#include <vector>
#include <string.h>
#include <iostream>
#include "feature.h"

#define NAME_LEN 200

using namespace std;

typedef struct _mString {
	char str[NAME_LEN];
	friend bool operator < (const _mString s1, const _mString s2){ return strncmp(s1.str, s2.str, NAME_LEN) < 0;}
	void operator = (const _mString s) { sprintf_s(str, "%s",s.str); }
	void operator = (const char s[]) { sprintf_s(str, "%s",s);}
	void show() { fprintf(stderr, "%s\n", str); }	
}mString;


/* Find a random value around the median value*/
float median_random_select(float* data_array,int start,int end,int median,float value);

/* Calculate the Euclidian distance between two features */
template <typename DataType>
DataType dist_features(Feature<DataType>* a, Feature<DataType>* b)
{
	DataType dist = 0;
	DataType delta;
	for(int i = 0; i < a->dimension; i++)
	{
		delta = a->descriptor[i]-b->descriptor[i];
		dist += delta*delta;
	}
	return dist;
}

/* Get content(files or folders) in one directory
 * Input:	path	The directory to be searched
 *			files	The names of obtained contents
 *			attrib	The type of the file(files or folders) to be searched
 */
int get_content_in_dir(char *path, vector<mString> &files,unsigned int attrib);

