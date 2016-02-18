#include "kits.h"

/* Get content(files or folders) in one directory
 * Input:	path	The directory to be searched
 *			files	The names of obtained contents
 *			attrib	The type of the file(files or folders) to be searched
 */
int get_content_in_dir(char *path, vector<mString> &files,unsigned int attrib)
{
	struct _finddata_t fa;
	__int64 fHandle;
	bool flag = false;

	if(attrib) flag = true;
	
	char npath[NAME_LEN];
	sprintf_s(npath, "%s/*", path);

	if( (fHandle=_findfirst(npath,&fa))==-1L )
	{
		printf( "Directory %s Open failed!\n",path);
		return 0;
	}
	else
	do
	{
		if( strcmp(fa.name,".")!=0 && strcmp(fa.name,"..")!=0)
		{
			// folders
			if(flag)
			{
				char subfolder[NAME_LEN];
				mString str;

				sprintf_s(subfolder, "%s/%s", path, fa.name);
				if (fa.attrib & _A_SUBDIR)
				{
					str = subfolder;
					files.push_back(str);
					get_content_in_dir(str.str,files,_A_SUBDIR);
				}
			}
			// files
			else
			{
				if (fa.attrib & _A_SUBDIR) continue;
				mString str;
				str = fa.name;
				files.push_back(str);
			}	
		}
		
	}while( _findnext(fHandle,&fa)==0 );
	_findclose( fHandle );
	return 0;
}
