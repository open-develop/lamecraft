#ifndef AURORA_THREAD_H
#define AURORA_THREAD_H

extern "C" 
{
	#include <pthread.h>
}

namespace Aurora
{
	namespace System
	{
		class Thread
		{
		public:
		
			typedef void (*FuncType)(void*);
			
			Thread(FuncType Function, void* UserData = NULL);
			virtual ~Thread();
			
			void Launch();
			void Wait();
			void Terminate();
			
		private:
		
			virtual void Run();
			
			static void* ThreadFunc(void* UserData);

			pthread_t myThread;   ///< Unix thread instance
			bool      myIsActive; ///< Thread state (active or inactive)
			FuncType  myFunction; ///< Function to call as the thread entry point
			void*     myUserData; ///< Data to pass to the thread function
		
		protected :

			Thread();

		
		};
	}
}

#endif

