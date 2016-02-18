#include <fstream>
#include <io.h>
#include <ctime>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <float.h>
using namespace std;

#include <highgui.h>

#include "computehash.h"
#include "kits.h"
#include "words.h"

void ComputeHash::init_hash_sketches( char* hash_table_path)
{
	if(_access(hash_table_path,0)== 0)
	{
		ifstream infile;
		infile.open(hash_table_path,ios::in);
		infile >> sketchnum >> sketchsize;

		hash_sketches = new Hash_function*[sketchnum];
		for( int i = 0; i < sketchnum; i++)
		{
			hash_sketches[i] = new Hash_function[sketchsize];
		}

		for( int i = 0; i < sketchnum; i++)
		{
			for(int j = 0; j < sketchsize; j++)
			{
				infile >> hash_sketches[i][j].para_a;
				infile >> hash_sketches[i][j].para_b;
				infile >> hash_sketches[i][j].para_c;
			}
		}

		infile.close();

	}
	else
	{
		ofstream outfile;
		outfile.open(hash_table_path,ios::out);

		hash_sketches = new Hash_function*[sketchnum];
		for( int i = 0; i < sketchnum; i++)
		{
			hash_sketches[i] = new Hash_function[sketchsize];
		}

		srand((unsigned int)time(NULL));
		outfile << sketchnum << " " << sketchsize << endl;
		for( int i = 0; i < sketchnum; i++)
		{
			for(int j = 0; j < sketchsize; j++)
			{				
				hash_sketches[i][j].para_a = rand();		
				hash_sketches[i][j].para_b = rand();
				hash_sketches[i][j].para_c = rand();

				outfile << hash_sketches[i][j].para_a << " ";
				outfile << hash_sketches[i][j].para_b << " ";
				outfile << hash_sketches[i][j].para_c << endl;
			}
		}
		outfile.close();
	}	
}

void ComputeHash::build_database(int n )
{
	words_count = n;
	ofstream outfile;
	float* hash_values = new float[words_count ];
	char file_path[NAME_LEN];

	for(int i = 0; i < sketchnum; i++)
	{
		for(int j = 0; j < sketchsize; j++)
		{
			sprintf_s(file_path,"K:/hashvalues/%d_%d.hv",i,j);
			if(_access(file_path,0)== 0)
			{
				return;
			}
			outfile.open(file_path,ios::out|ios::binary);
			
			for(int k = 0; k  < words_count ; k++)
			{
				hash_values[k] = (float)(hash_sketches[i][j].hash(k)%words_count)/words_count;
			}
			outfile.write((char*)hash_values,words_count*sizeof(float));
			outfile.close();
		}
	}
}

void ComputeHash::load_hash_values()
{
	ifstream infile;
	hash_values = new float*[sketchnum*sketchsize/4];
	char file_path[NAME_LEN];
	for(int i = 0; i < sketchnum/4; i++)
	{
		for(int j = 0; j < sketchsize; j++)
		{
			hash_values[i*sketchsize+j] = new float[words_count];
			sprintf_s(file_path,"K:/hashvalues/%d_%d.hv",i+sketchnum*3/4,j);
			printf("%d_%d\t",i,j);
			infile.open(file_path,ios::in|ios::binary);
			infile.read((char*)hash_values[i*sketchsize+j],words_count*sizeof(float));
			infile.close();
		}
	}
}

