#include "storystruct.h"
#include <Windows.h>

CUniverse::CUniverse(string path)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	//hFind = FindFirstFile((LPCWSTR)path.c_str(), &FindFileData);
	hFind = FindFirstFile("F:\\globe\\class\\����PPT�γ�\\*", &FindFileData);
	
	if(hFind == INVALID_HANDLE_VALUE)
	{
		printf ("Invalid file handle.\n");
	}
	else
	{
		printf ("First file name is %s\n", FindFileData.cFileName);
		while (FindNextFile(hFind, &FindFileData) != 0)
		{
			printf ("Next file name is %s\n", FindFileData.cFileName);
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
				printf ("����һ��Ŀ¼\n");
				//CGalaxy galaxy(
			}
		}

		DWORD dwError = GetLastError();
		FindClose(hFind);
		if (dwError != ERROR_NO_MORE_FILES) 
		{
			printf ("FindNextFile error. Error is %u\n", dwError);
		}
	}
}