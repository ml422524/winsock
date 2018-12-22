#pragma once

#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/helpers/fileinfo.h>
#include <log4cplus/loggingmacros.h>

//
class Log{
public:
   static int InitLog();
   static log4cplus::Logger root;
};

//
#define LOG_INFO(logEvent)  LOG4CPLUS_INFO(Log::root, logEvent)
#define LOG_FATAL(logEvent) LOG4CPLUS_FATAL(Log::root, logEvent)
