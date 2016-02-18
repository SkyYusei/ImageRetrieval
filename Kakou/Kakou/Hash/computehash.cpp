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
//#include "./../Quantization/kits2.h"
#include "words.h"

HashSketches::HashSketches()
{
	sketchsize = 1;
	sketchnum = HASHES/sketchsize;
	hash_sketches = new Hash_function[HASHES];
}

HashSketches::~HashSketches()
{
	if(hash_sketches!= NULL)
	{
		delete[] hash_sketches;
		hash_sketches = NULL;
	}
}
/**
 * Initial the parameters of the functions
 * @param: char* hash_table_path	The path of the file to store the parameters
 */
void HashSketches::init_hash_sketches( char* hash_table_path)
{
	if(_access(hash_table_path,0)== 0)
	{
		ifstream infile;
		infile.open(hash_table_path,ios::in|ios::binary);
				

		for( int i = 0; i < HASHES; i++)
		{
			infile.read((char*)&(hash_sketches[i].para_a),sizeof(int));
			infile.read((char*)&(hash_sketches[i].para_b),sizeof(int));
			infile.read((char*)&(hash_sketches[i].para_c),sizeof(int));
		}

		infile.close();

	}
	else
	{
		ofstream outfile;
		outfile.open(hash_table_path,ios::out|ios::binary);

		srand((unsigned int)time(NULL));
		for( int i = 0; i < HASHES; i++)
		{
			hash_sketches[i].para_a = rand();
			hash_sketches[i].para_b = rand();
			hash_sketches[i].para_c = rand();

			outfile.write((char*)&hash_sketches[i].para_a,sizeof(int));
			outfile.write((char*)&hash_sketches[i].para_b,sizeof(int));
			outfile.write((char*)&hash_sketches[i].para_c,sizeof(int));
		}
		outfile.close();
	}	
}


/**
 * Hash for each qtz-file
 * @param: char* qtz_path	qtz-file
 * @param: char* hash_path		hash-file
 */
void HashSketches::hash_qtz(char* qtz_path, char* hash_path)
{	
	if(_access(hash_path,0)== 0)
	{
		return;
	}

	int width = 0;
	int height = 0;
	int count_n = 0;
	int neighbor_number = 1;


	ifstream infile;
	infile.open(qtz_path,ios::in|ios::binary);

	infile.read((char*)&width,sizeof(int));
	infile.read((char*)&height,sizeof(int));
	infile.read((char*)&count_n,sizeof(int));
	infile.read((char*)&neighbor_number,sizeof(int));

	Words* words = new Words[count_n];


	for(int i = 0; i< count_n; i++)
	{
		words[i].setNum(neighbor_number);
		for(int j =0; j < neighbor_number;j++)
		{
			infile.read((char*)&(words[i].labels[j]),sizeof(int));
		}
		infile.read((char*)&(words[i].x),sizeof(float));
		infile.read((char*)&(words[i].y),sizeof(float));	
	}

	infile.close();

	ofstream outfile;
	outfile.open(hash_path,ios::out|ios::binary);

	outfile.write((char*)&width,sizeof(int));
	outfile.write((char*)&height,sizeof(int));

	for(int i = 0; i < sketchnum; i++)
	{
		for(int j = 0; j < sketchsize; j++)
		{
			double min_value = DBL_MAX;
			
			vector<Words> wordslist;
			vector<int> labellist;
			for( int k = 0; k < count_n; k++)
			{
				for(int w = 0; w < neighbor_number; w++)
				{
					double mintemp = hash_sketches[i*sketchsize+j].hash(words[k].labels[w]);
					if( mintemp < min_value)
					{
						if(!wordslist.empty())
						{
							wordslist.clear();
							labellist.clear();
						}
						min_value = mintemp;				
					}
					if( mintemp == min_value)
					{
						wordslist.push_back(words[k]);
						labellist.push_back(w);
					}
				}				
			}

			int index = rand()%wordslist.size();
			int labelindex = labellist.at(index);
			outfile.write((char*)&(wordslist.at(index).labels[labelindex]),sizeof(int));
			outfile.write((char*)&(wordslist.at(index).x),sizeof(float));
			outfile.write((char*)&(wordslist.at(index).y),sizeof(float));

			if(!wordslist.empty())
			{
				wordslist.clear();
				labellist.clear();
			}
		}
	}
	
	outfile.close();
	delete[] words;
}


