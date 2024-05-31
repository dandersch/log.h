extern int log_verbosity_level;

#if !defined(LOG_ENTRY_FILE)
  #define LOG_ENTRIES_SEVERITY \
    LOG_ENTRY_SEVERITY(TRACE, (1<<0), "[TRACE]", LOG_COLOR_GRAY)   \
    LOG_ENTRY_SEVERITY(INFO,  (1<<1), "[INFO ]", LOG_COLOR_GREEN)  \
    LOG_ENTRY_SEVERITY(WARN,  (1<<2), "[WARN ]", LOG_COLOR_YELLOW) \
    LOG_ENTRY_SEVERITY(ERROR, (1<<3), "[ERROR]", LOG_COLOR_RED)    \
    LOG_ENTRY_SEVERITY(FATAL, (1<<4), "[FATAL]", LOG_COLOR_PURPLE)

  #define LOG_ENTRIES_SUBSYSTEM \
    LOG_ENTRY_SUBSYSTEM(PLATFORM, (1<<5), "[PLATF]", LOG_COLOR_YELLOW) \
    LOG_ENTRY_SUBSYSTEM(AUDIO,    (1<<6), "[AUDIO]", LOG_COLOR_YELLOW) \
    LOG_ENTRY_SUBSYSTEM(VIDEO,    (1<<7), "[VIDEO]", LOG_COLOR_YELLOW) \
    LOG_ENTRY_SUBSYSTEM(NETWORK,  (1<<8), "[NETWK]", LOG_COLOR_YELLOW) \
    LOG_ENTRY_SUBSYSTEM(TEST,     (1<<9), "[TEST ]", LOG_COLOR_YELLOW)

  #define LOG_ENTRIES_CATEGORY \
    LOG_ENTRY_CATEGORY(INIT,       (1<<20), "[INIT ]", LOG_COLOR_PURPLE) \
    LOG_ENTRY_CATEGORY(SHUTDOWN,   (1<<21), "[SHUTD]", LOG_COLOR_OFF)    \
    LOG_ENTRY_CATEGORY(ACTIVATE,   (1<<22), "[ACTIV]", LOG_COLOR_PURPLE) \
    LOG_ENTRY_CATEGORY(DEACTIVATE, (1<<23), "[DEACT]", LOG_COLOR_PURPLE)
#endif

#ifdef LOG_USE_COLOR
  #define LOG_COLOR_OFF    "\033[0m"
  #define LOG_COLOR_GRAY   "\e[38;5;245m"
  #define LOG_COLOR_BLUE   "\x1b[94m"
  #define LOG_COLOR_CYAN   "\x1b[36m"
  #define LOG_COLOR_GREEN  "\x1b[32m"
  #define LOG_COLOR_YELLOW "\x1b[33m"
  #define LOG_COLOR_RED    "\x1b[31m"
  #define LOG_COLOR_PURPLE "\x1b[35m"
#else
  #define LOG_COLOR_OFF    ""
  #define LOG_COLOR_GRAY   ""
  #define LOG_COLOR_BLUE   ""
  #define LOG_COLOR_CYAN   ""
  #define LOG_COLOR_GREEN  ""
  #define LOG_COLOR_YELLOW ""
  #define LOG_COLOR_RED    ""
  #define LOG_COLOR_PURPLE ""
#endif

enum
{
  /* severity */
  #define LOG_ENTRY_SEVERITY(name, value, string, color) LOG_##name = value,
  #define LOG_ENTRY_SUBSYSTEM(name, value, string, color)
  #define LOG_ENTRY_CATEGORY(name, value, string, color)
  #ifdef LOG_ENTRY_FILE
    #include LOG_ENTRY_FILE
  #else
    LOG_ENTRIES_SEVERITY
  #endif
  #undef LOG_ENTRY_SEVERITY