void ComputeHash::hash_image(char* folder_path, char* file_name,int threadNum)
{
	char qtz_path[NAME_LEN];
	sprintf_s(qtz_path, "%s/%s.txt",folder_path, file_name);

	char hash_path[NAME_LEN];
	strncpy_s(hash_path,folder_path,strstr(folder_path,"quantization")-folder_path);
	strcat_s(hash_path,"hash");
	strcat_s(hash_path,strstr(folder_path,"quantization")+strlen("quantization"));
	strcat_s(hash_path,"/");
	strcat_s(hash_path,file_name);
	strcat_s(hash_path,".has");

	/*if(_access(hash_path,0)== 0)
	{
		return;
	}*/

	int count_n = 0;;
	int dim = 0;

	ifstream infile;
	infile.open(qtz_path,ios::in);

	infile >> count_n >> dim;
	Words* words = new Words[count_n];
	float scl = 0.0;
	float ori = 0.0;

	for(int i = 0; i< count_n; i++)
	{
		infile >> words[i].label >> words[i].x >> words[i].y;
		infile >> scl >> ori;		
	}

	infile.close();

	ofstream outfile;
	outfile.open(hash_path,ios::out|ios::app);
	/*outfile << sketchnum << " " << sketchsize << endl;*/

	char file_path[NAME_LEN];
	Words* words_hash = new Words[sketchnum*sketchsize/4];
	int* listsize = new int[sketchnum*sketchsize/4];
	for(int i = 0; i < sketchnum/4; i++)
	{
		for(int j = 0; j < sketchsize; j++)
		{
			float min_value = FLT_MAX;

			/*sprintf_s(file_path,"K:/hashvalues/%d_%d.hv",i,j);
			infile.open(file_path,ios::in|ios::binary);
			infile.read((char*)hash_values,words_count*sizeof(float));
			infile.close();*/

			for( int k = 0; k < count_n; k++)
			{
				if(hash_values[i*sketchsize+j][words[k].label] < min_value)
				{
					min_value = hash_values[i*sketchsize+j][words[k].label];					
				}
			}
			vector<Words> wordslist;
			for( int k = 0; k < count_n; k++)
			{
				if(hash_values[i*sketchsize+j][words[k].label] == min_value)
				{
					wordslist.push_back(words[k]);					
				}
			}

			listsize[sketchsize*i+j] = wordslist.size();
			words_hash[sketchsize*i+j] = wordslist.at(rand()%listsize[sketchsize*i+j]);			
		}
	}


	for(int i = 0; i < sketchnum/4; i++)
	{
		for(int j = 0; j < sketchsize; j++)
		{
			int k = sketchsize*i+j;
			outfile << words_hash[k].label << " " <<words_hash[k].x << " " << words_hash[k].y << " " << listsize[k]<< endl;		
		}
	}
	
	outfile.close();
	
	delete[] listsize;
	delete[] words_hash;
	delete[] words;

}

int ComputeHash::add_image_size(char* folder_path, char* file_name)
{
	char hash_path[NAME_LEN];
	strncpy_s(hash_path,folder_path,strstr(folder_path,"quantization")-folder_path);
	strcat_s(hash_path,"hash");
	strcat_s(hash_path,strstr(folder_path,"quantization")+strlen("quantization"));
	strcat_s(hash_path,"/");
	strcat_s(hash_path,file_name);
	strcat_s(hash_path,".has");

	char image_path[NAME_LEN];
	strncpy_s(image_path,folder_path,strstr(folder_path,"quantization")-folder_path);
	strcat_s(image_path,"image");
	strcat_s(image_path,strstr(folder_path,"quantization")+strlen("quantization"));
	strcat_s(image_path,"/");
	strcat_s(image_path,file_name);

	char temp[NAME_LEN];
	strcpy_s(temp,image_path);
	strcat_s(temp,".jpg");
	if(_access(temp,0)!=0)
	{
		strcpy_s(temp,image_path);
		strcat_s(temp,".bmp");
		if(_access(temp,0)!=0)
		{
			strcpy_s(temp,image_path);
			strcat_s(temp,".jpeg");
			if(_access(temp,0)!=0)
			{
				return -1;	
			}
		}
	}

	IplImage *image = cvLoadImage(temp, 1);	


	ofstream outfile;
	outfile.open(hash_path,ios::out|ios::app);
	outfile << image->width << " " << image->height << endl;
	cvReleaseImage(&image);
	outfile.close();

	return 0;
}