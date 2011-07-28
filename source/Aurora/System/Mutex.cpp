#include <Aurora/System/Mutex.h>

namespace Aurora
{
	namespace System
	{
		Mutex::Mutex()
		{
			pthread_mutex_init(&myMutex, NULL);
		}
		
		Mutex::~Mutex()
		{
			pthread_mutex_destroy(&myMutex);
		}

		void Mutex::Lock()
		{
			pthread_mutex_lock(&myMutex);
		}

		void Mutex::Unlock()
		{
			pthread_mutex_unlock(&myMutex);
		}		
	}
}
