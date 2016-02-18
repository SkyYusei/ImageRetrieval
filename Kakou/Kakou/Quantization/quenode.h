#pragma once
#include "feature.h"
#include "kdtreenode.h"
#include <float.h>
#include "queue"

/* Keep a queque within a certain size */
template<class NodeType>
void que_push_with_MaxSize(priority_queue<NodeType> &que,NodeType* newNode, int maxsize);

/* Node for the priority queue to iterate and calculate the K nearest neighbors 
 *  Minimum priority queue
 */
class Queuenode
{
public:
	Queuenode()
	{
		node = NULL;
		dist_ki = 0;
	}
	friend bool operator<(Queuenode a,Queuenode b)
	{
		return (a.dist_ki < b.dist_ki);
	}
public:
	Kdtreenode<float>* node;
	float dist_ki;
};

/* Store the nearest neighbors in calculating 
 * Maximum priority queue
 */
class Queuenode2
{
public:
	Queuenode2()
	{
		_nbr = NULL;
		dist = FLT_MAX;
	}
	friend bool operator<(Queuenode2 a,Queuenode2 b)
	{
		return (a.dist > b.dist);
	}
public:
	Feature<float>* _nbr;
	float dist;
};

template<class NodeType>
void que_push_with_MaxSize(priority_queue<NodeType> &que,NodeType* newNode, int maxsize)
{
	if(!newNode)
		return;
	if(que.size()< maxsize)
	{
		que.push(*newNode);
	}
	else
	{
		NodeType topNode = que.top();
		if(topNode < *newNode)
		{
			que.pop();
			que.push(*newNode);
		}
	}
}