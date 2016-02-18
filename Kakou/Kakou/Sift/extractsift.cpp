#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <io.h>
#include <iostream>
#include <fstream>
using namespace std;

#include "extractsift.h"
#include "kits.h"
#include "sift.h"
#include "imgfeatures.h"


/**
A function to extract the sift features from the image.
@param image_path : the path by which we can access the image
@param sift_path:	the path we store the sift file.
*/
int extractsift(char* image_path,char* sift_path)
{
	IplImage *img = NULL;
	struct feature *feat = NULL;
	int nfeat = 0;
	int dim = 0;

	img = cvLoadImage(image_path, 1);
	if( ! img )
	{
		fatal_error( "unable to load image from %s", image_path );
	}

	if(_access(sift_path,0)==0)
	{
		return 0;
	}
	ofstream outfile;
	outfile.open(sift_path,ios::out|ios::binary);	

	nfeat = sift_features(img, &feat);
	

	if(!feat)
	{
		fprintf( stderr, "Error: no features extracted in  %s, line %s\n",
				__FILE__, __LINE__ );
	}

	dim = feat[0].d;

	outfile.write((char*)&(img->width),sizeof(int));
	outfile.write((char*)&(img->height),sizeof(int));
	outfile.write((char*)&nfeat,sizeof(int));
	outfile.write((char*)&dim,sizeof(int));

	for(int i = 0; i < nfeat; i++)
	{
		float dtemp = feat[i].x;
		outfile.write((char*)&dtemp,sizeof(float));
		dtemp = feat[i].y;
		outfile.write((char*)&dtemp,sizeof(float));		
		
		for( int j = 0; j < dim; j++ )
		{
			unsigned char ctemp = (unsigned char)feat[i].descr[j];
			outfile.write((char*)&ctemp,sizeof(unsigned char));
		}	

	}

	outfile.close();

	cvReleaseImage(&img);
	free(feat);

	return 0;
}