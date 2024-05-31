#include <stdio.h>

#define LOG_USE_COLOR
#define LOG_ENTRY_FILE "log_entry_table.h"
#define LOG_USE_SHORT_NAMES_GLOBALLY
#define LOG_TIME_FORMAT LOG_COLOR_GRAY "%H:%M:%S "
#include "log.h"

int log_verbosity_level = LOG_EVERYTHING;

int main()
{
    LOG(LOG_TRACE|LOG_PLATFORM,   "Startup");
    LOG(INFO|VIDEO,       "Video init");
    LOG(TRACE|PLATFORM,   "Trivial info here");
    LOG(AUDIO|ERROR|INIT, "Fatal error");
    LOG(TRACE|AUDIO|INIT, "Trivial info here");
    LOG(WARN|VIDEO|INIT,  "WARNING alksjdlkajskdljaskl");
    LOG(FATAL|SHUTDOWN,   "Aborting\n");
}
