#pragma once
#include "multithreadtask.h"
#include "akmeans.h"



/**
 * Quantize each sift feature in the sift-file 
 * Akmeans<DataType> *A		Akmeans tree for quantization
 * char* sift_path			SIFT-file
 * char* qtz_path			QTZ-file	
 */
template <typename DataType>
void quantization(Akmeans<DataType> *A,char* sift_path,char* qtz_path)
{
	if(_access(qtz_path,0)== 0)
	{
		return;
	}

	int count_n = 0;
	int dim = 0;
	int width = 0;
	int height = 0;

	Feature<DataType>* feats = NULL;
	float* x = NULL;
	float* y = NULL;


	// Read the features from the sift file
	ifstream infile;
	infile.open(sift_path,ios::in|ios::binary); 

	infile.read((char*)&width,sizeof(int));
	infile.read((char*)&height,sizeof(int));

	infile.read((char*)&count_n,sizeof(int));
	infile.read((char*)&dim,sizeof(int));

	feats = new Feature<DataType>[count_n];
	x = new float[count_n];
	y = new float[count_n];


	for (int i = 0; i < count_n; i++)
	{
		feats[i].assign_feature_dimension(dim);		
	}

	for(int i = 0; i < count_n; i++)
	{
		infile.read((char*)&x[i],sizeof(float));
		infile.read((char*)&y[i],sizeof(float));
		
		for(int j = 0; j < dim; j++)
		{
			infile.read((char*)&(feats[i].descriptor[j]),sizeof(unsigned char));
		}
	}

	infile.close();

	// Output the quantization results
	int neighbor_number = 1;
	Feature<float>** neighbors;

	ofstream outfile;
	outfile.open(qtz_path,ios::out|ios::binary);

	outfile.write((char*)&width,sizeof(int));
	outfile.write((char*)&height,sizeof(int));
	outfile.write((char*)&count_n,sizeof(int));
	outfile.write((char*)&neighbor_number,sizeof(int));

	for (int i = 0; i < count_n; i++)
	{
		neighbors = A->kdtree_bbf(&feats[i] ,neighbor_number, 200);
		for(int j = 0; j < neighbor_number; j++)
		{
			if(!neighbors[j])
				continue;

			outfile.write((char*)&(neighbors[j]->label),sizeof(int));		
		}
		
		outfile.write((char*)&x[i],sizeof(float));
		outfile.write((char*)&y[i],sizeof(float));

		if(neighbors)
		{
			delete neighbors;
			neighbors = NULL;
		}
	}

	outfile.close();

	if(feats!= NULL)
	{
		delete[] feats;
		feats = NULL;
	}
	
	if(x!=NULL)
	{
		delete[] x;
		x = NULL;
	}

	if(y!=NULL)
	{
		delete[] y;
		y = NULL;
	}
		
}
