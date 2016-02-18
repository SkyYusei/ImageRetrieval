
#include <iostream>
#include <math.h>
#include <io.h>
#include <vector>
#include <string.h>

#include "feature.h"
#include "kits.h"


/* Find a random value around the median value*/
float median_random_select(float* data_array,int start,int end,int median,float value)
{
	int i = 0;
	if((end - start)<= 1 || (data_array[start] -value) < 0.0001 )
	{
		i = start;
		if((end - start) != 0)
			i += rand()%(end - start);

		return data_array[i];
	}
	
	i = start;
	int j = end;
	
	float swaptemp;
	while(i < j)
	{
		while(data_array[i]<value && i < j) i++;
		while(data_array[j]>=value && i < j) j--;
		
		swaptemp = data_array[i];
		data_array[i] = data_array[j];
		data_array[j] = swaptemp;
	}


	if(median >= j)
	{
		for(int k = i; k <= end; k++)
		{
			value += data_array[k];
		}
		value /=(end - i + 1);

		return median_random_select(data_array,i,end,median,value);
	}
	else
	{
		for(int k = start; k <= j; k++)
		{
			value += data_array[k];
		}
		value /=(j - start + 1);

		return median_random_select(data_array,start,i-1,median,value);
	}
}

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
int get_content_in_dir(char *path, vector<mString> &files,unsigned int attrib)
{
	struct _finddata_t fa;
	__int64 fHandle;
	bool flag = false;

	if(attrib) flag = true;
	
	char npath[NAME_LEN];
	sprintf_s(npath, "%s/*", path);

	if( (fHandle=_findfirst(npath,&fa))==-1L )
	{
		printf( "Directory %s Open failed!\n",path);
		return 0;
	}
	else
	do
	{
		if( strcmp(fa.name,".")!=0 && strcmp(fa.name,"..")!=0)
		{
			// folders
			if(flag)
			{
				char subfolder[NAME_LEN];
				mString str;

				sprintf_s(subfolder, "%s/%s", path, fa.name);
				if (fa.attrib & _A_SUBDIR)
				{
					str = subfolder;
					files.push_back(str);
					get_content_in_dir(str.str,files,_A_SUBDIR);
				}
			}
			// files
			else
			{
				if (fa.attrib & _A_SUBDIR) continue;
				mString str;
				str = fa.name;
				files.push_back(str);
			}	
		}
		
	}while( _findnext(fHandle,&fa)==0 );
	_findclose( fHandle );
	return 0;
}
