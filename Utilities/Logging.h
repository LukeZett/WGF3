#pragma once
#include <string>
#include <chrono>

#define LOG_DEBUG(msg) WGF::Logging::DebugInfo(msg);
#define LOG_TIMING(msg) WGF::Logging::Notification(msg)
#define LOG_INFO(msg) WGF::Logging::Notification(msg)
#define LOG_WARN(msg) WGF::Logging::Warning(msg)
#define LOG_ERROR(msg) WGF::Logging::Error(msg)

namespace WGF {
	/**
	* @class Logging
	* @brief contains static functions for logging messages/warnings/errors
	*/
	class Logging
	{
	public:
		static void DebugInfo(const std::string& message);
		static void Notification(const std::string& message);
		static void Warning(const std::string& message);
		static void Error(const std::string& message);

		static inline Logging& Get() { return s_instance; };
	private: // functions
		Logging() = default;
		static time_t GetCurrentTime();
		static void PrintCurrentTime();

	private: // members
		static Logging s_instance;
	};
}

