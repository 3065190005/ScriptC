#pragma once
#ifndef _SCRIPTC_CERLEXICALMACRO_
#define _SCRIPTC_CERLEXICALMACRO_
#include "..\Global\globalDefine.h"

#define DebugTokenLog true

#define MACRO_VAR_MAX_LEN 255

#if DebugTokenLog && GlobalDebugOpend
#define toklog(var)\
std::cout << var
#else
#define toklog(var)
#endif

#endif