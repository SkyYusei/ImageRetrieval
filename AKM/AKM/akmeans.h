#pragma once

#include <iostream>
#include <fstream>
#include <cstdio>
#include <afx.h>
#include <Windows.h>
#include <cstdlib>
#include <cmath>
using namespace std;

#include "queue"

#include "kdtreenode.h"
#include "feature.h"
#include "kits.h"
#include "quenode.h"
#include "multithredtask.h"


/* Find a random value around the median value*/
extern float median_random_select(float* arr,int n);

/* Calculate the Euclidian distance between two features */
template <typename DataType>
extern DataType dist_features(Feature<DataType>* a, Feature<DataType>* b);

/* Keep a queque within a certain size */
template<class NodeType>
extern void que_push_with_MaxSize(priority_queue<NodeType> &que,NodeType* newNode, int maxsize);


/* Update_centers using multi-thread */
template <typename DataType>
extern DWORD WINAPI multi_thread_update_centers( LPVOID param );


template <typename DataType>
class Akmeans{
public:	
	Akmeans();
	~Akmeans();
	int assign_center_size(int k);				/* Assign the number of the centers, for example k = 100 */		
	int assign_feature_dimension(int dim);			/* Assign the dimension of the data */		
	int read_features(const char* file_feature_source);		/* Read the features that are used for k-means */		
	void random_set_centers();					/* Set the initial centers with a random way */
	void initial_sum();							/* Initial the sum of the data */
	void initial_root();						/* Initial the root of the kdtree */
	void kdtreenode_init(Kdtreenode<float>* &kdroot,Feature<float> *features, int n);		/* Initialize the root node of the kdtree */		
	int build_kdtree();							/* Build a kdtree with the centers */		
	int kmeans_with_kdtree(int iterate_number);					/* Approximate K-means */
	Feature<float>** kdtree_bbf( Kdtreenode<float>* &kdroot, 
		Feature<DataType> *feature,int neighbor_number, int max_nn );	/* Find the approximate K neighbors */
	void count_features();						/* Count the features around the center */
	void update_centers();						/* Update the centers */
	void release_data();						/* Release the allocated data */

	int save_centers(const char* file_kdtree );	/* Save kdtree after AKM */	
	int save_kdtree(const char* file_kdtree );	/* Save kdtree after AKM */
	int load_kdtree(const char* file_kdtree);	/* Load kdtree and centers */
	int load_centers(const char* file_centers);	/* Load kdtree and centers */	
	
private:
	void expand_kd_node_subtree( Kdtreenode<float>* &kdroot );	/* Expand the kd tree */
	Kdtreenode<float>* expand_kd_node_subtree2( Kdtreenode<float>* &kdroot );	/* Expand the kd tree */
	void assign_part_key( Kdtreenode<float>* kdroot );			/* Partition the node */
	void partition_features(Kdtreenode<float>* kdroot);		/* Partition the features into the subtree */	
	void kdtree_free(Kdtreenode<float>* &kdroot);				/* Free the allocated space*/
	void save_kdtree_part(ofstream &outfile,Kdtreenode<float>* kdroot);	/* Save kdtree for recursion */
	void load_kdtree_part(ifstream &infile,Kdtreenode<float>* kdroot);	/* Load kdtree for recursion */
public:		
	Feature<DataType>* features;		/* Source data */
	int feature_dimension;
	long int feature_number;
		
	Feature<float>* centers;			/* Centers */
	int center_size;

	Feature<float>* sum;				/* Statistic */
	
	Kdtreenode<float>* root;		/* KD tree */
	int count;
//	queue<Kdtreenode<float>*> que_task; /* Queue for building kdtree */
};


template <typename DataType>
Akmeans<DataType>::Akmeans()
{	
	features = NULL;
	feature_dimension = 0;
	feature_number = 0;
	centers = NULL;
	center_size = 0;
	sum = NULL;
	root = NULL;
	count = 0;
}

template <typename DataType>
Akmeans<DataType>::~Akmeans()
{
	release_data();
}



/* Assign the number of the centers, for example k = 100 */
template <typename DataType>
int Akmeans<DataType>::assign_center_size(int k)
{
	center_size = k;
	if(!centers)
		centers = new Feature<float>[center_size];

	for (int i = 0; i < center_size; i++)
	{
		centers[i].assign_feature_dimension(feature_dimension);		
	}

	return center_size;
}

/* Assign the dimension of the data */
template <typename DataType>
int Akmeans<DataType>::assign_feature_dimension(int dim)
{
	feature_dimension = dim;
	return feature_dimension;
}

