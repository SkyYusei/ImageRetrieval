#pragma once

/* Update_centers using multi-thread */
template <typename DataType>
DWORD WINAPI multi_thread_update_centers( LPVOID param );

template <typename DataType>
class Akmeans;

template <typename DataType>
class UpdateCentersParams
{
public:
	UpdateCentersParams()
	{
		_akm = NULL;
		start = 0;
		end = 0;
		_hMutex = NULL;
	}
	Akmeans<DataType> *_akm;
	int start;
	int end;
	HANDLE* _hMutex;
};


template <typename DataType>
DWORD WINAPI multi_thread_update_centers( LPVOID param )
{
	UpdateCentersParams<DataType>* _params = (UpdateCentersParams<DataType>*)param;

	Akmeans<DataType>* akm = _params->_akm;
	HANDLE* hMutex = _params->_hMutex;

	Kdtreenode<float>* root = akm->root;
	Feature<DataType> *features = akm->features;

	int neighbor_number = 1;
	Feature<float>** neighbors;
	int label_flag;

	for(int j = _params->start; j <= _params->end; j++)
	{
		neighbors = akm->kdtree_bbf(root,&features[j] ,neighbor_number, 2 );
		

		if(j%10000 == 0)
			printf("Feature:%d\t",j);
		WaitForSingleObject(hMutex,INFINITE);
		for(int i = 0; i < neighbor_number; i++)
		{
			if(!neighbors[i])
				continue;

			label_flag = neighbors[i]->label;
			for(int k = 0; k < akm->feature_dimension; k++)
			{
				akm->sum[label_flag].descriptor[k] += features[j].descriptor[k];
			}
			akm->sum[label_flag].label++;
		}
		if(neighbors)
		{
			delete neighbors;
			neighbors = NULL;
		}
		ReleaseMutex(hMutex);
	}

	return 0;
}