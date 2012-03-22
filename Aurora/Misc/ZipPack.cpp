#include <Aurora/Misc/ZipPack.h>

namespace Aurora
{
	namespace Misc
	{
		ZipPack::ZipPack(std::string filename)
		{
			_fileName = filename;
			_loadedFileName = filename;

			_loaded = false;
		}

		ZipPack::ZipPack(std::string filename,std::string loadedFileName)
		{
			_fileName = filename;
			_loadedFileName = loadedFileName;

			_loaded = false;
		}

		bool ZipPack::Open()
		{
			_zipFile = unzOpen(_loadedFileName.c_str());

			if (_zipFile != NULL)
			{
				_loaded = true;
			}

			return _loaded;
		}

		void ZipPack::Close()
		{
			if (_loaded)
			{
				unzClose(_zipFile);
			}
		}

		void ZipPack::ListFiles()
		{
			unz_global_info globalInfo;
			int errorFlag = 0;

			//get info from zip file
			errorFlag = unzGetGlobalInfo(_zipFile,&globalInfo);

			if (errorFlag != UNZ_OK)
			{
				//error
				return;
			}

			//go thou all files
			for (unsigned int i = 0;i < globalInfo.number_entry;i++)
			{
				char zipFileName[256];
				unz_file_info fileInfo;

				//get file info
				errorFlag = unzGetCurrentFileInfo(_zipFile,&fileInfo,zipFileName,sizeof(zipFileName),NULL,0,NULL,0);

				if (errorFlag != UNZ_OK)
				{
					//printf("error %d with zipfile in unzGetCurrentFileInfo\n",err);
					break;
				}


				//go to next file
				if ((i+1) < globalInfo.number_entry)
				{
					errorFlag = unzGoToNextFile(_zipFile);
					if (errorFlag != UNZ_OK)
					{
						//printf("error %d with zipfile in unzGoToNextFile\n",err);
						break;
					}
				}
			}
		}

		ZipFile* ZipPack::GetZipFile( std::string fileName, std::string loadName )
		{
			ZipFile* newFile = new ZipFile(fileName,loadName);
			newFile->_zipFile = _zipFile;

			return newFile;
		}

		bool ZipPack::FileExists(std::string fileName)
		{
			//locate file in zip 
			if (unzLocateFile(_zipFile,fileName.c_str(),0) != UNZ_OK)
			{
				return false;
			}

			return true;
		}
	}
}
