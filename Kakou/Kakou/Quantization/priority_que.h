#pragma once
#include <cstdio>

template<class NodeType>
class Priority_que
{
public:
	Priority_que();
	~Priority_que();
	int push(NodeType *newnode);
	int pop();
	NodeType* top();
	bool empty();
	void clear();
	int getsize();
private:
	int insert(NodeType *newnode);
	int double_Array();
	int parent(int i);
public:
	NodeType *que;
	int length;	
	int size;
};

/* Construct function */
template<class NodeType>
Priority_que<NodeType>::Priority_que()
{
	length = 16;
	que = new NodeType[length];
	size = 0; 
}

/* Deconstruct function */
template<class NodeType>
Priority_que<NodeType>::~Priority_que()
{
	size = 0;
	length = 16;
	delete[] que;
}

/* Push a new node into the array and consider the length of the array */
template<class NodeType>
int Priority_que<NodeType>::push(NodeType *newnode)
{
	if(size >= length)
	{
		double_Array();
		if(!que)
		{
			printf("Array Extended error\n");
			return -1;
		}
	}
	insert(newnode);
	return 0;
}

/* If the length of array is not enough, then double the length of the array*/
template<class NodeType>
int Priority_que<NodeType>::double_Array()
{
	NodeType *newque = new NodeType[2*length];
	for(int i = 0; i < length; i++)
	{
		newque[i] = que[i];
	}
	
	delete[] que;
	que = newque;
	length = 2 * length;
	return 0;
}

/* Insert a new node into the array */
template<class NodeType>
int Priority_que<NodeType>::insert(NodeType *newnode)
{
	NodeType sorttemp;
	int i = size;
	size++;	
	que[i] = newnode;
	
	while(i > 0)
	{
		int j = parent(i);
		if(que[i] < que[j])
		{
			sorttemp = que[j];
			que[j] = que[i];
			que[i] = sorttemp;
		}
		i = j;
	}
	return 0;	
}

/* Get the parent node of a node */
template<class NodeType>
int Priority_que<NodeType>::parent(int i)
{
	if( i < 1)
		return -1;
	return (i-1)/2;
}

/* Pop the top node */
template<class NodeType>
int Priority_que<NodeType>::pop()
{
	int i = 0;
	NodeType sorttemp;
	
	/* Find the last node which don't have two children*/	
	while(2*i+1 < size-1 && 2*i+2 < size -1)
	{
		if(que[2*i+1] < que[2*i+2])
		{
			que[i] = que[2*i+1];
			i = 2*i+1;
		}
		else
		{
			que[i] = que[2*i+2];
			i = 2*i+2;
		}					
	}
	
	/* substitute the node with the end node */
	que[i] = que[size - 1];
	
	/* check the node whether it has children*/
	while(i < 0)
	{
		int j = parent(i);
		if(que[i] < que[j])
		{
			sorttemp = que[j];
			que[j] = que[i];
			que[i] = sorttemp;
		}
		i = j;
	}
	
	size--;
	return 0;	
}

/* Get the top node in the que */
template<class NodeType>
NodeType* Priority_que<NodeType>::top()
{
	if(!size) return NULL;
	return &que[0];
}

/* Get the size of the que*/
template<class NodeType>
int Priority_que<NodeType>::getsize()
{
	return size;
}

/* Check whether the que is empty*/
template<class NodeType>
bool Priority_que<NodeType>::empty()
{
	return !size;
}

template<class NodeType>
bool Priority_que<NodeType>::clear()
{
	size = 0;
	length = 16;
	delete[] que;
	que = new NodeType[length];
}