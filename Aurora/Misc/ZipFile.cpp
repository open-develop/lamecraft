#include <Aurora/Misc/ZipFile.h>

namespace Aurora
{
	namespace Misc
	{
		ZipFile::ZipFile()
		{
			_fileName = "";
			_loadFileName = "";

			_loaded = false;
		}

		ZipFile::ZipFile(std::string fileName,std::string loadFileName)
		{
			_fileName = fileName;
			_loadFileName = loadFileName;

			_loaded = false;
		}

		bool ZipFile::Open()
		{
			int errorFlag = 0;
			char zipFileName[256];
			unz_file_info fileInfo;

			//locate file in zip 
			if (unzLocateFile(_zipFile,_loadFileName.c_str(),0) != UNZ_OK)
			{
				_loaded = false;
				return false;
			}

			//get info about file
			errorFlag = unzGetCurrentFileInfo(_zipFile,&fileInfo,zipFileName,sizeof(zipFileName),NULL,0,NULL,0);

			if (errorFlag != UNZ_OK)
			{
				_loaded = false;
				return false;
			}

			errorFlag = unzOpenCurrentFile(_zipFile);
			if (errorFlag != UNZ_OK)
			{
				_loaded = false;
				return false;
			}

			_loaded = true;
			return true;
		}

		void ZipFile::Close()
		{
			unzCloseCurrentFile(_zipFile);
		}		

		void ZipFile::Read(void *data,unsigned int size,int count)
		{
			unzReadCurrentFile(_zipFile,data,size * count);
		}

		unsigned char* ZipFile::GetData(int &dataSize)
		{
			int errorFlag = 0;
			char zipFileName[256];
			unz_file_info fileInfo;
			unsigned char* data;

			//locate file in zip 
			if (unzLocateFile(_zipFile,_loadFileName.c_str(),0) != UNZ_OK)
			{
				return NULL;
			}

			//get info about file
			errorFlag = unzGetCurrentFileInfo(_zipFile,&fileInfo,zipFileName,sizeof(zipFileName),NULL,0,NULL,0);

			if (errorFlag != UNZ_OK)
				return NULL;

			//create data buffer
			dataSize = fileInfo.uncompressed_size;
			data = new unsigned char[fileInfo.uncompressed_size];

			//open file
			errorFlag = unzOpenCurrentFile(_zipFile);
			if (errorFlag != UNZ_OK)
				return NULL;

			//read file content
			unzReadCurrentFile(_zipFile,data,fileInfo.uncompressed_size);

			unzCloseCurrentFile(_zipFile);

			return data;
		}

	}
}