/* Read the features that are used for k-means */
template <typename DataType>
int Akmeans<DataType>::read_features(const char* file_feature_source)
{	
	ifstream infile;
	infile.open(file_feature_source,ios::in|ios::binary); 
	
	infile.seekg(0,ios::end);
	long int length = infile.tellg(); 
	infile.seekg(0,ios::beg);

	feature_number = length / (feature_dimension * sizeof(DataType));
	//feature_number = 10000;

	printf("Total features %d \n",feature_number );
	features = new Feature<DataType>[feature_number];

	for (int i = 0; i < feature_number; i++)
	{
		features[i].assign_feature_dimension(feature_dimension);
		infile.read((char*)features[i].descriptor,feature_dimension*sizeof(DataType));
	}

	infile.close();

	return 0;
}

/* Set the initial centers with a random way */
template <typename DataType>
void Akmeans<DataType>::random_set_centers()
{
	int center_step = feature_number/center_size;

	int index =0;
	srand(time(NULL));

	for (int i = 0; i < center_size; i++)
	{
		centers[i].assign_feature_dimension(this->feature_dimension);
		index = i * center_step + rand()%center_step;
		for(int j = 0; j < feature_dimension; j++)
		{			
			centers[i].descriptor[j]= (float)features[index].descriptor[j] ;
		}
		centers[i].label = i;
	}

}

/* Initial the sum of the data */
template <typename DataType>
void Akmeans<DataType>::initial_sum()
{
	if(!sum)
		sum = new Feature<float>[center_size];

	for (int i = 0; i < center_size; i++)
	{
		sum[i].assign_feature_dimension(feature_dimension);
		sum[i].label = 0;
	}
}


/* Initial the sum of the data */
template <typename DataType>
void Akmeans<DataType>::initial_root()
{
	if(root == NULL)
		root = new Kdtreenode<float>;
	kdtreenode_init(root,centers,center_size);
}

/* Initialize the root node of the kdtree */
template <typename DataType>
void Akmeans<DataType>::kdtreenode_init(Kdtreenode<float>* &kdroot,Feature<float>* features, int n)
{
	if(!kdroot)
		return;

	kdroot->features = features;
	kdroot->n = n;
}


/* Build a kdtree with the centers */
template <typename DataType>
int Akmeans<DataType>::build_kdtree()
{
	/*Kdtreenode<float>** subroot = new Kdtreenode<float>*[8];*/
	
	expand_kd_node_subtree2(root);
	printf("\nStart left part\n");
	expand_kd_node_subtree(root->kd_left);
	printf("\nStart right part\n");
	expand_kd_node_subtree(root->kd_right);

	/*temproot = kdroot->kd_left;
	temproot = expand_kd_node_subtree2(temproot);
	subroot[0] = temproot->kd_left;
	subroot[1] = temproot->kd_right;

	temproot = kdroot->kd_right;
	temproot = expand_kd_node_subtree2(temproot);
	subroot[2] = temproot->kd_left;
	subroot[3] = temproot->kd_right;

	for(int i = 0; i < 4; i++)
	{
		printf("Start %d quarter\n",i+1);
		expand_kd_node_subtree(subroot[i]);
		printf("Complete %d quarter\n",i+1);
	}*/

	printf("Total Times: %d\n",count);
	count = 0;
	return 0;
}

/* Approximate K-means */
template <typename DataType>
int Akmeans<DataType>::kmeans_with_kdtree(int iterate_number)
{	
	for( int i = 0; i < iterate_number; i++)
	{		
		if(root == NULL)
			root = new Kdtreenode<float>;
		printf("\nThe %dth teration for build kd-tree.\n",i);

		initial_sum();
		initial_root();
		build_kdtree();
		printf("Build Completed\n");

		printf("Save kdtree\n");
		save_kdtree("D:/Users/Surge/Desktop/kdtree.whole1m");
		count_features();
		update_centers();
		printf("Save centers\n");
		save_centers("D:/Users/Surge/Desktop/centers.whole1m");
		kdtree_free(root);
	}

	return 0;
}



