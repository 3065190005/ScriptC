#pragma once
#ifndef _CERVER_CERPARSERMACRO_
#define _CERVER_CERPARSERMACRO_
#include "..\Global\globalDefine.h"
#define DebugASTLog true

#if DebugASTLog && GlobalDebugOpend
#define astlog(var)\
std::cout << var
#else
#define astlog(var)
#endif


#endif