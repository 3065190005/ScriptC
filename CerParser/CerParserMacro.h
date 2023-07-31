#pragma once
#ifndef _SCRIPTC_CERPARSERMACRO_
#define _SCRIPTC_CERPARSERMACRO_
#include "..\Global\globalDefine.h"
#define DebugASTLog true

#if DebugASTLog && GlobalDebugOpend
#define astlog(var)\
std::cout << var
#else
#define astlog(var)
#endif


#endif