
#ifndef AURORA_LOGGER_H
#define AURORA_LOGGER_H

#include <pspkernel.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>


using namespace std;

namespace Aurora
{
	namespace Utils
	{

		class Logger
		{
		private:

			int fdout;

		public:

			static Logger m_Logger;

			static Logger* Instance() {
				return &m_Logger;
			}

			void Init();
			void LogMessage(const char *message, ...);
			void Close();

			int appendBufferToFile(const char * path, void * buffer, int buflen)
			{
				  // Written Bytes
				  int written = 0;

				  // Open File for Appending
				  SceUID file = sceIoOpen(path, PSP_O_APPEND | PSP_O_CREAT | PSP_O_WRONLY, 0777);

				  // Opened File
				  if(file >= 0)
				  {
				    // Write Buffer
				    written = sceIoWrite(file, buffer, buflen);

				    // Close File
				    sceIoClose(file);
				  }

				  // Return Written Bytes
				  return written;
			}

		};
	}
}



#endif
