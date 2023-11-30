#pragma once
#ifndef _SCRIPTC_GLOBAL_DEFINE_
#define _SCRIPTC_GLOBAL_DEFINE_

#define SPECIAL_FUNC_ATTR "_attr"	// Attr 特殊函数名
#define SPECIAL_FUNC_GC "_gc"		// Gc 特殊函数名
#define SPECIAL_FUNC_INIT "_init"	// Init 特殊函数名
#define CLASS_NAME "_class_name"			// 接口变量的默认接口变量名
#define CLASS_PARENT "_class_parent"		// 接口变量的默认父接口变量名
#define CLASS_AUTO_GC_CALL "_class_auto_gc_call"	// 特殊函数Gc是否开启调用变量名
#define REQUIRE_FILE_PREFIX "%::"		// 编译器 require 导入文件的文件特殊前缀与普通变量做区分
#define SYSTEM_FILE_NAME "__FILE_NAME__" // 特殊变量 FILE_NAME 的变量名
#define SYSTEM_MAIN_NAME "__MAIN_NAME__" // 特殊变量 MAIN_NAME 的变量名
#define DEBUG_TRY_CATCH false	// debug模式是否进行try和 catch

#ifdef _DEBUG
#define GlobalDebugOpend true
#else
#define GlobalDebugOpend false
#endif

#include <map>



// 全局变量管理
namespace ScriptC {
	namespace Obj {
		class GlobalManager
		{
		public:
			static size_t global_debug_depth;
		};

	}
}

#endif