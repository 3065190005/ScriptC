#pragma once
#ifndef _SCRIPTC_GLOBAL_DEFINE_
#define _SCRIPTC_GLOBAL_DEFINE_

#define SPECIAL_FUNC_ATTR "_attr" 
#define SPECIAL_FUNC_GC "_gc" 
#define SPECIAL_FUNC_INIT "_init" 
#define CLASS_NAME "_class_name"
#define CLASS_PARENT "_class_parent"
#define CLASS_AUTO_GC_CALL "_class_auto_gc_call"

#ifdef _DEBUG
#define GlobalDebugOpend true
#else
#define GlobalDebugOpend false
#endif

#endif