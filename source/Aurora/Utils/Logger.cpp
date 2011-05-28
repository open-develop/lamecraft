#include <Aurora/Utils/Logger.h>
namespace Aurora
{
	namespace Utils
	{
		Logger Logger::m_Logger;

		void Logger::Init()
		{
			//fdout = sceIoOpen("Log.txt", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}

		void Logger::LogMessage(const char *message, ...)
		{
			va_list argList;
			char cbuffer[1024];
			va_start(argList, message);
			vsnprintf(cbuffer, 1024, message, argList);
			va_end(argList);
			//sceIoWrite(fdout, cbuffer, strlen(cbuffer));
			// Written Bytes
			  int written = 0;

			  // Open File for Appending
			  SceUID file = sceIoOpen("Log.txt", PSP_O_APPEND | PSP_O_CREAT | PSP_O_WRONLY, 0777);

			  // Opened File
			  if(file >= 0)
			  {
				// Write Buffer
				written = sceIoWrite(file, cbuffer, strlen(cbuffer));

				// Close File
				sceIoClose(file);
			  }
		}


		void Logger::Close()
		{
			//sceIoClose(fdout);
		}
	}
}
