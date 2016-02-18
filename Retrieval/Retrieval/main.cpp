#include <iostream>
#include "kits.h"
#include "words.h"
#include "similarity.h"
#include "fstream"


struct ranknode
{
	int i;
	int label;
};

using namespace std;

extern int get_content_in_dir(char *path, vector<mString> &files,unsigned int attrib);

int main(int argc, char* argv[])
{
	vector<mString> hashnames;
	char file_path[NAME_LEN] = "K:/test_1500/image/截取/18_veh";
	get_content_in_dir(file_path,hashnames,_A_NORMAL);
	int num = hashnames.size();
	ImageHash * imagehashes = new ImageHash[num];
	ranknode * ranklist = new ranknode[num];

	ofstream outfile;
	ofstream outfile2;
	outfile2.open("test41.txt",ios::out);

	for(int i = 0; i < num; i++)
	{
		imagehashes[i].load_image_hash_values(file_path,hashnames.at(i).str);
		ranklist[i].i = i;
		ranklist[i].label = i;
	}

	char file_name[NAME_LEN];
	for(int i = 0; i < num; i++)
	{
		//outfile << imagehashes[i].imagepath << " " << imagehashes[i].imagename << endl;
		for(int j = 0; j < num; j++)
		{
			ranklist[j].i= imagehashes[i].compare(imagehashes[j]);
			cout << ranklist[j].i << "\t" ;
		}
		cout << endl;

		for( int k = 0; k < num; k++)
		{
			for(int m = k; m < num; m++)
			{
				if(ranklist[k].i < ranklist[m].i)
				{
					int temp = 0;
					temp = ranklist[k].i; 
					ranklist[k].i = ranklist[m].i;
					ranklist[m].i = temp;

					temp = ranklist[k].label; 
					ranklist[k].label = ranklist[m].label;
					ranklist[m].label = temp;
				}
			}
		}
		sprintf_s(file_name,"D:/Users/Surge/Desktop/results/%s.html",imagehashes[i].imagename);
		outfile.open(file_name,ios::out);
		outfile << "<html><body>" << endl;
		for( int k = 0; k < num; k++)
		{			
			sprintf_s(file_name,"<img src=\"%s/%s\"height=150 width=200>",imagehashes[ranklist[k].label].imagepath,imagehashes[ranklist[k].label].imagename);
			outfile << file_name << endl;
			outfile2 << ranklist[k].i << '\t';
		}
		outfile << "</body></html>"<< endl;
		outfile.close();
		outfile2 << endl;
	}
	
	outfile2.close();

	
	//vector<mString> hashfolders;
	//get_content_in_dir("K:/test_1500/quantization",hashfolders,_A_SUBDIR);
	//get_content_in_dir("K:/imageBase/quantization",hashfolders,_A_SUBDIR);
	//mString str;
	//str = "K:/20140303第一批模板/quantization";
	//hashfolders.push_back(str);


	//char file_name[NAME_LEN];
	//for (int i = 0; i < hashfolders.size(); i++)
	//{
	//	vector<mString> hashnames;
	//	get_content_in_dir(hashfolders.at(i).str,hashnames,_A_NORMAL);
	//	printf("In the directory: %s\n",hashfolders.at(i).str);
	//	for (int j = 0; j < hashnames.size(); j++)
	//	{
	//		printf("%d\t",j);
	//		strcpy_s(file_name,hashnames.at(j).str);
	//		file_name[strrchr(file_name,'.')-file_name]='\0';
	//	//	ch.hash_image(hashfolders.at(i).str,file_name,1);
	//	}
	//}

	//printf("Hash Over!\n");
	system("pause");
	return 0;
}