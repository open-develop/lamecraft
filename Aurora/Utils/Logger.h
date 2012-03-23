#ifndef AURORA_LOGGER_H
#define AURORA_LOGGER_H

namespace Aurora
{
	namespace Utils
	{

		class Logger
		{
		private:

			Logger()
			{
				logActive = true;
			}

			static Logger m_Logger;
			bool logActive;

		public:

			static Logger* Instance()
			{
				return &m_Logger;
			}

			void LogMessage(const char *message, ...);
			void SetLoggerActive(bool on);
		};
	}
}

#endif
