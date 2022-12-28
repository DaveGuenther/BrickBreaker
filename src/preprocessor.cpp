// Cross-platform logic
#if !defined(PREDEF_PLATFORM_UNIX) && !defined(PREDEF_PLATFORM_WINDOWS) && !defined(PREDEF_PLATFORM_OTHER) // do this once for the engine
    #if defined(unix) || defined(__unix__) || defined(__unix) || defined(__linux__)
    #	define PREDEF_PLATFORM_UNIX
    #	pragma message("Linux/Unix platform detected!")
    #elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
    #	define PREDEF_PLATFORM_WINDOWS
    #	pragma message("Windows platform detected!")
    #else 
    #	define PREDEF_PLATFORM_OTHER
    #	pragma message("Non-Windows/Linux platform detected!")
    #endif
#endif
// Set conditions that determine the OS here, then test for PREDEF_PLATFORM_UNIX, PREDEF_PLATFORM_WINDOWS, and PREDEF_PLATFORM_OTHER elsewhere in .h and .cpp files