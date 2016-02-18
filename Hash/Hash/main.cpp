#include <iostream>
#include "kits.h"
#include "computehash.h"
#include "words.h"
#include "hash.h"

using namespace std;

extern int get_content_in_dir(char *path, vector<mString> &files,unsigned int attrib);

int main(int argc, char* argv[])
{
	ComputeHash ch;
//	ch.init_hash_sketches("K:/test_1500/hashtable.txt");
//	ch.build_database(1000000);
//	ch.load_hash_values();
	
	vector<mString> qtzfolders;
	get_content_in_dir("K:/test_1500/quantization",qtzfolders,_A_SUBDIR);
	get_content_in_dir("K:/imageBase/quantization",qtzfolders,_A_SUBDIR);
	mString str;
	str = "K:/20140303第一批模板/quantization";
	qtzfolders.push_back(str);


	char file_name[NAME_LEN];
	for (int i = 0; i < qtzfolders.size(); i++)
	{
		vector<mString> qtznames;
		get_content_in_dir(qtzfolders.at(i).str,qtznames,_A_NORMAL);
		printf("In the directory: %s\n",qtzfolders.at(i).str);
		for (int j = 0; j < qtznames.size(); j++)
		{
			printf("%d\t",j);
			strcpy_s(file_name,qtznames.at(j).str);
			file_name[strrchr(file_name,'.')-file_name]='\0';
			//ch.hash_image(qtzfolders.at(i).str,file_name,1);
			ch.add_image_size(qtzfolders.at(i).str,file_name);
		}
	}

	printf("Hash Over!\n");
	return 0;
}