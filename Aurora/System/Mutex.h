#ifndef AURORA_MUTEX_H
#define AURORA_MUTEX_H

extern "C" 
{
	#include <pthread.h>
}

namespace Aurora
{
	namespace System
	{
		class Mutex
		{
		private:

			pthread_mutex_t myMutex;
			
		public:
		
			Mutex();
			~Mutex();
			
			void Lock();
			void Unlock();
		};
	}
}

#endif

