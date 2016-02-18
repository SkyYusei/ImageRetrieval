#pragma once

#include <iostream>
#include <math.h>
#include <io.h>
#include <vector>
#include <string.h>

#define NAME_LEN 200

using namespace std;

typedef struct _mString {
	char str[NAME_LEN];
	friend bool operator < (const _mString s1, const _mString s2){ return strncmp(s1.str, s2.str, NAME_LEN) < 0;}
	void operator = (const _mString s) { sprintf_s(str, "%s",s.str); }
	void operator = (const char s[]) { sprintf_s(str, "%s",s);}
	void show() { fprintf(stderr, "%s\n", str); }	
}mString;


///* Find a random value around the median value*/
//float median_random_select(float* arr,int n);
//
///* Calculate the Euclidian distance between two features */
//template <typename DataType>
//DataType dist_features(Feature<DataType>* a, Feature<DataType>* b);


/* Get content(files or folders) in one directory
 * Input:	path	The directory to be searched
 *			files	The names of obtained contents
 *			attrib	The type of the file(files or folders) to be searched
 */
int get_content_in_dir(char *path, vector<mString> &files,unsigned int attrib);


//
///* Find a random value around the median value*/
//float median_random_select(float* data_array,int start,int end,int median,float value)
//{
//	int i = 0;
//	if((end - start)<= 1 || (data_array[start] -value) < 0.0001 )
//	{
//		i = start;
//		if((end - start) != 0)
//			i += rand()%(end - start);
//
//		return data_array[i];
//	}
//	
//	i = start;
//	int j = end;
//	
//	float swaptemp;
//	while(i < j)
//	{
//		while(data_array[i]<value && i < j) i++;
//		while(data_array[j]>=value && i < j) j--;
//		
//		swaptemp = data_array[i];
//		data_array[i] = data_array[j];
//		data_array[j] = swaptemp;
//	}
//
//
//	if(median >= j)
//	{
//		for(int k = i; k <= end; k++)
//		{
//			value += data_array[k];
//		}
//		value /=(end - i + 1);
//
//		return median_random_select(data_array,i,end,median,value);
//	}
//	else
//	{
//		for(int k = start; k <= j; k++)
//		{
//			value += data_array[k];
//		}
//		value /=(j - start + 1);
//
//		return median_random_select(data_array,start,i-1,median,value);
//	}
//}
//
///* Calculate the Euclidian distance between two features */
//template <typename DataType>
//DataType dist_features(Feature<DataType>* a, Feature<DataType>* b)
//{
//	DataType dist = 0;
//	DataType delta;
//	for(int i = 0; i < a->dimension; i++)
//	{
//		delta = a->descriptor[i]-b->descriptor[i];
//		dist += delta*delta;
//	}
//	return dist;
//}
//