  #define LOG_ENTRY_SEVERITY(name, value, string, color) value |
  #define LOG_SEVERITY_MASK LOG_ENTRIES_SEVERITY LOG_ENTRIES_CATEGORY 0
  #ifdef LOG_ENTRY_FILE
    LOG_SEVERITY =
      #include LOG_ENTRY_FILE
    0,
  #else
    LOG_SEVERITY = LOG_SEVERITY_MASK,
  #endif
  #undef LOG_ENTRY_SEVERITY
  #undef LOG_ENTRY_SUBSYSTEM
  #undef LOG_ENTRY_CATEGORY

  /* subsystems */
  #define LOG_ENTRY_SEVERITY(name, value, string, color)
  #define LOG_ENTRY_SUBSYSTEM(name, value, string, color) LOG_##name = value,
  #define LOG_ENTRY_CATEGORY(name, value, string, color)
  #ifdef LOG_ENTRY_FILE
    #include LOG_ENTRY_FILE
  #else
    LOG_ENTRIES_SUBSYSTEM
  #endif
  #undef LOG_ENTRY_SUBSYSTEM

  #define LOG_ENTRY_SUBSYSTEM(name, value, string, color) value |
  #define LOG_SUBSYSTEM_MASK LOG_ENTRIES_SUBSYSTEM 0
  #ifdef LOG_ENTRY_FILE
    LOG_SUBSYSTEMS =
      #include LOG_ENTRY_FILE
    0,
  #else
    LOG_SUBSYSTEMS = LOG_SUBSYSTEM_MASK,
  #endif
  #undef LOG_ENTRY_SEVERITY
  #undef LOG_ENTRY_SUBSYSTEM
  #undef LOG_ENTRY_CATEGORY

  /* categories */
  #define LOG_ENTRY_SEVERITY(name, value, string, color)
  #define LOG_ENTRY_SUBSYSTEM(name, value, string, color)
  #define LOG_ENTRY_CATEGORY(name, value, string, color) LOG_##name = value,
  #ifdef LOG_ENTRY_FILE
    #include LOG_ENTRY_FILE
  #else
    LOG_ENTRIES_CATEGORY
  #endif
  #undef LOG_ENTRY_CATEGORY

  #define LOG_ENTRY_CATEGORY(name, value, string, color) value |
  #define LOG_CATEGORY_MASK LOG_ENTRIES_CATEGORY 0
  #ifdef LOG_ENTRY_FILE
    LOG_CATEGORIES =
      #include LOG_ENTRY_FILE
    0,
  #else
    LOG_CATEGORIES = LOG_CATEGORY_MASK,
  #endif
  #undef LOG_ENTRY_SEVERITY
  #undef LOG_ENTRY_SUBSYSTEM
  #undef LOG_ENTRY_CATEGORY

  /* useful masks */
  LOG_EVERYTHING       = 0x7fffffff,
};

