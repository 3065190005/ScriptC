#pragma once
#ifndef _SCRIPTC_CERVMMACRO_
#define _SCRIPTC_CERVMMACRO_
#include "..\Global\globalDefine.h"
#define DebuvmLog true

#if DebuvmLog && GlobalDebugOpend
#define vmlog(var)\
std::cout << var
#else
#define vmlog(var)
#endif

#endif