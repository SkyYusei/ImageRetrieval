#include <iostream>
using namespace std;

#include "feature.h"
#include  "kdtreenode.h"
#include "kits.h"
#include "quenode.h"
#include "akmeans.h"
#include "multithreadtask.h"
#include "quantization.h"

int main(int argc, char* argv[])
{
	int center_size = 1000000;


	Akmeans<unsigned char> A;
	A.assign_feature_dimension(128);
	A.assign_center_size(center_size);
	
	
	cout << "Load centers" << endl;
	A.load_centers("D:/Users/Surge/Desktop/centers.whole1m");
	cout << "Load Kdtree" << endl;
	A.load_kdtree("D:/Users/Surge/Desktop/kdtree.whole1m");

	// Get all folders contain sift files
	vector<mString> siftfolders;
	get_content_in_dir("K:/imageBase/sift",siftfolders,_A_SUBDIR);
	/*mString str;
	str = "K:/20140303第一批模板/sift";
	siftfolders.push_back(str);*/


	char file_name[NAME_LEN];
	for (int i = 0; i < siftfolders.size(); i++)
	{
		vector<mString> siftnames;
		get_content_in_dir(siftfolders.at(i).str,siftnames,_A_NORMAL);
		printf("In the directory: %s\n",siftfolders.at(i).str);
		for (int j = 0; j < siftnames.size(); j++)
		{
			printf("%d\t",j);
			strcpy_s(file_name,siftnames.at(j).str);
			file_name[strrchr(file_name,'.')-file_name]='\0';
			quantization(&A,siftfolders.at(i).str,file_name, 1);
		}
	}



	A.release_data();
	printf("Data Release Over!\n");
	return 0;
}