/* the core of the log macro */
#define _LOG(Flags, Format, ...)                                                                  \
  if ((Flags) & log_verbosity_level)                                                              \
  {                                                                                               \
    printf("%s%s%s %12.12s:%4i " Format LOG_COLOR_OFF"\n",                                        \
           LOG_PrioritiesLabel(Flags), LOG_SubsystemsLabel(Flags), LOG_SubcategoriesLabel(Flags), \
           __FILE__, __LINE__, ##__VA_ARGS__);                                                    \
  }

#if defined(LOG_USE_SHORT_NAMES_GLOBALLY) && defined(LOG_ENTRY_FILE)
/* NOTE this fills the global namespace with unprefixed names of your log entries (e.g. TRACE instead of LOG_TRACE) */
#define LOG_ENTRY_SEVERITY(name, value, string, color)  name = LOG_##name ,
#define LOG_ENTRY_SUBSYSTEM(name, value, string, color) name = LOG_##name ,
#define LOG_ENTRY_CATEGORY(name, value, string, color)  name = LOG_##name ,
enum {
    #include "log_entry_table.h"
};
#undef LOG_ENTRY_SEVERITY
#undef LOG_ENTRY_SUBSYSTEM
#undef LOG_ENTRY_CATEGORY
#endif

#define LOG_ENTRY_SEVERITY(name, value, string, color) name = LOG_##name ,
#define LOG_ENTRY_SUBSYSTEM(name, value, string, color) name = LOG_##name ,
#define LOG_ENTRY_CATEGORY(name, value, string, color) name = LOG_##name ,
#if defined(LOG_ENTRY_FILE)
  #define LOG(Flags, Format, ...)          \
    {                                      \
        _LOG(Flags, Format, ##__VA_ARGS__) \
    }
#else
  #define LOG(Flags, Format, ...)           \
    {                                       \
        enum {                              \
        /* remove LOG_ prefix */            \
        LOG_ENTRIES_SEVERITY                \
        LOG_ENTRIES_SUBSYSTEM               \
        LOG_ENTRIES_CATEGORY                \
        };                                  \
        _LOG(Flags, Format, ##__VA_ARGS__)  \
    }
#endif
/* NOTE redefined to above #define at bottom of file */
#undef LOG_ENTRY_SEVERITY
#undef LOG_ENTRY_SUBSYSTEM
#undef LOG_ENTRY_CATEGORY

#define LOG_ENTRY_SEVERITY(name, value, string, color) case LOG_##name: return color string LOG_COLOR_OFF;
#define LOG_ENTRY_SUBSYSTEM(name, value, string, color)
#define LOG_ENTRY_CATEGORY(name, value, string, color)
static inline const char* LOG_PrioritiesLabel(int Flags)
{
  switch (Flags & LOG_SEVERITY)
  {
    #ifdef LOG_ENTRY_FILE
      #include LOG_ENTRY_FILE
    #else
      LOG_ENTRIES_SEVERITY
    #endif
    case 0:          return LOG_COLOR_OFF    "[     ]";
    default:         return LOG_COLOR_OFF    "[invalid]";
  }
}
#undef LOG_ENTRY_SEVERITY
#undef LOG_ENTRY_SUBSYSTEM
#undef LOG_ENTRY_CATEGORY

#define LOG_ENTRY_SEVERITY(name, value, string, color)
#define LOG_ENTRY_SUBSYSTEM(name, value, string, color) case LOG_##name: return color string LOG_COLOR_OFF;
#define LOG_ENTRY_CATEGORY(name, value, string, color)
static inline const char* LOG_SubsystemsLabel(int Flags)
{
  switch (Flags & LOG_SUBSYSTEMS)
  {
    #ifdef LOG_ENTRY_FILE
      #include LOG_ENTRY_FILE
    #else
      LOG_ENTRIES_SUBSYSTEM
    #endif
    case 0:                return LOG_COLOR_OFF "[     ]";
    default:               return LOG_COLOR_OFF "[invalid ]";
  }
}
#undef LOG_ENTRY_SEVERITY
#undef LOG_ENTRY_SUBSYSTEM
#undef LOG_ENTRY_CATEGORY

#define LOG_ENTRY_SEVERITY(name, value, string, color)
#define LOG_ENTRY_SUBSYSTEM(name, value, string, color)
#define LOG_ENTRY_CATEGORY(name, value, string, color) case LOG_##name: return color string LOG_COLOR_OFF;
static inline const char* LOG_SubcategoriesLabel(int Flags)
{
  switch (Flags & LOG_CATEGORIES)
  {
    #ifdef LOG_ENTRY_FILE
      #include LOG_ENTRY_FILE
    #else
      LOG_ENTRIES_CATEGORY
    #endif
    case 0:                  return LOG_COLOR_OFF "[     ]";
    default:                 return LOG_COLOR_OFF "[invalid]";
  }
}
#undef LOG_ENTRY_SEVERITY
#undef LOG_ENTRY_SUBSYSTEM
#undef LOG_ENTRY_CATEGORY

/* NOTE these #define's are used in the LOG macro, so we cannot keep them #undef'ed */
#define LOG_ENTRY_SEVERITY(name, value, string, color) name = LOG_##name ,
#define LOG_ENTRY_SUBSYSTEM(name, value, string, color) name = LOG_##name ,
#define LOG_ENTRY_CATEGORY(name, value, string, color) name = LOG_##name ,
