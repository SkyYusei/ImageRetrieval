#include <iostream>
using namespace std;

#include "feature.h"
#include  "kdtreenode.h"
#include "kits.h"
#include "quenode.h"
#include "akmeans.h"
#include "multithredtask.h"

int main(int argc, char* argv[])
{
	int center_size = 1000000;
	char filepath[3][100] = {"D:/Users/Surge/Desktop/words.whole","K:/20140303第一批模板/word_dic_feat.bin","K:/test_1500/word_dic_feat.bin"};

	//if(argc>=3)
	//{
	//	center_size = atoi(argv[1]);
	//	strcpy_s(filepath,argv[2]);
	//}

	Akmeans<unsigned char> A;
	A.assign_feature_dimension(128);
	A.assign_center_size(center_size);
	//A.random_set_centers();

	
	A.load_centers("D:/Users/Surge/Desktop/centers.whole1m");
	A.initial_root();
	A.build_kdtree();
	A.save_kdtree("D:/Users/Surge/Desktop/kdtree.whole1m");
	
	
	/*cout << "Load centers" << endl;
	A.load_centers("D:/Users/Surge/Desktop/centers.whole1m");
	cout << "Load Kdtree" << endl;
	A.load_kdtree("D:/Users/Surge/Desktop/kdtree.whole1m");
	A.initial_sum();
	A.initial_root();

	for(int i = 0; i <3; i++)
	{
		cout << "Read features: " << filepath[i] << endl;
		A.read_features(filepath[i]);
		A.count_features();
	}


	cout << "Update Centers" << endl;
	A.update_centers();
	cout << "Save Centers" << endl;
	A.save_centers("D:/Users/Surge/Desktop/centers.whole1m");*/

	//A.kmeans_with_kdtree(1);	

	A.release_data();
	printf("Data Release Over!\n");
	return 0;
}