#include <Aurora/System/Thread.h>

namespace Aurora
{
	namespace System
	{
		Thread::Thread() :
		myIsActive(false),
		myFunction(NULL),
		myUserData(NULL)
		{

		}

		Thread::Thread(Thread::FuncType Function, void* UserData) :
		myIsActive(false),
		myFunction(Function),
		myUserData(UserData)
		{

		}

		Thread::~Thread()
		{
			// Wait for the thread to finish before destroying the instance
			Wait();
		}

		void Thread::Launch()
		{
			// Wait for the thread to finish, in case it was already running
			Wait();

			// Create the thread
			myIsActive = true;
			int Error = pthread_create(&myThread, NULL, &Thread::ThreadFunc, this);

			// Error ?
			if (Error != 0)
			{
				//std::cerr << "Failed to create thread" << std::endl;
				myIsActive = false;
			}
		}

		void Thread::Wait()
		{
			if (myIsActive)
			{
				// Wait for the thread to finish, no timeout
				pthread_join(myThread, NULL);

				// Reset the thread state
				myIsActive = false;
			}
		}

		void Thread::Terminate()
		{
			if (myIsActive)
			{
				pthread_cancel(myThread);
				myIsActive = false;
			}
		}

		void Thread::Run()
		{
			if (myFunction)
				myFunction(myUserData);
		}

		void* Thread::ThreadFunc(void* UserData)
		{
			// The sfThread instance is stored in the user data
			Thread* ThreadToRun = reinterpret_cast<Thread*>(UserData);

			// Tell the thread to handle cancel requests immediatly
			pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

			// Forward to the instance
			ThreadToRun->Run();

			return NULL;
		}
	}
}