/* Finds approximate k nearest neighbors in a kd tree usingBest Bin First search */
template <typename DataType>
Feature<float>** Akmeans<DataType>::kdtree_bbf( Kdtreenode<float>* &kdroot, Feature<DataType> *feature,int neighbor_number, int max_nn )
{

	if( !kdroot || !feature )
	{
		fprintf(stderr,"Warning: NULL pointer error!\n");
		return NULL;
	}

	/* Queue for recording the searched node in the kdtree */
	Queuenode* explore_node  = new Queuenode;
	Queuenode* unexplore_node = new Queuenode;
	priority_queue<Queuenode> que;

	explore_node->node = kdroot;
	que.push(*explore_node);

	
	Feature<float>* feat = new Feature<float>;
	feat->assign_feature_dimension(feature_dimension);

	for(int i = 0; i <feature_dimension; i++)
	{
		feat->descriptor[i] = (float)feature->descriptor[i];
	}
	
	/* Record the K nearest neighbors */
	Feature<float>** nbrs = new Feature<float>*[neighbor_number];
	for(int i = 0; i < neighbor_number; i++)
		nbrs[i] = NULL;	

	/* Queue for recording the the K nearest neighbors */
	Queuenode2* neighbornode  = new Queuenode2;
	Queuenode2* neighbortop  = new Queuenode2;
	priority_queue<Queuenode2> que_neighbors;

	int t = 0;
	int que_times = 0;
	while(!que.empty() &&  t < max_nn)
	{
		*explore_node = que.top();
		que.pop();

		if(!explore_node)
		{
			printf("Priority_Queue unexceptedly empty\n");
		}

		/* explore to leave*/
		while(!explore_node->node->leaf)
		{
			float kv = explore_node->node->kv;
			int ki = explore_node->node->ki;

			if(feat->descriptor[ki] <= kv)
			{
				unexplore_node->node = explore_node->node->kd_right;
				explore_node->node = explore_node->node->kd_left;
			}
			else
			{
				unexplore_node->node = explore_node->node->kd_left;
				explore_node->node = explore_node->node->kd_right;
			}			

			if(unexplore_node->node!=NULL)
			{
				float delta = unexplore_node->node->features->descriptor[ki] - feat->descriptor[ki];
				delta = (delta>0)?delta:-delta;
				unexplore_node->dist_ki = delta;
				que.push(*unexplore_node);
			}
			que_times++;
		}

		/* Compute the distance between the feat and the leaf node center*/
		if(explore_node->node!=NULL)
		{
			neighbornode->dist = dist_features(feat,explore_node->node->features);
			neighbornode->_nbr = explore_node->node->features;
			que_push_with_MaxSize(que_neighbors,neighbornode,neighbor_number);
		}

		t++;	
	}
	/*printf("que_times: %d\n",que_times);*/
	
	/* Clear the que */
	while(!que.empty())
	{
		que.pop();
	}

	t = 0;
	while(!que_neighbors.empty())
	{
		*neighbortop = que_neighbors.top();
		nbrs[t] = neighbortop->_nbr;
		que_neighbors.pop();
		t++;
	}

	delete neighbortop;
	delete neighbornode;

	delete explore_node;
	delete unexplore_node;
	delete feat;
	return nbrs;
}

template <typename DataType>
void Akmeans<DataType>::count_features()
{
	/* Compute the approximate K nearest neighbors */
	int threadNum = 10;
	int step = feature_number/threadNum;
	UpdateCentersParams<DataType> **threadParams = new UpdateCentersParams<DataType>*[threadNum];
	DWORD *dwThreadId = new DWORD[threadNum];
	HANDLE* hThreads = new HANDLE[threadNum];
	HANDLE hMutex = CreateMutex(NULL,false,NULL);
	
	for(int i = 0; i < threadNum; i++)
	{
		threadParams[i] = new UpdateCentersParams<DataType>;
		threadParams[i]->_akm = this;
		threadParams[i]->_hMutex = &hMutex;
		threadParams[i]->start = i*step;
		threadParams[i]->end = ((i+1)*step < feature_number)?((i+1)*step-1):(feature_number-1);
		hThreads[i] = CreateThread(
				NULL,
				0,
				multi_thread_update_centers<DataType> ,
				threadParams[i],
				0,
				&dwThreadId[i]);
	}

	WaitForMultipleObjects(threadNum,hThreads,true,INFINITE);

	for(int i = 0; i < threadNum; i++)
	{
		printf("Thread: %d over\t",i);
	}


	if(features)
	{
		delete[] features;
		features = NULL;
	}
}

template <typename DataType>
void Akmeans<DataType>::update_centers()
{	
	/* Update the centers with sum */
	int label_flag;
	int a = 0;
	for(int j = 0; j < center_size; j++)
	{
		label_flag = centers[j].label;

		if(!sum[label_flag].label)
		{
			continue;
		}
				
		a++;

		for(int k = 0; k < feature_dimension; k++)
		{			
			centers[j].descriptor[k] = sum[label_flag].descriptor[k]/sum[label_flag].label;
		}		
	}

	/* Reasign the label for the centers */
	for(int j = 0; j < center_size; j++)
	{
		centers[j].label = j;				
	}

	printf("Update Number %d\n",a);
}

