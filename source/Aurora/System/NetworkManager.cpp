#include <Aurora/System/NetworkManager.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

#include <pspnet.h>
#include <psputility.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <pspnet_resolver.h>
#include <psphttp.h>

#include <pspwlan.h>

namespace Aurora
{
	namespace System
	{
		NetworkManager NetworkManager::m_NetworkManager;

		NetworkManager* NetworkManager::Instance()
		{
			return &m_NetworkManager;
		}

		void NetworkManager::Init()
		{
			connectionInitialized = false;
		}

		int NetworkManager::InitializeConnection()
		{
			int result;

			result = sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON);

			if(result < 0)
				return result;

			result = sceUtilityLoadNetModule(PSP_NET_MODULE_INET);

			if(result < 0)
				return result;

			result = sceUtilityLoadNetModule(PSP_NET_MODULE_PARSEURI);

			if(result < 0)
				return result;

			result = sceUtilityLoadNetModule(PSP_NET_MODULE_PARSEHTTP);

			if(result < 0)
				return result;

			result = sceUtilityLoadNetModule(PSP_NET_MODULE_HTTP);

			if(result < 0)
				return result;

			result = sceNetInit(128*1024, 42, 0, 42, 0);

			if(result < 0)
				return result;

			result = sceNetInetInit();

			if(result < 0)
				return result;

			result = sceNetApctlInit(0x10000, 48);

			if(result < 0)
				return result;

			result = sceNetResolverInit();

			if(result < 0)
				return result;

			return(1);
		}

		int NetworkManager::ShutDownConnection()
		{
			int result;

			result = sceNetApctlTerm();

			if(result < 0)
				return result;

			result = sceNetResolverTerm();

			if(result < 0)
				return result;

			result = sceNetInetTerm();

			if(result < 0)
				return result;

			result = sceNetTerm();

			if(result < 0)
				return result;

			result = sceUtilityUnloadNetModule(PSP_NET_MODULE_HTTP);

			if(result < 0)
				return result;

			result = sceUtilityUnloadNetModule(PSP_NET_MODULE_PARSEHTTP);

			if(result < 0)
				return result;

			result = sceUtilityUnloadNetModule(PSP_NET_MODULE_PARSEURI);

			if(result < 0)
				return result;

			result = sceUtilityUnloadNetModule(PSP_NET_MODULE_INET);

			if(result < 0)
				return result;

			result = sceUtilityUnloadNetModule(PSP_NET_MODULE_COMMON);

			if(result < 0)
				return result;

			return 1;
		}

		int NetworkManager::GetFile(const char *url, const char *filepath)
		{
			int templ, connection, request, ret, status, dataend, fd, byteswritten;
			SceULong64 contentsize;
			unsigned char readbuffer[8192];

			ret = sceHttpInit(20000);

			if(ret < 0)
				return 0;

			templ = sceHttpCreateTemplate("PGE-agent/0.0.1 libhttp/1.0.0", 1, 1);

			if(templ < 0)
				return 0;

			ret = sceHttpSetResolveTimeOut(templ, 3000000);

			if(ret < 0)
				return 0;

			ret = sceHttpSetRecvTimeOut(templ, 60000000);

			if(ret < 0)
				return 0;

			ret = sceHttpSetSendTimeOut(templ, 60000000);

			if(ret < 0)
				return 0;

			connection = sceHttpCreateConnectionWithURL(templ, url, 0);

			if(connection < 0)
				return 0;

			request = sceHttpCreateRequestWithURL(connection, PSP_HTTP_METHOD_GET, (char*)url, 0);

			if(request < 0)
				return 0;

			ret = sceHttpSendRequest(request, 0, 0);

			if(ret < 0)
				return 0;

			ret = sceHttpGetStatusCode(request, &status);

			if(ret < 0)
				return 0;

			if(status != 200)
				return 0;

			ret = sceHttpGetContentLength(request, &contentsize);

			if(ret < 0)
				return 0;

			dataend = 0;
			byteswritten = 0;

			fd = sceIoOpen(filepath, PSP_O_WRONLY | PSP_O_CREAT, 0777);

			while(dataend == 0)
			{
				ret = sceHttpReadData(request, readbuffer, 8192);

				if(ret < 0)
				{
					sceIoWrite(fd, filepath, 4);
					sceIoClose(fd);
					return 0;
				}

				if(ret == 0)
					dataend = 1;

				if(ret > 0)
				{
					byteswritten += ret;
					sceIoWrite(fd, readbuffer, ret);
					//printf("Got and wrote %d bytes...\n", ret);
				}
			}

			sceIoClose(fd);

			//printf("File saved (size = %d bytes)... Exiting!\n", bytes_written);

			sceHttpDeleteRequest(request);

			sceHttpDeleteConnection(connection);

			sceHttpDeleteTemplate(templ);

			sceHttpEnd();

			return 1;
		}


	}
}
