#pragma once
#include <mutex>
#include <stratega/Logging/FileLogger.h>
#include <stratega/Logging/LoggingScope.h>

namespace SGA::Log {
inline std::unique_ptr< FileLogger > defaultLogger;
inline std::vector< LoggingScope* > loggingScopes;
inline std::mutex loggingMutex;

FileLogger& getDefaultLogger();
void setDefaultLogger(std::unique_ptr< FileLogger > logger);
std::string getLoggingScopeKey(const std::string& lastKey);

template < typename T >
void logSingleValue(const std::string& key, const T& value)
{
   std::lock_guard< std::mutex > logGuard(loggingMutex);
   getDefaultLogger().logSingleValue(getLoggingScopeKey(key), value);
}

template < typename T >
void logValue(const std::string& key, const T& value)
{
   std::lock_guard< std::mutex > logGuard(loggingMutex);
   getDefaultLogger().logValue(getLoggingScopeKey(key), value);
}
}  // namespace SGA::Log
