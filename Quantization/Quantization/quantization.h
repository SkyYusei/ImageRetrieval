#pragma once
#include "multithreadtask.h"
#include "akmeans.h"

template <typename DataType>
void quantization(Akmeans<DataType> *A,char* folder_path,char* file_name, int threadNum)
{
	char sift_path[NAME_LEN];
	sprintf_s(sift_path, "%s/%s.sift",folder_path, file_name);


	char qtz_path[NAME_LEN];
	strncpy_s(qtz_path,folder_path,strstr(folder_path,"sift")-folder_path);
	strcat_s(qtz_path,"quantization");
	strcat_s(qtz_path,strstr(folder_path,"sift")+strlen("sift"));
	strcat_s(qtz_path,"/");
	strcat_s(qtz_path,file_name);
	strcat_s(qtz_path,".txt");

	if(_access(qtz_path,0)== 0)
	{
		return;
	}

	int count_n = 0;
	int dim = 0;
	Feature<DataType>* feats = NULL;
	float* x = NULL;
	float* y = NULL;
	float* scl = NULL;
	float* ori = NULL;

	// Read the features from the sift file
	ifstream infile;
	infile.open(sift_path,ios::in); 

	infile>> count_n >> dim;

	feats = new Feature<DataType>[count_n];
	x = new float[count_n];
	y = new float[count_n];
	scl = new float[count_n];
	ori = new float[count_n];

	for (int i = 0; i < count_n; i++)
	{
		feats[i].assign_feature_dimension(dim);		
	}

	for(int i = 0; i < count_n; i++)
	{
		int m = 0;
		infile >> y[i] >> x[i]>> scl[i] >> ori[i];

		for(int j = 0; j < dim; j++)
		{
			infile >> m;
			feats[i].descriptor[j] = (unsigned char)m;
		}
	}

	infile.close();

	// Output the quantization results
	int neighbor_number = 1;
	Feature<float>** neighbors;

	ofstream outfile;
	outfile.open(qtz_path,ios::out);

	outfile << count_n << " " << dim << endl;

	for (int i = 0; i < count_n; i++)
	{
		neighbors = A->kdtree_bbf(&feats[i] ,neighbor_number, 20 );
		for(int j = 0; j < neighbor_number; j++)
		{
			if(!neighbors[j])
				continue;

			outfile << neighbors[j]->label << " ";			
		}

		outfile << x[i] << " " << y[i] << " " << scl[i] << " " << ori[i] << endl;

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

	if(scl!=NULL)
	{
		delete[] scl;
		scl = NULL;
	}

	if(ori!=NULL)
	{
		delete[] ori;
		ori = NULL;
	}	
}