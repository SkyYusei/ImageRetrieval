#include <float.h>
#include "queue"

#include "feature.h"
#include "kdtreenode.h"
#include "quenode.h"

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