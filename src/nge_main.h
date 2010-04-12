#ifndef NGE_MAIN_H_
#define NGE_MAIN_H_

#ifdef WIN32
#include "../../win32depends/include/SDL.h"
#else
#define main	NGE_main
#define printf pspDebugScreenPrintf
#endif


#endif
