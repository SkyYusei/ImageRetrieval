#include <SDKDDKVer.h>
#include "./Sift/extractsift.h"
#include "./Quantization/Akmeans.h"
//#include "./Quantization/quantization.h"
#include "./Quantization/quantization.h"
#include "./Quantization/quenode.h"
#include "./Hash/computehash.h"


#include <iostream>
using namespace std;

int main(int argc,char* argv[])
{
	//* Extract sift*/
	extractsift("D:/Users/Surge/Desktop/test/test1.bmp","D:/Users/Surge/Desktop/test/test1.sift");
	extractsift("D:/Users/Surge/Desktop/test/test2.bmp","D:/Users/Surge/Desktop/test/test2.sift");

	//* Quantization */
	int center_size = 1000000;
	Akmeans<unsigned char> A;
	A.assign_feature_dimension(128);
	A.assign_center_size(center_size);
	
	cout << "Load centers" << endl;
	A.load_centers("D:/Users/Surge/Desktop/centers.whole1m");
	cout << "Load Kdtree" << endl;
	A.load_kdtree("D:/Users/Surge/Desktop/kdtree.whole1m");

	quantization(&A,"D:/Users/Surge/Desktop/test/test1.sift","D:/Users/Surge/Desktop/test/test1.qtz");
	quantization(&A,"D:/Users/Surge/Desktop/test/test2.sift","D:/Users/Surge/Desktop/test/test2.qtz");

	//* Hash*/
	HashSketches  hashes;
	hashes.init_hash_sketches("D:/Users/Surge/Desktop/test/hashtable.ht");
	hashes.hash_qtz("D:/Users/Surge/Desktop/test/test1.qtz","D:/Users/Surge/Desktop/test/test1.has");
	hashes.hash_qtz("D:/Users/Surge/Desktop/test/test2.qtz","D:/Users/Surge/Desktop/test/test2.has");

	//* Compare*/
	hashes.cmp_hash("D:/Users/Surge/Desktop/test/test1.has","D:/Users/Surge/Desktop/test/test2.has");

	/*int center_size = 1000000;
	Akmeans<unsigned char> A;
	A.assign_feature_dimension(128);
	A.assign_center_size(center_size);*/
	/*cout << "Read features: "<<"K:/20140303第一批模板/word_dic_feat2.bin";
	A.read_features("K:/20140303第一批模板/word_dic_feat2.bin");		
	cout << " over" << endl;

	A.random_set_centers();
	A.initial_sum();
	A.initial_root();
	A.build_kdtree();
	A.save_kdtree("D:/Users/Surge/Desktop/kdtree.whole10w");*/	
	
	
	/*cout << "Load centers";
	A.load_centers("D:/Users/Surge/Desktop/centers.whole1m");
	cout <<" over!" << endl;*/

	/*A.initial_root();
	A.build_kdtree();
	cout << "Save kdtree ";
	A.save_kdtree("D:/Users/Surge/Desktop/kdtree.whole1m");
	cout << "over!" << endl;*/


	/*cout << "Load Kdtree";
	A.load_kdtree("D:/Users/Surge/Desktop/kdtree.whole1m");
	cout << " over" << endl;*/
	/*A.initial_sum();
	A.initial_root();*/
	

	
	/*cout << "Start count features " << endl;
	A.count_features();
	cout << "Count over" << endl;
	
	cout << "Update Centers" << endl;
	A.update_centers();
	cout << "Save Centers" << endl;
	A.save_centers("D:/Users/Surge/Desktop/centers.whole10w");*/

	system("pause");
	return 0;
}