/* Release the allocated data */
template <typename DataType>
void Akmeans<DataType>::release_data()
{
	kdtree_free(root);

	if(root)
	{
		delete root;
		root = NULL;
	}

	if(sum)
	{
		delete[] sum;
		sum = NULL;
	}

	if(centers)
	{
		delete[] centers;
		centers = NULL;
	}

	if(features)
	{
		delete[] features;
		features = NULL;
	}
}

/* Free the allocated space*/
template <typename DataType>
void Akmeans<DataType>::kdtree_free(Kdtreenode<float>* &kdroot)
{
	if(kdroot==NULL)
		return;

	kdtree_free(kdroot->kd_left);
	kdtree_free(kdroot->kd_right);

	if(kdroot!= NULL)
	{
		delete kdroot;
		kdroot = NULL;
	}
}

/* Save the centers after AKM */
template <typename DataType>
int Akmeans<DataType>::save_centers(const char* file_centers )
{
	if(root==NULL)
		return -1;

	ofstream outfile;
	outfile.open(file_centers,ios::out|ios::binary|ios::trunc);
	

	for(int i = 0; i < center_size; i++)
	{
		outfile.write((char*)&(centers[i].label),sizeof(int));
		outfile.write((char*)&(centers[i].dimension),sizeof(int));
		outfile.write((char*)centers[i].descriptor,sizeof(float)*centers[i].dimension);
	}	

	outfile.close();

	return 0;
}

/* Save kdtree after AKM */
template <typename DataType>
int Akmeans<DataType>::save_kdtree(const char* file_kdtree )
{
	if(root==NULL)
		return -1;

	ofstream outfile;
	outfile.open(file_kdtree,ios::out|ios::binary|ios::trunc);

	save_kdtree_part(outfile,root);

	outfile.close();

	return 0;
}

/* Save kdtree for recursion */
template <typename DataType>
void Akmeans<DataType>::save_kdtree_part(ofstream &outfile,Kdtreenode<float>* kdroot)
{
	if(kdroot==NULL)
		return;

	outfile.write((char*)&kdroot->flag,sizeof(int));
	outfile.write((char*)&kdroot->kv,sizeof(float));
	outfile.write((char*)&kdroot->ki,sizeof(int));
	outfile.write((char*)&kdroot->n,sizeof(int));
	outfile.write((char*)&kdroot->n_left,sizeof(int));
	outfile.write((char*)&kdroot->n_right,sizeof(int));
	
	save_kdtree_part(outfile,kdroot->kd_left);
	save_kdtree_part(outfile,kdroot->kd_right);
}

/*  Load the centers  */
template <typename DataType>
int Akmeans<DataType>::load_kdtree(const char* file_kdtree)
{
	if(root==NULL)
		root = new Kdtreenode<float>;

	if(centers==NULL)
		return -1;

	ifstream infile;
	infile.open(file_kdtree,ios::in|ios::binary); 
	
	load_kdtree_part(infile,root);

	infile.close();

	return 0;
}

/* Load kdtree and centers */
template <typename DataType>
int Akmeans<DataType>::load_centers(const char* file_centers)
{

	ifstream infile;
	infile.open(file_centers,ios::in|ios::binary); 


	infile.seekg(0,ios::end);
	long int length = infile.tellg(); 
	infile.seekg(0,ios::beg);

	if(center_size != length/(2*sizeof(int)+feature_dimension*sizeof(float)))
	{
		if(centers!=NULL)
		{
			delete[] centers;
			centers = NULL;
		}
		assign_center_size(length/(2*sizeof(int)+feature_dimension*sizeof(float)));
	}


	for(int i = 0; i < center_size; i++)
	{
		
		infile.read((char*)&(centers[i].label),sizeof(int));
		infile.read((char*)&(centers[i].dimension),sizeof(int));
		infile.read((char*)centers[i].descriptor,sizeof(float)*centers[i].dimension);
	}

	for(int i = 0; i < center_size; i++)
	{		
		centers[i].label = i;
	}

	infile.close();
	return 0;
}


