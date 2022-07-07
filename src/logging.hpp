#pragma once

#include <easylogging++.h>
#define NLOG(LEVEL) CLOG(LEVEL, "consoleLogger", "default")

// *****************************************************************************
static void NLOGINFO( std::string& s, int width = 70 ) {
  auto logline = []( char*& e, char* b ){
    while (*e == ' ') ++e;
    char tmp = *e;
    *e = 0;
    NLOG(INFO) << b;
    *e = tmp;
  };
  char* b = &s[0];
  char* e = b;
  while (*(++e) != 0) {
    if (*e == '\n' || (e-b > width && (*e == ' '))) {
      logline( e, b );
      b = e;
    }
    if (*b == '\n') ++b;
  }
  if (e > b) logline( e, b );
}

// *****************************************************************************
static void NLOGINFO( std::string&& s, int width = 70 ) {
  NLOGINFO( std::move(s), width );
}

// *****************************************************************************
static void setup_logging( const std::string& executable,
                           void (*crash_handler)(int),
                           const std::string& logfile = {},
                           bool file_logging = false,
                           bool console_logging = false )
{
  el::Configurations fileLogConf;
  fileLogConf.parseFromText(
    "* GLOBAL:\n ENABLED = " + std::to_string(file_logging) + '\n' +
                 "FORMAT = \"%datetime %host " + executable +
                           "[%thread]: %level: %msg\"\n"
                 "FILENAME = " + logfile + '\n' +
                 "TO_FILE = true\n"
                 "TO_STANDARD_OUTPUT = false\n"
                 "MAX_LOG_FILE_SIZE = 2097152\n"
    "* DEBUG:\n ENABLED = true\n" );
  el::Configurations consoleLogConf;
  consoleLogConf.parseFromText(
    "* GLOBAL:\n ENABLED = " + std::to_string(console_logging) + '\n' +
                "FORMAT = \"%msg\"\n"
                "TO_FILE = false\n"
                "TO_STANDARD_OUTPUT = true\n"
    "* DEBUG:\n ENABLED = false\n" );
  el::Loggers::addFlag( el::LoggingFlag::MultiLoggerSupport );
  el::Loggers::addFlag( el::LoggingFlag::ColoredTerminalOutput );
  el::Helpers::setThreadName( "main" );
  el::Helpers::setCrashHandler( crash_handler );
  el::Logger* consoleLogger = el::Loggers::getLogger( "consoleLogger" );
  el::Loggers::reconfigureLogger( "default", consoleLogConf );
  el::Loggers::reconfigureLogger( consoleLogger, fileLogConf );
}
