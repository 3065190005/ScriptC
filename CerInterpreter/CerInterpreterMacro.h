#pragma once
#ifndef _CERVER_CERINTERPRETERMACRO_
#define _CERVER_CERINTERPRETERMACRO_
#include "..\Global\globalDefine.h"
#define DebuInterLog true

#if DebuInterLog && GlobalDebugOpend
#define interlog(var)\
std::cout << var
#else
#define interlog(var)
#endif

#endif