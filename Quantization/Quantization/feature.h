#pragma once

#include <iostream>
#include <cstdio>
#include <cstdlib>

template <typename DataType>
class Feature
{
public:	
	Feature();
	~Feature();	
	void assign_feature_dimension(int dim);		/* Assign the dimension of the descriptor of feature*/	
	Feature* operator=(Feature &feature_source);	/* Overloaded opeartor = */	
	Feature* copyData(DataType* desc_array, int n);	/* copyData from array*/
public:
	DataType* descriptor;			/* Descriptors of the feature*/
	int dimension;					/* The dimension of the descriptors*/
	int label;						/* label for one center */
};


template <typename DataType>
Feature<DataType>::Feature()
{
	descriptor = NULL;			/* Initialize the attributes */
	dimension = 0;
	label = -1;
}

template <typename DataType>
Feature<DataType>::~Feature()
{
	label = -1;
	dimension = 0;
	if(descriptor!= NULL)		/* Release the allocated heap resource */
	{
		delete[] descriptor;
		descriptor = NULL;
	}
}

/* Assign the dimension of the descriptor of feature*/
template <typename DataType>
void Feature<DataType>::assign_feature_dimension(int dim)
{
	if(dimension == dim || dim == 0 )		/* Meaningless for the assignment of dimension*/
		return;

	if (descriptor != NULL)			/* Release the held data when assigned a different dimension value */
	{
		delete[] descriptor;
		descriptor = NULL;
	}

	dimension = dim;
	descriptor = new DataType[dimension];		/* Allocate heap space */

	for(int i = 0; i < dimension; i++)			/* Initialize the value of descriptors with zero */
	{
		descriptor[i] = 0;
	}
}


/* Overloaded opeartor = */
template <typename DataType>
Feature<DataType>* Feature<DataType>::operator=(Feature<DataType> &feature_source)
{		
	if (feature_source.dimension == 0)			/* Assigned with an empty feature*/
	{
		if(descriptor != NULL)			/* Release the held data when assigned a different dimension value */
		{
			delete[] descriptor;			
			descriptor = NULL;
			dimension = 0;
		}
		return this;
	}

	if(this == &feature_source)			/* Assigened with itself */
		return this;

	if(dimension == feature_source.dimension) /* With the same dimension */
	{
		for( int i =0; i < dimension; i++)
		{
			descriptor[i] = feature_source.descriptor[i];
		}
		return this;
	}
	else                /* With the different dimension */
	{
		if(descriptor != NULL)
		{
			delete[] descriptor;			
			descriptor = NULL;
			dimension = 0;
		}

		dimension = feature_source.dimension;
		descriptor = new DataType[dimension];		/* Allocate heap space */

		for(int i = 0; i < dimension; i++)			/* Copy the descriptors */
		{
			descriptor[i] = feature_source.descriptor[i];
		}
		return this;
	}	
}

/* copyData from array*/
template <typename DataType>
Feature<DataType>* Feature<DataType>::copyData(DataType* desc_array, int n)
{
	if( descriptor = desc_array && dimension == n)		/* Copy data from itself */
		return this;

	if(dimension == feature_source.dimension) /* With the same dimension */
	{
		for( int i =0; i < dimension; i++)
		{
			descriptor[i] = desc_array[i];
		}
		return this;
	}
	else                /* With the different dimension */
	{
		if(descriptor != NULL)
		{
			delete[] descriptor;			
			descriptor = NULL;
			dimension = 0;
		}

		dimension = n;

		if( dimension == 0 )						/* Dimension with zero */
			return this;

		descriptor = new DataType[dimension];		/* Allocate heap space */

		for(int i = 0; i < dimension; i++)			/* Copy the descriptors */
		{
			descriptor[i] = desc_array[i];
		}
		return this;
	}	
}

