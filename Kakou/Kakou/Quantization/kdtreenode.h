#pragma once

#include <iostream>
#include <cstdio>

#include "feature.h"

template <typename DataType>
class Kdtreenode
{
public:
	Kdtreenode()
	{
		ki = -1;
		kv = -1;
		n = 0;
		leaf = true;
		n_left = 0;
		n_right = 0;
		flag = -1;


		features = NULL;
		kd_left = NULL;
		kd_right = NULL;
	}

	~Kdtreenode()
	{
		if(kd_left!=NULL)
		{
			delete kd_left;
			kd_left = NULL;
		}

		if(kd_right!=NULL)
		{
			delete kd_right;
			kd_right = NULL;
		}
		
		features = NULL;
	}

	void set_kdtreenode(int _ki, int _kv, int _leaf, int _n)
	{
		ki = _ki;
		kv = _kv;
		leaf = _leaf;
		n = _n;
	}

public:
	int ki;							/* partition key index */	
	float kv;						/* partition key value */	
	bool leaf;						/* label for leaf */	
	Feature<DataType>* features;	/* features */	
	int n;							/* numbers of the features */
	int n_left;						/* numbers of features in the left subtree*/
	int n_right;					/* numbers of features in the right subtree*/
	int flag;						/* flag for the features int the data array*/


	Kdtreenode<DataType>* kd_left;		/* left child*/
	Kdtreenode<DataType>* kd_right;		/* right child*/
};



