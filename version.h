#ifndef __VERSION_H__
#define __VERSION_H__

#define VERSION_MAJOR 2
#define VERSION_MINOR 3

#define xstr(s) str(s)
#define str(s) #s

#define VERSION xstr(VERSION_MAJOR) "." xstr(VERSION_MINOR)
#define COMPILED_TS __DATE__ " " __TIME__

#endif