/**
 * Compare two hash-file
 * @param: char* hash_path1		hash-file1
 * @param: char* hash_path2		hash-file2
 */
void HashSketches::cmp_hash(char* hash_path1,char* hash_path2)
{
	score result_score;
	int width1 = 0;
	int height1 = 0;
	int* labels1;
	float* x1;
	float*y1;

	labels1 = new int[HASHES];
	x1 = new float[HASHES];
	y1 = new float[HASHES];

	ifstream infile1;
	infile1.open(hash_path1,ios::in|ios::binary);

	infile1.read((char*)&width1,sizeof(int));
	infile1.read((char*)&height1,sizeof(int));
	for(int i = 0; i < HASHES;i++)
	{
		infile1.read((char*)&labels1[i],sizeof(int));
		infile1.read((char*)&x1[i],sizeof(float));
		infile1.read((char*)&y1[i],sizeof(float));
	}
	infile1.close();

	int width2 = 0;
	int height2 = 0;
	int* labels2;
	float* x2;
	float*y2;

	labels2 = new int[HASHES];
	x2 = new float[HASHES];
	y2 = new float[HASHES];

	ifstream infile2;
	infile2.open(hash_path2,ios::in|ios::binary);

	infile2.read((char*)&width2,sizeof(int));
	infile2.read((char*)&height2,sizeof(int));
	for(int i = 0; i < HASHES;i++)
	{
		infile2.read((char*)&labels2[i],sizeof(int));
		infile2.read((char*)&x2[i],sizeof(float));
		infile2.read((char*)&y2[i],sizeof(float));
	}
	infile2.close();

	int bins[2*STEP-1][2*STEP-1];
	for(int i = 0; i < 2*STEP-1; i++)
	{
		for(int j = 0; j < 2*STEP-1;j++)
		{
			bins[i][j] = 0;
		}
	}

	result_score.count = 0;
	for(int i = 0; i < sketchnum; i++)
	{
		bool flag = true;
		for(int j = 0; j < sketchsize; j++)
		{
			if(labels1[i*sketchsize+j] != labels2[i*sketchsize+j])
			{
				flag = false;
			}
			else
			{
				result_score.count++;
			}
		}

		if(flag)
		{			
			for(int j = 0; j < sketchsize; j++)
			{
				int gridx1 = (int)x1[i*sketchsize+j]*STEP/width1%STEP;
				int gridy1 = (int)y1[i*sketchsize+j]*STEP/height1%STEP;
				int gridx2 = (int)x2[i*sketchsize+j]*STEP/width1%STEP;
				int gridy2 = (int)y2[i*sketchsize+j]*STEP/height1%STEP;
				int deltax = gridx1-gridx2+STEP-1;
				int deltay = gridy1-gridy2+STEP-1;
				bins[deltax][deltay]++;
			}
		}
	}

	result_score.maxbin = 0;
	result_score.gvp = 0;
	for(int i = 0; i < 2*STEP-1;i++)
	{
		for(int j = 0; j < 2*STEP-1;j++)
		{
			if(bins[i][j] > result_score.maxbin)
			{
				result_score.maxbin = bins[i][j];
			}

			if(bins[i][j] >= LENGTH)
			{
				int prod1 = 1;
				int prod2 = 1;
				for(int k = 0; k < LENGTH;k++)
				{
					prod1 *= (bins[i][j]-k);
					prod2 *= (k+1);
				}
				result_score.gvp += prod1/prod2;
			}
		}
	}
	cout << result_score.count << " " << result_score.maxbin << " " << result_score.gvp << endl;

	delete[] x1;
	delete[] y1;
	delete[] labels1;
	delete[] x2;
	delete[] y2;
	delete[] labels2;
}
