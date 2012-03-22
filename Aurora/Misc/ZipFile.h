#ifndef ZIPFILE_H 
#define ZIPFILE_H 

#include <Aurora/Misc/unzip.h>
#include <string>

namespace Aurora
{
	namespace Misc
	{
		class ZipPack;

		class ZipFile
		{
		private:

			unzFile _zipFile;
			std::string _fileName;
			std::string _loadFileName;

			bool _loaded;

		public:

			ZipFile();
			ZipFile(std::string fileName,std::string loadFileName);

			bool Open();
			void Close();

			bool IsLoaded() { return _loaded; }

			void Read(void *data,unsigned int size,int count);
			unsigned char* GetData(int &dataSize);

			friend class ZipPack;
		};
	}
}


#endif
