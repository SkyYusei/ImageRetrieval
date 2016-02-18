#include "similarity.h"
#include "kits.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
using namespace std;

ImageHash::ImageHash()
{
	words = NULL;
	listsize = NULL;
	sketchnum = 0;
	sketchsize = 0;
}

void ImageHash::load_image_hash_values(char* folder_path,char *file_name)
{
	char hash_path[NAME_LEN];
	strncpy_s(hash_path,folder_path,strstr(folder_path,"image")-folder_path);
	strcat_s(hash_path,"hash");
	strcat_s(hash_path,strstr(folder_path,"image")+strlen("image"));
	strcat_s(hash_path,"/");
	char hash_file_name[NAME_LEN];
	strcpy_s(hash_file_name,file_name);
	hash_file_name[strrchr(hash_file_name,'.')-hash_file_name] = '\0';
	strcat_s(hash_path,hash_file_name);
	strcat_s(hash_path,".has");

	//sprintf_s(hash_path, "%s/%s",folder_path, file_name);
	strcpy_s(imagepath,folder_path);
	strcpy_s(imagename,file_name);

	ifstream infile;
	infile.open(hash_path,ios::in);

	infile >> sketchnum >> sketchsize;

	words = new Words*[sketchnum];
	listsize = new int*[sketchnum];

	for(int i = 0; i < sketchnum;i++)
	{
		words[i] = new Words[sketchsize];
		listsize[i] = new int[sketchsize];
	}

	for(int i = 0; i < sketchnum;i++)
	{
		for(int j = 0; j < sketchsize; j++)
		{
			infile >> words[i][j].label >> words[i][j].x >> words[i][j].y >> listsize[i][j];
		}
	}

	infile >> width >> height;

	infile.close();	
}


int ImageHash::compare(ImageHash &a)
{
	if(a.sketchnum != sketchnum || a.sketchsize != sketchsize)
	{
		return -1;
	}

	int step = 5;
	int grid = 2*step+1;
	int ** bins;
	bins = new int*[grid];
	for(int i = 0; i < grid; i++)
	{
		bins[i] = new int[grid];
		for(int j = 0; j < grid; j++)
		{
			bins[i][j] = 0;
		}
	}


	for(int i = 0; i < sketchnum;i++)
	{
		bool flag = true;
		for(int j = 0; j < sketchsize; j++)
		{
			if(words[i][j].label != a.words[i][j].label)
			{
				flag = false;
			}
		}

		if(flag)
		{
			for(int j = 0; j < sketchsize; j++)
			{
				int x1,x2,y1,y2;
				x1 = (int)(words[i][j].x) *step/width%step;
				y1 = (int)(words[i][j].y) *step/height%step;
				x2 = (int)(a.words[i][j].x) *step/a.width%step;
				y2 = (int)(a.words[i][j].y) *step/a.height%step;
				bins[x1-x2+step][y1-y2+step]+= listsize[i][j]* a.listsize[i][j];
			}
		}
	}

	int length = 2;
	int sim = 0;
	
	for(int i = 0; i < grid; i++)
	{	
		for(int j = 0; j < grid; j++)
		{
			if(bins[i][j] < length)
				continue;
			int product1 = 1;
			int product2 = 1;
			for(int k = 1; k <= length; k++)
			{		
				product1 *= bins[i][j] -k +1;
				product2 *= k;
			}
			sim += product1/product2;
	
		}
	}

	for(int i = 0; i < grid; i++)
	{
		if(bins[i])
		{
			delete[] bins[i];
			bins[i] = NULL;
		}
	}

	if(bins)
	{
		delete[] bins;
		bins = NULL;
	}			

	return sim;
}