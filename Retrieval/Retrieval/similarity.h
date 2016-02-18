#pragma once
#include "words.h"
#include "kits.h"

class ImageHash
{
public:
	ImageHash();
	void load_image_hash_values(char* folder,char* file_name);
	int compare(ImageHash & a);
public:
	int sketchnum;
	int sketchsize;
	Words** words;
	int** listsize;
	char imagepath[NAME_LEN];
	char imagename[NAME_LEN];
	int height;
	int width;
};