/* Load the centers  for recursion */
template <typename DataType>
void Akmeans<DataType>::load_kdtree_part(ifstream &infile,Kdtreenode<float>* kdroot)
{
	if(kdroot==NULL)
		return;

	infile.read((char*)&kdroot->flag,sizeof(int));
	infile.read((char*)&kdroot->kv,sizeof(float));
	infile.read((char*)&kdroot->ki,sizeof(int));
	infile.read((char*)&kdroot->n,sizeof(int));
	infile.read((char*)&kdroot->n_left,sizeof(int));
	infile.read((char*)&kdroot->n_right,sizeof(int));

	kdroot->features = &centers[kdroot->flag];

	if((kdroot->n_left+kdroot->n_right))
	{
		kdroot->leaf = false;
	}

	if(kdroot->n_left)
	{
		kdroot->kd_left = new Kdtreenode<float>;
		load_kdtree_part(infile,kdroot->kd_left);
	}
	if(kdroot->n_right)
	{
		kdroot->kd_right = new Kdtreenode<float>;
		load_kdtree_part(infile,kdroot->kd_right);
	}
}

/* Expand the kd tree */
template <typename DataType>
void Akmeans<DataType>::expand_kd_node_subtree( Kdtreenode<float>* &kdroot )
{
	if(kdroot == NULL)
		return;
	kdroot = expand_kd_node_subtree2(kdroot);

	if( kdroot->kd_left )
	{
		expand_kd_node_subtree( kdroot->kd_left );
		//que_task.push(kdroot->kd_left);
	}
	if( kdroot->kd_right )
	{		
		expand_kd_node_subtree( kdroot->kd_right );
		//que_task.push(kdroot->kd_right);
	}
}

/* Expand the kd tree */
template <typename DataType>
Kdtreenode<float>* Akmeans<DataType>::expand_kd_node_subtree2( Kdtreenode<float>* &kdroot )
{
	if( kdroot==NULL )
		return NULL;

	count++;
	//if(count%10000==0)
		printf("times: %d\t",count);

	assign_part_key( kdroot );
	partition_features(kdroot);
	return kdroot;
}

/* Determines the descriptor index at which and the value with which to
partition a kd tree node's features. */
template <typename DataType>
void Akmeans<DataType>::assign_part_key( Kdtreenode<float>* kdroot )
{
	float var = 0.0;
	float var_max = 0.0;
	float mean= 0.0;
	float delta = 0.0;
	int var_max_ki = -1;
	float kv = 0.0;

	float* temp;
	
	/* Compute the index with maximum variance */
	for(int i = 0; i < feature_dimension; i++ )
	{
		mean = 0;
		var = 0;
		for( int j = 0; j < kdroot->n; j++)
		{
			mean += kdroot->features[j].descriptor[i];
		}
		mean /= kdroot->n;

		for( int j = 0; j < kdroot->n; j++)
		{
			delta = kdroot->features[j].descriptor[i] - mean;
			var += delta * delta;
		}		

		if( var_max < var)
		{
			var_max = var;
			var_max_ki = i;
		}
	}

	temp = new float[kdroot->n];
	
	mean = 0;
	for(int j = 0; j < kdroot->n; j++)
	{
		temp[j] = kdroot->features[j].descriptor[var_max_ki];
		mean += temp[j];
	}
	mean /= kdroot->n;

	kv = median_random_select(temp,0,kdroot->n-1,kdroot->n/2,mean);

	delete[]temp;

	kdroot->ki = var_max_ki;
	kdroot->kv = kv;

}


/* Partition the features into the subtree */
template <typename DataType>
void Akmeans<DataType>::partition_features(Kdtreenode<float>* kdroot)
{
	Feature<float>* featuress = kdroot->features;
	Feature<float> feature_temp;
	
	int ki = kdroot->ki;
	float kv = kdroot->kv;
	int n = kdroot->n;	
	int i = 0;
	int j = n -1;
	float delta = 256;


	while( ((j - i) >=1) || delta > 0.0001)				/* Resort the features */
	{
		while(featuress[i].descriptor[ki] < kv && i < j) i++;
		while(featuress[j].descriptor[ki] >= kv && i < j) j--;

		feature_temp = featuress[i];
		featuress[i] = featuress[j];
		featuress[j] = feature_temp;

		delta = featuress[i].descriptor[ki]-featuress[j].descriptor[ki];
		delta = (delta > 0)? delta: -delta;
	}

	/* select the median one */
	j = n/2;

	
	kdroot->features = &featuress[j];	
	kdroot->flag = &featuress[j]- &centers[0];
	
	if(n > 1)
		kdroot->leaf = false;
	else
	{
		return;
	}

	kdroot->n_left = j;
	kdroot->n_right = n -j;

	if(kdroot->n_left!=0)
	{
		kdroot->kd_left = new Kdtreenode<float>;
		kdtreenode_init(kdroot->kd_left,featuress,j);
	}

	if(kdroot->n_right!=0)
	{
		kdroot->kd_right = new Kdtreenode<float>;
		kdtreenode_init(kdroot->kd_right,featuress+j,(n-j));
	}
}

