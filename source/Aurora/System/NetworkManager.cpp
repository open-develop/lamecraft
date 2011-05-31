#include <Aurora/System/NetworkManager.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

#include <pspnet.h>
#include <psputility.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <pspnet_resolver.h>

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

		void NetworkManager::InitializeConnection()
		{
			//if already initialized don't do this again
			if(connectionInitialized)
				return;

			memset(local_address, 0, 32);
			sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON); //Load the net module
			sceUtilityLoadNetModule(PSP_NET_MODULE_INET); //Load the internet module
			sceNetInit(128*1024, 42, 4*1024, 42, 4*1024); //No documentation.  Dunno what these values are.
			sceNetInetInit(); //Start up the inet engine
			sceNetApctlInit(0x8000, 48); //Start up the AP control system
			sceNetResolverInit(); //start the resolver lib (does this need to come after ap conn?)

			connectionInitialized = true;
		}

		void NetworkManager::ConnectAP()
		{

		}
	}
}
