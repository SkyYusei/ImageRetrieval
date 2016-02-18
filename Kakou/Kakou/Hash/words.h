#pragma once

class Words
{
public:
	Words():labels(NULL),labelnum(-1),x(-1.0),y(-1.0)
	{
	}

	void setNum(int num)
	{
		if(labelnum == num)
			return;
		if(labels)
		{
			delete[] labels;
			labels = NULL;
		}
		labelnum = num;
		labels = new int[labelnum];
	}

public:
	int* labels;
	int labelnum;
	float x;
	float y;
};